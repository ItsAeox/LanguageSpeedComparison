import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.io.FileWriter;
import java.io.IOException;

public class Benchmark {
    static boolean isPrime(long n) {
        if (n < 2) return false;
        for (long i = 2; i <= Math.sqrt(n); i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    static List<Integer> findPrimesRange(long start, long end) {
        List<Integer> primes = new ArrayList<>();
        for (long n = start; n <= end; n++) {
            if (isPrime(n)) {
                primes.add((int)n);
            }
        }
        return primes;
    }

    static class PrimeFinderTask implements Callable<List<Integer>> {
        private final long start;
        private final long end;

        PrimeFinderTask(long start, long end) {
            this.start = start;
            this.end = end;
        }

        @Override
        public List<Integer> call() {
            return findPrimesRange(start, end);
        }
    }

    static List<Integer> threadedFindPrimes(long rangeStart, long rangeEnd, int numThreads) throws Exception {
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);
        List<Future<List<Integer>>> futures = new ArrayList<>();
        long chunkSize = (rangeEnd - rangeStart) / numThreads;

        for (int i = 0; i < numThreads; i++) {
            long start = rangeStart + i * chunkSize;
            long end = (i == numThreads - 1) ? rangeEnd : start + chunkSize - 1;
            futures.add(executor.submit(new PrimeFinderTask(start, end)));
        }

        List<Integer> result = new ArrayList<>();
        for (Future<List<Integer>> future : futures) {
            result.addAll(future.get());
        }

        executor.shutdown();
        return result;
    }

    public static void main(String[] args) throws Exception {
        System.out.println("Running Java benchmark...");
        
        final long rangeStart = 1;
        final long rangeEnd = 10000000;
        final int numThreads = 4;

        // Single-threaded benchmark
        long startTime = System.nanoTime();
        List<Integer> singleThreadPrimes = findPrimesRange(rangeStart, rangeEnd);
        double singleThreadTime = (System.nanoTime() - startTime) / 1e9;

        // Multi-threaded benchmark
        startTime = System.nanoTime();
        List<Integer> multiThreadPrimes = threadedFindPrimes(rangeStart, rangeEnd, numThreads);
        double multiThreadTime = (System.nanoTime() - startTime) / 1e9;

        // Save results as JSON
        try (FileWriter file = new FileWriter("java_results.json")) {
            file.write("{\n");
            file.write("  \"language\": \"Java\",\n");
            file.write("  \"range\": [" + rangeStart + ", " + rangeEnd + "],\n");
            file.write("  \"primes_found\": " + singleThreadPrimes.size() + ",\n");
            file.write("  \"single_thread_time\": " + singleThreadTime + ",\n");
            file.write("  \"multi_thread_time\": " + multiThreadTime + ",\n");
            file.write("  \"speedup\": " + (singleThreadTime / multiThreadTime) + "\n");
            file.write("}\n");
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Java benchmark completed.");
    }
}