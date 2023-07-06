#include <iostream>
#include <queue>
#include <vector>

// Structure representing a real-time task
struct RealTimeTask {
    int taskId;
    int executionTime;
};

// Structure representing a node
struct Node {
    int nodeId;
    int taskCount;
    std::queue<RealTimeTask> taskQueue;
};

// Function to find the node with the least busy queue
Node& findLeastBusyNode(std::vector<Node>& nodes) {
    Node* leastBusyNode = &nodes[0];

    for (Node& node : nodes) {
        if (node.taskCount < leastBusyNode->taskCount) {
            leastBusyNode = &node;
        }
    }

    return *leastBusyNode;
}

// Function to assign a real-time task to a specific node
void assignTaskToNode(Node& node, RealTimeTask task) {
    // Add the task with a special header to the front of the queue
    RealTimeTask specialHeaderTask { -1, 0 };
    node.taskQueue.push(specialHeaderTask);
    node.taskQueue.push(task);

    // Increment the task count for the node
    node.taskCount++;

    std::cout << "Task " << task.taskId << " assigned to Node " << node.nodeId << std::endl;
}

int main() {
    // Create nodes
    std::vector<Node> nodes {
        { 1, 0, {} },
        { 2, 0, {} },
        { 3, 0, {} }
    };

    // Create real-time tasks
    RealTimeTask task1 { 1, 10 };
    RealTimeTask task2 { 2, 5 };

    // Find the least busy node and assign task1 to it
    Node& leastBusyNode = findLeastBusyNode(nodes);
    assignTaskToNode(leastBusyNode, task1);

    // Assign task2 to a specific node (Node 2)
    assignTaskToNode(nodes[1], task2);

    return 0;
}
