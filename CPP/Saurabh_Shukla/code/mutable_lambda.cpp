#include <iostream>

class Test {
    mutable int counter = 0;  // `mutable` allows modification even in a `const` function

public:
    void increment() const {
        counter++;  // Allowed because `counter` is mutable
    }

    int getCounter() const {
        return counter;
    }
};

int main() {
    // ---------------------------- //
    // 1. Demonstrating `mutable` with class members
    // ---------------------------- //

    Test obj;
    obj.increment();  // Can modify `counter` even though `increment` is `const`
    std::cout << "Counter after increment: " << obj.getCounter() << std::endl;

    // ---------------------------- //
    // 2. Lambda with `mutable`
    // ---------------------------- //

    // Define two variables
    int x = 10, y = 20;

    // Define a lambda function capturing `x` by value and `y` by reference
    auto lambda = [x, &y]() mutable {
        /*
            - `x` is captured **by value**, meaning a copy is created inside the lambda.
            - Normally, `x` would be immutable inside the lambda.
            - The `mutable` keyword allows us to modify the copy of `x` inside the lambda.

            - `y` is captured **by reference**, meaning modifications to `y` inside the lambda
              will reflect on the original `y` variable outside the lambda.
        */
        x++;  // Allowed due to `mutable` (modifies local copy of `x`).
        y++;  // Allowed because `y` is captured by reference (modifies original `y`).
        
        std::cout << "Inside lambda: x = " << x << ", y = " << y << std::endl;
    };

    lambda();  // Call the lambda function

    // ---------------------------- //
    // 3. Checking values after lambda execution
    // ---------------------------- //
    
    /*
        - `x` was captured by value inside the lambda, so changes to `x` inside the lambda
          do **not** affect the original `x` in `main()`.
        - `y` was captured by reference, so changes to `y` inside the lambda
          **do** affect the original `y` in `main()`.
    */
    
    std::cout << "Outside lambda: x = " << x << ", y = " << y << std::endl;

    return 0;
}
