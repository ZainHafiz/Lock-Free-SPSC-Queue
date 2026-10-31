# Lock-Free SPSC Queue

A high-throughput, Single-Producer Single-Consumer (SPSC) ring buffer implemented in C++.

I built this project to explore high-performance concurrency models and hardware-level memory interactions. Standard mutex-based queues introduce significant operating system overhead through context switching and thread blocking. To eliminate this bottleneck, this queue relies entirely on `std::atomic` operations and strict memory ordering (acquire/release semantics) to synchronise threads without ever using a lock.

## Key Features

* **Zero-Lock Concurrency:** Utilises C++ `std::atomic` to safely pass data between two threads without OS-level mutexes.
* **Cache-Line Aligned:** The read (`head`) and write (`tail`) pointers are explicitly aligned to 64-byte boundaries (`alignas(64)`) to avoid hardware false sharing across CPU L1 cache lines.
* **Bounded Ring Buffer:** Uses a fixed-capacity buffer allocated at startup, ensuring zero dynamic memory allocation (`new`/`malloc`) on the critical hot path.

## Usage

Header-only. Capacity is a compile-time template parameter.

```cpp
#include "SPSCQueue.hpp"

SPSCQueue<int, 1024> queue;

// Producer thread
if (!queue.push(42)) {
    // queue is full, caller decides whether to retry or drop
}

// Consumer thread
int value;
if (queue.pop(value)) {
    // value is valid
}
```

`push` and `pop` never block. Both return `false` rather than waiting, so the caller controls what happens when the queue is full or empty.

## Memory Ordering

Each index is read with `memory_order_relaxed` by the thread that owns it, since no other thread writes to it. The opposing index is read with `memory_order_acquire` and written with `memory_order_release`, which is what guarantees that the data written into a slot is visible to the other thread before it observes the index change.

## Performance

A simple benchmarking script is included in `main.cpp`. It passes 10,000,000 integers between two threads and reports the sustained transfer rate.

Measured on [CPU model], compiled with `g++ -O3 -std=c++20`, the queue sustains **8 to 11.5 million messages per second** across repeated runs. Throughput varies between runs with scheduling and core placement.

## Limitations

* Correctness is currently demonstrated by the benchmark completing without data loss, not by a formal test suite. Unit tests covering wraparound, full and empty boundaries are the next addition.
* Capacity must be a power of two.
* One slot is left unused so that the full and empty states can be distinguished without an extra counter.
* Indices are not cached locally between operations, so each call reads the opposing atomic rather than reusing a previously observed value.

## Building and Running

This project uses standard C++ features and requires no external dependencies.

```bash
# Clone the repository
git clone https://github.com/ZainHafiz/Lock-Free-SPSC-Queue.git
cd Lock-Free-SPSC-Queue

# Compile the benchmark (requires a C++17/20 compatible compiler)
g++ -O3 -std=c++20 main.cpp -o benchmark -pthread

# Run
./benchmark
```
