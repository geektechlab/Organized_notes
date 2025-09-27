#include <iostream>

/// Base class without virtual destructor (Problem Demonstration)
class BaseWithoutVirtual {
public:
    BaseWithoutVirtual() { std::cout << "BaseWithoutVirtual Constructor\n"; }
    ~BaseWithoutVirtual() { std::cout << "BaseWithoutVirtual Destructor\n"; } // Not virtual
};

/// Derived class inheriting from BaseWithoutVirtual
class DerivedWithoutVirtual : public BaseWithoutVirtual {
public:
    DerivedWithoutVirtual() { std::cout << "DerivedWithoutVirtual Constructor\n"; }
    ~DerivedWithoutVirtual() { std::cout << "DerivedWithoutVirtual Destructor\n"; } // Won't be called
};

/// Base class with virtual destructor (Correct Approach)
class BaseWithVirtual {
public:
    BaseWithVirtual() { std::cout << "BaseWithVirtual Constructor\n"; }
    
    // Virtual destructor ensures correct cleanup
    virtual ~BaseWithVirtual() { std::cout << "BaseWithVirtual Destructor\n"; }
};

/// Derived class inheriting from BaseWithVirtual
class DerivedWithVirtual : public BaseWithVirtual {
public:
    DerivedWithVirtual() { std::cout << "DerivedWithVirtual Constructor\n"; }
    ~DerivedWithVirtual() { std::cout << "DerivedWithVirtual Destructor\n"; } // Will be correctly called
};

/// Abstract base class with a pure virtual function and a virtual destructor
class AbstractBase {
public:
    virtual void show() = 0; // Pure virtual function

    // Pure virtual destructor (must be defined outside class)
    virtual ~AbstractBase() = 0;
};

// Definition of pure virtual destructor (Required)
AbstractBase::~AbstractBase() {
    std::cout << "AbstractBase Destructor\n";
}

/// Derived class from AbstractBase
class ConcreteDerived : public AbstractBase {
public:
    void show() override { std::cout << "ConcreteDerived show function\n"; }
    ~ConcreteDerived() { std::cout << "ConcreteDerived Destructor\n"; }
};

/// Base class where destructor deletion is enforced (Prevents deletion via base pointer)
class BaseDeletedDestructor {
public:
    virtual ~BaseDeletedDestructor() = delete; // Prevents deletion through a base pointer
};

int main() {
    std::cout << "----- Without Virtual Destructor (Incorrect) -----\n";
    BaseWithoutVirtual* obj1 = new DerivedWithoutVirtual();
    delete obj1; // Incorrect behavior: Only base destructor is called

    std::cout << "\n----- With Virtual Destructor (Correct) -----\n";
    BaseWithVirtual* obj2 = new DerivedWithVirtual();
    delete obj2; // Correct behavior: Both destructors are called

    std::cout << "\n----- Abstract Class with Virtual Destructor -----\n";
    AbstractBase* obj3 = new ConcreteDerived();
    obj3->show();
    delete obj3; // Ensures complete destruction

    std::cout << "\n----- Preventing Deletion via Base Pointer (Error) -----\n";
    // Uncommenting the below lines will cause a compilation error
    // BaseDeletedDestructor* obj4 = new BaseDeletedDestructor();
    // delete obj4; // Error: Destructor is deleted

    return 0;
}
