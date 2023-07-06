#include <iostream>
#include <vector>
#include <unordered_map>

// Node structure representing each node in the system
struct Node {
    int id;
    int powerConsumption;
    int throughput;
    int numCores;
};

// Hash table representing the shared data structure
std::unordered_map<int, Node> hashTable;

// Function to update the hash table when a node joins
void updateHashTable(Node node) {
    hashTable[node.id] = node;
}

// Function to determine if a node should send gossip based on power consumption
bool shouldSendGossip(Node node) {
    static std::unordered_map<int, int> previousPowerConsumption;
    
    // Check if the node's power consumption has increased by 5%
    if (previousPowerConsumption.find(node.id) != previousPowerConsumption.end()) {
        int previousValue = previousPowerConsumption[node.id];
        int currentValue = node.powerConsumption;
        if (currentValue >= (previousValue + (previousValue * 0.05))) {
            previousPowerConsumption[node.id] = currentValue;
            return true;
        }
    } else {
        previousPowerConsumption[node.id] = node.powerConsumption;
    }
    
    return false;
}

// Function to perform gossip between two nodes
void performGossip(Node sourceNode, Node destinationNode) {
    // Gossip implementation goes here
    std::cout << "Performing gossip between Node " << sourceNode.id << " and Node " << destinationNode.id << std::endl;
    // Update destinationNode's hash table entry based on sourceNode's information
    hashTable[destinationNode.id] = hashTable[sourceNode.id];
}

// Function to select the least busy node for gossiping the hash table to a recently joined node
Node selectLeastBusyNode() {
    Node leastBusyNode;
    int minThroughput = INT_MAX;

    for (const auto& entry : hashTable) {
        const Node& node = entry.second;
        
        // Check if the node has a lower throughput than the current minimum
        if (node.throughput < minThroughput) {
            leastBusyNode = node;
            minThroughput = node.throughput;
        }
    }

    return leastBusyNode;
}

int main() {
    // Simulated example: Two nodes joining the system
    Node node1 { 1, 10, 100, 4 }; // Node with ID 1 joining
    Node node2 { 2, 20, 200, 8 }; // Node with ID 2 joining
    
    // Update the hash table with the newly joined nodes
    updateHashTable(node1);
    updateHashTable(node2);
    
    // Simulated example: Power consumption of node 1 increases by 10%
    node1.powerConsumption += (node1.powerConsumption * 0.10);
    
    // Check if node 1 should send gossip
    if (shouldSendGossip(node1)) {
        Node leastBusyNode = selectLeastBusyNode();
        performGossip(node1, leastBusyNode);
    }
    
    return 0;
}
