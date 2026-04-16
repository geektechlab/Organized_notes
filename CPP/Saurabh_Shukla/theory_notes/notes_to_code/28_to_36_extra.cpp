#include <iostream>
using namespace std;

/*
===========================================================
🔥 HOW TO USE THIS FILE
===========================================================

- Read top to bottom
- Every section has embedded interview traps
- Pause at each "INTERVIEW QUESTION" and think

This is NOT just code → it's your interview simulator
===========================================================
*/

class Complex {
private:
    int a, b;

public:
    Complex(int x = 0, int y = 0) : a(x), b(y) {}

    /*
    ===========================================================
    🔥 MEMBER BINARY OPERATOR (+)
    ===========================================================

    c3 = c1 + c2  →  c1.operator+(c2)

    INTERVIEW QUESTION 1:
    Why does this fail?
        Complex c;
        Complex x = 5 + c;

    👉 Because LEFT operand must be Complex
    */

    Complex operator+(const Complex& c) const {
        return Complex(this->a + c.a, this->b + c.b);
    }

    /*
    INTERVIEW QUESTION 2:
    Why const Complex& and not Complex?

    👉 Avoid copy + performance + correctness
    */

    /*
    ===========================================================
    🔥 UNARY OPERATOR (-)
    ===========================================================

    c2 = -c1 → c1.operator-()

    INTERVIEW QUESTION 3:
    How many arguments here?

    👉 0 (member), 1 (if friend)
    */

    Complex operator-() const {
        return Complex(-a, -b);
    }

    /*
    ===========================================================
    🔥 PRE-INCREMENT (++c)
    ===========================================================

    INTERVIEW QUESTION 4:
    Why return Complex& and NOT Complex?

    👉 Enables chaining:
        (++c1).operator++();

    If return by value → breaks chaining + extra copy
    */

    Complex& operator++() {
        ++a;
        ++b;
        return *this;
    }

    /*
    ===========================================================
    🔥 POST-INCREMENT (c++)
    ===========================================================

    INTERVIEW QUESTION 5:
    Why dummy int parameter?

    👉 Differentiates from pre-increment
    */

    Complex operator++(int) {
        Complex temp = *this;
        a++;
        b++;
        return temp;
    }

    /*
    INTERVIEW QUESTION 6:
    What does this do?

        Complex c;
        c++++;

    👉 (c++)++
    👉 Valid but dangerous / confusing
    */

    /*
    ===========================================================
    🔥 FRIEND BINARY OPERATOR (-)
    ===========================================================

    c3 = c1 - c2 → operator-(c1, c2)

    INTERVIEW QUESTION 7:
    Why use friend here instead of member?

    👉 Enables symmetry:
        int + Complex
        Complex + int
    */

    friend Complex operator-(const Complex& c1, const Complex& c2);

    /*
    ===========================================================
    🔥 STREAM OPERATORS
    ===========================================================

    INTERVIEW QUESTION 8:
    Why can't this be member?

        c1 << cout; ❌

    👉 Because left operand must be object if member
    */

    friend ostream& operator<<(ostream& out, const Complex& c);
    friend istream& operator>>(istream& in, Complex& c);
};

/*
===========================================================
FRIEND IMPLEMENTATIONS
===========================================================
*/

Complex operator-(const Complex& c1, const Complex& c2) {
    return Complex(c1.a - c2.a, c1.b - c2.b);
}

/*
===========================================================
🔥 STREAM INSERTION
===========================================================

INTERVIEW QUESTION 9:
Why return ostream& ?

👉 Enables:
    cout << c1 << c2;

Expands to:
    operator<<(operator<<(cout, c1), c2);
*/

ostream& operator<<(ostream& out, const Complex& c) {
    out << c.a << " + " << c.b << "i";
    return out;
}

/*
===========================================================
🔥 STREAM EXTRACTION
===========================================================

INTERVIEW QUESTION 10:
Why NOT const Complex& ?

👉 Because we MODIFY object
*/

istream& operator>>(istream& in, Complex& c) {
    in >> c.a >> c.b;
    return in;
}

/*
===========================================================
🔥 MULTI-CLASS FRIEND FUNCTION
===========================================================
*/

class B;

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

/*
INTERVIEW QUESTION 11:
Why friend needed?

👉 Because function is NOT member of either class
*/

void accessBoth(const A& a, const B& b) {
    cout << "Sum: " << a.x + b.y << endl;
}

/*
===========================================================
🔥 MEMBER FUNCTION AS FRIEND
===========================================================
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

/*
INTERVIEW QUESTION 12:
Why fully qualified name required?

👉 Compiler must know WHICH class function is friend
*/

void Y::showX(const X& obj) {
    cout << obj.data << endl;
}

/*
===========================================================
🔥 FRIEND CLASS
===========================================================
*/

class Secret {
private:
    int value = 100;

    friend class Hacker;
};

/*
INTERVIEW QUESTION 13:
Difference:
    friend function vs friend class ?

👉 Friend class → ALL functions get access
*/

class Hacker {
public:
    void reveal(const Secret& s) {
        cout << s.value << endl;
    }
};

/*
===========================================================
🔥 MAIN TEST
===========================================================
*/

int main() {

    Complex c1(3, 4), c2(5, 6);

    // Member operator
    Complex c3 = c1 + c2;

    // INTERVIEW QUESTION 14:
    // Is this same?
    Complex c4 = c1.operator+(c2);
    // 👉 YES

    // Friend operator
    Complex c5 = c2 - c1;

    // Unary
    Complex c6 = -c1;

    // Pre increment
    ++c1;

    // Post increment
    Complex old = c1++;

    /*
    INTERVIEW QUESTION 15:
    Output difference between:
        cout << ++c1;
        cout << c1++;

    👉 Pre: updated value
    👉 Post: old value
    */

    cout << "c1: " << c1 << endl;
    cout << "old: " << old << endl;

    // Multi-class friend
    A a(10);
    B b(20);
    accessBoth(a, b);

    // Member function friend
    X x(99);
    Y y;
    y.showX(x);

    // Friend class
    Secret s;
    Hacker h;
    h.reveal(s);

    /*
    ===========================================================
    🔥 FINAL BRAIN-BENDER (APPLE LEVEL)
    ===========================================================

    INTERVIEW QUESTION 16:

    Why is this invalid?

        (c1 + c2) = c1;

    👉 Because operator+ returns temporary (rvalue)

    Fix?
    👉 Return reference (NOT recommended)
    */

    return 0;
}
