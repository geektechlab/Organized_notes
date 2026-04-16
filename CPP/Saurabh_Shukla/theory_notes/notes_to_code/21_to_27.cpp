#include <iostream>
using namespace std;

/*
=====================================================================
🔥 MASTER FILE: C++ OOP + STATIC + CONSTRUCTORS + DESTRUCTORS
=====================================================================

If you understand EVERYTHING in this file → you are interview-ready
for Apple / Arm / Low-level systems roles (for these topics).

COVERS:
- Instance vs Static variables
- Static local variables
- Static member variables (definition rule)
- Static member functions (limitations)
- Constructors (default, param, single arg, overload)
- Copy constructor (correct vs WRONG)
- Destructor
- Object lifecycle
- Hidden compiler behavior
- ALL interview traps

=====================================================================
*/


class Complex {
private:
    /*
    ================================================================
    INSTANCE VARIABLES (OBJECT STATE)
    ================================================================

    - Each object has its OWN copy
    - Stored inside object memory
    */

    int a, b;

    /*
    ================================================================
    STATIC MEMBER VARIABLES (CLASS STATE)
    ================================================================

    - Shared across ALL objects
    - Only ONE copy exists in program
    - NOT part of object memory layout

    🔥 INTERVIEW TRAP:
    sizeof(object) DOES NOT include static members
    */

    static int objectCount;
    static float roi;

public:

    /*
    ================================================================
    🔥 CONSTRUCTORS
    ================================================================

    Rules:
    - Same name as class
    - No return type (NOT even void)
    - Called automatically
    - Cannot be static
    */

    // DEFAULT CONSTRUCTOR
    Complex() {
        a = 0;
        b = 0;
        objectCount++;

        cout << "[Default Constructor]\n";
    }

    // PARAMETERIZED CONSTRUCTOR
    Complex(int x, int y) {
        a = x;
        b = y;
        objectCount++;

        cout << "[Parameterized Constructor]\n";
    }

    /*
    ================================================================
    🔥 SINGLE ARG CONSTRUCTOR (VERY IMPORTANT)
    ================================================================

    ⚠️ INTERVIEW TRAP:
    This allows implicit conversion

        Complex c = 5;

    👉 To prevent:
        use 'explicit' keyword

        explicit Complex(int x)

    */

    Complex(int x) {
        a = x;
        b = 0;
        objectCount++;

        cout << "[Single Param Constructor]\n";
    }

    /*
    ================================================================
    🔥 COPY CONSTRUCTOR
    ================================================================

    CORRECT:
        Complex(const Complex& other)

    ❌ WRONG:
        Complex(Complex other)

    WHY WRONG?
    → Passing by value → needs copy → recursion → crash
    */

    Complex(const Complex& other) {
        a = other.a;
        b = other.b;
        objectCount++;

        cout << "[Copy Constructor]\n";
    }

    /*
    ================================================================
    🔥 DESTRUCTOR
    ================================================================

    - Name: ~ClassName()
    - No return type
    - No arguments
    - Cannot be overloaded
    - Cannot be static

    🔥 INTERVIEW:
    - Called in REVERSE order of creation
    - Automatically invoked
    */

    ~Complex() {
        objectCount--;

        cout << "[Destructor] (" << a << "," << b << ")\n";
    }

    /*
    ================================================================
    INSTANCE MEMBER FUNCTIONS (OBJECT BEHAVIOR)
    ================================================================
    */

    void setData(int x, int y) {
        a = x;
        b = y;
    }

    void print() const {
        cout << "Complex(" << a << ", " << b << ")\n";
    }

    /*
    ================================================================
    🔥 STATIC MEMBER FUNCTIONS
    ================================================================

    Rules:
    - Called WITHOUT object
    - Can access ONLY static members
    - NO 'this' pointer

    ❌ Cannot access:
        a, b (instance variables)
    */

    static void setROI(float r) {
        roi = r;
    }

    static float getROI() {
        return roi;
    }

    static int getObjectCount() {
        return objectCount;
    }

    /*
    ================================================================
    INSTANCE CAN ACCESS STATIC
    ================================================================
    */

    void showROI() {
        cout << "ROI = " << roi << endl;
    }
};


/*
=====================================================================
🔥 STATIC MEMBER VARIABLE DEFINITION (MANDATORY)
=====================================================================

- Declaration inside class → NO memory allocated
- Definition outside class → memory allocated

❌ If missing → LINKER ERROR
*/

int Complex::objectCount = 0;
float Complex::roi = 0.0f;


/*
=====================================================================
🔥 STATIC LOCAL VARIABLE
=====================================================================

- Created ONLY ONCE
- Lifetime = entire program
- Scope = function

🔥 DIFFERENCE:
static int x → persists
int y → recreated every call
*/

void staticLocalDemo() {
    static int x = 0;
    int y = 0;

    x++;
    y++;

    cout << "[StaticLocal] x=" << x << " y=" << y << endl;
}


/*
=====================================================================
🔥 OBJECT LIFETIME
=====================================================================

- Stack object → destroyed automatically
- Destructor called at scope end
*/

void lifecycleDemo() {
    cout << "Entering function\n";

    Complex temp(10, 20);

    cout << "Leaving function\n";

    // Destructor will be called here automatically
}


/*
=====================================================================
🔥 MAIN: ALL TEST CASES
=====================================================================
*/

int main() {

    cout << "===== STATIC LOCAL =====\n";
    staticLocalDemo();
    staticLocalDemo();
    staticLocalDemo();

    /*
    EXPECT:
    x = 1,2,3 (persistent)
    y = 1,1,1 (recreated)
    */

    cout << "\n===== OBJECT CREATION =====\n";

    Complex c1;        // default
    Complex c2(3, 4);  // parameterized
    Complex c3(5);     // single param

    /*
    ================================================================
    🔥 DIRECT vs COPY INITIALIZATION
    ================================================================

    Both call constructor (NOT assignment)
    */

    Complex c4 = c2; // copy constructor

    cout << "\n===== PRINT =====\n";
    c1.print();
    c2.print();
    c3.print();
    c4.print();

    /*
    ================================================================
    🔥 STATIC VARIABLE USAGE
    ================================================================
    */

    cout << "\n===== STATIC MEMBER =====\n";

    Complex::setROI(7.5f);

    c1.showROI();
    c2.showROI();

    cout << "ROI via class: " << Complex::getROI() << endl;

    /*
    ================================================================
    🔥 OBJECT COUNT (CLASS LEVEL STATE)
    ================================================================
    */

    cout << "\nTotal Objects: " << Complex::getObjectCount() << endl;

    /*
    ================================================================
    🔥 LIFECYCLE DEMO
    ================================================================
    */

    cout << "\n===== LIFECYCLE =====\n";
    lifecycleDemo();

    /*
    ================================================================
    🔥 SIZEOF TRAP
    ================================================================

    Static variables NOT included
    */

    cout << "\nSize of object: " << sizeof(c1) << endl;

    /*
    ================================================================
    🔥 IMPLICIT CONVERSION TRAP
    ================================================================

    Complex c5 = 10;

    This works due to single-arg constructor

    To prevent:
        use 'explicit'
    */

    Complex c5 = 10;
    c5.print();

    cout << "\n===== END =====\n";

    return 0;
}