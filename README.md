# Programming Language Performance Comparison

This project compares the performance of different programming languages (C, C++, Java, and Python) in both single-threaded and multi-threaded scenarios. The benchmark performs CPU-intensive calculations (finding prime numbers) to demonstrate the relative performance and threading capabilities of each language.

## Features

- Benchmarks four popular programming languages: C, C++, Java, and Python
- Compares single-threaded vs multi-threaded performance
- Tests CPU-intensive prime number calculations
- Visualizes results with an interactive GUI
- Tests range: 1 to 10,000,000

## Requirements

To run all benchmarks, you'll need:

### C/C++
- GCC compiler with pthread support
- Make sure `gcc` and `g++` are available in your system PATH

### Java
- Java Development Kit (JDK) 8 or higher
- `java` and `javac` commands available in your system PATH

### Python
- Python 3.6 or higher
- Required Python packages:
  ```
  matplotlib
  tkinter (usually comes with Python)
  ```

## Installation

1. Clone this repository:
```bash
git clone https://github.com/yourusername/language-comparison.git
cd language-comparison
```

2. Install Python dependencies:
```bash
pip install matplotlib
```

## Running the Benchmarks

### Windows
Simply run the batch file:
```bash
run_benchmarks.bat
```

### Linux/Mac
Run each benchmark individually or create a shell script:
```bash
# C benchmark
cd src/c
gcc benchmark.c -o benchmark -lpthread -lm
./benchmark

# C++ benchmark
cd ../cpp
g++ benchmark.cpp -o benchmark -lpthread
./benchmark

# Java benchmark
cd ../java
javac Benchmark.java
java Benchmark

# Python benchmark
cd ../python
python benchmark.py
```

## Project Structure

```
.
├── run_benchmarks.bat     # Windows batch file to run all benchmarks
└── src/
    ├── c/                 # C implementation
    ├── cpp/               # C++ implementation
    ├── java/             # Java implementation
    └── python/           # Python implementation with GUI visualization
```

## How It Works

1. Each implementation performs the same task: finding prime numbers in a range from 1 to 10,000,000
2. The task is performed in both single-threaded and multi-threaded modes (4 threads)
3. Each language implementation:
   - Measures execution time for both modes
   - Calculates the speedup ratio
   - Saves results to a JSON file
4. The Python implementation provides a GUI visualization comparing all results

## Results Interpretation

The GUI will show:
- Bar chart comparing single-threaded and multi-threaded performance
- Execution times in seconds for each language
- Speedup ratios (single-threaded time / multi-threaded time)

Typically:
- C and C++ show the best raw performance
- Java demonstrates good balance of performance and ease of use
- Python's performance is affected by the Global Interpreter Lock (GIL) in CPU-bound tasks

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## License

This project is licensed under the MIT License - see the LICENSE file for details.