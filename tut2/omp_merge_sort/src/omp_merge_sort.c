#include <stdio.h> 
#include <stdlib.h>
#include <omp.h>
#include "fileio.h"

#define MAX_ARR_SZ 100000000 

void serial_merge_sort(int arr[], int start, int end);
void par_merge_sort(int arr[], int start, int end);
void nested_par_merge_sort(int arr[], int start, int end);
int* merge_sorted_arrays(int* arr1, int start1, int end1, int* arr2, int start2, int end2);
void generate_list(int a[], int n);
void print_array(int a[], int n, char* title);
int is_sorted(int* array, int size);
void usage();

int main(int argc, char* argv[]) {
    printf("i get stuck");
    if (argc != 4) usage();

    //TODO: Read in the commandline parameters
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int data = atoi(argv[3]);

    //TODO: Allocate memory for 
    //      - a filename of at most 32 characters and 
    //      - an array of <size> integers
    char* filename = malloc(32*sizeof(char)); 
    int* array = calloc(sizeof(int),size); 
    int success = 0;
    if (data == 0) {
        //TODO: Populate array with random integers
        generate_list(array, size);
    } else {
        //TODO: Read the array from data/unsorted<data>.txt 
        //      using read_array_from_file from fileio.h
        sprintf(filename, "data/unsorted%d.txt", data);
        read_array_from_file(filename,array,size);
    }

    if (success == 0) {
        // Print the array to sort 
        if (size < 20) print_array(array, size, "Array to sort.txt");

        //TODO: Sort array: call one of the sorting algorithms */
        //serial_merge_sort(array,0,size);
        par_merge_sort(array,0,size);

        //TODO: if (data > 0) write the sorted array to sorted<data>.txt 
        //      using write_array_to_file from fileio.h
        if (data > 0)
        {
            sprintf(filename, "sorted%d.txt", data);
            write_array_to_file(filename,array,size);
        }
        //TODO: Use is_sorted to check whether array is sorted
        is_sorted(array,size);
    }

    return 0;
}

/**
 * serial_merge_sort sorts arr from start to (and including) end 
 * using a divide and conquer technique
 */
void serial_merge_sort(int arr[], int start, int end) {
    int middle;
    int* sorted_array;
    
    if (start < end) {
        middle = (start + end) / 2;
        serial_merge_sort(arr, start, middle);
        serial_merge_sort(arr, middle + 1, end);

        sorted_array = merge_sorted_arrays(arr, start, middle, arr, middle + 1, end);

        for(int i = start; i < end + 1; i++) {
            arr[i] = sorted_array[i - start];
        }
    }
    
}

 
/**
 * par_merge_sort sorts arr from start to (and including) end 
 * using a parallelised divide and conquer implementation of merge sort 
 */ 
void par_merge_sort(int arr[], int start, int end)
{
    int middle = 0;
    if (start < end)
    {
        middle = (start + end)/ 2;
        #pragma omp parallel sections
        {
           
            #pragma omp section
            //TODO change back to par
            {
               serial_merge_sort(arr,start,middle);
            }
            #pragma omp section
            {
                serial_merge_sort(arr,middle+1,end);
            }
        }
        
        int* sorted_array = merge_sorted_arrays(arr,start,middle,arr,middle+1,end);    
        for(int i = start; i < end + 1; i++) {
        arr[i] = sorted_array[i - start];   
        }
    }
    
}

/** 
 * Merge sorted subarrays in arr1 and arr2 
 * - start1 (end1) is the first (last) index in the subarray of arr1 (start1 <= end1) 
 * - start2 (end2) is the first (last) index in the subarray of arr2 (start2 <= end2) 
 * - arr1 and arr2 can be the same array
 */
int* merge_sorted_arrays(int* arr1, int start1, int end1, int* arr2, int start2, int end2)
{
    int k = 0, i = start1, j = start2;
    int tmp_size = 0;
    int* tmp = NULL;

    //TODO: Calculate tmp_size and allocate memory in temp for tmp_size integers
    tmp_size = end1 + end2 + 2;    
    tmp = (int*) calloc(tmp_size, sizeof(int)); 

    /* merge arr1 and arr2 into tmp in sorted order */
    while (k < tmp_size) {

        /* inside arr1 and arr2, copy smaller value */
        if(i <= end1 && j <= end2 && arr1[i] <= arr2[j]){
            tmp[k] = arr1[i];
            i+=1;
        } else if (i <= end1 && j <= end2 && arr1[i] > arr2[j])
        {
            tmp[k] = arr2[j];
            j+=1;
        }

        /* past end of arr1 => copy rest of arr2 */
        if (i > end1 && j <= end2)
        {
            tmp[k] = arr2[j];
            j++;
        }
        
        /* past end of arr2 => copy rest of arr1 */
        if (j > end2 && i <= end1)
        {
            tmp[k] = arr1[i];
            i++;
        }
        
    }

    return tmp;    
}

void generate_list(int a[], int n) {
    int i;

    srandom(1);
    for (i = 0; i < n; i++) {
        a[i] = random() % MAX_ARR_SZ;
    }
}  

void print_array(int array[], int n, char* msg) {
   printf("%s:\n", msg);
   for (int i = 0; i < n; i++)
      printf("%d ", array[i]);
   printf("\n");
}

int is_sorted(int* array, int size)
{
    int i = 0;
    
    if (size <= 1) return 1;
    while (i < size-1 && array[i] <= array[i+1]) i++; 

    if (i < size-1) return 0;
    else return 1; 
}

void usage()
{
    printf ("Usage: ./omp_merge_sort <n> <numthreads> \
        <data: 0-generate list, (i>0)-read list from data/unsorted<i>.txt>\n");
    exit(0);
}

