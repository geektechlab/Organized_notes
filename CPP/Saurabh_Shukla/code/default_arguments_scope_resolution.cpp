#include <iostream>

// Global variable with default value
int defaultAge = 18;

// Class with static variable and function
class Person {
public:
    static std::string defaultCountry; // Static variable (declaration)
    
    /* if default argumnet is set, then all subsequent left argumnets should be set */
    static void displayInfo(std::string name = "Guest", int age = defaultAge, std::string country = defaultCountry);
};

// Define static variable outside the class using scope resolution
std::string Person::defaultCountry = "USA";

// Function definition using scope resolution ::
void Person::displayInfo(std::string name, int age, std::string country) {
    std::cout << "Name: " << name << ", Age: " << age << ", Country: " << country << std::endl;
}

int main() {
    // 1. Call function with no arguments (uses all default values)
    std::cout << "Case 1: No arguments\n";
    Person::displayInfo();

    // 2. Override only the first argument
    std::cout << "\nCase 2: Only name provided\n";
    Person::displayInfo("Alice");

    // 3. Override first two arguments
    std::cout << "\nCase 3: Name and age provided\n";
    Person::displayInfo("Bob", 25);

    // 4. Override all arguments
    std::cout << "\nCase 4: All arguments provided\n";
    Person::displayInfo("Charlie", 30, "Canada");

    // Modify global and static variable using scope resolution
    defaultAge = 21;
    Person::defaultCountry = "UK";

    std::cout << "\nAfter modifying global and static variables:\n";
    Person::displayInfo();  // Should now use updated values

    return 0;
}
