#pragma once
#include <atomic>
#include <cstddef>
#include <vector>
#include <new>

// alignas(64) prevents "false sharing" by keeping variables on separate CPU cache lines
template <typename T, size_t Capacity>
class SPSCQueue {
private:
    std::vector<T> buffer;
    
    // alignas(64) matches the typical L1 cache line size
    alignas(64) std::atomic<size_t> head{0}; 
    alignas(64) std::atomic<size_t> tail{0};

public:
    SPSCQueue() : buffer(Capacity) {}

    bool push(const T& item) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) % Capacity;

        // memory_order_acquire ensures we see the latest consumer reads
        if (next_tail == head.load(std::memory_order_acquire)) {
            return false; // Queue is full
        }

        buffer[current_tail] = item;
        
        // memory_order_release ensures the item write is visible before tail updates
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        size_t current_head = head.load(std::memory_order_relaxed);

        // memory_order_acquire ensures we see the latest producer writes
        if (current_head == tail.load(std::memory_order_acquire)) {
            return false; // Queue is empty
        }

        item = buffer[current_head];
        
        // memory_order_release ensures item read is complete before head updates
        head.store((current_head + 1) % Capacity, std::memory_order_release);
        return true;
    }
};