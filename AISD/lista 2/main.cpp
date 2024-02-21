#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <stdio.h>
#include <algorithm>

int compares_nn;
int swaps_nn; 
int compares_n_temp;
int swaps_n_temp;
int d_compares_n;
int d_swaps_n;
int h_compares_n;
int h_swaps_n;
int n;
int ic;
int is;

int avgswapsi = 0;
int avgswapsm = 0;
int avgswapsq = 0;
int avgswapsd = 0;
int avgswapsh = 0;

int avgcompsi = 0;
int avgcompsm = 0;
int avgcompsq = 0;
int avgcompsd = 0;
int avgcompsh = 0;


void randomGenerator(int *arr, int n) {
    std::random_device rd;
    arr[n];
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 2*n-1);

    for (int i=0; i<n; ++i) {
        arr[i] = (int)dist(mt);
    }
}

void printArray(int arr[], int size) {
    for(int i = 0; i < size; i++) {
        if(arr[i] < 10) {
            std::cout<<"0"<<arr[i] << " ";
        } else {
            std::cout<<arr[i] << " ";
        }

    }
    std::cout<<std::endl;
} 

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
    swaps_n_temp++;
}

bool isSorted(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        if(arr[i] > arr[i + 1]) {
            std::cout<<"Tablica nieposortowana\n";
            return false;
        }
    }
    std::cout<<"Tablica posortowana\n";
    return true;
}

//INSERTION SORT
void insertionSort(int arr[], int n, int &compares_n, int &swaps_n) {
    compares_n = 0;
    swaps_n = 0;
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            compares_n++;

            arr[j + 1] = arr[j];
            swaps_n++;

            j = j - 1;
        }
        arr[j + 1] = key;
        swaps_n++;
        printArray(arr, n);
    }
}



//MERGE SORT
void merge(int array[], int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    auto *leftArray = new int[subArrayOne],
        *rightArray = new int[subArrayTwo];

    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, 
    indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;


    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        compares_nn += 2;
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            compares_nn++;
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
            swaps_nn++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            swaps_nn++;
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        compares_nn++;
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        swaps_nn++;
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        compares_nn++;
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        swaps_nn++;
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void mergeSort(int array[], int const begin, int const end) {
    if (begin >= end)
        return;

    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);

    std::cout<<"Array before merge \n";
    printArray(array, end);
    merge(array, begin, mid, end);

    std::cout<<"Array after merge \n";
    printArray(array, end);
}

void mergeSortRec(int array[], int const begin, int const end, int &compares, int &swaps) {
    mergeSort(array,begin,end);
    compares = compares_nn;
    swaps = swaps_nn;
}


//QUICK SORT
int partition_quick (int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            compares_n_temp++;
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    printArray(arr, high);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_quick(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void quickSortRec(int arr[], int low, int high, int &compares_n, int &swaps_n) {
    quickSort(arr, low, high);
    compares_n = compares_n_temp;
    swaps_n = swaps_n_temp;
}



//DUAL PIVOT SORT
void swapPivot(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    d_swaps_n++;
}

int partition(int* arr, int low, int high, int* lp) {
    d_compares_n++;
    if (arr[low] > arr[high])
        swapPivot(&arr[low], &arr[high]);
    int j = low + 1;
    int g = high - 1, k = low + 1, p = arr[low], q = arr[high];
    d_compares_n++;
    while (k <= g) {
        d_compares_n += 2;
        if (arr[k] < p) {
            swapPivot(&arr[k], &arr[j]);
            j++;
        }
        else if (arr[k] >= q) {

            while (arr[g] > q && k < g) {
                d_compares_n += 2;
                g--;
            }
            swapPivot(&arr[k], &arr[g]);
            g--;
            d_compares_n++;
            if (arr[k] < p) {
                swapPivot(&arr[k], &arr[j]);
                j++;
            }
        }
        k++;
        printArray(arr, high);
    }
    j--;
    g++;

    swapPivot(&arr[low], &arr[j]);
    swapPivot(&arr[high], &arr[g]);
    *lp = j;

    return g;
}


void DualPivotQuickSort(int* arr, int low, int high) {
    if (low < high) {
        int lp, rp;
        rp = partition(arr, low, high, &lp);
        DualPivotQuickSort(arr, low, lp - 1);
        DualPivotQuickSort(arr, lp + 1, rp - 1);
        DualPivotQuickSort(arr, rp + 1, high);
    }
}

void dualPivotQuickSortRec(int arr[], int low, int high, int &compares_n, int &swaps_n) {
    DualPivotQuickSort(arr,low,high);
    compares_n = d_compares_n;
    swaps_n = d_swaps_n;
}

//HYBRID
void insertion_sort(int arr[], int low, int n) {

    for(int i=low+1;i<n+1;i++) {
        int val = arr[i] ;
        int j = i ;

        while (j>low && arr[j-1]>val) {
            h_compares_n += 2;
            arr[j]= arr[j-1] ;
            h_swaps_n++;
            j-= 1;
        }
        arr[j]= val ;
        printArray(arr, n);
    }
}

int partitionHybrid(int arr[], int low, int high)
{
    int pivot = arr[high] ;
    int i ,j;
    i = low;
    j = low;

    for (int i = low; i < high; i++)
    {
        h_compares_n++;
        if(arr[i]<pivot)
        {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            h_swaps_n++;
            j += 1;
        }
    }

    int temp = arr[j];
    arr[j] = arr[high];
    arr[high] = temp;
    h_swaps_n++;

    return j;
}

void quick_sort(int arr[], int low,int high)
{
    if (low < high)
    {
        int pivot = partitionHybrid(arr, low, high);
        quick_sort(arr, low, pivot-1) ;
        quick_sort(arr, pivot + 1, high) ;
    }
}

void hybridSort(int* arr, int low, int high)
{
    while (low < high)
    {
        if (high-low + 1 < 10)
        {
            insertion_sort(arr, low, high);
            break;
        }

        else

        {
            int pivot = partitionHybrid(arr, low, high) ;
            h_compares_n++;
            if (pivot-low<high-pivot)
            {
                hybridSort(arr, low, pivot - 1);
                low = pivot + 1;
            }
            else
            {
                hybridSort(arr, pivot + 1, high);
                high = pivot-1;
            }
        }
    }
}

void hybridSortRec(int arr[], int low, int high, int &compares_n, int &swaps_n) {
    hybridSort(arr, low, high);
    compares_n = h_compares_n;
    swaps_n = h_swaps_n;
}

//SWAP
void swapArray(int array[], int array1[], int n)
{
    for(int i = 0; i < n; i++) {
        array[i] = array1[i];
    }
}

void globalVarZero() {
    compares_nn = 0;
    swaps_nn = 0; 
    compares_n_temp = 0;
    swaps_n_temp = 0;
    d_compares_n = 0;
    d_swaps_n = 0;
    h_compares_n = 0;
    h_swaps_n = 0;
    ic = 0;
    is = 0;
    avgswapsi = 0;
    avgswapsm = 0;
    avgswapsq = 0;
    avgswapsd = 0;
    avgswapsh = 0;

    avgcompsi = 0;
    avgcompsm = 0;
    avgcompsq = 0;
    avgcompsd = 0;
    avgcompsh = 0;
}

void ascGenerator(int arr[], int n) {
    std::random_device dev;
    arr[n];
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(1, 2*n-1);

    for(int i = 0; i < n; i++ ) {
        arr[i] = ((int)dist(dev));
    }

    std::sort(arr, arr + n);
}

void descGenerator(int arr[], int n){
    std::random_device rd;

    arr[n];

    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1.0, 2*n-1);

    for(int i = 0; i < n; i++ ) {
        arr[i] = ((int)dist(mt));
    }

    std::sort(arr, arr + n, std::greater<int>());
}

int main() {
    int n,ic,is;
    std::cout<<"Podaj ilosc kluczy: "<<std::endl;
    std::cin>>n;
    int arr[n];
    int arr1[n];
    int arr2[n];
    int arr3[n];
    int arr4[n];
    randomGenerator(arr , n);
    swapArray(arr1, arr, n);
    swapArray(arr2, arr, n);
    swapArray(arr3, arr, n);
    swapArray(arr4, arr, n);

    std::cout<<"Insertion sort start:  "<<std::endl;
    insertionSort(arr,n,ic,is);
    std::cout<<"After Insertion Sort"<<std::endl;
    printArray(arr, n);
    std::cout<<"|||||||||||||||||||||||||\n";
    std::cout<<"Amount of swaps: "<<is<<std::endl;
    std::cout<<"Amount of compares: "<<ic<<std::endl;
    std::cout<<"Merge sort start:  "<<std::endl;
    mergeSortRec(arr1, 0, n - 1, ic, is);
    std::cout<<"After Merge Sort"<<std::endl;
    printArray(arr1, n);
    std::cout<<"|||||||||||||||||||||||||\n";
    std::cout<<"Amount of swaps: "<<is<<std::endl;
    std::cout<<"Amount of compares: "<<ic<<std::endl;
    std::cout<<"Quick sort start:  "<<std::endl;
    quickSortRec(arr2, 0, n - 1, ic, is);
    std::cout<<"After Quick Sort"<<std::endl;
    printArray(arr2, n);
    std::cout<<"|||||||||||||||||||||||||\n";
    std::cout<<"Amount of swaps: "<<is<<std::endl;
    std::cout<<"Amount of compares: "<<ic<<std::endl;
    std::cout<<"Dual Pivot Quick Sort Start"<<std::endl;
    dualPivotQuickSortRec(arr3, 0, n - 1, ic, is);
    std::cout<<"After Dual-Pivot Quick Sort"<<std::endl;
    printArray(arr3, n);
    std::cout<<"|||||||||||||||||||||||||\n";
    std::cout<<"Amount of swaps: "<<is<<std::endl;
    std::cout<<"Amount of compares: "<<ic<<std::endl;
    std::cout<<"Hybrid Quick-Insertion Sort Start"<<std::endl;
    hybridSortRec(arr4, 0, n - 1, ic, is);
    std::cout<<"After Hybrid Quick-Insertion Sort"<<std::endl;
    printArray(arr4, n);
    std::cout<<"|||||||||||||||||||||||||\n";
    std::cout<<"Amount of swaps: "<<is<<std::endl;
    std::cout<<"Amount of compares: "<<ic<<std::endl;
    return 0;
}