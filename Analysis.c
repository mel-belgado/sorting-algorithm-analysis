#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
const unsigned long int MAX_RANGE = 1000000; //max value for unsigned long int

void copyArray(unsigned long int *source, unsigned long int *dest, size_t n) { //copy the original unsorted array
    for (size_t i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

int askUser() {
    int choice;
    printf("Select data generation method: \n");
    printf("  1. Randomly Generate Integers\n");
    printf("  2. Increasing Sequence\n");
    printf("Enter choice: ");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        while (getchar() != '\n');
        printf("Invalid input. Enter 1 or 2: ");
    }
    return choice;
}

void genRandInt(unsigned long int *arr, size_t size) { //generate random integer
    for (size_t i = 0; i < size; i++) {
        unsigned long int r = (unsigned long int)rand() * (RAND_MAX + 1U) + rand();
        arr[i] = (unsigned long int)(r % (MAX_RANGE + 1));
    }
}

void genIncreasing(unsigned long int *arr, size_t size, unsigned long int start){ //generate increasing sequence
    for (size_t i = 0; i < size; i++) {
        arr[i] = start + i;
    }
}

void showArr(unsigned long int *arr, size_t n){
    for (size_t i = 0; i < n; i++) { 
        printf("%lu ", arr[i]);
    }
    printf("\n");
}

void swap(unsigned long int *a, unsigned long int *b);
int partition(unsigned long int arr[], int low, int high);
void merge(unsigned long int *arr, unsigned long int *temp, size_t l, size_t m, size_t r);
void heapify(unsigned long int *arr, size_t n, size_t i);

//sorting algorithms
void bubbleSort(unsigned long int *arr, size_t n);
void selectionSort(unsigned long int *arr, size_t n);
void insertionSort(unsigned long int *arr, size_t n);
void quickSort(unsigned long int *arr, int low, int high);
void mergeSort(unsigned long int *arr, unsigned long int *temp, size_t l, size_t r);
void heapSort(unsigned long int *arr, size_t n);


int main(int argc, char** argv) {
    size_t n; //number of integers to be sorted
    printf("Enter number of integers: ");
    if (scanf("%zu", &n) != 1 || n == 0) {
        printf("Enter positive number.");
        return 1;
    } 

    //allocate memory to heap instead of stack to prevent stack overflow when user inputs large number
    //dont use variable length arrays
    unsigned long int *arr = malloc(n * sizeof(unsigned long int)); //original array
    unsigned long int *arrCopy = malloc(n * sizeof(unsigned long int)); //copy
    unsigned long int *temp = malloc(n * sizeof(unsigned long int)); //pre-allocated array for merge sort
    if (!arr || !arrCopy || !temp) {
        printf("Memory allocation failed.\n");
        free(arr);
        free(arrCopy);
        free(temp);
        return 1;
    }
    int choice = askUser();
    srand((unsigned int)time(NULL)); //seed to prevent same numbers
    if(choice == 1){
        genRandInt(arr, n);
    } else if (choice == 2){
        unsigned long int x; //starting point
        printf("Enter starting value: ");
        if (scanf("%lu", &x) != 1) {
            printf("Invalid input.\n");
            free(arr); 
            free(arrCopy); 
            free(temp);
            return 1;
        }
        genIncreasing(arr, n, x);
    } else {
        printf("Invalid choice.");
        free(arr);
        free(arrCopy);
        free(temp);
        return 1;
    }

    printf("\nTesting performance...please wait. \n");

    clock_t start, end;
    double time_bubble, time_selection, time_insertion, time_quick, time_merge, time_heap;

    // TEST BUBBLE SORT
    copyArray(arr, arrCopy, n);
    start = clock();
    bubbleSort(arrCopy, n);
    end = clock();
    time_bubble = ((double)(end - start)) / CLOCKS_PER_SEC;

    // TEST SELECTION SORT
    copyArray(arr, arrCopy, n);
    start = clock();
    selectionSort(arrCopy, n);
    end = clock();
    time_selection = ((double)(end - start)) / CLOCKS_PER_SEC;

    // TEST INSERTION SORT
    copyArray(arr, arrCopy, n);
    start = clock();
    insertionSort(arrCopy, n);
    end = clock();
    time_insertion = ((double)(end - start)) / CLOCKS_PER_SEC;

    // TEST QUICK SORT
    copyArray(arr, arrCopy, n);
    start = clock();
    quickSort(arrCopy, 0, (int)n - 1);
    end = clock();
    time_quick = ((double)(end - start)) / CLOCKS_PER_SEC;

    // TEST MERGE SORT
    copyArray(arr, arrCopy, n);
    start = clock();
    mergeSort(arrCopy, temp, 0, n - 1);
    end = clock();
    time_merge = ((double)(end - start)) / CLOCKS_PER_SEC;

    // TEST HEAP SORT
    copyArray(arr, arrCopy, n);
    start = clock();
    heapSort(arrCopy, n);
    end = clock();
    time_heap = ((double)(end - start)) / CLOCKS_PER_SEC;

    // DISPLAY SUMMARY TABLE
    printf("\nPerformance Results (N = %zu):\n", n);
    printf("Bubble Sort:   %f seconds\n", time_bubble);
    printf("Selection Sort:    %f seconds\n", time_selection);
    printf("Insertion Sort:   %f seconds\n", time_insertion);
    printf("Quick Sort:    %f seconds\n", time_quick);
    printf("Merge Sort:    %f seconds\n", time_merge);
    printf("Heap Sort:    %f seconds\n", time_heap);

    //WRITE OUTPUT FILE
    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        free(arr);
        free(arrCopy);
        free(temp);
        return 1;
    }
    fprintf(fp, "Unsorted:\n");
    size_t newline = 0;
    for (size_t i = 0; i < n; i++) { 
        fprintf(fp, "%lu ", arr[i]);
        newline++;
        if(newline == 10){
            fprintf(fp, "\n");
            newline = 0;
        }
    }
    fprintf(fp, "\n\n");
    
    fprintf(fp, "Sorted: \n");
    newline = 0;
    for (size_t i = 0; i < n; i++) {
        fprintf(fp, "%lu ", arrCopy[i]);
        newline++; 
        if(newline == 10){
            fprintf(fp, "\n");
            newline = 0;
        }
    }
    fclose(fp);

    free(arr);
    free(arrCopy);
    free(temp);
    return 0;
}

void bubbleSort(unsigned long int *arr, size_t n){
    if (n == 0) return;
    for (size_t i = 0; i < n - 1; i++){
        for (size_t j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                unsigned long int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        } 
    }
        
}

void selectionSort(unsigned long int *arr, size_t n){
    if (n == 0) return;
    for (size_t i = 0; i < n - 1; i++) {
      
        // Assume the current position holds
        // the minimum element
        size_t min_idx = i;
        
        // Iterate through the unsorted portion
        // to find the actual minimum
        for (size_t j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
              
                // Update min_idx if a smaller element is found
                min_idx = j;
            }
        }
        
        // Move minimum element to its
        // correct position
        unsigned long int temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

void insertionSort(unsigned long int *arr, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        unsigned long int key = arr[i];
        int j = (int)i - 1;

        /* Move elements of arr[0..i-1], that are
           greater than key, to one position ahead
           of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void swap(unsigned long int *a, unsigned long int *b){
    unsigned long int t = *a;
    *a = *b;
    *b = t;
}

int partition(unsigned long int arr[], int low, int high) {
    
    //Pick a random index between low and high
    int random = low + rand() % (high - low + 1);

    //Swap it with the high element
    swap(&arr[random], &arr[high]);

    unsigned long int pivot = arr[high];
    
    // Index of smaller element and indicates 
    // the right position of pivot found so far
    int i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements to the left side. Elements from low to 
    // i are smaller after every iteration  
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    
    // Move pivot after smaller elements and
    // return its position
    swap(&arr[i + 1], &arr[high]);  
    return i + 1;
}

void quickSort(unsigned long int *arr, int low, int high) {
        if (low < high) {
        
        // pi is the partition return index of pivot
        int pi = partition(arr, low, high);

        // recursion calls for smaller elements
        // and greater or equals elements
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
        }
}

void merge(unsigned long int *arr, unsigned long int *temp, size_t l, size_t m, size_t r) {
    size_t i = l;      // Initial index of first subarray
    size_t j = m + 1;  // Initial index of second subarray
    size_t k = l;      // Initial index of merged subarray

    // Copy data to temp array
    for (size_t idx = l; idx <= r; idx++) {
        temp[idx] = arr[idx];
    }

    // Merge the temp arrays back into arr[l..r]
    while (i <= m && j <= r) {
        if (temp[i] <= temp[j]) {
            arr[k++] = temp[i++];
        } else {
            arr[k++] = temp[j++];
        }
    }

    // Copy remaining elements of the left half, if any
    while (i <= m) {
        arr[k++] = temp[i++];
    }
    // Note: Right half elements are already in place in 'arr' if 'temp' is a copy
}

void mergeSort(unsigned long int *arr, unsigned long int *temp, size_t l, size_t r) {
    if (l < r) {
        size_t m = l + (r - l) / 2; 

        // Sort first and second halves
        mergeSort(arr, temp, l, m);
        mergeSort(arr, temp, m + 1, r);

        merge(arr, temp, l, m, r);
    }
}

void heapify(unsigned long int *arr, size_t n, size_t i) {
    while (1) {
        size_t largest = i;
        size_t l = 2 * i + 1;
        size_t r = 2 * i + 2;

        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;

        if (largest != i) {
            unsigned long int temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;
            i = largest; // Move down the tree
        } else {
            break;
        }
    }
}

void heapSort(unsigned long int *arr, size_t n) {
    if (n == 0) return;
    // Build heap (rearrange vector)
    for (int i = (int)n / 2 - 1; i >= 0; i--)
        heapify(arr, n, (size_t)i);

    // One by one extract an element from heap
    for (int i = (int)n - 1; i > 0; i--) {

        // Move current root to end
        unsigned long int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Call max heapify on the reduced heap
        heapify(arr, (size_t)i, 0);
    }
}