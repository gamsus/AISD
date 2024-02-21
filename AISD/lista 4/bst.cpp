#include <iostream>
#include <stdlib.h>

using namespace std;

int n = 0;
long noKeyComparisons = 0;
long noPointerReads = 0;
long noPointerAssigns = 0;
int treeHeightAfterInserts = 0;
int treeHeightAfterDeletes = 0;

bool compareKeys(int key1, int key2) {
  noKeyComparisons++;
  if(key1 > key2) return true;
  return false;
}

bool compareKeysEquals(int key1, int key2) {
  noKeyComparisons++;
  if(key1 == key2) return true;
  return false;
}

struct BSTNode {
  int key;
  struct BSTNode *left, *right, *parent;
};

struct BSTNode* newNode(int value) {
  struct BSTNode* temp = (struct BSTNode*)malloc(sizeof(struct BSTNode));
  temp->key = value;
  temp->left = NULL;
  temp->right = NULL;
  temp->parent = NULL;
  return temp;
}

struct BSTNode* parent(struct BSTNode* node) {
  noPointerReads++;
  return node->parent;
}

struct BSTNode* left(struct BSTNode* node) {
  noPointerReads++;
  return node->left;
}

struct BSTNode* right(struct BSTNode* node) {
  noPointerReads++;
  return node->right;
}

void assignPointer(struct BSTNode*& node, struct BSTNode* node2) {
  node = node2;
  noPointerAssigns++;
}

void newInsert(struct BSTNode** root, int key) {
  struct BSTNode* toInsert = newNode(key);
  struct BSTNode* help = NULL;
  struct BSTNode* x = *root;
  while (x != NULL) {
    help = x;
    if (compareKeys(x->key, toInsert->key))
      x = left(x);
    else
      x = right(x);
  }
  assignPointer(toInsert->parent, help);
  noPointerReads++;
  if(help == NULL)
    *root = toInsert;
  else {
    if (compareKeys(help->key, toInsert->key))
      assignPointer(help->left, toInsert);
    else
      assignPointer(help->right, toInsert);
  }
}

struct BSTNode* smallestValueNode(struct BSTNode* node) {
  struct BSTNode* minNode = node;
  if (minNode != NULL) {
    while (left(minNode) != NULL)
      minNode = left(minNode);
  }
  return minNode;
}

//zwracamy nowy korzeń (np. gdy usuwana wartość znajduje się w korzeniu, to trzeba nowy korzeń)
struct BSTNode* deleteElement(struct BSTNode* root, int key) {
  if (root == NULL)
    return root;

  if (compareKeys(root->key, key)) {
    root->left = deleteElement(root->left, key);
    noPointerReads++;
  }
  
  else if (compareKeys(key, root->key)) {
    root->right = deleteElement(root->right, key);
    noPointerReads++;
  }

  else {
    //lewe poddrzewo puste - przesuwamy prawe poddrzewo w górę
    if (left(root) == NULL) {
      struct BSTNode* temp = right(root);
      if(temp != NULL) {
        assignPointer(temp->parent, parent(root));
        noPointerReads++;
      }
      free(root);
      noPointerAssigns++;
      return temp;
    }
    //prawe poddrzewo puste - przesuwamy lewe poddrzewo w górę
    else if (right(root) == NULL) {
      struct BSTNode* temp = left(root);
      if(temp != NULL) {
        assignPointer(temp->parent, parent(root));
        noPointerReads++;
      }
      free(root);
      noPointerAssigns++;
      return temp;
    }
    // gdy dwa poddrzewa to szukamy następnika 
    //(najbliższą wartość większą od usuwanej)
    struct BSTNode* temp = smallestValueNode(right(root));
    root->key = temp->key;
    assignPointer(root->right, deleteElement(right(root), temp->key));
    noPointerReads++;
  }
  return root;
}

//nie badamy złożoności tej operacji, stąd tu nie używam funkcji
int treeHeight(struct BSTNode* root) {
  if(root == NULL)
    return 0;
  int leftHeight = treeHeight(root->left);
  int rightHeight = treeHeight(root->right);
  return ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
}

// global variables used in `print_BST`
char* left_trace; // needs to be allocaded with size
char* right_trace; // needs to be allocaded with size


void print_BST( struct BSTNode* root, int depth, char prefix) {
  if( root == NULL ) return;
  if( root->left != NULL ){
    print_BST(root->left, depth+1, '/');
  }
  if(prefix == '/') left_trace[depth-1]='|';
  if(prefix == '\\') right_trace[depth-1]=' ';
  if( depth==0) printf("-");
  if( depth>0) printf(" ");
  for(int i=0; i<depth-1; i++)
    if( left_trace[i]== '|' || right_trace[i]=='|' ) {
      printf("| ");
    } else {
      printf("  ");
    }
  if( depth>0 ) printf("%c-", prefix);
  printf("[%d]\n", root->key);
  left_trace[depth]=' ';
  if( root->right != NULL ){
    right_trace[depth]='|';
    print_BST(root->right, depth+1, '\\');
  }
}

int main(int argc, char **argv){
  cin >> n;
  int element = 0;
  int i = 0;

  left_trace = (char*)malloc( n*sizeof(char) );
  right_trace = (char*)malloc( n*sizeof(char) );
  for(int i=0; i<=n; i++){
    left_trace[i]=' ';
    left_trace[i]=' ';
  }

  struct BSTNode* tree = NULL;
  double avgKeyCompare, avgPointerReads, avgPointerAssigns, avgTreeHeights = 0.0;
  long maxKeyCompare = 0, maxPointerReads = 0, maxPointerAssigns = 0;
  long long allKeyCompare = 0, allPointerReads = 0, allPointerAssigns = 0, allTreeHeights = 0;

  while(i < n && cin >> element) {
    newInsert(&tree, element);
    if(noKeyComparisons > maxKeyCompare)
      maxKeyCompare = noKeyComparisons;
    if(noPointerAssigns > maxPointerAssigns)
      maxPointerAssigns = noPointerAssigns;
    if(noPointerReads > maxPointerReads)
      maxPointerReads = noPointerReads;
    
    allKeyCompare += noKeyComparisons;
    allPointerAssigns += noPointerAssigns;
    allPointerReads += noPointerReads;
    allTreeHeights += treeHeight(tree);

    noPointerAssigns = 0;
    noPointerReads = 0;
    noKeyComparisons = 0;

    if(n <= 50) {
      cout << "insert " << element << endl;
      print_BST(tree, 0, '-');
      cout << endl;
    }
    i++;
  } 
  avgKeyCompare = (double)allKeyCompare / (double)n;
  avgPointerAssigns = (double)allPointerAssigns / (double)n;
  avgPointerReads = (double)allPointerReads / (double)n;
  avgTreeHeights = (double)allTreeHeights / (double)n;
  treeHeightAfterInserts = treeHeight(tree);

  cout << "I;" << n << ";" << avgKeyCompare << ";" << maxKeyCompare << ";" <<
    avgPointerReads << ";" << maxPointerReads << ";" << avgPointerAssigns << ";" <<
    maxPointerAssigns << ";" << avgTreeHeights << ";" << treeHeightAfterInserts << endl;


  i = 0;
  allKeyCompare = allPointerAssigns = allPointerReads = allTreeHeights = 0;
  maxKeyCompare = maxPointerAssigns = maxPointerReads = 0;

  while(i < n && cin >> element) {
    tree = deleteElement(tree, element);
    if(noKeyComparisons > maxKeyCompare)
      maxKeyCompare = noKeyComparisons;
    if(noPointerAssigns > maxPointerAssigns)
      maxPointerAssigns = noPointerAssigns;
    if(noPointerReads > maxPointerReads)
      maxPointerReads = noPointerReads;
    
    allKeyCompare += noKeyComparisons;
    allPointerAssigns += noPointerAssigns;
    allPointerReads += noPointerReads;
    allTreeHeights += treeHeight(tree);

    noPointerAssigns = 0;
    noPointerReads = 0;
    noKeyComparisons = 0;

    if(n <= 50) {
      cout << "delete " << element << endl;
      print_BST(tree, 0, '-');
      cout << endl;
    }
    i++;
  }

  treeHeightAfterDeletes = treeHeight(tree);
  avgKeyCompare = (double)allKeyCompare / (double)n;
  avgPointerAssigns = (double)allPointerAssigns / (double)n;
  avgPointerReads = (double)allPointerReads / (double)n;
  avgTreeHeights = (double)allTreeHeights / (double)n;

  cout << "D;" << n << ";" << avgKeyCompare << ";" << maxKeyCompare << ";" <<
    avgPointerReads << ";" << maxPointerReads << ";" << avgPointerAssigns << ";" <<
    maxPointerAssigns << ";" << avgTreeHeights << ";" << treeHeightAfterDeletes << endl;
  return 0;
}


