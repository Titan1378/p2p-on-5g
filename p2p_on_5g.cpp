#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Node structure representing each node in the P2P network
struct Node {
    std::string id;
    std::unordered_map<std::string, std::string> neighborMap;
};

// Function to establish P2P connections between nodes
void establishP2PConnections(std::vector<Node>& network) {
    for (Node& node : network) {
        for (const auto& neighbor : node.neighborMap) {
            std::string neighborId = neighbor.first;
            Node& neighborNode = network[std::stoi(neighborId) - 1];

            neighborNode.neighborMap[node.id] = node.id;
            std::cout << "Established P2P connection between Node " << node.id << " and Node " << neighborId << std::endl;
        }
    }
}

int main() {
    // Create network members
    std::vector<Node> network {
        { "1", {} },
        { "2", {} },
        { "3", {} },
        { "4", {} },
        { "5", {} }
    };

    // Define P2P connections between nodes
    network[0].neighborMap["2"] = "2";
    network[1].neighborMap["1"] = "1";
    network[1].neighborMap["3"] = "3";
    network[2].neighborMap["2"] = "2";
    network[2].neighborMap["4"] = "4";
    network[3].neighborMap["3"] = "3";
    network[3].neighborMap["5"] = "5";
    network[4].neighborMap["4"] = "4";

    // Establish P2P connections between nodes
    establishP2PConnections(network);

    return 0;
}
