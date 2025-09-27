#include <iostream>
using namespace std;

// Base class
class Base {
public:
    int* ptr; // Pointer for dynamic memory allocation

    // Constructor - Allocating memory using new
    Base() {
        ptr = new int(100); // Allocates an integer in heap
        cout << "Base Constructor: Memory allocated." << endl;
    }

    // Non-virtual function (Method Hiding will occur in Derived)
    void show() { 
        cout << "Base class show()" << endl;
    }

    // Virtual function (Will be overridden in Derived)
    virtual void display() { 
        cout << "Base class display()" << endl;
    }

    // Destructor - Freeing allocated memory
    virtual ~Base() {
        delete ptr;  // Free memory
        cout << "Base Destructor: Memory deallocated." << endl;
    }
};

// Derived class
class Derived : public Base {
public:
    // Method Hiding: This hides Base::show() (No virtual keyword)
    void show() { 
        cout << "Derived class show()" << endl; 
    }

    // Method Overriding: Overrides Base::display()
    void display() override { 
        cout << "Derived class display()" << endl; 
    }
};

int main() {
    // Dynamic memory allocation
    Base* obj = new Derived(); 

    cout << "\nCalling show() function:" << endl;
    obj->show();  // Calls Base::show() because it's NOT virtual ( maybe because base* refers to memeory layout of class like pointer in c refers to ptr memory layout type instead of variable it points to ?

    cout << "\nCalling display() function:" << endl;
    obj->display();  // Calls Derived::display() due to method overriding, does vptr play a role here ? how does it work internally ?

    // Deleting the dynamically allocated object
    delete obj; 

    return 0;
}
