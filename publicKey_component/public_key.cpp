#include <stdexcept>
#include <string>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <pybind11/pybind11.h>

/**
 * Recovers a public key from a signed message using ECDSA.
 * @param signature: The signature of the signed message.
 * @param message: The original message.
 * @returns: The recovered public key.
 * @throws std::runtime_error: If the recovery of the public key fails.
 */
std::string recover_public_key(const std::string& signature, const std::string& message) {
    // Setup ECDSA
    EC_KEY *eckey = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!eckey) {
        throw std::runtime_error("Error setting up ECDSA key");
    }

    // Prepare the signature
    size_t sig_len = signature.length() / 2;
    unsigned char* sig_bytes = new unsigned char[sig_len];
    for(size_t i = 0; i < sig_len; i++) {
        sscanf(signature.c_str() + 2*i, "%02x", &sig_bytes[i]);
    }

    // Create an ECDSA_SIG object from the byte array
    const unsigned char* p = sig_bytes;
    ECDSA_SIG* sig = d2i_ECDSA_SIG(NULL, &p, sig_len);
    if (!sig) {
        delete[] sig_bytes;
        EC_KEY_free(eckey);
        throw std::runtime_error("Failed to create ECDSA_SIG");
    }

    // Verify the signature and recover the public key
    int ret = ECDSA_do_verify(reinterpret_cast<const unsigned char*>(message.c_str()), message.size(), sig, eckey);

    // Free the ECDSA_SIG object now that we're done with it
    ECDSA_SIG_free(sig);

    if (1 != ret) {
        delete[] sig_bytes;
        EC_KEY_free(eckey);
        throw std::runtime_error("Failed to recover public key");
    }

    // Convert public key to hex string representation
    const EC_POINT* pub_key = EC_KEY_get0_public_key(eckey);
    char* pub_key_hex = EC_POINT_point2hex(EC_KEY_get0_group(eckey), pub_key, POINT_CONVERSION_COMPRESSED, NULL);

    delete[] sig_bytes;
    EC_KEY_free(eckey);

    return std::string(pub_key_hex);
}

/**
 * Pybind11 module definition.
 * The module is named "public_key" and exposes the function recover_public_key to Python.
 */
PYBIND11_MODULE(public_key, m) {
    m.doc() = "ECDSA Public Key Recovery module";
    m.def("recover_public_key", &recover_public_key, "Recover ECDSA public key from signature and message");
}
