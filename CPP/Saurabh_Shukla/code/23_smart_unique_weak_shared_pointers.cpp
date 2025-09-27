#include <iostream>
#include <memory>

// Class representing a Resource that prints messages on construction and destruction
class Resource {
public:
    // Constructor that initializes the resource with an ID and prints a message
    Resource(int id) : id(id) { 
        std::cout << "Resource " << id << " acquired\n"; 
    }
    
    // Destructor that prints a message when the resource is destroyed
    ~Resource() { 
        std::cout << "Resource " << id << " destroyed\n"; 
    }
    
    // Function to simulate using the resource
    void use() { 
        std::cout << "Using Resource " << id << "\n"; 
    }
private:
    int id; // Unique identifier for the resource
};

// Unique Pointer Example: Demonstrates exclusive ownership of a resource
void uniquePtrExample() {
    std::cout << "Creating unique_ptr" << std::endl;
    // Create a unique_ptr that manages a new Resource instance
    std::unique_ptr<Resource> uptr = std::make_unique<Resource>(1);
    uptr->use();
    
    // std::unique_ptr<Resource> uptr2 = uptr; // ❌ Error: Cannot copy unique_ptr
    
    // Transferring ownership using std::move
    std::unique_ptr<Resource> uptr2 = std::move(uptr); // ✅ Ownership moved to uptr2
    
    // uptr is now empty, as ownership was transferred
    if (!uptr) std::cout << "uptr is now empty after move\n";
}

// Shared Pointer Example: Demonstrates multiple ownership
void sharedPtrExample() {
    std::cout << "Creating shared_ptr" << std::endl;
    // Create a shared_ptr that manages a new Resource instance
    std::shared_ptr<Resource> sp1 = std::make_shared<Resource>(2);
    
    // Multiple shared_ptr instances can share ownership
    std::shared_ptr<Resource> sp2 = sp1; // ✅ Reference count increases
    
    sp1->use(); // Using the resource
    
    // Print the number of shared_ptr owners
    std::cout << "Shared count: " << sp1.use_count() << "\n"; // Count = 2
}

// Weak Pointer Example: Observing a shared pointer without increasing reference count
void weakPtrExample() {
    std::cout << "Creating weak_ptr from shared_ptr" << std::endl;
    // Create a shared_ptr managing a new Resource instance
    std::shared_ptr<Resource> sp = std::make_shared<Resource>(3);
    
    // Create a weak_ptr that observes the shared_ptr without increasing the reference count
    std::weak_ptr<Resource> wp = sp;
    
    // Check if the weak_ptr is still valid
    if (auto spt = wp.lock()) { // lock() creates a temporary shared_ptr if resource is still valid
        spt->use();
        std::cout << "Weak ptr is valid\n";
    }
    
    // Reset the shared_ptr, which destroys the resource if no other owners exist
    sp.reset(); 
    
    // Check if the weak_ptr has expired
    if (wp.expired()) std::cout << "Weak ptr expired, resource deleted\n";
}

// Circular Reference Issue with Shared Pointers: Creates a memory leak
class Node {
public:
    int data; // Node value
    std::shared_ptr<Node> next; // Problem: Creates cyclic reference
    
    // Constructor
    Node(int val) : data(val) { std::cout << "Node " << data << " created\n"; }
    
    // Destructor (will not be called due to cyclic reference)
    ~Node() { std::cout << "Node " << data << " destroyed\n"; }
};

void circularReferenceIssue() {
    std::cout << "Demonstrating circular reference issue with shared_ptr" << std::endl;
    // Creating two shared_ptr nodes
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    
    // Creating cyclic reference
    node1->next = node2;
    node2->next = node1; // Memory leak: Nodes will never be deleted
}

// Fix Circular Reference with Weak Pointers
class SafeNode {
public:
    int data; // Node value
    std::weak_ptr<SafeNode> next; // Weak pointer avoids cyclic dependency
    
    // Constructor
    SafeNode(int val) : data(val) { std::cout << "SafeNode " << data << " created\n"; }
    
    // Destructor (will be called properly)
    ~SafeNode() { std::cout << "SafeNode " << data << " destroyed\n"; }
};

void fixCircularReference() {
    std::cout << "Fixing circular reference using weak_ptr" << std::endl;
    // Creating two shared_ptr nodes
    auto node1 = std::make_shared<SafeNode>(1);
    auto node2 = std::make_shared<SafeNode>(2);
    
    // Creating weak_ptr-based references, avoiding cyclic dependency
    node1->next = node2;
    node2->next = node1;
}

int main() {
    std::cout << "=== Unique Pointer Example ===\n";
    uniquePtrExample();

    std::cout << "\n=== Shared Pointer Example ===\n";
    sharedPtrExample();

    std::cout << "\n=== Weak Pointer Example ===\n";
    weakPtrExample();

    std::cout << "\n=== Circular Reference Issue ===\n";
    circularReferenceIssue(); // Memory leak!

    std::cout << "\n=== Fixing Circular Reference ===\n";
    fixCircularReference(); // No memory leak

    return 0;
}
