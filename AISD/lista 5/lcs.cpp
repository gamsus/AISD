#include <iostream>
#include <vector>

using namespace std;

int n = 0;
int m = 0;
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
  if (i == j)
    return true;
  return false;
}

//nie można przekazać wielowymiarowej tablicy, przechodzę na wektor
// wektor memory musi być zalokowany jak tablica rozmiaru mxn
int LCSLength(int X[], int Y[], vector<vector<char>>& memory) {
  //tablica[m+1][n+1] PRZESUNIĘTA WZGLĘGEM POZOSTAŁYCH o jeden
  vector<vector<int>> helpCount( m+1 , vector<int> (n+1, 0)); 
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      if(compareEquals(X[i-1], Y[j-1])) {
        helpCount[i][j] = helpCount[i-1][j-1] + 1;
        memory[i-1][j-1] = 'b';
      }
      else if(compare(helpCount[i-1][j], helpCount[i][j-1])) {
        helpCount[i][j] = helpCount[i-1][j];
        memory[i-1][j-1] = '^';
      }
      else {
        helpCount[i][j] = helpCount[i][j-1];
        memory[i-1][j-1] = '<';
      }
    }
  }
  return helpCount[m][n];
}

void PrintLCS(int X[], vector<vector<char>> memory, int i, int j) {
  if(i < 0 || j < 0)
    return;
  if(memory[i][j] == 'b') {
    PrintLCS(X, memory, i-1, j-1);
    cout << X[i] << " ";
  }
  else if (memory[i][j] == '^') {
    PrintLCS(X, memory, i-1, j);
  }
  else {
    PrintLCS(X, memory, i, j-1);
  }
}


int main() {
  cin >> n;
  int Y[n];
  int element = 0;
  int i = 0;
  while(i < n && cin >> element) {
    Y[i] = element;
    i++;
  }
  cin >> m;
  int X[m];
  i = 0;
  while(i < m && cin >> element) {
    X[i] = element;
    i++;
  }
  
  vector<vector<char>> memory( m , vector<char> (n));

  int length = LCSLength(X, Y, memory);
  if(m <= 50 && n <= 50) {
    cout << "X: ";
    for(i = 0; i < m; i++)
      cout << X[i] << " ";
    cout << endl << "Y: ";
    for(int j = 0; j < n; j++)
      cout << Y[j] << " ";
    cout << endl << "Najdłuższy wspólny podciąg X i Y ma długość " << length << " i tak wygląda: " << endl;
    PrintLCS(X, memory, m-1, n-1);
    cout << endl;
  }
  
  cout << no_comparisons << endl;
  return 0;
}
