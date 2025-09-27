#include <iostream>
using namespace std;

class ClassB; // Forward declaration

class ClassA {
private:
    int numA;
public:
    ClassA(int a) : numA(a) {}  // Constructor

    // Friend function declaration for operator overloading
    friend int operator+(ClassA, ClassB);

    // Function to display value
    void display() {
        cout << "ClassA: " << numA << endl;
    }
};

class ClassB {
private:
    int numB;
public:
    ClassB(int b) : numB(b) {}  // Constructor

    // Friend function declaration for operator overloading
    friend int operator+(ClassA, ClassB);

    // Function to display value
    void display() {
        cout << "ClassB: " << numB << endl;
    }
};

// Overloaded + operator using friend function
int operator+(ClassA objA, ClassB objB) {
    return objA.numA + objB.numB;
}

int main() {
    ClassA obj1(10);
    ClassB obj2(20);

    obj1.display();
    obj2.display();

    int result = obj1 + obj2;  // Calls overloaded + operator
    cout << "Sum = " << result << endl;

    return 0;
}
