#include <iostream>
using namespace std;

/*
===========================================================
🔥 CORE IDEA OF THIS FILE
===========================================================

This file is NOT just code — it's a complete mental model.

If you deeply understand:
- Member vs Friend operators
- Unary vs Binary mapping
- Stream operator design
- Pre/Post increment behavior

You are already ahead of 90% candidates.

===========================================================
*/

class Complex {
private:
    int a, b;

public:
    /*
    ===========================================================
    CONSTRUCTOR
    ===========================================================

    - Default + parameterized
    - Important for implicit conversions in operator overloading
    */
    Complex(int x = 0, int y = 0) : a(x), b(y) {}

    void setData(int x, int y) {
        a = x;
        b = y;
    }

    void showData() const {
        cout << a << " + " << b << "i\n";
    }

    /*
    ===========================================================
    🔥 MEMBER BINARY OPERATOR (+)
    ===========================================================

    Expression:
        c3 = c1 + c2;

    Internally:
        c1.operator+(c2)

    KEY INSIGHT:
    - Left operand → caller object (this)
    - Right operand → argument

    TRAP:
    - Only works if LEFT operand is of this class
    - Fails for:  5 + c1  ❌
    */

    Complex operator+(const Complex& c) const {
        // this->a refers to caller object
        return Complex(this->a + c.a, this->b + c.b);
    }

    /*
    ===========================================================
    🔥 UNARY OPERATOR (-)
    ===========================================================

    Expression:
        c2 = -c1;

    Internally:
        c1.operator-()

    TRAP:
    - No arguments in member version
    */

    Complex operator-() const {
        return Complex(-a, -b);
    }

    /*
    ===========================================================
    🔥 PRE-INCREMENT (++c)
    ===========================================================

    Signature:
        Complex& operator++()

    WHY reference return?
    - Avoid copy
    - Enable chaining: (++c1).showData();

    TRAP:
    If you return by value → inefficient + breaks chaining
    */

    Complex& operator++() {
        ++a;
        ++b;
        return *this; // critical
    }

    /*
    ===========================================================
    🔥 POST-INCREMENT (c++)
    ===========================================================

    Signature:
        Complex operator++(int)

    KEY:
    - int is dummy parameter
    - distinguishes from pre-increment

    BEHAVIOR:
    - return OLD value
    */

    Complex operator++(int) {
        Complex temp = *this; // copy OLD state
        a++;
        b++;
        return temp; // return old
    }

    /*
    ===========================================================
    🔥 FRIEND BINARY OPERATOR (-)
    ===========================================================

    Expression:
        c3 = c1 - c2;

    Internally:
        operator-(c1, c2)

    KEY DIFFERENCE:
    - No caller object
    - Both operands passed explicitly

    ADVANTAGE:
    - Supports symmetry (important in real systems)
    */

    friend Complex operator-(const Complex& c1, const Complex& c2);

    /*
    ===========================================================
    🔥 STREAM OPERATORS
    ===========================================================

    WHY FRIEND?
    Because:
        cout << c1;

    means:
        operator<<(cout, c1)

    NOT:
        c1.operator<<(cout)

    IMPORTANT:
    - cout is NOT your class → cannot be member function
    */

    friend ostream& operator<<(ostream& out, const Complex& c);
    friend istream& operator>>(istream& in, Complex& c);
};

/*
===========================================================
FRIEND FUNCTION IMPLEMENTATIONS
===========================================================
*/

Complex operator-(const Complex& c1, const Complex& c2) {
    return Complex(c1.a - c2.a, c1.b - c2.b);
}

/*
===========================================================
🔥 STREAM INSERTION (<<)
===========================================================

CRITICAL DESIGN RULES:

1. Return ostream&
   → enables chaining

2. Take stream by reference

3. Take object as const reference

TRAP:
If you return void → chaining breaks ❌
*/

ostream& operator<<(ostream& out, const Complex& c) {
    out << c.a << " + " << c.b << "i";
    return out;
}

/*
===========================================================
🔥 STREAM EXTRACTION (>>)
===========================================================

TRAP:
Must take NON-const reference because we MODIFY object
*/

istream& operator>>(istream& in, Complex& c) {
    in >> c.a >> c.b;
    return in;
}

/*
===========================================================
🔥 MULTI-CLASS FRIEND FUNCTION
===========================================================

Use case:
- One function accessing private members of multiple classes
*/

class B; // forward declaration

class A {
private:
    int x;

public:
    A(int val = 0) : x(val) {}

    friend void accessBoth(const A&, const B&);
};

class B {
private:
    int y;

public:
    B(int val = 0) : y(val) {}

    friend void accessBoth(const A&, const B&);
};

void accessBoth(const A& a, const B& b) {
    cout << "A + B = " << a.x + b.y << endl;
}

/*
===========================================================
🔥 MEMBER FUNCTION AS FRIEND
===========================================================

Advanced concept:
- Specific function of one class can access another class
*/

class X;

class Y {
public:
    void showX(const X& obj);
};

class X {
private:
    int data;

public:
    X(int d = 0) : data(d) {}

    friend void Y::showX(const X&);
};

void Y::showX(const X& obj) {
    cout << "X data: " << obj.data << endl;
}

/*
===========================================================
🔥 FRIEND CLASS
===========================================================

ALL functions of one class can access another
*/

class Secret {
private:
    int value = 999;

    friend class Hacker;
};

class Hacker {
public:
    void reveal(const Secret& s) {
        cout << "Secret: " << s.value << endl;
    }
};

/*
===========================================================
🔥 MAIN (EXECUTION FLOW)
===========================================================
*/

int main() {
    Complex c1(3, 4), c2(5, 6);

    // Member binary
    Complex c3 = c1 + c2;

    // Explicit call (rare but important)
    Complex c4 = c1.operator+(c2);

    // Friend binary
    Complex c5 = c2 - c1;

    // Unary
    Complex c6 = -c1;

    // Pre increment
    ++c1;

    // Post increment
    Complex old = c1++;

    cout << "c1: " << c1 << endl;
    cout << "old: " << old << endl;

    // Multi-class friend
    A a(10);
    B b(20);
    accessBoth(a, b);

    // Member function as friend
    X x(42);
    Y y;
    y.showX(x);

    // Friend class
    Secret s;
    Hacker h;
    h.reveal(s);

    return 0;
}
