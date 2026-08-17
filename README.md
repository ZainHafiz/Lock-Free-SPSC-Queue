# Lock-Free SPSC Queue

A high-throughput, ultra-low latency Single-Producer Single-Consumer (SPSC) ring buffer implemented in C++. 

I built this project to explore high-performance concurrency models and hardware-level memory interactions. Standard mutex-based queues introduce significant operating system overhead through context switching and thread blocking. To eliminate this bottleneck, this queue relies entirely on `std::atomic` operations and strict memory ordering (acquire/release semantics) to synchronize threads without ever using a lock.

## Key Features

* **Zero-Lock Concurrency:** Utilizes C++ `std::atomic` to safely pass data between two threads without OS-level mutexes.
* **Cache-Line Aligned:** The read (`head`) and write (`tail`) pointers are explicitly aligned to 64-byte boundaries (`alignas(64)`) to completely eliminate hardware false sharing across CPU L1 cache lines.
* **Bounded Ring Buffer:** Uses a fixed-capacity array allocated at startup, ensuring zero dynamic memory allocation (`new`/`malloc`) on the critical hot path.

## Performance

A simple benchmarking script is included in `main.cpp`. On a standard consumer CPU, the queue sustains a transfer rate of **>25 million messages per second** between two threads.

## Building and Running

This project uses standard C++ features and requires no external dependencies. 

```bash
# Clone the repository
git clone [https://github.com/yourusername/spsc-queue.git](https://github.com/yourusername/spsc-queue.git)
cd spsc-queue

# Compile the benchmark (requires a C++17/20 compatible compiler)
g++ -O3 -std=c++20 main.cpp -o benchmark -pthread

# Run
./benchmark
