// Memory representation of an object of Derived class:
//
//  ------------------------------------
// | vptr  -> Points to Derived's vtable|  // Hidden pointer (inserted by compiler)
//  ------------------------------------
// | Base::x (inherited from Base class)|  // Normal integer data member from Base
//  ------------------------------------
// | Derived::y (extra member in Derived)|  // Extra integer data member in Derived
//  ------------------------------------

// Base class vtable
//  --------------------------------------
// | 0 | Pointer to Base::show()         |
// | 1 | Pointer to Base::display()      |
//  --------------------------------------

// Derived class vtable
//  --------------------------------------
// | 0 | Pointer to Derived::show()      |
// | 1 | Pointer to Derived::display()   |
//  --------------------------------------

// 1. Each object of a class with virtual functions contains a hidden `vptr` (virtual table pointer).
// 2. Each class with virtual functions has a single `vtable`, which stores function pointers.
// 3. Function calls through a base class pointer/reference are **resolved dynamically** via vtable lookup.
// 4. If a function is non-virtual, the function call is **resolved at compile-time** instead of using the vtable.

#include <iostream>
using namespace std;

class Base {
public:
    int x;  // Normal data member

    // Virtual functions create a vtable for the class
    virtual void show() { cout << "Base::show" << endl; }      
    virtual void display() { cout << "Base::display" << endl; } 
};

class Derived : public Base {
public:
    int y;  // Additional member in the derived class

    // Overriding virtual functions
    void show() override { cout << "Derived::show" << endl; }   
    void display() override { cout << "Derived::display" << endl; } 
};

int main() {
    Base* bptr; // Base class pointer
    Derived d;  // Derived class object

    bptr = &d; // Base class pointer pointing to derived class object

    // Calls Derived class's overridden functions due to virtual mechanism

    // Internally:
    // 1. `bptr` is a pointer of type `Base*`, but it points to a Derived object.
    // 2. `bptr->vptr` points to the vtable of `Derived` class.
    // 3. The function `show()` is resolved at runtime via vptr lookup.
    // 4. Since Derived class overrides `show()`, `Derived::show()` is called.
    bptr->show();    

    // Internally:
    // 1. The `vptr` of `bptr` still points to the Derived class vtable.
    // 2. The function `display()` is resolved using the vtable.
    // 3. Since Derived class overrides `display()`, `Derived::display()` is called.
    bptr->display(); 

    return 0;
}
