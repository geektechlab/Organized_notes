#include <iostream>
#include <cstring>

class CopyExample {
private:
    char* str;
    bool deepCopy;  // Flag to indicate deep or shallow copy

public:
    // Constructor
    CopyExample(const char* s, bool isDeepCopy = true) : deepCopy(isDeepCopy) {
        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }

    // Copy Constructor (Handles both Deep and Shallow Copy)
    CopyExample(const CopyExample& obj) {
        deepCopy = obj.deepCopy;  // Copy the flag

        if (deepCopy) {
            // Perform Deep Copy
            str = new char[strlen(obj.str) + 1];
            strcpy(str, obj.str);
            std::cout << "Deep Copy performed.\n";
        } else {
            // Perform Shallow Copy
            str = obj.str;
            std::cout << "Shallow Copy performed.\n";
        }
    }

    // Display function
    void display() {
        std::cout << "String: " << str << std::endl;
    }

    // Destructor
    ~CopyExample() {
        if (deepCopy) {
            delete[] str;  // Free memory only in deep copy mode
            std::cout << "Memory Freed (Deep Copy).\n";
        } else {
            std::cout << "Shallow Copy - No memory freed.\n";
        }
    }
};

int main() {
    std::cout << "Creating obj1 (Deep Copy)...\n";
    CopyExample obj1("Hello", true);  // Deep Copy enabled

    std::cout << "\nCopying obj1 to obj2 (Deep Copy)...\n";
    CopyExample obj2 = obj1;  // Deep Copy

    obj1.display();
    obj2.display();

    std::cout << "\nCreating obj3 (Shallow Copy)...\n";
    CopyExample obj3("World", false);  // Shallow Copy enabled

    std::cout << "\nCopying obj3 to obj4 (Shallow Copy)...\n";
    CopyExample obj4 = obj3;  // Shallow Copy

    obj3.display();
    obj4.display();

    // Destructor will be called automatically for all objects at the end of main()

    return 0;
}
