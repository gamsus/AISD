#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <iostream>

using namespace std;

int n = 0;
int no_assigns = 0;
int no_comparisons = 0;

static random_device rd;
static mt19937 rng(rd()); 

void assignKey(int& i, int& j) {
  i = j;
  no_assigns++;
}

void swap(int& i, int& j) {
  int help;
  assignKey(help, i);
  assignKey(i, j);
  assignKey(j, help);
}

bool compare(int i, int j) {
  no_comparisons++;
  if(i > j) {
    return true;
  }
  return false;
}

int lomutoPartition(int array[], int start, int end) {
  int pivot = array[start];
  int i = start;
  for(int j = start+1; j <= end; j++) {
    if(pivot > array[j]) {
      i++;
      int help = array[i];
      array[i] = array[j];
      array[j] = help;
    }
  }
  int help = array[i];
      array[i] = array[start];
      array[start] = help;
  return i;
}

void quickSort(int array[], int start, int end) {
  if(start < end) {
    int pivotIndex = lomutoPartition(array, start, end);
    quickSort(array, start, pivotIndex-1);
    quickSort(array, pivotIndex+1, end);
  }
}

int randomise(int from, int to)
{
  uniform_int_distribution<int> uid(from, to);
  return uid(rng);
}

void printArray(int array[]) {
  if(n < 50) {
    for(int i = 0; i < n; i++) {
      cout << array[i] << " ";
    }
    cout << endl;
  }
}

int randPartition(int arr[], int p, int q) {
  int pivotIndex = randomise(p, q);
  if(pivotIndex != p)
    swap(arr[pivotIndex], arr[p]);
  pivotIndex = p;
  int j = p;
  for(int i = p+1; i <= q; i++) {
    if(compare(arr[pivotIndex], arr[i])) {
      j++;
      swap(arr[j], arr[i]);
    }
  }
  swap(arr[j], arr[p]);
  return j;
}

int randomizedSelect(int arr[], int p, int q, int i) {

  if (p == q){
    return arr[p];
  } 
  int r = randPartition(arr, p, q);
  int k = r - p + 1;
  if(n <= 50) {
    cout << "Wybrany pivot: " << arr[r] << ", p = " 
    << p << ", q = " << q << endl << "Stan całej tablicy A po partition na podtablicy A[p...q]: " << endl;
    printArray(arr);
    cout << "Pivot jest " << k << ". statystyką pozycyjną w tablicy A[p...q]" << endl << endl;
  }
  if (k == i) return arr[r];
  else if (i < k) return randomizedSelect(arr, p, r-1, i);
  else return randomizedSelect(arr, r+1, q, i-k);

}

int main(int argc, char **argv){
  if(argc > 1) {

    if (strlen(argv[1]) == 0) {
      return 1;
    }
    char* p;
    errno = 0;
    long arg = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0) {
      return 1;
    }
    cin >> n;
    if (arg > n) {
      cout << "za duży numer szukanej statystyki!" << endl;
      return 0;
    }
    int array[n];
    int element = 0;
    int i = 0;
    while(cin >> element) {
      array[i] = element;
      i++;
    }
    if(n <= 50) {
      cout << "Uwaga indeksy tablicy to liczby od 0 do n-1!" << endl;
      cout << "Tablica początkowa:" << endl << endl;
      printArray(array);
      cout << endl;
    }
    int result = randomizedSelect(array, 0, n-1, arg);
    if(n <= 50) {
      cout << "Końcowy stan tablicy:" << endl;
      printArray(array);
      cout << endl;
      cout << "Wynik: " << result << " to " << arg << ". statystyka pozycyjna w tablicy: " << endl;
      quickSort(array, 0, n-1);
      printArray(array);
      cout << endl;
    }
    cout << n << ";" << no_assigns << ";" << no_comparisons << endl;
  }
  else {
    cout << "brakujący argument - statystyka pozycyjna" << endl;
  }
  return 0;
}