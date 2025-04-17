import time
import threading
from concurrent.futures import ThreadPoolExecutor
from typing import List
import tkinter as tk
from tkinter import ttk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import json
import os

def is_prime(n: int) -> bool:
    if n < 2:
        return False
    for i in range(2, int(n ** 0.5) + 1):
        if n % i == 0:
            return False
    return True

def find_primes_range(start: int, end: int) -> List[int]:
    return [n for n in range(start, end + 1) if is_prime(n)]

def threaded_find_primes(range_start: int, range_end: int, num_threads: int = 4) -> List[int]:
    chunk_size = (range_end - range_start) // num_threads
    ranges = [(range_start + i * chunk_size, 
               range_start + (i + 1) * chunk_size if i < num_threads - 1 else range_end)
              for i in range(num_threads)]
    
    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        results = list(executor.map(lambda r: find_primes_range(r[0], r[1]), ranges))
    
    return [prime for sublist in results for prime in sublist]

def run_benchmark(range_start: int, range_end: int, num_threads: int = 4):
    # Non-threaded benchmark
    start_time = time.time()
    single_thread_primes = find_primes_range(range_start, range_end)
    single_thread_time = time.time() - start_time
    
    # Threaded benchmark
    start_time = time.time()
    multi_thread_primes = threaded_find_primes(range_start, range_end, num_threads)
    multi_thread_time = time.time() - start_time
    
    results = {
        "language": "Python",
        "range": [range_start, range_end],
        "primes_found": len(single_thread_primes),
        "single_thread_time": single_thread_time,
        "multi_thread_time": multi_thread_time,
        "speedup": single_thread_time/multi_thread_time
    }
    
    # Save results to file
    with open("python_results.json", "w") as f:
        json.dump(results, f)
    
    return results

class BenchmarkGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Language Performance Comparison")
        
        # Create main frame
        main_frame = ttk.Frame(root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Create matplotlib figure
        self.fig, self.ax = plt.subplots(figsize=(10, 6))
        self.canvas = FigureCanvasTkAgg(self.fig, master=main_frame)
        self.canvas.get_tk_widget().grid(row=0, column=0, columnspan=2)
        
        # Create buttons
        ttk.Button(main_frame, text="Run All Benchmarks", command=self.run_benchmarks).grid(row=1, column=0, pady=10)
        ttk.Button(main_frame, text="Show Results", command=self.show_results).grid(row=1, column=1, pady=10)

    def run_benchmarks(self):
        os.system("cd .. && ..\run_benchmarks.bat")
        self.show_results()

    def show_results(self):
        languages = []
        single_thread = []
        multi_thread = []
        speedups = []
        
        # Read results from all languages
        result_files = {
            "Python": "python_results.json",
            "C": "../c/c_results.json",
            "C++": "../cpp/cpp_results.json",
            "Java": "../java/java_results.json"
        }
        
        for lang, file in result_files.items():
            try:
                with open(file, "r") as f:
                    data = json.load(f)
                    languages.append(lang)
                    single_thread.append(data["single_thread_time"])
                    multi_thread.append(data["multi_thread_time"])
                    speedups.append(data["speedup"])
            except:
                print(f"Could not load results for {lang}")
        
        # Clear previous plot
        self.ax.clear()
        
        # Create grouped bar chart
        x = range(len(languages))
        width = 0.35
        
        self.ax.bar([i - width/2 for i in x], single_thread, width, label='Single Thread', color='skyblue')
        self.ax.bar([i + width/2 for i in x], multi_thread, width, label='Multi Thread', color='lightcoral')
        
        # Add labels and title
        self.ax.set_ylabel('Time (seconds)')
        self.ax.set_title('Language Performance Comparison')
        self.ax.set_xticks(x)
        self.ax.set_xticklabels(languages)
        self.ax.legend()
        
        # Add speedup annotations
        for i, speedup in enumerate(speedups):
            self.ax.text(i, max(single_thread[i], multi_thread[i]), f'Speedup: {speedup:.2f}x', 
                        ha='center', va='bottom')
        
        self.canvas.draw()

if __name__ == "__main__":
    # Run benchmark first
    results = run_benchmark(1, 10000000, 4)
    
    # Then show GUI
    root = tk.Tk()
    app = BenchmarkGUI(root)
    root.mainloop()