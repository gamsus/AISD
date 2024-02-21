#include <iostream>
#include <chrono>

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

void printArray(int array[]) {
  if(n < 50) {
    for(int i = 0; i < n; i++) {
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
  if (k == i) return arr[r];
  else if (i < k) return select(arr, p, r-1, i);
  else return select(arr, r+1, q, i-k);
}

void quickSortSelect(int array[], int start, int end) {
  if(start < end) {
    int pivotValue = select(array, start, end, ((end-start+1)/2) );
    int pivotIndex = partition(array, start, end, pivotValue);
    //cout << pivotIndex << endl;
    printArray(array);
    quickSortSelect(array, start, pivotIndex-1);
    quickSortSelect(array, pivotIndex+1, end);
  }
}

bool isArraySorted(int array[]) {

  for(int i = 0; i < n; i++) 
  if(i > 0) {
    if(array[i-1] > array[i]) {
      return false;
    }
  }

  return true;
}

void printArrayStatus(int array[]) {
  if(isArraySorted(array)) {
    cout << "Tablica jest posortowana" << endl;
  }
  else {
    cout << "Tablica nie jest posortowana" << endl;
  }
}

int main() {
  cin >> n;
  int array[n];
  int element = 0;
  int i = 0;
  while(cin >> element) {
    array[i] = element;
    i++;
  }
  if(n<50) printArrayStatus(array);
  printArray(array);
  auto start = chrono::high_resolution_clock::now();
  quickSortSelect(array, 0, n-1);
  auto finish = chrono::high_resolution_clock::now();
  int time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
  if(n<50) printArrayStatus(array);
  cout << n << ";" << no_comparisons << ";" << time << endl;
  return 0;
}