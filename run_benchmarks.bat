@echo off
echo Starting Language Performance Benchmarks...
echo.

cd src/c
gcc benchmark.c -o benchmark -lpthread -lm
.\benchmark.exe

cd ../cpp
g++ benchmark.cpp -o benchmark -lpthread
.\benchmark.exe

cd ../java
javac Benchmark.java
java Benchmark

cd ../python
python benchmark.py

cd ../..