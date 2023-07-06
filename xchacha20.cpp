#include <iostream>
#include <sodium.h>
#include <vector>

// Function to encrypt data using XChaCha20
std::vector<unsigned char> encryptXChaCha20(const std::vector<unsigned char>& key,
                                            const std::vector<unsigned char>& nonce,
                                            const std::vector<unsigned char>& plaintext) {
    std::vector<unsigned char> ciphertext(plaintext.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES);
    unsigned long long ciphertextLen;

    crypto_aead_xchacha20poly1305_ietf_encrypt(
        ciphertext.data(), &ciphertextLen, plaintext.data(), plaintext.size(),
        nullptr, 0, nullptr, nonce.data(), key.data()
    );

    ciphertext.resize(ciphertextLen);
    return ciphertext;
}

// Function to decrypt data using XChaCha20
std::vector<unsigned char> decryptXChaCha20(const std::vector<unsigned char>& key,
                                            const std::vector<unsigned char>& nonce,
                                            const std::vector<unsigned char>& ciphertext) {
    std::vector<unsigned char> plaintext(ciphertext.size() - crypto_aead_xchacha20poly1305_ietf_ABYTES);
    unsigned long long plaintextLen;

    if (crypto_aead_xchacha20poly1305_ietf_decrypt(
        plaintext.data(), &plaintextLen, nullptr, ciphertext.data(), ciphertext.size(),
        nullptr, 0, nonce.data(), key.data()) != 0) {
        throw std::runtime_error("Decryption error: Invalid ciphertext");
    }

    plaintext.resize(plaintextLen);
    return plaintext;
}

int main() {
    // Initialize libsodium
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium" << std::endl;
        return 1;
    }

    // Generate random key and nonce
    std::vector<unsigned char> key(crypto_aead_xchacha20poly1305_ietf_KEYBYTES);
    std::vector<unsigned char> nonce(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
    randombytes_buf(key.data(), key.size());
    randombytes_buf(nonce.data(), nonce.size());

    // Plain text message
    std::string message = "Hello, XChaCha20!";
    std::vector<unsigned char> plaintext(message.begin(), message.end());

    // Encrypt the plaintext
    std::vector<unsigned char> ciphertext = encryptXChaCha20(key, nonce, plaintext);

    // Decrypt the ciphertext
    std::vector<unsigned char> decryptedText = decryptXChaCha20(key, nonce, ciphertext);

    // Convert decrypted text to string and print
    std::string decryptedMessage(decryptedText.begin(), decryptedText.end());
    std::cout << "Decrypted Message: " << decryptedMessage << std::endl;

    return 0;
}
