#include <iostream>
#include <vector>
#include <unordered_map>

// Node structure representing each member in the network
struct Node {
    char id;
    bool isBusy;
    std::unordered_map<char, std::string> dataChunks;
};

// Function to handle the request from node A
void handleRequestFromA(Node& A, std::vector<Node>& network) {
    std::vector<char> standbyNodes;

    // Send request from A to B, C, and D
    for (Node& member : network) {
        if (member.id != A.id) {
            if (!member.isBusy) {
                standbyNodes.push_back(member.id);
                member.isBusy = true;
                std::cout << "Request sent from Node " << A.id << " to Node " << member.id << std::endl;
            }
        }
    }

    // Receive chunks of data from B, C, and D
    for (char nodeId : standbyNodes) {
        std::unordered_map<char, std::string>& dataChunks = A.dataChunks;
        std::unordered_map<char, std::string>& receivedChunks = network[nodeId - 'A'].dataChunks;

        for (const auto& chunk : receivedChunks) {
            dataChunks[chunk.first] = chunk.second;
            std::cout << "Received data chunk " << chunk.first << " from Node " << nodeId << std::endl;
        }
    }

    // Change the connection status between E and B, C, D to standby mode
    for (char nodeId : standbyNodes) {
        Node& standbyNode = network[nodeId - 'A'];
        if (standbyNode.dataChunks.find('E') != standbyNode.dataChunks.end()) {
            standbyNode.dataChunks.erase('E');
            std::cout << "Connection between Node " << standbyNode.id << " and Node E changed to standby mode" << std::endl;
        }
    }

    // Send data from E to A
    std::unordered_map<char, std::string>& EDataChunks = network['E' - 'A'].dataChunks;
    std::unordered_map<char, std::string>& ADataChunks = A.dataChunks;

    for (const auto& chunk : EDataChunks) {
        ADataChunks[chunk.first] = chunk.second;
        std::cout << "Received data chunk " << chunk.first << " from Node E" << std::endl;
    }

    // Change the connection status between E and B, C, D back to default mode
    for (char nodeId : standbyNodes) {
        Node& standbyNode = network[nodeId - 'A'];
        if (standbyNode.dataChunks.find('E') == standbyNode.dataChunks.end()) {
            standbyNode.dataChunks['E'] = "Data chunk from Node E";
            std::cout << "Connection between Node " << standbyNode.id << " and Node E changed back to default mode" << std::endl;
        }
    }

    // Clear the data chunks in nodes B, C, D, and E
    for (char nodeId : standbyNodes) {
        Node& standbyNode = network[nodeId - 'A'];
        standbyNode.dataChunks.clear();
    }
    network[nodeId - 'A'].dataChunks.clear();
    network['E' - 'A'].dataChunks.clear();
}

int main() {
    // Create network members
    std::vector<Node> network {
        { 'A', false, {} },
        { 'B', false, {} },
        { 'C', false, {} },
        { 'D', false, {} },
        { 'E', true, {} }
    };

    // Create node A
    Node A { 'A', false, {} };

    // Handle the request from node A
    handleRequestFromA(A, network);

    return 0;
}
