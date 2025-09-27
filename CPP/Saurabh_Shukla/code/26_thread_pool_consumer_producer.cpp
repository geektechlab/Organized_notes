#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <future>
#include <vector>
#include <queue>
#include <condition_variable>
#include <functional>

// ✅ Shared Resources for Synchronization
std::mutex mtx1, mtx2;  // Mutex for deadlock example
std::atomic<int> counter(0); // Atomic variable to prevent race conditions
std::queue<int> dataQueue; // Shared queue for producer-consumer
std::mutex queueMutex; // Mutex for protecting queue
std::condition_variable queueCV; // Condition variable for producer-consumer
bool done = false; // Flag to stop consumer

// 🚨 Prevents: Race Condition ✅ (Atomic counter)
void safeIncrement() {
    for (int i = 0; i < 1000; ++i) {
        counter++; // Atomic operation prevents race conditions.
    }
}

// 🚨 Prevents: Deadlock ✅ (Using std::lock for proper mutex locking)
void deadlockSafeFunction() {
    std::lock(mtx1, mtx2); // Locks both mutexes safely
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
    std::cout << "Deadlock-free function executed safely.\n";
}

// 🚨 Prevents: Undefined Behavior in Detached Threads ✅
void detachedFunction() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    std::cout << "Detached thread executed after main function.\n";
}

// 🚨 Prevents: Inaccurate Timing ✅ (Using std::chrono)
void timedFunction() {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::thread t1(safeIncrement);
    std::thread t2(safeIncrement);
    
    t1.join();
    t2.join(); // Ensure threads complete execution before proceeding

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "Final Counter Value: " << counter.load() << "\n";
    std::cout << "Execution Time: " << duration.count() << " seconds\n";
}

// 🚨 Prevents: Manual Thread Management Issues ✅ (Using std::async)
int computeValue() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate delay
    return 42; // Sample computation
}

// ✅ Producer-Consumer Model
// 🚨 Prevents: Busy-Waiting ✅ (Using condition variables)
void producer() {
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
        std::lock_guard<std::mutex> lock(queueMutex);
        dataQueue.push(i);
        std::cout << "Produced: " << i << "\n";
        queueCV.notify_one(); // Notify consumer
    }
    done = true; // Stop flag for consumer
    queueCV.notify_one();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCV.wait(lock, [] { return !dataQueue.empty() || done; }); // Wait until queue is non-empty or done
        while (!dataQueue.empty()) {
            int item = dataQueue.front();
            dataQueue.pop();
            std::cout << "Consumed: " << item << "\n";
        }
        if (done) break; // Stop if producer is done
    }
}

// ✅ Thread Pool Implementation
// 🚨 Prevents: Overhead of Creating/Destroying Threads ✅ (Using a fixed pool)
class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return !tasks.empty() || stop; });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task(); // Execute the task
                }
            });
        }
    }

    // Enqueue task
    void enqueue(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    // Destructor (Ensures all threads finish execution)
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};

int main() {
    std::cout << "Starting Extended Multithreading Example...\n";

    // ✅ Prevents: Race Conditions using atomic variables
    timedFunction();

    // ✅ Prevents: Deadlock by ensuring proper locking order
    std::thread deadlockThread1(deadlockSafeFunction);
    std::thread deadlockThread2(deadlockSafeFunction);
    deadlockThread1.join();
    deadlockThread2.join();

    // ✅ Prevents: Undefined behavior in detached threads
    std::thread detachedThread(detachedFunction);
    detachedThread.detach(); // Runs independently

    // ✅ Prevents: Manual Thread Management issues using `std::async`
    std::future<int> futureResult = std::async(std::launch::async, computeValue);
    std::cout << "Waiting for async computation...\n";
    int result = futureResult.get();
    std::cout << "Async Computation Result: " << result << "\n";

    // ✅ Start Producer-Consumer Threads
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);
    producerThread.join();
    consumerThread.join();

    // ✅ Demonstrate Thread Pool Usage
    ThreadPool pool(3); // Create a pool with 3 worker threads
    pool.enqueue([] { std::cout << "Task 1 executed by thread pool\n"; });
    pool.enqueue([] { std::cout << "Task 2 executed by thread pool\n"; });
    pool.enqueue([] { std::cout << "Task 3 executed by thread pool\n"; });

    // ✅ Ensures detached thread completes execution before `main()` exits
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Main function completed successfully.\n";
    return 0;
}
