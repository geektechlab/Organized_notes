#include <iostream>
using namespace std;

// Function that modifies a variable using reference
void modifyValue(int &ref) {
    ref *= 2;  // Doubles the value of the original variable
}

// Function returning a reference
int& getReference(int &x) {
    return x;  // Returns a reference to the variable
}

// Function using constant reference (no modification allowed)
void printValue(const int &ref) {
    cout << "Value: " << ref << endl;
}

int main() {
    int a = 5;

    // reference variables are not ordinary like pointer variable
    // it is internal pointer which must be initialized during declaration with already declared variable only
    // it can't be updated to take any other variable's reference
    int &ref=a;

    /* both will print 5 */
    cout << "a=" << a << " ref=" << ref << endl;
    
    /* following will increment both ref and a */
    ref++;
    cout << "a=" << a << " ref=" << ref << endl;

    /* following will increment both ref and a */
    a++;
    cout << "a=" << a << " ref=" << ref << endl;

    // 3. References as function parameters
    modifyValue(a);
    cout << "After modifyValue function: " << a << endl;

    // 4. References as function return values
    int &b = getReference(a);
    b += 5; // Modifies 'a' indirectly
    cout << "After getReference function: " << a << endl;

    // 5. Constant references
    printValue(a);
    
    // 6. Reference to a pointer
    int x = 30;
    int *ptr = &x;
    int* &refPtr = ptr; // Reference to pointer
    *refPtr = 40; // Changes 'x' through pointer reference
    cout << "Modified x through pointer reference: " << x << endl;

    // 7. Array references
    int arr[3] = {1, 2, 3};
    int (&refArr)[3] = arr; // Reference to entire array
    refArr[1] = 99; // Modifies original array
    cout << "Array after modification: " << arr[0] << ", " << arr[1] << ", " << arr[2] << endl;

    return 0;
}
