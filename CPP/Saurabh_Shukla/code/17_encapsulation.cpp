#include <iostream>

/*✅ Static variables are shared across all instances
  ✅ Static functions can access only static members
  ✅ Static members must be initialized outside the class
  ✅ Static members persist for the program's lifetime
  ❌ Cannot use this pointer inside static functions
  ❌ Static functions cannot be virtual*/
class Demo {
private:
    static int privateStaticVar;  // Private static variable

public:
    static int counter;  // Public static variable

    // Constructor: Increments counter on every object creation
    Demo() {
        counter++;
    }

    // Static function to access static members
    static void showCounter() {
        // Static functions CAN access static variables
        std::cout << "Counter: " << counter << std::endl;
        
        // Static functions CAN access private static variables
        std::cout << "Private Static Variable: " << privateStaticVar << std::endl;
        
        // Static functions CANNOT access non-static members
        // std::cout << this->nonStaticVar;  // ERROR: 'this' pointer is not available
    }

    // Static function cannot be virtual (not tied to an instance)
    // virtual static void invalidFunction();  // ERROR

    // Static functions CAN be called using an object, but it's discouraged
    void callStaticFunction() {
        std::cout << "Calling static function from non-static method.\n";
        showCounter();
    }

    // Non-static function CAN access static members
    void incrementPrivateStatic() {
        privateStaticVar++;
    }
};

// Initialize static members outside the class
int Demo::counter = 0;
int Demo::privateStaticVar = 10;

int main() {
    std::cout << "Initial Counter: " << Demo::counter << std::endl;

    Demo obj1, obj2;
    obj1.callStaticFunction();

    std::cout << "Counter after creating objects: " << Demo::counter << std::endl;

    // Access static variable without object
    Demo::counter += 10;
    std::cout << "Counter after direct access: " << Demo::counter << std::endl;

    // Access static function without object
    Demo::showCounter();

    // Accessing private static member through a member function
    obj1.incrementPrivateStatic();
    Demo::showCounter();

    return 0;
}
