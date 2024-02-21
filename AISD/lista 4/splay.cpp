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
  if (key1 > key2) return true;
  return false;
}

bool compareKeysEquals(int key1, int key2) {
  noKeyComparisons++;
  if (key1 == key2) return true;
  return false;
}

struct SplayNode {
  int key;
  struct SplayNode *left, *right, *parent;
};

struct SplayNode* newNode(int value) {
  struct SplayNode* temp = (struct SplayNode*)malloc(sizeof(struct SplayNode));
  temp->key = value;
  temp->left = NULL;
  temp->right = NULL;
  temp->parent = NULL;
  return temp;
}

struct SplayNode* parent(struct SplayNode* node) {
  noPointerReads++;
  return node->parent;
}

struct SplayNode* left(struct SplayNode* node) {
  noPointerReads++;
  return node->left;
}

struct SplayNode* right(struct SplayNode* node) {
  noPointerReads++;
  return node->right;
}

void assignPointer(struct SplayNode*& node, struct SplayNode* node2) {
  node = node2;
  noPointerAssigns++;
}

struct SplayNode* maxNode(SplayNode* node) {
  while(right(node) != NULL)
    node = right(node);
  return node;
}
void printRBT( struct SplayNode* root, int depth, char prefix);

/*
      A                         B
    /   \                      / \
   a1    B        ->          A   a3
        /  \                 / \
       a2   a3              a1 a2
*/
void leftRotate(struct SplayNode* node, struct SplayNode** root) {
  //node to A, help to B
  SplayNode* help = right(node);

  if (*root == node)
    *root = help;

  assignPointer(node->right, left(help));
  noPointerReads++;
  if (left(help) != NULL) {
    help->left->parent = node;
    noPointerReads += 2;
    noPointerAssigns++;
  }
  assignPointer(help->parent, parent(node));
  noPointerReads++;
  if (parent(node) != NULL){
    if (node == left(parent(node))) {
      node->parent->left = help;
      noPointerReads += 2;
      noPointerAssigns++;
    }
    else if (node == right(parent(node))) {
      node->parent->right = help;
      noPointerReads += 2;
      noPointerAssigns++;
    }
  }
  assignPointer(help->left, node);
  assignPointer(node->parent, help);
  noPointerReads += 2;
}

/*
      A                      B
    /   \                   / \
   B    a3       ->        a1  A
  / \                         / \
 a1 a2                       a2 a3
*/
void rightRotate(struct SplayNode* node, struct SplayNode** root) {
  //node to A, help to B
  SplayNode* help = left(node);

  if (*root == node)
    *root = help;

  assignPointer(node->left, right(help));
  noPointerReads++;
  if (right(help) != NULL) {
    help->right->parent = node;
    noPointerReads += 2;
    noPointerAssigns++;
  }
  assignPointer(help->parent, parent(node));
  noPointerReads++;
  if (parent(node)!= NULL) {
    if (node == right(parent(node))) {
      node->parent->right = help;
      noPointerReads += 2;
      noPointerAssigns++;
    }
    else if (node == left(parent(node))) {
      node->parent->left = help;
      noPointerReads += 2;
      noPointerAssigns++;
    }
  }
  assignPointer(help->right, node);
  assignPointer(node->parent, help);
  noPointerReads += 2;
}

//metoda przyciągająca dany węzeł do korzenia
void splay(struct SplayNode* node, struct SplayNode** root) {
  //dopóki nie wciągnęliśmy node do korzenia
  while (parent(node) != NULL) {
    //node jest synem
    if (parent(node) == *root) {
      if(node == left(parent(node))) {
        rightRotate(parent(node), root);
      }
      else {
        leftRotate(parent(node), root);
      }
    }
    else {
      SplayNode* help_parent = parent(node);
      SplayNode* help_grandparent = parent(help_parent);
      //lewo lewo
      if (left(parent(node)) == node && left(parent(help_parent)) == help_parent) {
        rightRotate(help_grandparent, root);
        rightRotate(help_parent, root);
      }
      //prawo prawo
      else if(right(parent(node)) == node && right(parent(help_parent)) == help_parent) {
        leftRotate(help_grandparent, root);
        leftRotate(help_parent, root);
      }
      //prawo lewo
      else if(right(parent(node)) == node && left(parent(help_parent)) == help_parent) {
        leftRotate(help_parent, root);
        rightRotate(help_grandparent, root);
      }
      //prawo prawo
      else if(left(parent(node)) == node && right(parent(help_parent)) == help_parent) {
        rightRotate(help_parent, root);
        leftRotate(help_grandparent, root);
      }
    }
  }
}

void newInsert(struct SplayNode** root, int key) {
  struct SplayNode* toInsert = newNode(key);
  struct SplayNode* help = NULL;
  struct SplayNode* x = *root;
  while (x != NULL) {
    help = x;
    if (compareKeys(x->key, toInsert->key))
      x = left(x);
    else
      x = right(x);
  }
  assignPointer(toInsert->parent, help);
  noPointerReads++;
  if (help == NULL)
    *root = toInsert;
  else {
    if (compareKeys(help->key, toInsert->key))
      assignPointer(help->left, toInsert);
    else
      assignPointer(help->right, toInsert);
  }

  //przyciągamy nowowstawiony węzeł do korzenia
  splay(toInsert, root);
}

struct SplayNode* smallestValueNode(struct SplayNode* node) {
  struct SplayNode* minNode = node;
  if (minNode != NULL) {
    while (left(minNode) != NULL)
      minNode = left(minNode);
  }
  return minNode;
}

struct SplayNode* findBSTToReplace(struct SplayNode* x) {
  return smallestValueNode(x->right);
}


void deleteNode(struct SplayNode* node, struct SplayNode** root) {
  splay(node, root);
  struct SplayNode** left_subtree = &((*root)->left);
  noPointerReads++;
  if(*left_subtree != NULL)
    (*left_subtree)->parent = NULL;
  noPointerAssigns++;
  
  struct SplayNode** right_subtree = &((*root)->right);
  noPointerReads++;
  if(*right_subtree != NULL)
    (*right_subtree)->parent = NULL;
  noPointerAssigns++;

  if(*left_subtree != NULL) {
    struct SplayNode* help = *left_subtree;
    struct SplayNode* max = maxNode(help);
    splay(max, left_subtree);
    (*left_subtree)->right = (*right_subtree);
    if(*right_subtree != NULL)
      (*right_subtree)->parent = (*left_subtree);
    noPointerReads++;
    noPointerAssigns++;
    *root = *left_subtree;
  }
  else {
    *root = *right_subtree;
  }

  delete node;
}

void deleteElement(struct SplayNode** root, int key) {
  SplayNode* n = *root;
  while(n != NULL) {
    if(compareKeysEquals(n->key, key))
      break;
    if(compareKeys(n->key, key))
      n = left(n);
    else 
      n = right(n);
  }
  if(n != NULL) {
    deleteNode(n, root);
  }
  
}

//nie badamy złożoności tej operacji, stąd tu nie używam funkcji
int treeHeight(struct SplayNode* root) {
  if (root == NULL)
    return 0;
  int leftHeight = treeHeight(root->left);
  int rightHeight = treeHeight(root->right);
  return ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
}

// global variables used in `print_BST`
char* left_trace; // needs to be allocaded with size
char* right_trace; // needs to be allocaded with size


void printRBT( struct SplayNode* root, int depth, char prefix) {
  if ( root == NULL ) return;
  if ( root->left != NULL ){
    printRBT(root->left, depth+1, '/');
  }
  if (prefix == '/') left_trace[depth-1]='|';
  if (prefix == '\\') right_trace[depth-1]=' ';
  if ( depth==0) printf("-");
  if ( depth>0) printf(" ");
  for(int i=0; i<depth-1; i++)
    if ( left_trace[i]== '|' || right_trace[i]=='|' ) {
      printf("| ");
    } else {
      printf("  ");
    }
  if ( depth>0 ) printf("%c-", prefix);
  printf("[%d]\n", root->key);
  left_trace[depth]=' ';
  if ( root->right != NULL ){
    right_trace[depth]='|';
    printRBT(root->right, depth+1, '\\');
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

  struct SplayNode* tree = NULL;
  double avgKeyCompare, avgPointerReads, avgPointerAssigns, avgTreeHeights = 0.0;
  long maxKeyCompare = 0, maxPointerReads = 0, maxPointerAssigns = 0;
  long long allKeyCompare = 0, allPointerReads = 0, allPointerAssigns = 0, allTreeHeights = 0;

  while(i < n && cin >> element) {
    newInsert(&tree, element);
    if (noKeyComparisons > maxKeyCompare)
      maxKeyCompare = noKeyComparisons;
    if (noPointerAssigns > maxPointerAssigns)
      maxPointerAssigns = noPointerAssigns;
    if (noPointerReads > maxPointerReads)
      maxPointerReads = noPointerReads;
    
    allKeyCompare += noKeyComparisons;
    allPointerAssigns += noPointerAssigns;
    allPointerReads += noPointerReads;
    allTreeHeights += treeHeight(tree);

    noPointerAssigns = 0;
    noPointerReads = 0;
    noKeyComparisons = 0;

    if (n <= 50) {
      cout << "insert " << element << endl;
      printRBT(tree, 0, '-');
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
    deleteElement(&tree, element);
    if (noKeyComparisons > maxKeyCompare)
      maxKeyCompare = noKeyComparisons;
    if (noPointerAssigns > maxPointerAssigns)
      maxPointerAssigns = noPointerAssigns;
    if (noPointerReads > maxPointerReads)
      maxPointerReads = noPointerReads;
    
    allKeyCompare += noKeyComparisons;
    allPointerAssigns += noPointerAssigns;
    allPointerReads += noPointerReads;
    allTreeHeights += treeHeight(tree);

    noPointerAssigns = 0;
    noPointerReads = 0;
    noKeyComparisons = 0;

    if (n <= 50) {
      cout << "delete " << element << endl;
      printRBT(tree, 0, '-');
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


