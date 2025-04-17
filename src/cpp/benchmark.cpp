#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <mutex>

std::mutex print_mutex;

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return false;
    }
    return true;
}

std::vector<int> find_primes_range(int start, int end) {
    std::vector<int> primes;
    for (int n = start; n <= end; n++) {
        if (is_prime(n)) {
            primes.push_back(n);
        }
    }
    return primes;
}

void thread_task(int start, int end, std::vector<int>& result, std::mutex& result_mutex) {
    std::vector<int> local_primes = find_primes_range(start, end);
    
    std::lock_guard<std::mutex> lock(result_mutex);
    result.insert(result.end(), local_primes.begin(), local_primes.end());
}

std::vector<int> threaded_find_primes(int range_start, int range_end, int num_threads) {
    std::vector<std::thread> threads;
    std::vector<int> result;
    std::mutex result_mutex;
    
    int chunk_size = (range_end - range_start) / num_threads;
    
    for (int i = 0; i < num_threads; i++) {
        int start = range_start + i * chunk_size;
        int end = (i == num_threads - 1) ? range_end : start + chunk_size - 1;
        threads.push_back(std::thread(thread_task, start, end, std::ref(result), std::ref(result_mutex)));
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return result;
}

int main() {
    const int range_start = 1;
    const int range_end = 100000;
    const int num_threads = 4;
    
    // Single-threaded benchmark
    auto start_time = std::chrono::high_resolution_clock::now();
    auto single_thread_primes = find_primes_range(range_start, range_end);
    auto single_thread_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start_time);
    
    // Multi-threaded benchmark
    start_time = std::chrono::high_resolution_clock::now();
    auto multi_thread_primes = threaded_find_primes(range_start, range_end, num_threads);
    auto multi_thread_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start_time);
    
    std::cout << "\nC++ Benchmark Results:" << std::endl;
    std::cout << "Range: " << range_start << " to " << range_end << std::endl;
    std::cout << "Number of primes found: " << single_thread_primes.size() << std::endl;
    std::cout << "Single-threaded time: " << single_thread_duration.count() / 1000.0 << " seconds" << std::endl;
    std::cout << "Multi-threaded time (" << num_threads << " threads): " 
              << multi_thread_duration.count() / 1000.0 << " seconds" << std::endl;
    std::cout << "Speedup: " << (float)single_thread_duration.count() / multi_thread_duration.count() << "x" << std::endl;
    
    return 0;
}