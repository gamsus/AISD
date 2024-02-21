#include <bits/stdc++.h>
#include <chrono>
using namespace std;

int n = 0;
int no_comparisons = 0;

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

int binarySearch(int array[], int left, int right, int key) {
  if (right >= left) {
    int mid = left + (right - left) / 2;

    if (compareEquals(array[mid], key))
      return 1;
    
    if (compare(array[mid], key))
      return binarySearch(array, left, mid-1, key);
    else
      return binarySearch(array, mid+1, right, key);
  }
  return 0;
}

void printArray(int array[], int arrayLength) {
  if(n < 50) {
    for(int i = 0; i < arrayLength; i++) {
      cout << array[i] << " ";
    }
    cout << endl;
  }
}

int main(int argc, char **argv){
  int v;
  //pobieramy z stdin najpierw rozmiar tablicy, potem elementy tablicy
  //a na koniec szukaną wartość
  cin >> n;
  int array[n];
  int element = 0;
  int i = 0;
  while(i < n && cin >> element) {
    array[i] = element;
    i++;
  }
  cin >> v;
  auto start = chrono::high_resolution_clock::now();
  bool isElementIn = binarySearch(array, 0, n-1, v);
  auto finish = chrono::high_resolution_clock::now();
  if(n <= 50) {
    if(isElementIn) {
      cout << "Wartość " << v << " znajduje się w poniższej tablicy:" << endl;
    }
    else {
      cout << "Wartość " << v << " nie znajduje się w poniższej tablicy:" << endl;
    }
    printArray(array,n);
  }
  int time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
  cout << n << ";" << no_comparisons << ";" << time << endl;
  return 0;
}

//Binary search takes an input of size n,
//spends a constant amount of non-recursive overhead comparing the middle
//element to the searched for element, breaks the original input into half,
//and recursive on only one half of the array. Now plug this into the master
//theorem with a=1, subproblems of size n/b where b=2, and d=0
//Only way to calculate master theorem is to plug in case 2.
//d = log_b(a).
//Thus, the total running time is O(n^0*log_n) = O(log_n).
//T(n) = T(n/2) + O(1)
//O(1) = T(n) - T(n/2) = 2.