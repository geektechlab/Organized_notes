#include <iostream>
#include <cstring>
using namespace std;

/*========================================================
==================== CORE MENTAL MODEL ====================

C++ execution model (IMPORTANT for interviews):

1. Code → compiled → functions resolved (compile-time)
2. Stack → function calls, local variables
3. Data segment → static variables
4. Heap → dynamic allocation (new/delete)

Key idea:
- "What happens at compile-time vs runtime?" → BIG interview theme

========================================================*/


/*========================================================
1. FUNCTION DECLARATION / DEFINITION / CALL
========================================================*/

// Forward declaration required if function is defined later
// Compiler must know signature before usage
void declaredLater();

void caller()
{
    // Without declaration → COMPILE ERROR
    declaredLater();
}

// Definition
void declaredLater()
{
    cout << "[Function] Declaration vs Definition works\n";
}


/*========================================================
2. INLINE FUNCTION
========================================================*/
/*
INLINE = request to compiler to replace function call with body

IMPORTANT CAVEATS:
- NOT guaranteed (compiler decides)
- NOT used if:
    • function too big
    • recursion
    • virtual functions
- Reduces function call overhead BUT increases code size

Used for:
- Small, frequently called functions
*/
inline int inlineAdd(int a, int b)
{
    return a + b;
}


/*========================================================
3. DEFAULT ARGUMENTS
========================================================*/
/*
RULES:
1. Must be from RIGHT side
   ❌ int f(int a=0, int b); → ERROR
   ✅ int f(int a, int b=0);

2. Bound at COMPILE TIME (not runtime)

3. Avoid repeating defaults in multiple declarations

4. Works like function overloading internally
*/
int sum(int a, int b, int c = 0)
{
    return a + b + c;
}


/*========================================================
4. FUNCTION OVERLOADING
========================================================*/
/*
Compile-time polymorphism

RULES:
- Different parameter list required
- Return type alone NOT sufficient

TRAPS:
- Ambiguity due to type promotion
*/
int func(int x)
{
    cout << "int version\n";
    return x;
}

double func(double x)
{
    cout << "double version\n";
    return x;
}


/*========================================================
5. TYPE PROMOTION / AMBIGUITY
========================================================*/
/*
C++ prefers:
1. Exact match
2. Promotion (char → int)
3. Conversion

TRAP:
promo('a') → calls int version (not char version if absent)
*/
void promo(int x)
{
    cout << "int called\n";
}

void promo(double x)
{
    cout << "double called\n";
}


/*========================================================
6. STRUCT MEMORY + ARRAY BEHAVIOR
========================================================*/
/*
STRUCT = collection of variables

KEY POINTS:
- Stored CONTIGUOUSLY in memory
- Array inside struct:
    ❌ cannot assign directly
    ✅ must use strcpy

- Struct does NOT allocate memory until variable created
*/
struct Book
{
    int id;
    char title[20];   // FIXED array → not assignable
    float price;
};


/*========================================================
7. PASSING STRUCT (VALUE vs REFERENCE)
========================================================*/
/*
VALUE:
- Copy created
- Changes NOT reflected

REFERENCE:
- No copy
- Changes reflected

INTERVIEW TIP:
- Large structs → always pass by reference
*/
void modifyByValue(Book b)
{
    b.id = 999; // no effect outside
}

void modifyByRef(Book &b)
{
    b.id = 888; // modifies original
}


/*========================================================
8. RETURNING STRUCT
========================================================*/
/*
Older C++ → expensive copy

Modern C++:
- NRVO (Named Return Value Optimization)
- Copy elision → avoids copy

INTERVIEW:
"Is returning struct expensive?"
→ Answer: Usually optimized away
*/
Book createBook()
{
    Book b = {1, "CPP", 100.0};
    return b;
}


/*========================================================
9. CLASS (ENCAPSULATION)
========================================================*/
/*
Encapsulation = bind data + functions

KEY IDEA:
- Protect invariants
- Prevent invalid states

Difference from struct:
- struct → default public
- class → default private
*/
class BookClass
{
private:
    int id;
    char title[20];
    float price;

public:
    void setData(int i, const char* t, float p)
    {
        id = i;

        // SAFE copy (important!)
        strncpy(title, t, sizeof(title));
        title[sizeof(title)-1] = '\0';

        // Protect invariant
        if (p >= 0)
            price = p;
        else
            price = 0;
    }

    void display() const // const = no modification
    {
        cout << id << " " << title << " " << price << endl;
    }
};


/*========================================================
10. STATIC VARIABLE
========================================================*/
/*
Stored in DATA SEGMENT (not stack)

PROPERTIES:
- Initialized once
- Lifetime = entire program
- Shared across calls

TRAP:
- Not thread-safe by default
*/
void staticDemo()
{
    static int x = 0; // persists
    int y = 0;        // resets

    x++;
    y++;

    cout << "static: " << x
         << " | normal: " << y << endl;
}


/*========================================================
11. CLASS + OBJECT STATE + BEHAVIOR
========================================================*/
/*
Object = state + behavior

State = member variables
Behavior = member functions

GOOD PRACTICE:
- pass objects as const reference
*/
class Complex
{
private:
    int a, b;

public:
    // Constructor (important)
    Complex(int x = 0, int y = 0)
    {
        a = x;
        b = y;
    }

    void show() const
    {
        cout << a << " + " << b << "i\n";
    }

    // Efficient passing
    Complex add(const Complex &c) const
    {
        return Complex(a + c.a, b + c.b);
    }

    /*
    OPERATOR OVERLOADING

    Interview extension:
    - makes objects behave like primitives
    */
    Complex operator+(const Complex &c) const
    {
        return Complex(a + c.a, b + c.b);
    }
};


/*========================================================
12. SCOPE RESOLUTION OPERATOR
========================================================*/
/*
Used when function defined outside class

Syntax:
ClassName::functionName
*/
class Demo
{
public:
    void show();
};

void Demo::show()
{
    cout << "Using scope resolution\n";
}


/*========================================================
13. STRUCT vs CLASS
========================================================*/
/*
struct:
- default public

class:
- default private

Otherwise SAME in C++
*/
struct S
{
    int x;
};

class C
{
    int x;
};


/*========================================================
14. SHALLOW COPY DANGER
========================================================*/
/*
VERY IMPORTANT INTERVIEW TOPIC

Default copy = SHALLOW COPY

Problem:
- pointer copied → both objects share memory
- double free / corruption

Solution:
- Deep copy (copy constructor)
- Rule of 3/5 (beyond your notes but important)
*/
struct Dangerous
{
    char *ptr;

    Dangerous()
    {
        ptr = new char[10];
        strcpy(ptr, "Hi");
    }

    // Missing:
    // - copy constructor
    // - destructor
};


/*========================================================
MAIN
========================================================*/
int main()
{
    cout << "=== FUNCTION BASICS ===\n";
    caller();

    cout << "\n=== INLINE ===\n";
    cout << inlineAdd(2,3) << endl;

    cout << "\n=== DEFAULT ARG ===\n";
    cout << sum(1,2) << endl;
    cout << sum(1,2,3) << endl;

    cout << "\n=== OVERLOADING ===\n";
    func(10);
    func(10.5);

    cout << "\n=== TYPE PROMOTION ===\n";
    promo('a'); // char → int

    cout << "\n=== STRUCT ===\n";
    Book b1 = {1, "C++", 100};

    Book b2;
    b2.id = 2;

    // IMPORTANT: cannot assign array directly
    strcpy(b2.title, "Systems");

    b2.price = 200;

    cout << b1.title << endl;
    cout << b2.title << endl;

    cout << "\n=== PASSING STRUCT ===\n";
    modifyByValue(b1);
    cout << "After value: " << b1.id << endl;

    modifyByRef(b1);
    cout << "After ref: " << b1.id << endl;

    cout << "\n=== RETURN STRUCT ===\n";
    Book b3 = createBook();
    cout << b3.title << endl;

    cout << "\n=== CLASS ENCAPSULATION ===\n";
    BookClass bc;
    bc.setData(10, "OS", -50); // invalid handled
    bc.display();

    cout << "\n=== STATIC ===\n";
    staticDemo();
    staticDemo();
    staticDemo();

    cout << "\n=== COMPLEX ===\n";
    Complex c1(3,4), c2(5,6);

    Complex c3 = c1.add(c2);
    c3.show();

    Complex c4 = c1 + c2; // operator overloading
    c4.show();

    cout << "\n=== SCOPE RESOLUTION ===\n";
    Demo d;
    d.show();

    return 0;
}
