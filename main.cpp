#include "SPSCQueue.hpp"
#include <iostream>
#include <thread>
#include <chrono>

const size_t NUM_MESSAGES = 10'000'000;

int main() {
    SPSCQueue<int, 1024> queue;
    
    auto start = std::chrono::high_resolution_clock::now();

    std::thread producer([&]() {
        for (int i = 0; i < NUM_MESSAGES; ++i) {
            while (!queue.push(i)) {
                // busy-wait
            }
        }
    });

    std::thread consumer([&]() {
        int val;
        for (int i = 0; i < NUM_MESSAGES; ++i) {
            while (!queue.pop(val)) {
                // busy-wait
            }
        }
    });

    producer.join();
    consumer.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Transferred " << NUM_MESSAGES << " messages in " 
              << elapsed.count() << " seconds.\n";
    std::cout << "Throughput: " << (NUM_MESSAGES / elapsed.count()) / 1'000'000 
              << " Million ops/sec\n";

    return 0;
}