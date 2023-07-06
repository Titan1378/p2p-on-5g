#include <iostream>
#include <unordered_map>
#include <string>

// ICN Content structure
struct ICNContent {
    std::string name;
    std::string data;
};

// ICN Router class
class ICNRouter {
private:
    std::unordered_map<std::string, ICNContent> contentStore;

public:
    // Method to handle content requests
    std::string handleContentRequest(const std::string& contentName) {
        if (contentStore.find(contentName) != contentStore.end()) {
            return contentStore[contentName].data;
        } else {
            return "Content not found.";
        }
    }

    // Method to handle content insertion
    void handleContentInsertion(const ICNContent& content) {
        contentStore[content.name] = content;
        std::cout << "Inserted content: " << content.name << std::endl;
    }
};

int main() {
    // Create ICN router
    ICNRouter router;

    // Insert content into the router
    ICNContent content1 { "example.com/image.jpg", "Binary data of an image file" };
    router.handleContentInsertion(content1);

    // Handle content requests
    std::string requestedContent = "example.com/image.jpg";
    std::string retrievedContent = router.handleContentRequest(requestedContent);

    // Display the retrieved content
    std::cout << "Retrieved content: " << retrievedContent << std::endl;

    return 0;
}
