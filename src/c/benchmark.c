#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    long long start;
    long long end;
    int* primes;
    int* count;
    pthread_mutex_t* mutex;
} ThreadArgs;

int is_prime(long long n) {
    if (n < 2) return 0;
    for (long long i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void find_primes_range(long long start, long long end, int* primes, int* count) {
    for (long long n = start; n <= end; n++) {
        if (is_prime(n)) {
            primes[*count] = n;
            (*count)++;
        }
    }
}

void* thread_task(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    int local_count = 0;
    int* local_primes = malloc(sizeof(int) * 1000000);  // Increased buffer size
    
    for (long long n = thread_args->start; n <= thread_args->end; n++) {
        if (is_prime(n)) {
            local_primes[local_count++] = n;
        }
    }
    
    pthread_mutex_lock(thread_args->mutex);
    for (int i = 0; i < local_count; i++) {
        thread_args->primes[*thread_args->count + i] = local_primes[i];
    }
    *thread_args->count += local_count;
    pthread_mutex_unlock(thread_args->mutex);
    
    free(local_primes);
    return NULL;
}

int main() {
    const long long range_start = 1;
    const long long range_end = 10000000;
    const int num_threads = 4;
    int* single_thread_primes = malloc(sizeof(int) * 1000000);  // Increased buffer size
    int* multi_thread_primes = malloc(sizeof(int) * 1000000);   // Increased buffer size
    int single_count = 0;
    int multi_count = 0;
    clock_t start, end;
    double single_thread_time, multi_thread_time;
    
    // Single-threaded benchmark
    start = clock();
    find_primes_range(range_start, range_end, single_thread_primes, &single_count);
    end = clock();
    single_thread_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    // Multi-threaded benchmark
    pthread_t threads[num_threads];
    ThreadArgs thread_args[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    start = clock();
    long long chunk_size = (range_end - range_start + 1) / num_threads;
    
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].start = range_start + i * chunk_size;
        thread_args[i].end = (i == num_threads - 1) ? range_end : range_start + (i + 1) * chunk_size - 1;
        thread_args[i].primes = multi_thread_primes;
        thread_args[i].count = &multi_count;
        thread_args[i].mutex = &mutex;
        pthread_create(&threads[i], NULL, thread_task, &thread_args[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    end = clock();
    multi_thread_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    // Output results as JSON
    FILE *fp = fopen("c_results.json", "w");
    fprintf(fp, "{\n");
    fprintf(fp, "  \"language\": \"C\",\n");
    fprintf(fp, "  \"range\": [%lld, %lld],\n", range_start, range_end);
    fprintf(fp, "  \"primes_found\": %d,\n", single_count);
    fprintf(fp, "  \"single_thread_time\": %f,\n", single_thread_time);
    fprintf(fp, "  \"multi_thread_time\": %f,\n", multi_thread_time);
    fprintf(fp, "  \"speedup\": %f\n", single_thread_time / multi_thread_time);
    fprintf(fp, "}\n");
    fclose(fp);
    
    printf("\nC Benchmark Results:\n");
    printf("Range: %lld to %lld\n", range_start, range_end);
    printf("Number of primes found: %d\n", single_count);
    printf("Single-threaded time: %.4f seconds\n", single_thread_time);
    printf("Multi-threaded time (%d threads): %.4f seconds\n", num_threads, multi_thread_time);
    printf("Speedup: %.2fx\n", single_thread_time / multi_thread_time);
    
    free(single_thread_primes);
    free(multi_thread_primes);
    return 0;
}