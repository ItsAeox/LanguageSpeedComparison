@echo off
echo Running Language Performance Benchmarks...

echo.
echo Compiling and running C program...
cd src/c
gcc benchmark.c -o benchmark -lpthread -lm
benchmark.exe

echo.
echo Compiling and running C++ program...
cd ../cpp
g++ benchmark.cpp -o benchmark -lpthread
benchmark.exe

echo.
echo Running Python program...
cd ../python
python benchmark.py

echo.
echo Compiling and running Java program...
cd ../java
javac Benchmark.java
java Benchmark

cd ../..
echo.
echo All benchmarks completed!