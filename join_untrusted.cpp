#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

// Node structure representing each node in the network
struct Node {
    int id;
    bool isSuperNode;
    std::unordered_map<int, std::string> hashTable;
};

// Function to calculate the Euclidean distance between two nodes
double calculateDistance(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// Function to find the closest member to the new node
Node& findClosestMember(const std::vector<Node>& network, const Node& newNode) {
    double minDistance = std::numeric_limits<double>::max();
    Node* closestMember = nullptr;
    
    for (const Node& member : network) {
        if (member.id != newNode.id) {
            double distance = calculateDistance(newNode.x, newNode.y, member.x, member.y);
            if (distance < minDistance) {
                minDistance = distance;
                closestMember = &const_cast<Node&>(member);
            }
        }
    }
    
    return *closestMember;
}

// Function to authenticate and share the hash table with the recently joined node
void authenticateAndShareHashTable(Node& member, Node& newNode) {
    // Authenticating and sharing hash table implementation goes here
    std::cout << "Authenticating and sharing hash table with Node " << newNode.id << " from Node " << member.id << std::endl;
    
    // Share member's hash table with the new node
    newNode.hashTable = member.hashTable;
}

int main() {
    std::vector<Node> network;
    
    // Create initial network members
    Node node1 { 1, true, {} };
    Node node2 { 2, false, {} };
    Node node3 { 3, false, {} };
    
    // Add the initial network members
    network.push_back(node1);
    network.push_back(node2);
    network.push_back(node3);
    
    // Create a new node to join the network
    Node newNode { 4, false, {} };
    newNode.x = 10; // Set the x-coordinate of the new node
    newNode.y = 15; // Set the y-coordinate of the new node
    
    // Find the member responsible for authentication and sharing the hash table
    Node& responsibleMember = newNode.isSuperNode ? node1 : findClosestMember(network, newNode);
    
    // Authenticate and share the hash table with the new node
    authenticateAndShareHashTable(responsibleMember, newNode);
    
    // Add the new node to the network
    network.push_back(newNode);
    
    std::cout << "Node " << newNode.id << " joined the network." << std::endl;
    
    return 0;
}
