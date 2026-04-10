#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
const unsigned long int MAX_RANGE = 1000000; //max value for unsigned long int

int askUser(){ //asks what data generation method to use
    int choice;
    printf("Select data generation method: \n");
    printf("  1. Randomly Generate Integers\n");
    printf("  2. Increasing Sequence\n");
    scanf("%d", &choice);
    return choice;
}

int askSort(){ //asks sorting algorithm to use
    int choice;
    printf("Select sorting algorithm: \n");
    printf("  1. Bubble Sort\n");
    printf("  2. Selection Sort\n");
    printf("  3. Insertion Sort\n");
    printf("  4. Quick Sort\n");
    printf("  5. Merge Sort\n");
    printf("  6. Heap Sort\n");
    scanf("%d", &choice);
    return choice;
}

void genRandInt(unsigned long int *arr, size_t size){ //generate random integer
    for (size_t i = 0; i < size; i++) {
        unsigned long long high = rand();
        unsigned long long low = rand();
        unsigned long long combined = (high << 15) | low; 
        arr[i] = (unsigned long int)(combined % (MAX_RANGE + 1));
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
        printf("\n");
    }
}

void swap(unsigned long int *a, unsigned long int *b);
int partition(unsigned long int arr[], int low, int high);
void merge(unsigned long int *arr, int l, int m, int r);
void heapify(unsigned long int *arr, size_t n, size_t i);

//sorting algorithms
void bubbleSort(unsigned long int *arr, size_t n);
void selectionSort(unsigned long int *arr, size_t n);
void insertionSort(unsigned long int *arr, size_t n);
void quickSort(unsigned long int *arr, int low, int high);
void mergeSort(unsigned long int *arr, int l, int r);
void heapSort(unsigned long int *arr, size_t n);



int main(int argc, char** argv) {
    size_t n; //number of integers to be sorted
    printf("Enter number of integers: ");
    if (scanf("%zu", &n) != 1 || n <= 0) {
        printf("Enter positive number.");
        return 1;
    } 

    //allocate memory to heap instead of stack to prevent stack overflow when user inputs large number
    //dont use variable length arrays
    unsigned long int *arr = malloc(n * sizeof(unsigned long int)); //store integers
    if (!arr) return 1;

    int choice = askUser();
    if(choice == 1){
        srand((unsigned int)time(NULL)); //seed to prevent same numbers
        genRandInt(arr, n);
        printf("Here is the randomly generated integers: \n");
    } else if (choice == 2){
        unsigned long int x; //starting point
        printf("Enter starting value: ");
        scanf("%lu", &x);
        genIncreasing(arr, n, x);
        printf("Here is the increasing sequence of integers: \n");
    } else {
        printf("Invalid choice.");
        free(arr);
        return 1;
    }

    showArr(arr, n);

    int sortChoice = askSort();
    switch (sortChoice)
    {
    case 1:
        bubbleSort(arr, n);
        break;
    
    case 2:
        selectionSort(arr, n);
        break;
    
    case 3:
        insertionSort(arr, n);
        break;

    case 4:
        quickSort(arr, 0, n - 1);
        break;

    case 5:
        mergeSort(arr, 0, n - 1);
        break;

    case 6:
        heapSort(arr, n);
        break;

    default:
        printf("Invalid choice.");
        return 1;
    }

    printf("Sorted: \n");
    showArr(arr, n);

    free(arr);
    return 0;
}

void bubbleSort(unsigned long int *arr, size_t n){
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
        long long j = (long long)i - 1;

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

// partition function
int partition(unsigned long int arr[], int low, int high) {
    
    // Choose the pivot
    unsigned long int pivot = arr[high];
    
    // Index of smaller element and indicates 
    // the right position of pivot found so far
    long long i = low - 1;

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

void merge(unsigned long int *arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    unsigned long int *L = malloc(n1 * sizeof(unsigned long int));
    unsigned long int *R = malloc(n2 * sizeof(unsigned long int));

    if (!L || !R) {
        printf("Merge allocation failed\n");
        exit(1);
    }

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(unsigned long int *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2; 

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// To heapify a subtree rooted with node i
void heapify(unsigned long int *arr, size_t n, size_t i) {

    // Initialize largest as root
    size_t largest = i;

    // left index = 2*i + 1
    size_t l = 2 * i + 1;

    // right index = 2*i + 2
    size_t r = 2 * i + 2;

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        unsigned long int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void heapSort(unsigned long int *arr, size_t n) {
    // Build heap (rearrange vector)
    for (long long i = (long long)n / 2 - 1; i >= 0; i--)
        heapify(arr, n, (size_t)i);

    // One by one extract an element from heap
    for (long long i = (long long)n - 1; i > 0; i--) {

        // Move current root to end
        unsigned long int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Call max heapify on the reduced heap
        heapify(arr, (size_t)i, 0);
    }
}