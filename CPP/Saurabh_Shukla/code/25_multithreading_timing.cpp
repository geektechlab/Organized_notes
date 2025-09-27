#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <future>

// Shared resources
std::mutex mtx1, mtx2;  // Mutex for deadlock example
std::atomic<int> counter(0); // Atomic variable to prevent race conditions

// 🚨 Prevents: Race Condition ✅
/* Bug Fix:
   - Multiple threads updating `counter` simultaneously could lead to incorrect values.
   - Solution: Using `std::atomic<int>` ensures thread-safe operations.
*/
void safeIncrement() {
    for (int i = 0; i < 1000; ++i) {
        counter++; // Atomic operation prevents race conditions.
    }
}

// 🚨 Prevents: Deadlock ✅
/* Bug Fix:
   - If `mtx1` and `mtx2` are locked in different orders by different threads, it causes deadlock.
   - Solution: `std::lock(mtx1, mtx2);` locks both mutexes **simultaneously** in a fixed order.
*/
void deadlockSafeFunction() {
    std::lock(mtx1, mtx2); // Locks both mutexes safely
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
    
    std::cout << "Deadlock-free function executed safely.\n";
}

// 🚨 Prevents: Undefined Behavior in Detached Threads ✅
/* Bug Fix:
   - Detached threads may still be running when `main()` exits, leading to undefined behavior.
   - Solution: Ensure the main function **sleeps** long enough for the detached thread to finish.
*/
void detachedFunction() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    std::cout << "Detached thread executed after main function.\n";
}

// 🚨 Prevents: Inaccurate Timing ✅
/* Bug Fix:
   - `clock()` or other timing methods might not be **precise** in multithreading.
   - Solution: Use `std::chrono::high_resolution_clock` for **accurate timing**.
*/
void timedFunction() {
    auto start = std::chrono::high_resolution_clock::now(); // Start timing

    std::thread t1(safeIncrement);
    std::thread t2(safeIncrement);

    t1.join();
    t2.join(); // Ensure threads complete execution before proceeding

    auto end = std::chrono::high_resolution_clock::now(); // End timing
    std::chrono::duration<double> duration = end - start;

    std::cout << "Final Counter Value: " << counter.load() << "\n";
    std::cout << "Execution Time: " << duration.count() << " seconds\n";
}

// 🚨 Prevents: Manual Thread Management Issues ✅
/* Bug Fix:
   - Manually managing threads (`std::thread`) requires tracking them and calling `join()`.
   - Solution: Use `std::async` to handle threads **automatically**.
*/
int computeValue() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate delay
    return 42; // Sample computation
}

int main() {
    std::cout << "Starting Multithreading Example...\n";

    // ✅ Prevents: Race Conditions using atomic variables
    timedFunction();

    // ✅ Prevents: Deadlock by ensuring proper locking order
    std::thread deadlockThread1(deadlockSafeFunction);
    std::thread deadlockThread2(deadlockSafeFunction);
    deadlockThread1.join();
    deadlockThread2.join();

    // ✅ Prevents: Undefined behavior in detached threads
    std::thread detachedThread(detachedFunction);
    detachedThread.detach(); // Ensures it runs independently

    // ✅ Prevents: Manual Thread Management issues using `std::async`
    std::future<int> futureResult = std::async(std::launch::async, computeValue);
    std::cout << "Waiting for async computation...\n";
    int result = futureResult.get(); // Automatically waits for completion
    std::cout << "Async Computation Result: " << result << "\n";

    // ✅ Ensures detached thread completes execution before `main()` exits
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Main function completed successfully.\n";
    return 0;
}
