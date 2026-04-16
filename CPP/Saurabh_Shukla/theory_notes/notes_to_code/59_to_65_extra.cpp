/*
===========================================================
    MASTER C++ FILE — INTERVIEW LEVEL (APPLE / ARM UK)
    Covers:
    - Exception Handling (deep)
    - Dynamic Constructor / Memory mgmt
    - Virtual Destructor & Polymorphism
    - Namespace (all caveats)
    - Nested Class & Composition
    - Header vs Library concepts (simulated)
    - STL overview (minimal but correct)
===========================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
===========================================================
1. EXCEPTION HANDLING (CORE + CAVEATS)
===========================================================

Key Rules:
- throw transfers control immediately (stack unwinding starts)
- Code after throw NEVER executes
- catch must follow try immediately (no code in between)
- Matching is STRICT (type must match or implicit conversion allowed)
- Order of catch matters (derived before base)
- catch(...) = catch-all (should be last)
- If no catch → std::terminate()

Interview traps:
- throw inside constructor/destructor
- rethrow using "throw;"
- exception slicing
*/

// Function throwing exception
void mayThrow(int x) {
    if (x == 0) throw 0;           // int
    if (x == 1) throw 1.5;         // double
    if (x == 2) throw string("error");
}

// Demonstrates rethrow
void rethrowExample() {
    try {
        throw 42;
    } catch (int e) {
        cout << "[rethrowExample] caught: " << e << endl;
        throw; // rethrow SAME exception
    }
}

void exceptionDemo() {
    cout << "\n--- Exception Demo ---\n";

    try {
        mayThrow(1);

        cout << "This line will NOT execute\n"; // unreachable
    }
    catch (double e) {
        cout << "Caught double: " << e << endl;
    }
    catch (int e) {
        cout << "Caught int: " << e << endl;
    }
    catch (...) {
        cout << "Caught unknown exception\n";
    }

    // Rethrow demo
    try {
        rethrowExample();
    } catch (int e) {
        cout << "[main] caught rethrown: " << e << endl;
    }

    /*
    ⚠️ Important:
    catch(int) will NOT catch double unless conversion allowed.
    Always order from specific → generic.
    */
}

/*
===========================================================
2. DYNAMIC CONSTRUCTOR (HEAP MEMORY)
===========================================================

- Constructor allocating memory using new
- Must free in destructor → else MEMORY LEAK

Interview trap:
- shallow copy → double free
- need Rule of 3/5 (copy ctor, assignment, destructor)
*/

class DynamicArray {
private:
    int size;
    int* data;

public:
    // Dynamic constructor
    DynamicArray(int n) : size(n) {
        data = new int[n]; // heap allocation
        cout << "Allocated " << n << " ints\n";
    }

    // Destructor
    ~DynamicArray() {
        delete[] data;
        cout << "Freed memory\n";
    }

    void set(int i, int val) {
        data[i] = val;
    }

    int get(int i) const {
        return data[i];
    }

    /*
    ⚠️ Missing copy constructor → dangerous
    This class is NOT safe for copying (interview catch)
    */
};

/*
===========================================================
3. VIRTUAL DESTRUCTOR + POLYMORPHISM
===========================================================

Critical Interview Topic

Problem:
Base* p = new Derived;
delete p;  // if destructor NOT virtual → leak

Rule:
If class has ANY virtual function → destructor MUST be virtual
*/

class Base {
public:
    Base() { cout << "Base ctor\n"; }

    // Make destructor virtual
    virtual ~Base() {
        cout << "Base dtor\n";
    }

    virtual void foo() {
        cout << "Base foo\n";
    }
};

class Derived : public Base {
private:
    int* resource;

public:
    Derived() {
        resource = new int[10];
        cout << "Derived ctor\n";
    }

    ~Derived() {
        delete[] resource;
        cout << "Derived dtor (freed resource)\n";
    }

    void foo() override {
        cout << "Derived foo\n";
    }
};

void polymorphismDemo() {
    cout << "\n--- Polymorphism Demo ---\n";

    Base* p = new Derived();

    p->foo(); // late binding

    delete p; 
    /*
    If Base destructor NOT virtual:
    - Derived destructor NOT called
    - MEMORY LEAK
    */
}

/*
===========================================================
4. NAMESPACE (ALL EDGE CASES)
===========================================================

- Avoid name conflicts
- Can be extended across files
- No object creation
- Can alias
- Can use "using"

Interview traps:
- using namespace std; in header → BAD
*/

namespace MySpace {
    int x = 10;

    void func() {
        cout << "MySpace::func\n";
    }

    class A {
    public:
        void hello() {
            cout << "Hello from MySpace::A\n";
        }
    };
}

// Extension of namespace (as if in another file)
namespace MySpace {
    int y = 20;
}

void namespaceDemo() {
    cout << "\n--- Namespace Demo ---\n";

    cout << MySpace::x << endl;

    // Alias
    namespace ms = MySpace;

    cout << ms::y << endl;

    ms::func();

    ms::A obj;
    obj.hello();
}

/*
===========================================================
5. NESTED CLASS + COMPOSITION
===========================================================

- Class inside class
- No special access unless declared friend
- Outer does NOT get automatic access

Composition:
- "has-a" relationship
*/

class Student {
private:
    int roll;
    string name;

    // Nested class
    class Address {
    private:
        int houseNo;
        string city;

    public:
        void set(int h, string c) {
            houseNo = h;
            city = c;
        }

        void show() {
            cout << "Address: " << houseNo << ", " << city << endl;
        }
    };

    Address addr; // composition

public:
    void setData(int r, string n, int h, string c) {
        roll = r;
        name = n;
        addr.set(h, c);
    }

    void show() {
        cout << "Student: " << roll << ", " << name << endl;
        addr.show();
    }
};

void nestedClassDemo() {
    cout << "\n--- Nested Class Demo ---\n";

    Student s;
    s.setData(1, "Rahul", 101, "Ahmedabad");
    s.show();
}

/*
===========================================================
6. HEADER vs LIBRARY (SIMULATED)
===========================================================

Header:
- Declarations

Library:
- Definitions

In real:
.h → declarations
.cpp → definitions

Here we simulate separation conceptually.
*/

/*
===========================================================
7. STL OVERVIEW (INTERVIEW LEVEL BASICS)
===========================================================

3 pillars:
- Containers (vector, list, map)
- Algorithms (sort, find)
- Iterators

Important:
- Iterators = generalized pointers
*/

void stlDemo() {
    cout << "\n--- STL Demo ---\n";

    vector<int> v = {5, 2, 9, 1};

    sort(v.begin(), v.end());

    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    auto it = find(v.begin(), v.end(), 5);
    if (it != v.end()) {
        cout << "Found 5\n";
    }
}

/*
===========================================================
MAIN DRIVER
===========================================================
*/

int main() {

    exceptionDemo();

    {
        DynamicArray arr(5);
        arr.set(0, 100);
        cout << arr.get(0) << endl;
    } // destructor called here

    polymorphismDemo();

    namespaceDemo();

    nestedClassDemo();

    stlDemo();

    cout << "\n--- END ---\n";

    return 0;
}