#include <iostream>

class Base {
public:
    static int baseCount; // Static variable shared by all instances

    Base() {
        baseCount++; // Modify static variable
        std::cout << "Base Constructor\n";
        std::cout << "Base `this` pointer: " << this << std::endl;

        // DANGER: Calling a virtual function in a constructor
        this->show(); // Calls Base::show() even if Derived overrides it
    }

    virtual void show() { 
        std::cout << "Base show()\n";
    }

    virtual ~Base() { // Virtual destructor for proper cleanup
        std::cout << "Base Destructor\n";
    }

    static void staticFunction() { // Static function does not use `this`
        std::cout << "Static Function in Base. Count: " << baseCount << std::endl;
    }
};

int Base::baseCount = 0; // Definition of static variable

class Derived : public Base {
public:
    int* data;

    Derived() {
        std::cout << "Derived Constructor\n";
        std::cout << "Derived `this` pointer: " << this << std::endl;
        data = new int(42);
    }

    void show() override { 
        std::cout << "Derived show()\n"; 
    }

    ~Derived() {
        std::cout << "Derived Destructor\n";
        delete data; // Prevent memory leak
    }
};

int main() {
    std::cout << "Creating object...\n";
    Base* obj = new Derived(); // Base pointer to Derived object
    std::cout << "Object created.\n";

    obj->show(); // Calls Derived::show() (polymorphism)

    Base::staticFunction(); // Call static function without an instance

    delete obj; // Proper cleanup due to virtual destructor

    return 0;
}
