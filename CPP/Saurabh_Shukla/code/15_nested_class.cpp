#include <iostream>

// Outer class with nested class
class Outer {
private:
    int outerData;

public:
    Outer(int val) : outerData(val) {}

    // Nested class declaration
    class Nested {
    public:
        void show() {
            std::cout << "Inside Nested class" << std::endl;
        }

        // Attempt to access Outer class private member (without friendship)
        void tryAccessOuter(Outer& o) {
            // std::cout << "Outer Data: " << o.outerData << std::endl; // ERROR!
        }
    };

    // Granting access via friend declaration
    class FriendNested {
    public:
        void accessOuter(Outer& o) {
            std::cout << "Accessing Outer private member: " << o.outerData << std::endl;
        }
    };

    // Outer class accessing Nested members
    void accessNested() {
        Nested n;
        n.show();  // Allowed
    }

    friend class FriendNested; // Grant full access to FriendNested
};

// Nested template class inside a template Outer class
template <typename T>
class TemplateOuter {
private:
    T data;
public:
    TemplateOuter(T val) : data(val) {}

    class NestedTemplate {
    public:
        void display() {
            std::cout << "Inside template nested class" << std::endl;
        }
    };

    void showData() {
        std::cout << "Template Outer Data: " << data << std::endl;
    }
};

int main() {
    // Using simple nested class
    Outer::Nested nestedObj;
    nestedObj.show();

    // Using FriendNested class to access private data of Outer
    Outer outerObj(42);
    Outer::FriendNested friendNestedObj;
    friendNestedObj.accessOuter(outerObj);

    // Using template-based nested class
    TemplateOuter<int> templateOuter(100);
    templateOuter.showData();

    TemplateOuter<int>::NestedTemplate templateNestedObj;
    templateNestedObj.display();

    return 0;
}
