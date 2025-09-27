#include <iostream>
using namespace std;

/* read comments: https://chatgpt.com/share/67e0c54f-3dc0-8003-a99f-a27b94a7d529 */
class Number {
private:
    int value;

public:
    // Constructor
    Number(int v = 0) : value(v) {}

    // Copy Constructor (Important when dealing with deep copies)
    Number(const Number& other) {
        value = other.value;
    }

    // Overloading Assignment Operator
    Number& operator=(const Number& other) {
        if (this != &other) { // Prevent self-assignment
            value = other.value;
        }
        return *this;
    }

    // Overloading Unary Minus (-n)
    Number operator-() const {
        return Number(-value);
    }

    // Overloading Prefix Increment (++n)
    Number& operator++() {
        ++value;
        return *this;
    }

    // Overloading Postfix Increment (n++)
    Number operator++(int) {
        Number temp = *this;
        ++value;
        return temp;
    }

    // Overloading Binary Addition (n1 + n2)
    Number operator+(const Number& other) const {
        return Number(value + other.value);
    }

    // Overloading Binary Subtraction (n1 - n2)
    Number operator-(const Number& other) const {
        return Number(value - other.value);
    }

    // Overloading Multiplication
    Number operator*(const Number& other) const {
        return Number(value * other.value);
    }

    // Overloading Division (with zero-check)
    Number operator/(const Number& other) const {
        if (other.value == 0) {
            throw runtime_error("Division by zero!");
        }
        return Number(value / other.value);
    }

    // Overloading Modulus
    Number operator%(const Number& other) const {
        if (other.value == 0) {
            throw runtime_error("Modulo by zero!");
        }
        return Number(value % other.value);
    }

    // Overloading Comparison Operators
    bool operator==(const Number& other) const { return value == other.value; }
    bool operator!=(const Number& other) const { return value != other.value; }
    bool operator<(const Number& other) const { return value < other.value; }
    bool operator>(const Number& other) const { return value > other.value; }
    bool operator<=(const Number& other) const { return value <= other.value; }
    bool operator>=(const Number& other) const { return value >= other.value; }

    // Overloading Stream Insertion (<<) using Friend Function
    friend ostream& operator<<(ostream& os, const Number& n) {
        os << n.value;
        return os;
    }

    // Overloading Stream Extraction (>>) using Friend Function
    friend istream& operator>>(istream& is, Number& n) {
        is >> n.value;
        return is;
    }

    // Overloading Subscript Operator ([])
    int operator[](int index) const {
        return value * index;  // Just an example (no real use case)
    }

    // Overloading Function Call Operator ()
    void operator()() const {
        cout << "Number is: " << value << endl;
    }

    // Overloading Type Conversion (int)
    operator int() const {
        return value;
    }
};

int main() {
    Number n1(10), n2(5), n3;

    // Arithmetic Operations
    n3 = n1 + n2;
    cout << "n1 + n2 = " << n3 << endl;

    n3 = n1 - n2;
    cout << "n1 - n2 = " << n3 << endl;

    n3 = n1 * n2;
    cout << "n1 * n2 = " << n3 << endl;

    n3 = n1 / n2;
    cout << "n1 / n2 = " << n3 << endl;

    n3 = n1 % n2;
    cout << "n1 % n2 = " << n3 << endl;

    // Unary Minus
    cout << "-n1 = " << -n1 << endl;

    // Increment Operators
    cout << "n1++ = " << n1++ << endl; // Postfix
    cout << "After n1++: " << n1 << endl;
    
    cout << "++n1 = " << ++n1 << endl; // Prefix

    // Comparison Operators
    cout << "n1 == n2: " << (n1 == n2) << endl;
    cout << "n1 > n2: " << (n1 > n2) << endl;

    // Function Call Operator
    n1();

    // Type Conversion
    int val = (int)n1;
    cout << "Converted n1 to int: " << val << endl;

    // Subscript Operator
    cout << "n1[2] = " << n1[2] << endl; // Just an example

    // Input Operator Overload
    cout << "Enter a number: ";
    cin >> n3;
    cout << "You entered: " << n3 << endl;

    return 0;
}
