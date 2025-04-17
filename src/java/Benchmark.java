import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.util.stream.Collectors;

public class Benchmark {
    static boolean isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i <= Math.sqrt(n); i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    static List<Integer> findPrimesRange(int start, int end) {
        List<Integer> primes = new ArrayList<>();
        for (int n = start; n <= end; n++) {
            if (isPrime(n)) {
                primes.add(n);
            }
        }
        return primes;
    }

    static class PrimeFinderTask implements Callable<List<Integer>> {
        private final int start;
        private final int end;

        PrimeFinderTask(int start, int end) {
            this.start = start;
            this.end = end;
        }

        @Override
        public List<Integer> call() {
            return findPrimesRange(start, end);
        }
    }

    static List<Integer> threadedFindPrimes(int rangeStart, int rangeEnd, int numThreads) throws Exception {
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);
        List<Future<List<Integer>>> futures = new ArrayList<>();
        int chunkSize = (rangeEnd - rangeStart) / numThreads;

        for (int i = 0; i < numThreads; i++) {
            int start = rangeStart + i * chunkSize;
            int end = (i == numThreads - 1) ? rangeEnd : start + chunkSize - 1;
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
        final int rangeStart = 1;
        final int rangeEnd = 100000;
        final int numThreads = 4;

        // Single-threaded benchmark
        long startTime = System.nanoTime();
        List<Integer> singleThreadPrimes = findPrimesRange(rangeStart, rangeEnd);
        double singleThreadTime = (System.nanoTime() - startTime) / 1e9;

        // Multi-threaded benchmark
        startTime = System.nanoTime();
        List<Integer> multiThreadPrimes = threadedFindPrimes(rangeStart, rangeEnd, numThreads);
        double multiThreadTime = (System.nanoTime() - startTime) / 1e9;

        System.out.println("\nJava Benchmark Results:");
        System.out.println("Range: " + rangeStart + " to " + rangeEnd);
        System.out.println("Number of primes found: " + singleThreadPrimes.size());
        System.out.printf("Single-threaded time: %.4f seconds%n", singleThreadTime);
        System.out.printf("Multi-threaded time (%d threads): %.4f seconds%n", numThreads, multiThreadTime);
        System.out.printf("Speedup: %.2fx%n", singleThreadTime / multiThreadTime);
    }
}