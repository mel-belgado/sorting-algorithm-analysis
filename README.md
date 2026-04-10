# Empirical Analysis of Sorting Algorithms  

This project implements and compares the performance of six sorting algorithms: Selection Sort, Bubble Sort,
Insertion Sort, Mergesort, Quicksort, and Heapsort using C. It measures execution time by sorting randomly generated or increasing sequences of N integers to analyze algorithmic efficiency.  

### Notes:  
Quicksort uses a random pivot selection.  
Mergesort uses a pre-allocated array to minimize memory allocation overhead during recursion.  

### Usage:  
Compile the source code using gcc:
```bash
gcc Analysis.c -o Analysis
```
Run executable:
```bash
./Analysis
```  

An output.txt file will be generated after execution containing original and sorted data.
