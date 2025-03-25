#include <iostream>
#include <cstring>

namespace MyNamespace {
    class String {
    private:
        char* str;  // Pointer to dynamically allocated memory
    public:
        // Dynamic Constructor: Allocates memory dynamically
        String(const char* s) {
            std::cout << "Dynamic Constructor Called\n";
            str = new char[strlen(s) + 1]; // Allocate memory for string
            strcpy(str, s);  // Copy the string content
        }

        // Copy Constructor (Deep Copy)
        String(const String& other) {
            std::cout << "Copy Constructor Called\n";
            str = new char[strlen(other.str) + 1]; // Allocate new memory
            strcpy(str, other.str);  // Copy contents to avoid shallow copy issue
        }

        // Function to display the string
        void display() const {
            std::cout << "String: " << str << std::endl;
        }

        // Destructor: Releases dynamically allocated memory
        ~String() {
            std::cout << "Destructor Called\n";
            delete[] str;  // Free the allocated memory
        }
    };
} // End of namespace MyNamespace

int main() {
    // Creating an object using dynamic constructor
    MyNamespace::String s1("Hello, World!"); 
    s1.display();

    // Copy Constructor is called here (deep copy)
    MyNamespace::String s2 = s1;
    s2.display();

    // Destructor will be called automatically for both objects when they go out of scope
    return 0;
}
