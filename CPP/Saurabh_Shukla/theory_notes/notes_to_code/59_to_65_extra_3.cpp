/*
===========================================================
    FINAL BOSS: LOCK-FREE + KERNEL MEMORY + CONCURRENCY
===========================================================

Covers:
✔ Lock-free stack (atomic CAS)
✔ ABA problem awareness
✔ Thread-local allocator (no contention)
✔ False sharing avoidance
✔ Memory ordering (relaxed vs acquire/release)
✔ Hazard awareness (why GC schemes exist)

WARNING:
- This is simplified for learning
- Production systems add hazard pointers / epoch GC
===========================================================
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <cassert>

using namespace std;

/*
===========================================================
1. LOCK-FREE STACK (CORE PATTERN)
===========================================================

- Uses atomic compare_exchange (CAS)
- No locks → high performance
- Used in allocators, schedulers

Key idea:
retry loop until CAS succeeds
*/

template <typename T>
class LockFreeStack {
private:
    struct Node {
        T data;
        Node* next;
    };

    atomic<Node*> head;

public:
    LockFreeStack() : head(nullptr) {}

    void push(T val) {
        Node* newNode = new Node{val, nullptr};

        // CAS loop
        do {
            newNode->next = head.load(memory_order_relaxed);
        } while (!head.compare_exchange_weak(
            newNode->next,
            newNode,
            memory_order_release,
            memory_order_relaxed));
    }

    bool pop(T& result) {
        Node* oldHead;

        do {
            oldHead = head.load(memory_order_acquire);

            if (!oldHead) return false;

        } while (!head.compare_exchange_weak(
            oldHead,
            oldHead->next,
            memory_order_acquire,
            memory_order_relaxed));

        result = oldHead->data;

        /*
        ⚠️ BIG ISSUE:
        Another thread may still read oldHead → ABA problem
        Safe reclamation needed (hazard pointers / epoch GC)
        */
        delete oldHead;

        return true;
    }
};

/*
===========================================================
2. ABA PROBLEM (INTERVIEW KILLER)
===========================================================

Thread A reads pointer A
Thread B changes A → B → A
Thread A thinks nothing changed → WRONG

Solution:
- tagged pointers
- hazard pointers
- epoch GC

We simulate tagged pointer approach
*/

struct TaggedPtr {
    void* ptr;
    size_t tag;
};

/*
===========================================================
3. THREAD-LOCAL ALLOCATOR (NO LOCK CONTENTION)
===========================================================

Instead of global malloc lock:
- each thread gets its own pool

Used in:
- tcmalloc
- jemalloc
*/

class ThreadLocalPool {
private:
    struct Node {
        Node* next;
    };

    Node* freeList;

public:
    ThreadLocalPool() : freeList(nullptr) {}

    void* allocate(size_t size) {
        if (freeList) {
            Node* n = freeList;
            freeList = freeList->next;
            return n;
        }
        return operator new(size);
    }

    void deallocate(void* ptr) {
        Node* n = static_cast<Node*>(ptr);
        n->next = freeList;
        freeList = n;
    }
};

// Each thread gets its own allocator
thread_local ThreadLocalPool localPool;

void threadLocalDemo() {
    cout << "\n--- Thread Local Allocator ---\n";

    void* p = localPool.allocate(sizeof(int));
    localPool.deallocate(p);
}

/*
===========================================================
4. FALSE SHARING AVOIDANCE
===========================================================

Two threads updating different vars
→ same cache line → slow

Fix:
- padding / alignas(64)
*/

struct alignas(64) Counter {
    atomic<int> value;
};

void falseSharingDemo() {
    cout << "\n--- False Sharing Demo ---\n";

    Counter c1, c2;

    auto work = [](Counter& c) {
        for (int i = 0; i < 1000000; ++i)
            c.value.fetch_add(1, memory_order_relaxed);
    };

    thread t1(work, ref(c1));
    thread t2(work, ref(c2));

    t1.join();
    t2.join();

    cout << "Done\n";
}

/*
===========================================================
5. MEMORY ORDERING (DEEP INTERVIEW TOPIC)
===========================================================

memory_order_relaxed:
- no ordering, fastest

memory_order_acquire:
- read barrier

memory_order_release:
- write barrier

Rule:
Producer → release
Consumer → acquire
*/

atomic<int> dataReady{0};
int sharedData = 0;

void producer() {
    sharedData = 42;
    dataReady.store(1, memory_order_release);
}

void consumer() {
    while (dataReady.load(memory_order_acquire) == 0);

    cout << "Data: " << sharedData << endl;
}

/*
===========================================================
6. WHY LOCK-FREE IS HARD (REALITY)
===========================================================

Problems:
- ABA
- memory reclamation
- starvation
- debugging nightmare

Production solutions:
- Hazard pointers
- Epoch-based GC
- RCU (Linux kernel)
*/

/*
===========================================================
MAIN
===========================================================
*/

int main() {

    cout << "\n--- Lock-Free Stack ---\n";

    LockFreeStack<int> stack;

    stack.push(1);
    stack.push(2);

    int val;
    while (stack.pop(val)) {
        cout << val << endl;
    }

    threadLocalDemo();

    falseSharingDemo();

    cout << "\n--- Producer/Consumer ---\n";

    thread t1(producer);
    thread t2(consumer);

    t1.join();
    t2.join();

    cout << "\n--- FINAL BOSS COMPLETE ---\n";

    return 0;
}