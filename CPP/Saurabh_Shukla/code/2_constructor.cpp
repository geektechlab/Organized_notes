#include <iostream>
using namespace std;

class Car {
public:
    string brand;

    // 1. Default Constructor
    Car() {
        brand = "Unknown";
        cout << "Default Constructor Called!" << endl;
    }

    // 2. Parameterized Constructor
    Car(string b) {
        brand = b;
        cout << "Parameterized Constructor Called!" << endl;
    }

    // 3. Copy constructors must take const & otherwise temporaries (rvalues) can’t bind
    Car(const Car &obj) {
        brand = obj.brand;
        cout << "Copy Constructor Called!" << endl;
    }

    // 4. Move Constructor (C++11)
	//A move constructor (T(T&&) noexcept) steals resources from a temporary (rvalue) instead of copying, leaving the source valid but empty, and is faster than a copy constructor.
	//& binds to lvalues (named objects), && binds to rvalues (temporaries for moving), and noexcept guarantees the function won’t throw, enabling safe, efficient moves.
    Car(Car &&obj) noexcept {
        brand = move(obj.brand);
        cout << "Move Constructor Called!" << endl;
    }

    // Destructor can print values or object also and it destroy in sequence in which objects were declared
	// it will destroy c4, c3, c2, c1 in sequence
    ~Car() {
        cout << "Destructor Called for " << brand << endl;
    }

    void display() {
        cout << "Brand: " << brand << endl;
    }
};

int main() {
    cout << "--- Creating c1 using Default Constructor ---" << endl;
    Car c1;  // Default constructor

    cout << "\n--- Creating c2 using Parameterized Constructor ---" << endl;
    Car c2("Toyota");  // Parameterized constructor

    cout << "\n--- Creating c3 using Copy Constructor ---" << endl;
    Car c3 = c2;  // Copy constructor

    cout << "\n--- Creating c4 using Move Constructor ---" << endl;
    Car c4 = move(c2);  // Move constructor

    cout << "\n--- Displaying Car Brands ---" << endl;
    c1.display();
    c3.display();
    c4.display();

    cout << "\n--- Program Ends, Destructor will be called automatically ---" << endl;

    return 0;
}
