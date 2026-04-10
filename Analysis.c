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
    } else if (choice == 2){
        unsigned long int x; //starting point
        printf("Enter starting value: ");
        scanf("%lu", &x);
        genIncreasing(arr, n, x);
    } else {
        printf("Invalid choice.");
        free(arr);
        return 1;
    }
    
    for (size_t i = 0; i < n; i++) { 
        printf("%lu ", arr[i]); 
    }



    free(arr);
    return 0;
}
