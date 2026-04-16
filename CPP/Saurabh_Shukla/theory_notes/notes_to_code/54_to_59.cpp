#include <iostream>
#include <cstring>
#include <memory>

using namespace std;

/*
============================================================
SECTION 1: INITIALIZER LIST (CRITICAL)
============================================================

RULES:
1. const members MUST be initialized via initializer list
2. reference members MUST be initialized via initializer list
3. Order of initialization = declaration order (NOT list order)
4. Avoid assignment inside constructor body for performance
*/

class InitDemo {
private:
    const int cx;     // must be initialized
    int& ref;         // must be initialized
    int a;
    int b;

public:
    // Correct usage
    InitDemo(int x, int& r)
        : cx(x), ref(r), a(0), b(0)  // initialization happens BEFORE body
    {
        // assignment happens AFTER initialization (less efficient)
        a = 10;
        b = 20;
    }

    void print() const {
        cout << "InitDemo: cx=" << cx << " ref=" << ref
             << " a=" << a << " b=" << b << endl;
    }
};

/*
============================================================
SECTION 2: SHALLOW COPY (DEFAULT BEHAVIOR)
============================================================

Default copy constructor / assignment:
- Copies memory bitwise
- POINTERS ARE COPIED, NOT DATA THEY POINT TO

Danger:
- Two objects point to same memory
- Double free
*/

class Shallow {
public:
    int* p;

    Shallow(int val) {
        p = new int(val);
    }

    // NO custom copy constructor -> shallow copy

    ~Shallow() {
        delete p; // 💀 dangerous if copied
    }
};

/*
============================================================
SECTION 3: DEEP COPY + RULE OF 5 (IMPORTANT)
============================================================

If your class manages resource (heap/file/socket):
YOU MUST IMPLEMENT:
- Destructor
- Copy constructor
- Copy assignment
- Move constructor
- Move assignment

Otherwise → UB, leaks, crashes
*/

class Deep {
private:
    int* p;

public:
    // Constructor
    Deep(int val) {
        p = new int(val);
    }

    // Destructor
    ~Deep() {
        delete p;
    }

    // Copy Constructor (DEEP COPY)
    Deep(const Deep& other) {
        p = new int(*other.p);  // allocate new memory
    }

    // Copy Assignment
    Deep& operator=(const Deep& other) {
        if (this == &other) return *this;

        delete p; // free old
        p = new int(*other.p);

        return *this;
    }

    // Move Constructor
    Deep(Deep&& other) noexcept {
        p = other.p;
        other.p = nullptr;
    }

    // Move Assignment
    Deep& operator=(Deep&& other) noexcept {
        if (this == &other) return *this;

        delete p;

        p = other.p;
        other.p = nullptr;

        return *this;
    }

    void print() const {
        cout << "Deep value = " << *p << endl;
    }
};

/*
============================================================
SECTION 4: WHY SHALLOW COPY IS DANGEROUS
============================================================
*/

void shallow_demo() {
    cout << "\n--- SHALLOW COPY DEMO ---\n";

    Shallow s1(10);
    Shallow s2 = s1; // copy constructor (shallow)

    // Both point to SAME memory
    cout << "s1.p = " << s1.p << " s2.p = " << s2.p << endl;

    // When destructors run → double delete → UB
}

/*
============================================================
SECTION 5: DEEP COPY SAFE DEMO
============================================================
*/

void deep_demo() {
    cout << "\n--- DEEP COPY DEMO ---\n";

    Deep d1(20);
    Deep d2 = d1;  // deep copy

    d1.print();
    d2.print();
}

/*
============================================================
SECTION 6: PRIMITIVE → CLASS CONVERSION
============================================================

Using SINGLE ARG constructor

⚠️ Interview trap:
- Mark constructor "explicit" to avoid implicit conversions
*/

class Complex {
private:
    int a, b;

public:
    Complex() : a(0), b(0) {}

    // conversion constructor
    explicit Complex(int x) : a(x), b(0) {}

    Complex(int x, int y) : a(x), b(y) {}

    void print() const {
        cout << "Complex(" << a << "," << b << ")" << endl;
    }
};

/*
============================================================
SECTION 7: CLASS → PRIMITIVE CONVERSION
============================================================

Using casting operator
*/

class Number {
private:
    int val;

public:
    Number(int v) : val(v) {}

    // conversion operator
    operator int() const {
        return val;
    }
};

/*
============================================================
SECTION 8: CLASS → CLASS CONVERSION
============================================================

Approaches:
1. Conversion constructor
2. Conversion operator
*/

class Product;

class Item {
private:
    int a, b;

public:
    Item() : a(0), b(0) {}

    Item(int x, int y) : a(x), b(y) {}

    // Conversion constructor
    Item(const Product& p);

    void print() const {
        cout << "Item(" << a << "," << b << ")" << endl;
    }
};

class Product {
private:
    int m, n;

public:
    Product(int x, int y) : m(x), n(y) {}

    int getM() const { return m; }
    int getN() const { return n; }

    // Alternative: conversion operator
    operator Item() const {
        return Item(m, n);
    }
};

// Define conversion constructor
Item::Item(const Product& p) {
    a = p.getM();
    b = p.getN();
}

/*
============================================================
SECTION 9: RAII (APPLE/ARM LEVEL EXPECTATION)
============================================================

Never manually manage memory if possible
Use smart pointers
*/

class RAIIExample {
private:
    unique_ptr<int> p;

public:
    RAIIExample(int val)
        : p(make_unique<int>(val)) {}

    void print() const {
        cout << "RAII value = " << *p << endl;
    }
};

/*
============================================================
SECTION 10: MAIN DEMO
============================================================
*/

int main() {

    cout << "===== INITIALIZER LIST =====\n";
    int x = 5;
    InitDemo obj(10, x);
    obj.print();

    cout << "\n===== SHALLOW COPY (DANGEROUS) =====\n";
    shallow_demo();

    cout << "\n===== DEEP COPY (SAFE) =====\n";
    deep_demo();

    cout << "\n===== PRIMITIVE -> CLASS =====\n";
    Complex c1(5);  // explicit prevents: Complex c1 = 5;
    c1.print();

    cout << "\n===== CLASS -> PRIMITIVE =====\n";
    Number n(42);
    int val = n;  // conversion operator
    cout << "Converted int = " << val << endl;

    cout << "\n===== CLASS -> CLASS =====\n";
    Product p(3, 4);

    Item i1 = p;   // conversion operator
    i1.print();

    Item i2(p);    // conversion constructor
    i2.print();

    cout << "\n===== RAII =====\n";
    RAIIExample r(100);
    r.print();

    return 0;
}