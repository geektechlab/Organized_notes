#include <iostream>
using namespace std;

/*
============================================================
SECTION 1: STATIC vs DYNAMIC MEMORY
============================================================
*/

// Global/static memory (allocated at compile time / static storage)
int global_var = 100;

void static_memory_demo() {
    // Stack allocation (automatic storage)
    int x = 10;        // allocated at runtime, but size known at compile time
    float y = 20.5;

    cout << "[STATIC] x: " << x << ", y: " << y << endl;
}

/*
============================================================
SECTION 2: DYNAMIC MEMORY (new/delete)
============================================================
*/

void dynamic_memory_demo() {
    // Single object allocation
    int* p = new int(42);   // heap allocation
    cout << "[DMA] *p = " << *p << endl;

    delete p;  // MUST delete

    // Array allocation
    int n = 5;
    int* arr = new int[n];  // uninitialized

    for (int i = 0; i < n; i++) arr[i] = i * 10;

    cout << "[DMA ARRAY] ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;

    delete[] arr;  // MUST use delete[]
}

/*
============================================================
SECTION 3: MEMORY LEAK + DANGLING POINTER + DOUBLE DELETE
============================================================
*/

void memory_pitfalls() {
    int* p = new int(10);

    // Memory leak (lost reference)
    p = new int(20);   // OLD memory lost → leak

    // Correct way:
    int* q = new int(30);
    delete q;
    q = nullptr; // avoid dangling pointer

    // Dangling pointer example
    int* d = new int(50);
    delete d;
    // *d = 60; // ❌ UB (dangling)

    // Double delete
    int* dd = new int(70);
    delete dd;
    // delete dd; // ❌ UB
}

/*
============================================================
SECTION 4: FUNCTION OVERLOADING
============================================================
*/

class OverloadDemo {
public:
    void func(int x) {
        cout << "func(int): " << x << endl;
    }

    void func(double x) {
        cout << "func(double): " << x << endl;
    }

    // NOTE:
    // func(int) and func(int&) are NOT overloads
};

/*
============================================================
SECTION 5: METHOD OVERRIDING vs HIDING
============================================================
*/

class Base {
public:
    void f1() {
        cout << "Base::f1 (non-virtual)" << endl;
    }

    virtual void f2() {
        cout << "Base::f2 (virtual)" << endl;
    }

    virtual void f3() {
        cout << "Base::f3 (virtual)" << endl;
    }

    void f4() {
        cout << "Base::f4()" << endl;
    }

    void f4(int x) {
        cout << "Base::f4(int): " << x << endl;
    }
};

class Derived : public Base {
public:
    // Overriding (signature must match exactly)
    void f2() override {
        cout << "Derived::f2 (override)" << endl;
    }

    void f3() override {
        cout << "Derived::f3 (override)" << endl;
    }

    // Hiding (different signature → hides ALL base versions)
    void f4(int x) {
        cout << "Derived::f4(int): " << x << endl;
    }

    // Uncomment to fix hiding:
    // using Base::f4;
};

/*
============================================================
SECTION 6: EARLY vs LATE BINDING
============================================================
*/

void binding_demo() {
    Derived d;
    Base* ptr = &d;

    cout << "\n--- Binding Demo ---\n";

    ptr->f1(); // EARLY binding → Base::f1 (non-virtual)

    ptr->f2(); // LATE binding → Derived::f2
    ptr->f3(); // LATE binding → Derived::f3

    ptr->f4();     // EARLY binding → Base::f4()
    ptr->f4(10);   // EARLY binding → Base::f4(int)

    // NOTE: Derived::f4(int) is hidden, not overriding
}

/*
============================================================
SECTION 7: VPTR + VTABLE (INTERVIEW GOLD)
============================================================

IMPORTANT:
- Each object of class with virtual function has hidden pointer: vptr
- vptr → points to vtable
- vtable = array of function pointers
- One vtable per class (not per object)
- One vptr per object
*/

class VTableDemo {
public:
    virtual void foo() {
        cout << "VTableDemo::foo" << endl;
    }

    virtual void bar() {
        cout << "VTableDemo::bar" << endl;
    }
};

class VTableDerived : public VTableDemo {
public:
    void foo() override {
        cout << "VTableDerived::foo" << endl;
    }
};

/*
============================================================
SECTION 8: PURE VIRTUAL + ABSTRACT CLASS
============================================================
*/

class Abstract {
public:
    virtual void mustImplement() = 0;  // pure virtual

    void normalFunc() {
        cout << "Abstract::normalFunc" << endl;
    }

    virtual ~Abstract() {} // IMPORTANT: always virtual destructor
};

// Cannot instantiate Abstract
// Abstract a; ❌

class Concrete : public Abstract {
public:
    void mustImplement() override {
        cout << "Concrete::mustImplement" << endl;
    }
};

/*
============================================================
SECTION 9: BASE POINTER → DERIVED OBJECT
============================================================
*/

void polymorphism_demo() {
    Abstract* obj = new Concrete();

    obj->mustImplement();  // Late binding
    obj->normalFunc();     // Normal call

    delete obj;
}

/*
============================================================
SECTION 10: CRITICAL INTERVIEW PITFALLS
============================================================
*/

class NoVirtualDestructor {
public:
    ~NoVirtualDestructor() {
        cout << "Base destructor\n";
    }
};

class Child : public NoVirtualDestructor {
public:
    ~Child() {
        cout << "Child destructor\n";
    }
};

void destructor_pitfall() {
    NoVirtualDestructor* obj = new Child();

    delete obj; // ❌ Only base destructor called → memory/resource leak
}

/*
============================================================
MAIN
============================================================
*/

int main() {
    cout << "===== STATIC MEMORY =====\n";
    static_memory_demo();

    cout << "\n===== DYNAMIC MEMORY =====\n";
    dynamic_memory_demo();

    cout << "\n===== MEMORY PITFALLS =====\n";
    memory_pitfalls();

    cout << "\n===== FUNCTION OVERLOADING =====\n";
    OverloadDemo od;
    od.func(10);
    od.func(10.5);

    cout << "\n===== INHERITANCE + BINDING =====\n";
    binding_demo();

    cout << "\n===== POLYMORPHISM =====\n";
    polymorphism_demo();

    cout << "\n===== DESTRUCTOR PITFALL =====\n";
    destructor_pitfall();

    return 0;
}
