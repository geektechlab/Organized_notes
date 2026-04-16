#include <iostream>
using namespace std;

/*
===========================================================
SECTION 1: CLASS BASICS (Properties + Behavior)
===========================================================

- Class = blueprint
- Object = instance
- Members:
    - Data members (variables)
    - Member functions (behavior)
*/

class Car {
public:
    int speed;

    void drive() {
        cout << "Car driving at speed: " << speed << endl;
    }
};

/*
===========================================================
SECTION 2: INHERITANCE BASICS
===========================================================

Inheritance = "is-a" relationship

Derived class gets:
- ALL members of base class (BUT access rules apply)

Terminology:
- Base class / Parent / Superclass
- Derived class / Child / Subclass
*/

class SportsCar : public Car {  // public inheritance
public:
    void turbo() {
        cout << "Turbo boost enabled!" << endl;
    }
};

/*
===========================================================
CRITICAL INTERVIEW RULE (VERY IMPORTANT)
===========================================================

public inheritance:
    public    -> public
    protected -> protected
    private   -> NOT accessible

private inheritance:
    public    -> private
    protected -> private

protected inheritance:
    public    -> protected
    protected -> protected

DEFAULT:
- class => private inheritance
- struct => public inheritance
*/


/*
===========================================================
SECTION 3: TYPES OF INHERITANCE
===========================================================
*/

// 1. SINGLE INHERITANCE
class A {
public:
    void f() { cout << "A::f()" << endl; }
};

class B : public A {};

// 2. MULTILEVEL INHERITANCE
class C : public B {};

// 3. MULTIPLE INHERITANCE
class A1 {
public:
    void f1() { cout << "A1::f1()" << endl; }
};

class A2 {
public:
    void f2() { cout << "A2::f2()" << endl; }
};

class Multi : public A1, public A2 {};

// 4. HIERARCHICAL INHERITANCE
class Base {
public:
    void baseFunc() { cout << "Base function\n"; }
};

class Child1 : public Base {};
class Child2 : public Base {};


/*
===========================================================
SECTION 4: ACCESS SPECIFIERS
===========================================================

private:
    - Only inside same class

protected:
    - Same class + derived class

public:
    - Everywhere

CRITICAL INTERVIEW EDGE:
- Object cannot access protected members
- Derived class CAN access protected
*/

class AccessDemo {
private:
    int pri = 1;

protected:
    int pro = 2;

public:
    int pub = 3;

    void show() {
        cout << pri << " " << pro << " " << pub << endl;
    }
};

class DerivedAccess : public AccessDemo {
public:
    void test() {
        // cout << pri; ❌ ERROR (private)
        cout << pro << endl;  // ✅ OK
        cout << pub << endl;  // ✅ OK
    }
};


/*
===========================================================
SECTION 5: CONSTRUCTORS IN INHERITANCE
===========================================================

RULES:
1. Base constructor ALWAYS runs first
2. Then derived constructor runs
3. Order = inheritance order, NOT initializer list order

IMPORTANT:
- If base has NO default constructor → must call explicitly
*/

class Parent {
public:
    Parent() {
        cout << "Parent default constructor\n";
    }

    Parent(int x) {
        cout << "Parent parameter constructor: " << x << endl;
    }
};

class Child : public Parent {
public:
    Child() : Parent(10) {  // explicit call required
        cout << "Child constructor\n";
    }
};


/*
===========================================================
SECTION 6: DESTRUCTORS
===========================================================

RULES:
1. Reverse order of constructors
2. Child destroyed first, then parent
*/

class DParent {
public:
    ~DParent() {
        cout << "DParent destructor\n";
    }
};

class DChild : public DParent {
public:
    ~DChild() {
        cout << "DChild destructor\n";
    }
};


/*
===========================================================
SECTION 7: IS-A RELATIONSHIP
===========================================================

- Inheritance models "is-a"
- SportsCar IS-A Car

INTERVIEW TRICK:
❌ Wrong:
    Engine IS-A Car
✔ Correct:
    Car HAS-A Engine (composition)
*/


/*
===========================================================
SECTION 8: OBJECT POINTERS
===========================================================

- Pointer to object stores address of object
- Access via -> operator
*/

class Box {
    int l, b, h;

public:
    void setDimension(int x, int y, int z) {
        l = x; b = y; h = z;
    }

    void show() {
        cout << l << " " << b << " " << h << endl;
    }
};


/*
===========================================================
SECTION 9: THIS POINTER (CRITICAL)
===========================================================

- 'this' is implicit pointer
- Type: ClassName*
- Points to current object

IMPORTANT:
- Cannot reassign 'this'
- Used for chaining / disambiguation
*/

class ThisDemo {
    int x;

public:
    void setX(int x) {
        this->x = x;  // resolves shadowing
    }
};


/*
===========================================================
SECTION 10: STATIC VS INSTANCE FUNCTION
===========================================================

- Instance function:
    has 'this' pointer

- Static function:
    NO 'this'
    can only access static members
*/

class StaticDemo {
public:
    int x;

    static void foo() {
        // cout << x; ❌ ERROR
        cout << "Static function\n";
    }
};


/*
===========================================================
SECTION 11: POINTER TYPE SAFETY
===========================================================

- Pointer must match type
- C++ allows unsafe casts (but dangerous)

INTERVIEW TRICK:
*/

void pointerRules() {
    int x = 10;
    int* p = &x;

    // double* d = (double*)&x;  // compiles but unsafe
}


/*
===========================================================
SECTION 12: NEW / DELETE
===========================================================

- new → allocates memory (heap)
- delete → frees memory

INTERVIEW CAVEATS:
- delete vs delete[]
- memory leak if not freed
*/

void newDeleteDemo() {
    int* p = new int(5);

    delete p;   // correct

    int* arr = new int[5];
    delete[] arr;  // MUST use []
}


/*
===========================================================
SECTION 13: CONSTRUCTOR EDGE CASE (IMPORTANT)
===========================================================

If base has ONLY parameter constructor:

class A {
public:
    A(int x) {}
};

class B : public A {
public:
    B() {}  // ❌ ERROR → base not initialized
};

FIX:
B() : A(10) {}
*/


/*
===========================================================
SECTION 14: INTERVIEW TRICK QUESTIONS
===========================================================

Q1: Can object access protected member?
→ NO

Q2: Can derived class access private?
→ NO

Q3: Order of constructor?
→ Base → Derived

Q4: Order of destructor?
→ Derived → Base

Q5: What is default inheritance?
→ class = private

Q6: Does pointer to base call derived?
→ NO (unless virtual, out of scope here)

Q7: Can 'this' be null?
→ No (in standard usage)

Q8: Multiple inheritance ambiguity?
→ Yes (diamond problem, advanced)
*/


/*
===========================================================
MAIN FUNCTION (DEMO ALL)
===========================================================
*/

int main() {

    cout << "===== BASIC CLASS =====\n";
    Car c;
    c.speed = 100;
    c.drive();

    cout << "\n===== INHERITANCE =====\n";
    SportsCar sc;
    sc.speed = 200;
    sc.drive();
    sc.turbo();

    cout << "\n===== MULTIPLE INHERITANCE =====\n";
    Multi m;
    m.f1();
    m.f2();

    cout << "\n===== ACCESS SPECIFIER =====\n";
    DerivedAccess da;
    da.test();

    cout << "\n===== CONSTRUCTORS =====\n";
    Child ch;

    cout << "\n===== DESTRUCTORS =====\n";
    {
        DChild dc;
    } // scope ends → destructor called

    cout << "\n===== OBJECT POINTER =====\n";
    Box b;
    b.setDimension(1,2,3);
    b.show();

    Box* p = &b;
    p->setDimension(4,5,6);
    p->show();

    cout << "\n===== THIS POINTER =====\n";
    ThisDemo td;
    td.setX(10);

    cout << "\n===== NEW DELETE =====\n";
    newDeleteDemo();

    return 0;
}