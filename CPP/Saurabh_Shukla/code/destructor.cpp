#include <iostream>
#include <fstream>
using namespace std;

class ResourceHandler {
private:
    int* data;         // Dynamic memory allocation
    fstream file;      // File handling

public:
    ResourceHandler(const string& filename) {
        // Allocate memory
        data = new int[10];
        cout << "Memory allocated.\n";

        // Open a file
        file.open(filename, ios::out);
        if (file.is_open()) {
            cout << "File opened successfully.\n";
            file << "Writing some data...\n";
        }
    }

    ~ResourceHandler() {
        // Release memory
        delete[] data;
        cout << "Memory released.\n";

        // Close the file if open
        if (file.is_open()) {
            file.close();
            cout << "File closed.\n";
        }

        // Logging destruction
        cout << "Destructor called: Object is being destroyed.\n";
    }
};

int main() {
    ResourceHandler obj("example.txt"); // Constructor runs here

    cout << "Doing some operations...\n";

    return 0; // Destructor is automatically called when obj goes out of scope
}
