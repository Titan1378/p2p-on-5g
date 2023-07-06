#include <iostream>
#include <unordered_map>
#include <vector>
#include <sodium.h>

// Node structure representing each member in the network
struct Node {
    int id;
    std::unordered_map<int, int> points;
};

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

// Function to send data from source node to destination node
void send(Node& sourceNode, Node& destinationNode, const std::vector<unsigned char>& data) {
    std::vector<unsigned char> key(crypto_aead_xchacha20poly1305_ietf_KEYBYTES);
    std::vector<unsigned char> nonce(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
    randombytes_buf(key.data(), key.size());
    randombytes_buf(nonce.data(), nonce.size());

    std::vector<unsigned char> encryptedData = encryptXChaCha20(key, nonce, data);
    destinationNode.points[sourceNode.id] += encryptedData.size();

    std::cout << "Data sent from Node " << sourceNode.id << " to Node " << destinationNode.id << std::endl;
}

// Function to feed data from source node to destination node
void feed(Node& sourceNode, Node& destinationNode, const std::vector<unsigned char>& data) {
    std::vector<unsigned char> key(crypto_aead_xchacha20poly1305_ietf_KEYBYTES);
    std::vector<unsigned char> nonce(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);
    randombytes_buf(key.data(), key.size());
    randombytes_buf(nonce.data(), nonce.size());

    std::vector<unsigned char> encryptedData = encryptXChaCha20(key, nonce, data);
    destinationNode.points[sourceNode.id] += encryptedData.size();
    sourceNode.points[destinationNode.id] += encryptedData.size();

    std::cout << "Data fed from Node " << sourceNode.id << " to Node " << destinationNode.id << std::endl;
}

int main() {
    // Initialize libsodium
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium" << std::endl;
        return 1;
    }

    // Create network members
    std::vector<Node> network {
        { 1, {} },
        { 2, {} },
        { 3, {} },
        { 4, {} },
        { 5, {} }
    };

    // Simulate data transfer and feeding
    Node& sourceNode = network[0];
    Node& destinationNode = network[1];
    std::vector<unsigned char> data = { 'H', 'e', 'l', 'l', 'o' };

    send(sourceNode, destinationNode, data);
    feed(sourceNode, destinationNode, data);

    return 0;
}
