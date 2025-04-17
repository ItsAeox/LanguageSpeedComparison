#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <mutex>
#include <fstream>

std::mutex print_mutex;

bool is_prime(long long n) {
    if (n < 2) return false;
    for (long long i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return false;
    }
    return true;
}

std::vector<int> find_primes_range(long long start, long long end) {
    std::vector<int> primes;
    for (long long n = start; n <= end; n++) {
        if (is_prime(n)) {
            primes.push_back(n);
        }
    }
    return primes;
}

void thread_task(long long start, long long end, std::vector<int>& result, std::mutex& result_mutex) {
    std::vector<int> local_primes = find_primes_range(start, end);
    
    std::lock_guard<std::mutex> lock(result_mutex);
    result.insert(result.end(), local_primes.begin(), local_primes.end());
}

std::vector<int> threaded_find_primes(long long range_start, long long range_end, int num_threads) {
    std::vector<std::thread> threads;
    std::vector<int> result;
    std::mutex result_mutex;
    
    long long chunk_size = (range_end - range_start) / num_threads;
    
    for (int i = 0; i < num_threads; i++) {
        long long start = range_start + i * chunk_size;
        long long end = (i == num_threads - 1) ? range_end : start + chunk_size - 1;
        threads.push_back(std::thread(thread_task, start, end, std::ref(result), std::ref(result_mutex)));
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return result;
}

int main() {
    std::cout << "Running C++ benchmark..." << std::endl;
    
    const long long range_start = 1;
    const long long range_end = 10000000;
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
    
    double single_thread_time = single_thread_duration.count() / 1000.0;
    double multi_thread_time = multi_thread_duration.count() / 1000.0;
    
    // Output results as JSON
    std::ofstream json_file("cpp_results.json");
    json_file << "{\n";
    json_file << "  \"language\": \"C++\",\n";
    json_file << "  \"range\": [" << range_start << ", " << range_end << "],\n";
    json_file << "  \"primes_found\": " << single_thread_primes.size() << ",\n";
    json_file << "  \"single_thread_time\": " << single_thread_time << ",\n";
    json_file << "  \"multi_thread_time\": " << multi_thread_time << ",\n";
    json_file << "  \"speedup\": " << single_thread_time / multi_thread_time << "\n";
    json_file << "}\n";
    json_file.close();
    
    std::cout << "C++ benchmark completed." << std::endl;
    
    return 0;
}