#include <iostream>
using namespace std;

// Base class
class Animal {
protected: // Protected members are accessible in derived classes
    void protectedMethod() { cout << "Protected method in Animal." << endl; }
private: // Private members are not accessible in derived classes
    void privateMethod() { cout << "Private method in Animal." << endl; }
public:
    void eat() { cout << "This animal eats food." << endl; }
};

// Single Inheritance: Dog inherits from Animal
class Dog : public Animal {
public:
    void bark() { cout << "Dog barks." << endl; }
    void accessProtected() { protectedMethod(); } // Allowed
    // void accessPrivate() { privateMethod(); } // Error: privateMethod is inaccessible
};

// Multilevel Inheritance: Puppy inherits from Dog
class Puppy : public Dog {
public:
    void weep() { cout << "Puppy weeps." << endl; }
};

// Multiple Inheritance: Bird inherits from both Animal and Flyer
class Flyer {
protected:
    void protectedFly() { cout << "Protected flying method." << endl; }
public:
    void fly() { cout << "This can fly." << endl; }
};

class Bird : public Animal, public Flyer {
public:
    void chirp() { cout << "Bird chirps." << endl; }
    void accessFlyerProtected() { protectedFly(); } // Allowed
};

// Hierarchical Inheritance: Multiple classes inherit from Animal
class Cat : public Animal {
public:
    void meow() { cout << "Cat meows." << endl; }
};

// Hybrid Inheritance with Virtual Base Class (Diamond Problem Solution)
class Mammal : virtual public Animal {};
class Reptile : virtual public Animal {};

class Platypus : public Mammal, public Reptile {
public:
    void special() { cout << "Platypus lays eggs but is a mammal." << endl; }
};

// Composition: Has-a relationship (Dog has a Tail)
class Tail {
public:
    void wag() { cout << "Tail is wagging." << endl; }
};

class DogWithTail : public Dog {
private:
    Tail tail; // Composition: Dog has a Tail
public:
    void wagTail() { tail.wag(); }
};

// Aggregation: Has-a relationship but with separate lifetime (Owner and Dog)
class Owner {
public:
    void showOwnership() { cout << "Owner owns a dog." << endl; }
};

class Pet {
private:
    Owner* owner; // Aggregation: Pet has an owner, but owner exists independently
public:
    Pet(Owner* o) : owner(o) {}
    void showOwner() { owner->showOwnership(); }
};

int main() {
    Dog d;
    d.eat(); // Inherited from Animal
    d.bark();
    d.accessProtected(); // Works
    // d.protectedMethod(); // Error: protected method cannot be accessed directly
    // d.privateMethod(); // Error: private method cannot be accessed

    Puppy p;
    p.eat();  // Inherited from Animal -> Dog
    p.bark(); // Inherited from Dog
    p.weep();

    Bird b;
    b.eat(); // From Animal
    b.fly(); // From Flyer
    b.chirp();
    b.accessFlyerProtected(); // Works

    Cat c;
    c.eat(); // From Animal
    c.meow();

    Platypus pl;
    pl.eat(); // No ambiguity due to virtual base
    pl.special();

    DogWithTail dt;
    dt.bark();
    dt.wagTail();

    Owner o;
    Pet pet(&o);
    pet.showOwner();

    return 0;
}
