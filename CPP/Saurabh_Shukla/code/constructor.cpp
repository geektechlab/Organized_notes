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

    // 3. Copy Constructor
    Car(const Car &obj) {
        brand = obj.brand;
        cout << "Copy Constructor Called!" << endl;
    }

    // 4. Move Constructor (C++11)
    Car(Car &&obj) noexcept {
        brand = move(obj.brand);
        cout << "Move Constructor Called!" << endl;
    }

    // Destructor
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
