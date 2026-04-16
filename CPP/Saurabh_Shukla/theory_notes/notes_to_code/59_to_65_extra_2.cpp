/*
===========================================================
    KERNEL-LEVEL C++ MEMORY MODEL (APPLE / ARM LEVEL)
===========================================================

Covers:
✔ Custom STL allocator
✔ Pool / slab allocator (O(1) allocation)
✔ Intrusive reference counting
✔ Placement new + manual lifetime
✔ Cache alignment & false sharing
✔ Object vs memory separation

Think like:
- Kernel allocator (slab, buddy)
- No unnecessary heap calls
- Deterministic latency
===========================================================
*/

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <new>
#include <cstddef>

using namespace std;

/*
===========================================================
1. OBJECT vs MEMORY (CRITICAL CONCEPT)
===========================================================

Memory != Object

malloc → raw memory
placement new → construct object
destructor → destroy object
free → release memory

Interview killer question.
*/

void objectVsMemoryDemo() {
    cout << "\n--- Object vs Memory ---\n";

    void* raw = operator new(sizeof(int));  // allocate raw memory

    int* p = new (raw) int(42);             // construct object

    cout << *p << endl;

    p->~int();                              // destroy object
    operator delete(raw);                   // free memory
}

/*
===========================================================
2. CUSTOM STL ALLOCATOR
===========================================================

Allocator controls:
- how memory is allocated
- where it comes from

Used by:
vector<T, Allocator>

Interview:
- must define allocate/deallocate
*/

template <typename T>
class SimpleAllocator {
public:
    using value_type = T;

    SimpleAllocator() = default;

    template <typename U>
    SimpleAllocator(const SimpleAllocator<U>&) {}

    T* allocate(size_t n) {
        cout << "[Alloc] " << n << " elements\n";
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t) {
        cout << "[Dealloc]\n";
        ::operator delete(p);
    }
};

void customAllocatorDemo() {
    cout << "\n--- Custom Allocator Demo ---\n";

    vector<int, SimpleAllocator<int>> v;

    v.push_back(1);
    v.push_back(2);
}

/*
===========================================================
3. SLAB / POOL ALLOCATOR (KERNEL STYLE)
===========================================================

Goal:
- O(1) allocation
- no fragmentation
- reuse memory blocks

Used in:
- Linux slab allocator
- GPU drivers (KGSL style pools)
*/

class MemoryPool {
private:
    struct Block {
        Block* next;
    };

    Block* freeList;
    size_t blockSize;

public:
    MemoryPool(size_t size, size_t count) : blockSize(size) {
        freeList = nullptr;

        // Pre-allocate
        char* pool = new char[size * count];

        for (size_t i = 0; i < count; ++i) {
            Block* b = reinterpret_cast<Block*>(pool + i * size);
            b->next = freeList;
            freeList = b;
        }
    }

    void* allocate() {
        assert(freeList && "Out of memory!");

        Block* b = freeList;
        freeList = freeList->next;

        return b;
    }

    void deallocate(void* ptr) {
        Block* b = static_cast<Block*>(ptr);
        b->next = freeList;
        freeList = b;
    }
};

class PoolObject {
public:
    int x;

    PoolObject(int v) : x(v) {
        cout << "Construct " << x << endl;
    }

    ~PoolObject() {
        cout << "Destruct " << x << endl;
    }
};

void poolDemo() {
    cout << "\n--- Pool Allocator Demo ---\n";

    MemoryPool pool(sizeof(PoolObject), 10);

    void* mem = pool.allocate();

    PoolObject* obj = new (mem) PoolObject(100);

    obj->~PoolObject();        // manual destroy
    pool.deallocate(mem);      // return to pool
}

/*
===========================================================
4. INTRUSIVE REFERENCE COUNTING
===========================================================

shared_ptr:
- external control block (extra allocation)

intrusive_ptr:
- ref count inside object (faster, kernel style)

Used in:
- Chromium
- game engines
*/

class Intrusive {
private:
    int refCount = 0;

public:
    void addRef() {
        ++refCount;
    }

    void release() {
        if (--refCount == 0) {
            delete this;
        }
    }

protected:
    virtual ~Intrusive() {
        cout << "Intrusive destroyed\n";
    }
};

class MyObject : public Intrusive {
public:
    int val;
    MyObject(int v) : val(v) {}
};

void intrusiveDemo() {
    cout << "\n--- Intrusive Ptr Demo ---\n";

    MyObject* obj = new MyObject(10);

    obj->addRef();
    obj->addRef();

    obj->release();
    obj->release(); // deletes here
}

/*
===========================================================
5. CACHE ALIGNMENT & FALSE SHARING (ARM CRITICAL)
===========================================================

False sharing:
- two threads modify different variables
- same cache line → performance drop

Fix:
- align to cache line (typically 64 bytes)
*/

struct alignas(64) CacheLineSafe {
    int x;
};

void alignmentDemo() {
    cout << "\n--- Alignment Demo ---\n";

    cout << "Size: " << sizeof(CacheLineSafe) << endl;
    cout << "Alignment: " << alignof(CacheLineSafe) << endl;
}

/*
===========================================================
6. PLACEMENT NEW IN ALLOCATOR CONTEXT
===========================================================

Allocator gives memory
Placement new constructs object
*/

template <typename T>
class PoolAllocator {
private:
    MemoryPool& pool;

public:
    using value_type = T;

    PoolAllocator(MemoryPool& p) : pool(p) {}

    T* allocate(size_t n) {
        assert(n == 1); // simple case
        return static_cast<T*>(pool.allocate());
    }

    void deallocate(T* p, size_t) {
        pool.deallocate(p);
    }
};

/*
===========================================================
MAIN
===========================================================
*/

int main() {

    objectVsMemoryDemo();

    customAllocatorDemo();

    poolDemo();

    intrusiveDemo();

    alignmentDemo();

    cout << "\n--- END KERNEL LEVEL ---\n";

    return 0;
}