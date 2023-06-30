#include <stdexcept>
#include <string>
#include <pybind11/pybind11.h>
#include <stdio.h>
#include <string>
#include <secp256k1_recovery.h>

std::string recover_public_key(std::string signature) {
    // Check if the signature starts with '0x' and remove it
    if (signature.substr(0, 2) == "0x") {
        signature = signature.substr(2);
    }

    // Convert hex string to byte array
    std::vector<unsigned char> signatureBytes(signature.length() / 2);
    for (unsigned int i = 0; i < signature.length(); i += 2) {
        std::string byteString = signature.substr(i, 2);
        unsigned char byte = (unsigned char) strtol(byteString.c_str(), NULL, 16);
        signatureBytes[i / 2] = byte;
    }

    if (signatureBytes.size() != 65) {
        throw std::invalid_argument("Invalid signature length");
    }

    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN);
    secp256k1_ecdsa_recoverable_signature sig;
    if (!secp256k1_ecdsa_recoverable_signature_parse_compact(ctx, &sig, signatureBytes.data(), signatureBytes[64])) {
        throw std::invalid_argument("Invalid signature format");
    }

    secp256k1_pubkey pubkey;
    if (!secp256k1_ecdsa_recover(ctx, &pubkey, &sig, signatureBytes.data())) {
        throw std::runtime_error("Failed to recover public key");
    }

    char serialized_pubkey[65];
    size_t len = 65;
    secp256k1_ec_pubkey_serialize(ctx, (unsigned char*)serialized_pubkey, &len, &pubkey, SECP256K1_EC_UNCOMPRESSED);

    secp256k1_context_destroy(ctx);
    
    // Convert the public key to a hex string
    std::string pubkeyHex;
    for (unsigned int i = 0; i < len; i++) {
        char hex[3];
        sprintf(hex, "%02x", (unsigned char)serialized_pubkey[i]);
        pubkeyHex += hex;
    }
    return pubkeyHex;
}

/**
 * Pybind11 module definition.
 * The module is named "public_key" and exposes the function recover_public_key to Python.
 */
PYBIND11_MODULE(public_key, m) {
    m.doc() = "ECDSA Public Key Recovery module";
    m.def("recover_public_key", &recover_public_key, "Recover ECDSA public key from signature and message");
}
