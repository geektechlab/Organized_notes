#include <iostream>
#include <vector>

struct Person {
    std::string name;
    int age;
};

int main() {
    std::vector<Person> people = { {"Alice", 25}, {"Bob", 30}, {"Charlie", 22} };

    // Using iterator and arrow operator to access struct members
    for (auto it = people.begin(); it != people.end(); ++it) {
        std::cout << "Name: " << it->name << ", Age: " << it->age << std::endl;
    }

    return 0;
}
