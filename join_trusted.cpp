#include <iostream>
#include <vector>
#include <unordered_set>

// Node structure representing each node in the network
struct Node {
    int id;
    std::unordered_set<int> authorizedNodes;
};

// Function to authorize a node by another node (1-to-1)
void authorizeNode(Node& authorizer, Node& nodeToAuthorize) {
    authorizer.authorizedNodes.insert(nodeToAuthorize.id);
}

// Function to authorize a node by every member of the network (N-to-N)
void authorizeNodeByAll(std::vector<Node>& network, Node& nodeToAuthorize) {
    for (Node& member : network) {
        member.authorizedNodes.insert(nodeToAuthorize.id);
        nodeToAuthorize.authorizedNodes.insert(member.id);
    }
}

// Function to check if a node can join the network based on authorization rules
bool canJoinNetwork(std::vector<Node>& network, Node& nodeToJoin) {
    if (network.size() == 0) {
        // First node joining the network, no authorization required
        return true;
    }
    
    bool oneToOneAuthorized = false;
    bool nToNAuthorized = true;
    
    // Check 1-to-1 authorization
    for (Node& member : network) {
        if (member.authorizedNodes.find(nodeToJoin.id) != member.authorizedNodes.end()) {
            oneToOneAuthorized = true;
            break;
        }
    }
    
    // Check N-to-N authorization
    for (Node& member : network) {
        if (nodeToJoin.authorizedNodes.find(member.id) == nodeToJoin.authorizedNodes.end()) {
            nToNAuthorized = false;
            break;
        }
    }
    
    return oneToOneAuthorized || nToNAuthorized;
}

int main() {
    std::vector<Node> network;
    
    // Create initial network members
    Node node1 { 1 };
    Node node2 { 2 };
    Node node3 { 3 };
    
    // Authorize node1 to join the network (1-to-1)
    authorizeNode(node2, node1);
    
    // Authorize node2 to join the network (N-to-N)
    authorizeNodeByAll(network, node2);
    
    // Add the authorized nodes to the network
    network.push_back(node1);
    network.push_back(node2);
    
    // Create a new node to join the network
    Node nodeToJoin { 4 };
    
    // Check if the new node can join the network
    if (canJoinNetwork(network, nodeToJoin)) {
        // Authorize the new node to join the network (1-to-1)
        authorizeNode(node1, nodeToJoin);
        
        // Add the new node to the network
        network.push_back(nodeToJoin);
        
        std::cout << "Node " << nodeToJoin.id << " joined the network." << std::endl;
    } else {
        std::cout << "Node " << nodeToJoin.id << " cannot join the network." << std::endl;
    }
    
    return 0;
}
