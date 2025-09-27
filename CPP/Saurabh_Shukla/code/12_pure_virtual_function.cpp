#include <iostream>
using namespace std;

// Abstract Base Class (ABC) with a Pure Virtual Function
class Shape {
public:
    // Pure virtual function making this an abstract class
    virtual void draw() const = 0;

    // Virtual destructor for proper cleanup of derived class objects
    virtual ~Shape() {
        cout << "Shape Destructor Called" << endl;
    }
};

/* 
   Why Do We Need Pure Virtual Functions?
   --------------------------------------
   1. Enforce Interface Implementation:
      - Ensures that all derived classes implement a particular function, promoting polymorphism.
   
   2. Abstract Base Class (ABC):
      - Helps create a common interface for related classes, preventing instantiation of the base class.

   3. Runtime Polymorphism:
      - Enables dynamic binding of function calls, allowing different derived class implementations to be invoked at runtime.
*/

/* 
   Caveats of Pure Virtual Functions
   ---------------------------------
   1. Cannot Instantiate Abstract Classes:
      - A class with at least one pure virtual function cannot be instantiated directly.

   2. Must Be Overridden:
      - If a derived class does not override all pure virtual functions, it remains abstract and cannot be instantiated.

   3. Destructor Should Be Virtual:
      - If a base class has a pure virtual function, it must declare a virtual destructor to ensure proper cleanup of derived class objects.
*/

/*
   VPTR (Virtual Pointer) and VTABLE (Virtual Table)
   -------------------------------------------------
   - When a class has at least one virtual function, the compiler creates a **VTABLE** (Virtual Table).
   - Each object of such a class has a hidden **VPTR** (Virtual Pointer) that points to the VTABLE.
   - The VTABLE stores addresses of virtual functions, ensuring **dynamic dispatch**.
   - When a virtual function is called via a base class pointer, the **VPTR resolves the correct function at runtime**.
*/

// Derived Class 1: Circle
class Circle : public Shape {
public:
    void draw() const override {
        cout << "Drawing Circle" << endl;
    }

    ~Circle() {
        cout << "Circle Destructor Called" << endl;
    }
};

// Derived Class 2: Rectangle
class Rectangle : public Shape {
public:
    void draw() const override {
        cout << "Drawing Rectangle" << endl;
    }

    ~Rectangle() {
        cout << "Rectangle Destructor Called" << endl;
    }
};

int main() {
    // Polymorphic behavior using base class pointer
    Shape* s1 = new Circle();   // VPTR points to Circle's VTABLE
    Shape* s2 = new Rectangle(); // VPTR points to Rectangle's VTABLE

    /* 
       How VPTR & VTABLE Work in This Code
       -----------------------------------
       1. Base Class (`Shape`) has a **pure virtual function (`draw`)**, making it abstract.
       2. Derived Classes (`Circle` & `Rectangle`) override the `draw()` function.
       3. When creating objects dynamically (`Shape* s1 = new Circle();`), 
          each object gets a **hidden VPTR** pointing to its respective **VTABLE**.
       4. When calling `s1->draw();` or `s2->draw();`, the VPTR **dynamically resolves** 
          the function call at runtime using the **VTABLE**.
       5. Destructor is virtual, ensuring proper cleanup from derived to base.
    */

    // Calls are resolved dynamically using VPTR and VTABLE
    s1->draw(); // Calls Circle's draw()
    s2->draw(); // Calls Rectangle's draw()

    // Clean up memory
    delete s1; // Calls Circle's destructor, then Shape's destructor
    delete s2; // Calls Rectangle's destructor, then Shape's destructor

    return 0;
}
