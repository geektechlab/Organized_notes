#include <iostream>
using namespace std;

/*
===========================================================
🔥 SECTION 1: OBJECT MEMORY MODEL (VERY IMPORTANT)
===========================================================

- Object = contiguous memory of its data members
- Member functions are NOT part of object memory
- sizeof(object) = sum of members + padding

INTERVIEW TRAP:
"Does class contain functions in memory?"
→ NO (functions live in text segment)
*/

class MemoryLayout {
    int a;     // 4 bytes
    char b;    // 1 byte + padding
public:
    void f() {}
};

/*
Try:
cout << sizeof(MemoryLayout);

Expected:
- Likely 8 (due to padding)
*/


/*
===========================================================
🔥 SECTION 2: INHERITANCE MEMORY LAYOUT
===========================================================

Derived object layout:
[ Base part ][ Derived part ]

NO duplication of base members.
*/

class Base {
public:
    int x;
};

class Derived : public Base {
public:
    int y;
};

/*
Memory:
Derived:
| x (Base) | y (Derived) |

INTERVIEW TRICK:
sizeof(Derived) != sizeof(Base) + sizeof(Derived only)
→ due to padding
*/


/*
===========================================================
🔥 SECTION 3: OBJECT SLICING (VERY IMPORTANT)
===========================================================

When derived is assigned to base BY VALUE,
derived part is LOST.

This is silent and dangerous.
*/

class SliceBase {
public:
    int a = 10;
};

class SliceDerived : public SliceBase {
public:
    int b = 20;
};

void slicingDemo() {
    SliceDerived d;
    SliceBase b = d;  // ❗ SLICING happens

    cout << b.a << endl; // OK
    // cout << b.b; ❌ not accessible
}

/*
INTERVIEW:
"How to avoid slicing?"
→ Use pointers or references
*/


/*
===========================================================
🔥 SECTION 4: POINTER POLYMORPHISM (NO VIRTUAL HERE)
===========================================================

Base pointer → Derived object

BUT:
- Only base members accessible
- Static binding (since no virtual)
*/

class PolyBase {
public:
    void show() {
        cout << "Base show\n";
    }
};

class PolyDerived : public PolyBase {
public:
    void show() {
        cout << "Derived show\n";
    }
};

void polyDemo() {
    PolyDerived d;
    PolyBase* p = &d;

    p->show(); // ❗ calls Base::show (STATIC binding)
}

/*
INTERVIEW TRICK:
"Why not derived function?"
→ No virtual → compile-time binding
*/


/*
===========================================================
🔥 SECTION 5: MULTIPLE INHERITANCE MEMORY
===========================================================

Object layout:
[ A1 ][ A2 ][ Derived ]

Each base has its own subobject
*/

class A1 {
public:
    int x;
};

class A2 {
public:
    int y;
};

class MultiDerived : public A1, public A2 {
public:
    int z;
};

/*
INTERVIEW TRICK:
sizeof(MultiDerived) = sum + padding

Also:
Pointer adjustment required internally
*/


/*
===========================================================
🔥 SECTION 6: AMBIGUITY IN MULTIPLE INHERITANCE
===========================================================
*/

class AmbA {
public:
    void f() { cout << "AmbA\n"; }
};

class AmbB {
public:
    void f() { cout << "AmbB\n"; }
};

class AmbC : public AmbA, public AmbB {};

void ambiguityDemo() {
    AmbC obj;

    // obj.f(); ❌ ambiguous

    obj.AmbA::f(); // ✅ must specify
}

/*
INTERVIEW:
"How to resolve?"
→ Scope resolution OR virtual inheritance (out of scope)
*/


/*
===========================================================
🔥 SECTION 7: CONSTRUCTOR ORDER (DEEP)
===========================================================

ORDER:
1. Base classes (LEFT → RIGHT)
2. Then derived

NOT initializer list order!
*/

class B1 {
public:
    B1() { cout << "B1\n"; }
};

class B2 {
public:
    B2() { cout << "B2\n"; }
};

class D : public B1, public B2 {
public:
    D() : B2(), B1() { // ❗ misleading
        cout << "D\n";
    }
};

/*
OUTPUT:
B1
B2
D

INTERVIEW TRAP:
"Initializer list order matters?"
→ NO (declaration order matters)
*/


/*
===========================================================
🔥 SECTION 8: DESTRUCTOR + POLYMORPHISM TRAP
===========================================================

(Still within topic — destructor behavior)

If deleting via base pointer WITHOUT virtual destructor:
→ Undefined behavior

NOTE: virtual not in your notes, but THIS is a famous trap
*/

class NoVirtualBase {
public:
    ~NoVirtualBase() {
        cout << "Base destructor\n";
    }
};

class NoVirtualDerived : public NoVirtualBase {
public:
    ~NoVirtualDerived() {
        cout << "Derived destructor\n";
    }
};

void destructorTrap() {
    NoVirtualBase* p = new NoVirtualDerived;
    delete p;  // ❗ Only base destructor called (UB)
}


/*
===========================================================
🔥 SECTION 9: THIS POINTER DEEP TRAPS
===========================================================

- this = pointer to current object
- type = ClassName*

INTERVIEW EDGE:
- const member function → this is const pointer
*/

class ThisAdvanced {
    int x;

public:
    void set(int x) {
        this->x = x;
    }

    ThisAdvanced* getThis() {
        return this;  // chaining possible
    }
};


/*
===========================================================
🔥 SECTION 10: OBJECT POINTER EDGE CASE
===========================================================

- Pointer can be NULL
- Dereferencing NULL → crash

INTERVIEW TRICK:
"Can you call member function on NULL?"

Answer:
Depends — if function doesn't access data → MAY work
(but undefined behavior)
*/

class NullTest {
public:
    void f() {
        cout << "Hello\n";
    }
};

void nullCall() {
    NullTest* p = nullptr;

    // p->f(); ❗ UB (but may print)
}


/*
===========================================================
🔥 SECTION 11: NEW / DELETE EDGE CASES
===========================================================

1. delete vs delete[]
2. double delete → crash
3. dangling pointer
*/

void memoryBugs() {
    int* p = new int(5);

    delete p;
    // delete p; ❗ double free

    int* q = new int(10);
    delete q;

    // cout << *q; ❗ dangling pointer
}


/*
===========================================================
🔥 SECTION 12: FINAL INTERVIEW RAPID FIRE
===========================================================

Q: sizeof(empty class)?
→ 1 (minimum size)

Q: Can constructor be inherited?
→ No (but base constructor called)

Q: Can private members be inherited?
→ Yes (but NOT accessible)

Q: Can we access protected via object?
→ NO

Q: Why pointer to base works?
→ Base subobject exists inside derived

Q: Memory of object vs function?
→ Object = data only
*/


/*
===========================================================
MAIN
===========================================================
*/

int main() {

    cout << "===== MEMORY LAYOUT =====\n";
    cout << sizeof(MemoryLayout) << endl;

    cout << "\n===== SLICING =====\n";
    slicingDemo();

    cout << "\n===== POLYMORPHISM (STATIC) =====\n";
    polyDemo();

    cout << "\n===== MULTIPLE INHERITANCE =====\n";
    MultiDerived m;
    m.x = 1;
    m.y = 2;
    m.z = 3;

    cout << "\n===== AMBIGUITY =====\n";
    ambiguityDemo();

    cout << "\n===== CONSTRUCTOR ORDER =====\n";
    D obj;

    cout << "\n===== DESTRUCTOR TRAP =====\n";
    destructorTrap();

    cout << "\n===== THIS POINTER =====\n";
    ThisAdvanced t;
    t.set(10);
    cout << (t.getThis() == &t) << endl;

    cout << "\n===== NULL CALL =====\n";
    // nullCall(); // commented (danger)

    cout << "\n===== MEMORY BUGS =====\n";
    memoryBugs();

    return 0;
}