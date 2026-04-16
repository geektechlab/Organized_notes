#include <iostream>
#include <memory>
#include <cstring>
using namespace std;

/*
============================================================
MASTER SINGLE-FILE REFERENCE
Covers:
- Initializer list (const, reference, order issues)
- Copy constructor vs assignment
- Shallow vs deep copy
- Rule of 5 (copy/move/destructor)
- Destructor pitfalls
- Primitive <-> class conversion
- Class <-> class conversion
- RAII (modern C++)
- ALL interview caveats inline
============================================================
*/

/*
============================================================
1. INITIALIZER LIST (CRITICAL)
============================================================
KEY RULES:
- const and reference MUST be initialized
- Order = declaration order (NOT initializer list order)
- Prefer initialization over assignment (performance)
*/
class InitDemo {
    int x;          // initialized FIRST (order matters)
    int y;
    const int cx;
    int& ref;

public:
    InitDemo(int a, int b, int& r)
        : x(a), y(b), cx(a + b), ref(r) // correct
    {
        // BAD: assignment here would cause double work
    }

    void print() const {
        cout << "InitDemo -> x:" << x << " y:" << y
             << " cx:" << cx << " ref:" << ref << endl;
    }
};

/*
INTERVIEW TRAP:
class A {
    int x;
    int y;
public:
    A() : y(2), x(y) {} // ❌ UB: x initialized before y
};
*/

/*
============================================================
2. SHALLOW COPY (DEFAULT)
============================================================
- Bitwise copy
- Pointer copied, NOT memory
- Leads to double free / dangling pointer
*/
class Shallow {
public:
    int* p;

    Shallow(int v) {
        p = new int(v);
    }

    // No copy constructor -> shallow copy

    ~Shallow() {
        delete p; // 💀 double free risk
    }
};

/*
============================================================
3. DEEP COPY + RULE OF 5
============================================================
If class owns resource → implement all 5
*/
class Deep {
    int* p;

public:
    Deep(int v) {
        p = new int(v);
    }

    // Destructor
    ~Deep() {
        delete p;
    }

    // Copy constructor
    Deep(const Deep& other) {
        p = new int(*other.p);
    }

    // Copy assignment
    Deep& operator=(const Deep& other) {
        if (this == &other) return *this; // self-assignment guard

        delete p;
        p = new int(*other.p);
        return *this;
    }

    // Move constructor
    Deep(Deep&& other) noexcept {
        p = other.p;
        other.p = nullptr;
    }

    // Move assignment
    Deep& operator=(Deep&& other) noexcept {
        if (this == &other) return *this;

        delete p;
        p = other.p;
        other.p = nullptr;
        return *this;
    }

    void print() const {
        cout << "Deep value: " << *p << endl;
    }
};

/*
INTERVIEW CAVEATS:
- Missing move => performance loss
- noexcept required for STL optimizations
- Partial rule-of-5 => bugs
*/

/*
============================================================
4. DEMO: SHALLOW vs DEEP
============================================================
*/
void demo_copy() {
    cout << "\n--- Shallow Copy ---\n";
    Shallow s1(10);
    Shallow s2 = s1; // same pointer
    cout << "s1.p: " << s1.p << " s2.p: " << s2.p << endl;

    cout << "\n--- Deep Copy ---\n";
    Deep d1(20);
    Deep d2 = d1;
    d1.print();
    d2.print();
}

/*
============================================================
5. PRIMITIVE -> CLASS
============================================================
*/
class Complex {
    int a, b;

public:
    Complex() : a(0), b(0) {}

    explicit Complex(int x) : a(x), b(0) {} // explicit avoids implicit bugs

    Complex(int x, int y) : a(x), b(y) {}

    void print() const {
        cout << "Complex(" << a << "," << b << ")" << endl;
    }
};

/*
CAVEAT:
Complex c = 5; // ❌ if not explicit
*/

/*
============================================================
6. CLASS -> PRIMITIVE
============================================================
*/
class Number {
    int val;

public:
    Number(int v) : val(v) {}

    operator int() const { return val; }
};

/*
CAVEAT:
- implicit conversion may break logic
- avoid multiple conversion operators
*/

/*
============================================================
7. CLASS -> CLASS CONVERSION
============================================================
*/
class Product;

class Item {
    int a, b;

public:
    Item() : a(0), b(0) {}

    Item(int x, int y) : a(x), b(y) {}

    // conversion constructor
    Item(const Product& p);

    void print() const {
        cout << "Item(" << a << "," << b << ")" << endl;
    }
};

class Product {
    int m, n;

public:
    Product(int x, int y) : m(x), n(y) {}

    int getM() const { return m; }
    int getN() const { return n; }

    // conversion operator
    operator Item() const {
        return Item(m, n);
    }
};

Item::Item(const Product& p) {
    a = p.getM();
    b = p.getN();
}

/*
CAVEATS:
- avoid infinite recursion (ctor + operator both present)
- respect private access (use getters)
*/

/*
============================================================
8. RAII (MODERN C++)
============================================================
*/
class RAII {
    unique_ptr<int> p;

public:
    RAII(int v) : p(make_unique<int>(v)) {}

    void print() const {
        cout << "RAII value: " << *p << endl;
    }
};

/*
CAVEATS:
- unique_ptr cannot be copied (only moved)
- avoid exposing raw pointer
- shared_ptr cycles cause leaks
*/

/*
============================================================
9. DESTRUCTOR PITFALLS
============================================================
*/
class Base {
public:
    virtual ~Base() {} // MUST be virtual for polymorphism
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int(5); }
    ~Derived() { delete data; }
};

/*
CAVEATS:
- non-virtual destructor => UB
- throwing in destructor => terminate()
*/

/*
============================================================
10. MAIN (ALL DEMOS)
============================================================
*/
int main() {

    cout << "===== INITIALIZER LIST =====\n";
    int r = 5;
    InitDemo id(1, 2, r);
    id.print();

    cout << "\n===== COPY (SHALLOW vs DEEP) =====\n";
    demo_copy();

    cout << "\n===== PRIMITIVE -> CLASS =====\n";
    Complex c(5);
    c.print();

    cout << "\n===== CLASS -> PRIMITIVE =====\n";
    Number n(42);
    int x = n;
    cout << "Converted: " << x << endl;

    cout << "\n===== CLASS -> CLASS =====\n";
    Product p(3, 4);
    Item i1 = p;
    i1.print();

    Item i2(p);
    i2.print();

    cout << "\n===== RAII =====\n";
    RAII r1(100);
    r1.print();

    cout << "\n===== POLYMORPHIC DELETE =====\n";
    Base* b = new Derived();
    delete b; // safe due to virtual destructor

    cout << "\n===== DONE =====\n";
    return 0;


/*
============================================================
11. KERNEL-STYLE MEMORY PATTERNS (SYSTEMS / APPLE / ARM LEVEL)
============================================================
Focus:
- mmap vs new/delete
- aligned allocations
- zero-copy concepts
- manual lifetime control (placement new)
- cache-friendly patterns
*/

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

class MMapFile {
    int fd;
    size_t size;
    void* data;

public:
    MMapFile(const char* filename, size_t sz) : size(sz) {
        fd = open(filename, O_RDWR | O_CREAT, 0666);
        if (fd < 0) {
            perror("open");
            exit(1);
        }

        // Ensure file size
        if (ftruncate(fd, size) != 0) {
            perror("ftruncate");
            exit(1);
        }

        data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
    }

    void* get() { return data; }

    ~MMapFile() {
        munmap(data, size);
        close(fd);
    }
};

/*
CAVEATS:
- mmap avoids copies (zero-copy IO)
- must manually manage lifetime
- page aligned (usually 4KB)
- no constructor/destructor called automatically
*/

/*
============================================================
12. PLACEMENT NEW (CRITICAL FOR LOW-LEVEL SYSTEMS)
============================================================
*/
class PlacementDemo {
public:
    int x;
    PlacementDemo(int v) : x(v) {
        cout << "PlacementDemo ctor\n";
    }
    ~PlacementDemo() {
        cout << "PlacementDemo dtor\n";
    }
};

void placement_new_demo() {
    cout << "\n--- Placement New ---\n";

    void* buffer = malloc(sizeof(PlacementDemo));

    // construct object in pre-allocated memory
    PlacementDemo* obj = new(buffer) PlacementDemo(10);

    cout << "Value: " << obj->x << endl;

    // MUST manually call destructor
    obj->~PlacementDemo();

    free(buffer);
}

/*
CAVEATS:
- constructor NOT automatically paired with free
- destructor must be called manually
- used in allocators / kernel subsystems
*/

/*
============================================================
13. ALIGNED MEMORY (CACHE / SIMD / DMA FRIENDLY)
============================================================
*/
void aligned_memory_demo() {
    cout << "\n--- Aligned Allocation ---\n";

    size_t alignment = 64; // cache line size
    size_t size = 128;

    void* ptr = aligned_alloc(alignment, size);

    if (!ptr) {
        cout << "Allocation failed\n";
        return;
    }

    cout << "Aligned ptr: " << ptr << endl;

    free(ptr);
}

/*
CAVEATS:
- alignment must divide size
- critical for SIMD / GPU / DMA
- avoids cache line splits
*/

/*
============================================================
14. ZERO-COPY CONCEPT (IMPORTANT FOR GPU / NETWORKING)
============================================================
*/
void zero_copy_demo() {
    cout << "\n--- Zero Copy Concept ---\n";

    // Traditional copy
    char src[100] = "hello";
    char dst[100];
    memcpy(dst, src, strlen(src) + 1);

    // Zero-copy idea: just share pointer
    char* ptr = src;

    cout << "Zero-copy ptr: " << ptr << endl;
}

/*
CAVEATS:
- zero-copy reduces latency
- but introduces ownership issues
- used heavily in kernel, drivers, networking stacks
*/

/*
============================================================
15. CACHE FRIENDLY STRUCTURE (STRUCT OF ARRAYS vs ARRAY OF STRUCTS)
============================================================
*/
struct AoS {
    int x, y, z;
};

struct SoA {
    int* x;
    int* y;
    int* z;
};

/*
CAVEATS:
- SoA better for vectorization
- AoS better for locality of single object
- GPU prefers SoA
*/

/*
============================================================
UPDATE MAIN TO INCLUDE SYSTEM DEMOS
============================================================
*/
int main(); // forward declaration fix

int main() {

    cout << "===== INITIALIZER LIST =====\n";
    int r = 5;
    InitDemo id(1, 2, r);
    id.print();

    cout << "\n===== COPY (SHALLOW vs DEEP) =====\n";
    demo_copy();

    cout << "\n===== PRIMITIVE -> CLASS =====\n";
    Complex c(5);
    c.print();

    cout << "\n===== CLASS -> PRIMITIVE =====\n";
    Number n(42);
    int x = n;
    cout << "Converted: " << x << endl;

    cout << "\n===== CLASS -> CLASS =====\n";
    Product p(3, 4);
    Item i1 = p;
    i1.print();

    Item i2(p);
    i2.print();

    cout << "\n===== RAII =====\n";
    RAII r1(100);
    r1.print();

    cout << "\n===== POLYMORPHIC DELETE =====\n";
    Base* b = new Derived();
    delete b;

    placement_new_demo();
    aligned_memory_demo();
    zero_copy_demo();

    cout << "\n===== DONE =====\n";
    return 0;
}
