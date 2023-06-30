import pytest
import coincurve
import binascii
from hashlib import sha256
import public_key as ecdsa

def test_recover_pub_key_ecdsa_valid():
    # generate a private key
    private_key = coincurve.PrivateKey()
    # derive the public key
    public_key_original = private_key.public_key.format(False)
    # create a message
    message = b'Hello, World!'
    # hash the message
    hashed_message = sha256(message).digest()
    # sign the message
    signature = private_key.sign_recoverable(hashed_message, hasher=None)
    
    # convert the signature and the public key to hex string
    signature_hex = binascii.hexlify(signature).decode()
    public_key_hex = binascii.hexlify(public_key_original).decode()

    # recover the public key
    public_key_recovered = ecdsa.recover_public_key(signature_hex)
    
    assert public_key_recovered == public_key_hex


def generate_test_cases(n):
    # Create an empty list to hold the test cases
    test_cases = []

    for _ in range(n):
        # Generate a private key
        private_key = coincurve.PrivateKey()
        # Derive the public key
        public_key_original = private_key.public_key.format(False)
        # Create a message
        message = b'Hello, World!'
        # Hash the message
        hashed_message = sha256(message).digest()
        # Sign the message
        signature = private_key.sign_recoverable(hashed_message, hasher=None)
        
        # Convert the signature and the public key to hex string
        signature_hex = binascii.hexlify(signature).decode()
        public_key_hex = binascii.hexlify(public_key_original).decode()
        
        # Recover the public key
        public_key_recovered = ecdsa.recover_public_key(signature_hex)
        
        # Add the test case to the list
        test_cases.append({
            'original_public_key': public_key_hex,
            'signature': signature_hex,
            'recovered_public_key': public_key_recovered,
        })

    return test_cases

