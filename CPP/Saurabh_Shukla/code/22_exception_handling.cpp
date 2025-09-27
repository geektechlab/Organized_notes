#include <iostream>
#include <stdexcept>
#include <exception>

// Custom exception class
class CustomException : public std::exception {
public:
    const char* what() const noexcept override {
        return "CustomException occurred!";
    }
};

// Function that throws exceptions
void riskyFunction(int x) {
    if (x == 0) {
        throw std::runtime_error("Division by zero error!"); // Throwing standard exception
    } else if (x == 1) {
        throw CustomException(); // Throwing custom exception
    } else if (x == 2) {
        throw "Unknown exception type!"; // Throwing a string (not recommended)
    }
}

// Function that catches and rethrows exceptions
void wrapperFunction(int x) {
    try {
        riskyFunction(x);
    } catch (const std::exception& e) {
        std::cerr << "wrapperFunction caught: " << e.what() << "\n";
        throw; // Rethrowing the same exception
    }
}

// RAII (Resource Acquisition Is Initialization) class
class ResourceHandler {
public:
    ResourceHandler() { std::cout << "Resource acquired.\n"; }
    ~ResourceHandler() { std::cout << "Resource released.\n"; }
};

int main() {
    try {
        ResourceHandler res; // Ensures resource cleanup even if exception occurs

        int testCase;
        std::cout << "Enter a number (0-2) to trigger an exception: ";
        std::cin >> testCase;

        wrapperFunction(testCase); // Function that catches and rethrows exceptions
        std::cout << "Function executed successfully.\n";
    } 
    catch (const std::runtime_error& e) { // Catch specific standard exception
        std::cerr << "Caught runtime_error: " << e.what() << std::endl;
    } 
    catch (const CustomException& e) { // Catch custom exception
        std::cerr << "Caught CustomException: " << e.what() << std::endl;
    } 
    catch (const std::exception& e) { // Catch all standard exceptions
        std::cerr << "Caught std::exception: " << e.what() << std::endl;
    } 
    catch (...) { // Catch any other unknown exceptions
        std::cerr << "Caught an unknown exception!\n";
    }

    std::cout << "Program execution continues safely.\n";
    return 0;
}
