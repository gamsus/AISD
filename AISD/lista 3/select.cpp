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

bool compareEquals(int i, int j) {
  no_comparisons++;
  if(i == j) {
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

// ALGORYTM SORTUJĄCY TABLICE MEDIAN
void insertionSort(int array[], int p, int q) {
  int key;
  for(int j = p+1; j <= q; j++) {
    assignKey(key, array[j]);
    int i = j-1;
    while (i >= 0 && compare(array[i], key)) {
      assignKey(array[i+1], array[i]);
      i--;
    }
    assignKey(array[i+1], key);
  }
}

void printArray(int array[], int arrayLength) {
  if(n < 50) {
    for(int i = 0; i < arrayLength; i++) {
      cout << array[i] << " ";
    }
    cout << endl;
  }
}

int partition(int arr[], int p, int q, int pivot) {
  //znajdujemy indeks pivota (bo przy tablicy median go utraciliśmy)
  int pivotIndex;
  for(int i = p; i <= q; i++) {
    if(compareEquals(pivot, arr[i])) {
      pivotIndex = i;
      break;
    }
  }
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

int select(int arr[], int p, int q, int i) {
    if (p == q){
    if(n <= 50) {
    }
    return arr[p];
  }
  //zadeklarowanie tablicy median o rozmiarze sufit z rozmiaru tablicy przez 5
  int medianTabSize = ( ((q-p+1)%5 == 0) ? ((q-p+1)/5): (((q-p+1)/5)+1) );
  int medianTab[medianTabSize];
  int index = 0;
  for(int i = p; i <= q; i += 5) {
    if(i + 4 <= q) {
      insertionSort(arr, i, i+4);
      assignKey(medianTab[index], arr[i+2]);
      index++;
    }
    else {
      insertionSort(arr, i, q);
      assignKey(medianTab[index], arr[i + ((q-i)/2)]);
    }
  }
  int medianOfMedian = select(medianTab, 0, medianTabSize-1, ((medianTabSize + 1)/2));
  int r = partition(arr, p, q, medianOfMedian);
  int k = r - p + 1;
  if(n <= 50) {
    cout << "Wybrany pivot (mediana median): " << arr[r] << endl;
    cout << "Pivot jest " << k << ". statystyką pozycyjną w podtablicy" << endl << endl;
  }
  if (k == i) return arr[r];
  else if (i < k) return select(arr, p, r-1, i);
  else return select(arr, r+1, q, i-k);

}

int main(int argc, char **argv){
  if(argc > 1) {
    //OBSŁUGA ARGUMENTU Z WIERSZA
    if (strlen(argv[1]) == 0) {
      return 1; // pusty string
    }
    char* p;
    errno = 0;
    long arg = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0) {
      return 1;
    }
    if (arg < INT_MIN || arg > INT_MAX) {
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
      printArray(array, n);
      cout << endl;
    }
    int result = select(array, 0, n-1, arg);
    if(n <= 50) {
      cout << "Końcowy stan tablicy:" << endl;
      printArray(array, n);
      cout << endl;
      cout << "Wynik: " << result << " to " << arg << ". statystyka pozycyjna w tablicy: " << endl;
      quickSort(array, 0, n-1);
      printArray(array, n);
      cout << endl;
    }
    cout << n << ";" << no_assigns << ";" << no_comparisons << endl;
    
  }
  else {
    cout << "brakujący argument - statystyka pozycyjna" << endl;
  }
  return 0;
}