#include <iostream>

class Example {
private:
    const int value;  // Constant member must be initialized in an initializer list
    int& ref;         // Reference member must also be initialized in an initializer list

public:
    // Constructor using an initializer list to initialize both members
    Example(int val, int& r) : value(val), ref(r) {}

    void modifyRef() {
        ref += 10;  // Modifies the original referenced variable
    }

    void display() {
        std::cout << "Const Value: " << value << std::endl;
        std::cout << "Reference Value: " << ref << std::endl;
    }
};

int main() {
    int x = 50;
    
    // Create an object, passing a constant value and a reference to x
    Example obj(100, x);

    obj.display();
    obj.modifyRef();  // Modifies x
    obj.display();

    std::cout << "Modified x in main: " << x << std::endl; // Output: 60

    return 0;
}
