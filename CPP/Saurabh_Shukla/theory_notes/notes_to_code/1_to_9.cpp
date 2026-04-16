/*****************************************************************************************
    C++ COMPLETE MASTER REFERENCE (SINGLE FILE)

    Covers EVERYTHING from your notes:

    1. C vs C++ (procedural vs OOP)
    2. Data types, variables, memory
    3. cin/cout vs printf/scanf
    4. Class, Object
    5. Encapsulation + Data Hiding
    6. Abstraction
    7. Inheritance
    8. Polymorphism (compile-time + runtime)
    9. Reference variables
    10. Compilation pipeline
    11. Memory model (stack vs heap)
    12. Interview traps (object slicing, vtable basics)

    ⚠️ Read comments carefully — they ARE the theory
******************************************************************************************/

#include <iostream>   // C++ I/O (cout, cin)
#include <cstdio>     // C I/O (printf, scanf)

using namespace std;

/*****************************************************************************************
    ===================== 1. DATA TYPES + VARIABLES =====================
******************************************************************************************/

void data_types_demo() {
    // PRIMARY TYPES
    int a = 10;
    char c = 'A';
    float f = 3.14f;
    double d = 3.14159;

    // SECONDARY TYPES
    int arr[3] = {1,2,3};     // array
    char str[] = "Hello";     // string (null terminated)

    // POINTER
    int *p = &a;

    cout << "\n[DATA TYPES]\n";
    cout << "int: " << a << ", address: " << &a << endl;
    cout << "char: " << c << endl;
    cout << "float: " << f << endl;
    cout << "double: " << d << endl;

    cout << "Pointer *p: " << *p << endl;

    // 🔥 CAVEATS:
    // sizeof gives compile-time info
    cout << "sizeof(int): " << sizeof(int) << endl;
}

/*****************************************************************************************
    ===================== 2. VARIABLES RULES =====================
******************************************************************************************/

void variable_rules() {
    int valid = 10;
    int _ok = 20;

    // int 2bad = 30; ❌ illegal

    cout << "\n[VARIABLE RULES]\n";
    cout << valid << ", " << _ok << endl;
}

/*****************************************************************************************
    ===================== 3. C vs C++ =====================
******************************************************************************************/

void procedural_vs_cpp() {
    int a = 5, b = 3;

    // C style
    printf("\n[C STYLE]\n");
    printf("Sum: %d\n", a + b);

    // C++ style
    cout << "[C++ STYLE]\n";
    cout << "Sum: " << a + b << endl;

    // 🔥 KEY DIFFERENCE:
    // printf → no type safety
    // cout   → type safe, operator overloaded
}

/*****************************************************************************************
    ===================== 4. ENCAPSULATION + DATA HIDING =====================
******************************************************************************************/

class BankAccount {
private:
    int balance;  // 🔒 hidden

public:
    BankAccount(int b) : balance(b) {}

    void deposit(int amt) {
        if (amt > 0) balance += amt;
    }

    void withdraw(int amt) {
        if (amt <= balance) balance -= amt;
    }

    int getBalance() const {
        return balance;
    }
};

/*
    🔥 Encapsulation = data + methods
    🔥 Data hiding = private access
*/

/*****************************************************************************************
    ===================== 5. ABSTRACTION =====================
******************************************************************************************/

class Shape {
public:
    virtual double area() = 0;  // pure virtual

    virtual ~Shape() {}  // MUST in polymorphic base classes
};

/*
    🔥 Abstraction = WHAT (area), not HOW
*/

/*****************************************************************************************
    ===================== 6. INHERITANCE =====================
******************************************************************************************/

class Rectangle : public Shape {
    double l, w;
public:
    Rectangle(double l, double w) : l(l), w(w) {}

    double area() override {
        return l * w;
    }
};

class Circle : public Shape {
    double r;
public:
    Circle(double r) : r(r) {}

    double area() override {
        return 3.14159 * r * r;
    }
};

/*
    🔥 "is-a" relationship
*/

/*****************************************************************************************
    ===================== 7. POLYMORPHISM =====================
******************************************************************************************/

// Compile-time (overloading)
int add(int a, int b) { return a + b; }
double add(double a, double b) { return a + b; }

// Runtime (virtual dispatch)
void printArea(Shape* s) {
    cout << "Area: " << s->area() << endl;
}

/*
    🔥 Runtime polymorphism uses vtable (hidden pointer in object)
*/

/*****************************************************************************************
    ===================== 8. OBJECT + CLASS =====================
******************************************************************************************/

void object_demo() {
    BankAccount acc(1000);

    acc.deposit(500);
    acc.withdraw(200);

    cout << "\n[OBJECT DEMO]\n";
    cout << "Balance: " << acc.getBalance() << endl;

    // acc.balance = 10000; ❌ not allowed
}

/*****************************************************************************************
    ===================== 9. INPUT / OUTPUT =====================
******************************************************************************************/

void io_demo() {
    int x, y;

    cout << "\n[INPUT OUTPUT]\n";
    cout << "Enter 2 numbers: ";
    cin >> x >> y;

    cout << "Sum: " << x + y << endl;

    // 🔥 cin uses >>
    // 🔥 cout uses <<
}

/*****************************************************************************************
    ===================== 10. REFERENCES =====================
******************************************************************************************/

void reference_demo() {
    int x = 10;
    int &ref = x;

    ref++;

    cout << "\n[REFERENCE]\n";
    cout << "x: " << x << ", ref: " << ref << endl;

    // 🔥 MUST initialize
    // 🔥 Cannot reseat
    // 🔥 Alias, not pointer
}

/*****************************************************************************************
    ===================== 11. MEMORY MODEL =====================
******************************************************************************************/

void memory_demo() {
    int stackVar = 10;              // stack
    int* heapVar = new int(20);     // heap

    cout << "\n[MEMORY]\n";
    cout << "Stack: " << stackVar << endl;
    cout << "Heap: " << *heapVar << endl;

    delete heapVar;

    // 🔥 stack = fast, limited
    // 🔥 heap = flexible, manual management
}

/*****************************************************************************************
    ===================== 12. OBJECT SLICING (IMPORTANT TRAP) =====================
******************************************************************************************/

class Base {
public:
    virtual void show() { cout << "Base\n"; }
};

class Derived : public Base {
public:
    void show() override { cout << "Derived\n"; }
};

void slicing_demo() {
    Derived d;
    Base b = d;  // ❌ slicing

    cout << "\n[SLICING]\n";
    b.show();    // Base

    Base* ptr = &d;
    ptr->show(); // Derived
}

/*****************************************************************************************
    ===================== 13. COMPILATION PIPELINE =====================
******************************************************************************************/

/*
    .cpp
      ↓ Preprocessor (#include)
      ↓ Expanded code (RAM only)
      ↓ Compiler → .o
      ↓ Linker (libraries)
      ↓ Executable (.exe / a.out)

    🔥 Header = declarations
    🔥 Library = definitions
*/

/*****************************************************************************************
    ===================== MAIN =====================
******************************************************************************************/

int main() {

    cout << "===== MASTER C++ PROGRAM =====\n";

    data_types_demo();
    variable_rules();
    procedural_vs_cpp();

    object_demo();

    Shape* s1 = new Rectangle(10, 5);
    Shape* s2 = new Circle(3);

    printArea(s1);
    printArea(s2);

    delete s1;
    delete s2;

    cout << "\n[POLYMORPHISM]\n";
    cout << add(2,3) << endl;
    cout << add(2.5,3.5) << endl;

    io_demo();
    reference_demo();
    memory_demo();
    slicing_demo();

    cout << "\n===== END =====\n";

    return 0;
}
