#include <iostream>
#include <functional> // For std::function

// A regular function
int multiply(int a, int b) {
    return a * b;
}

// Function that accepts a function pointer
void processWithFunctionPointer(int a, int b, int (*funcPtr)(int, int)) {
    std::cout << "Using Function Pointer: " << funcPtr(a, b) << std::endl;
}

// Function that accepts std::function (can take function pointer or lambda)
void processWithStdFunction(int a, int b, std::function<int(int, int)> func) {
    std::cout << "Using std::function: " << func(a, b) << std::endl;
}

int main() {
    int x = 10;

    // Function Pointer
    int (*funcPtr)(int, int) = multiply;

    // Lambda Expression (without capture)
    auto lambdaSimple = [](int a, int b) {
        return a + b;
    };

    // Lambda Expression (with capture)
    auto lambdaWithCapture = [x](int a) {
        return a + x;
    };

    // Calling functions with function pointer
    processWithFunctionPointer(4, 5, funcPtr);

    // Calling function with lambda
    processWithStdFunction(4, 5, lambdaSimple);

    // Calling lambda directly
    std::cout << "Calling lambda directly: " << lambdaSimple(6, 7) << std::endl;

    // Calling lambda that captures a variable
    std::cout << "Lambda with capture: " << lambdaWithCapture(20) << std::endl;

    return 0;
}
