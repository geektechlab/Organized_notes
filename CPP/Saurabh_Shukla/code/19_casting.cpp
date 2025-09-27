#include <iostream>
using namespace std;

class Celsius {
    double temp;
public:
    Celsius(double t) : temp(t) {}

    // Conversion operator: Class to primitive type (Celsius to double)
    operator double() const {
        return temp;
    }

    double getTemp() const {
        return temp;
    }
};

class Fahrenheit {
    double temp;
public:
    Fahrenheit(double t) : temp(t) {}

    // Constructor for conversion: Celsius to Fahrenheit
    Fahrenheit(Celsius c) {
        temp = (static_cast<double>(c) * 9.0 / 5.0) + 32;
    }

    void display() const {
        cout << "Temperature in Fahrenheit: " << temp << "°F" << endl;
    }
};

int main() {
    Celsius c(25); // 25°C

    // Class to primitive conversion
    double tempInCelsius = c;
    cout << "Temperature in Celsius: " << tempInCelsius << "°C" << endl;

    // Class type casting (Celsius to Fahrenheit)
    Fahrenheit f = c; 
    f.display();

    return 0;
}
