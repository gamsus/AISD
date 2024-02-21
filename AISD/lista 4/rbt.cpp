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

enum Color {RED, BLACK};

struct RBTNode {
  int key;
  struct RBTNode *left, *right, *parent;
  Color color;
};

struct RBTNode* newNode(int value) {
  struct RBTNode* temp = (struct RBTNode*)malloc(sizeof(struct RBTNode));
  temp->key = value;
  temp->left = NULL;
  temp->right = NULL;
  temp->parent = NULL;
  temp->color = RED;
  return temp;
}

struct RBTNode* parent(struct RBTNode* node) {
  noPointerReads++;
  return node->parent;
}

struct RBTNode* left(struct RBTNode* node) {
  noPointerReads++;
  return node->left;
}

struct RBTNode* right(struct RBTNode* node) {
  noPointerReads++;
  return node->right;
}

void assignPointer(struct RBTNode*& node, struct RBTNode* node2) {
  node = node2;
  noPointerAssigns++;
}

/*
      A                         B
    /   \                      / \
   a1    B        ->          A   a3
        /  \                 / \
       a2   a3              a1 a2
*/
void leftRotate(struct RBTNode* node, struct RBTNode** root) {
  //node to A, help to B
  RBTNode* help = right(node);

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
void rightRotate(struct RBTNode* node, struct RBTNode** root) {
  //node to A, help to B
  RBTNode* help = left(node);

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

void insertFix(struct RBTNode* node, struct RBTNode** root) {
  RBTNode* help;
  //dopóki rodzic ma ten sam kolor co naprawiany węzeł
  while(parent(node) != NULL && parent(node)->color == RED) {
    
    if (parent(parent(node)) != NULL && parent(node) == right(parent(parent(node)))) {
      help = left(parent(parent(node)));

      //CASE 1 - ojciec i wujek czerwoni
      //Recolor i zmiana węzła do korekty na jego dziadka
      if (help != NULL && help->color == RED) {
        help->color = BLACK;
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        noPointerReads += 3;
        node = node->parent->parent;
      }

      else {
        //CASE 2 - zig-zag (po tym jest zawsze straight)
        if (node == left(parent(node))) {
          node = parent(node);
          rightRotate(node, root);
        }
        //CASE 3 - straight
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        noPointerReads += 3;
        leftRotate(node->parent->parent, root);
      }

    }
    else if (parent(parent(node)) != NULL){
      help = right(parent(parent(node)));

      //CASE 1 - ojciec i wujek czerwoni
      //Recolor i zmiana węzła do korekty na jego dziadka
      if (help != NULL && help->color == RED) {
        help->color = BLACK;
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        noPointerReads += 3;
        node = node->parent->parent;
      }

      else {
        //CASE 2 - zig-zag (po tym jest zawsze straight)
        if (node == right(parent(node))) {
          node = parent(node);
          leftRotate(node, root);
        }
        //CASE 3 - straight
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        noPointerReads += 3;
        rightRotate(node->parent->parent, root);
      }

    }
    //korzeń ma być czarny
    else if (parent(parent(node)) == NULL) {
      if (parent(node)->color == RED) {
        node->parent->color = BLACK;
        noPointerReads++;
      }
      break;
    }
  }
  //korzeń ma być czarny
  (*root)->color = BLACK;

}

void newInsert(struct RBTNode** root, int key) {
  struct RBTNode* toInsert = newNode(key);
  struct RBTNode* help = NULL;
  struct RBTNode* x = *root;
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

  insertFix(toInsert, root);
}

struct RBTNode* smallestValueNode(struct RBTNode* node) {
  struct RBTNode* minNode = node;
  if (minNode != NULL) {
    while (left(minNode) != NULL)
      minNode = left(minNode);
  }
  return minNode;
}

struct RBTNode* findBSTToReplace(struct RBTNode* x) {
  return smallestValueNode(x->right);
}

struct RBTNode* getSibling(struct RBTNode* x) {
  if (x != NULL) {
    if (parent(x) != NULL) {
      if (x == left(parent(x))) {
        return right(parent(x));
      }
      else {
        return left(parent(x));
      }
    }
  }
  return NULL;
}

bool hasRedChild(struct RBTNode* x) {
  struct RBTNode* left_help = left(x);
  struct RBTNode* right_help = right(x);
  if(left_help != NULL) {
    if(left_help->color == RED)
      return true;
  }

  if(right_help != NULL) {
    if(right_help->color == RED)
      return true;
  }

  return false;
}

void fixDoubleBlack(struct RBTNode* x, struct RBTNode** root) {
  //koniec naprawiania, jesteśmy w korzeniu
  if (x == *root)
    return;

  RBTNode* sibling = getSibling(x);
  RBTNode* help_parent = parent(x);

  if(sibling == NULL) {
    //brak rodzeństwa, idziemy w górę
    fixDoubleBlack(help_parent, root);
  }
  else {
    if (sibling->color == RED) {
      //czerwony brat, zmieniamy na czarnego a rodzica na czerwonego
      help_parent->color = RED;
      sibling->color = BLACK;
      if (sibling == left(parent(sibling))) {
        rightRotate(help_parent, root);
      }
      else {
        leftRotate(help_parent, root);
      }
      //wchodzimy z rekursją jeden poziom wyżej
      fixDoubleBlack(x, root);
    }
    else {
      if (hasRedChild(sibling)) {
        if (left(sibling) != NULL && left(sibling)->color == RED) {
          if (sibling == left(parent(sibling))) {
            left(sibling)->color = sibling->color;
            sibling->color = help_parent->color;
            rightRotate(help_parent, root);
          }
          else {
            left(sibling)->color = help_parent->color;
            rightRotate(sibling, root);
            leftRotate(help_parent, root);
          }
        }
        else {
          if (sibling == left(parent(sibling))) {
            right(sibling)->color = help_parent->color;
            leftRotate(sibling, root);
            rightRotate(help_parent, root);
          }
          else {
            right(sibling)->color = sibling->color;
            sibling->color = help_parent->color;
            leftRotate(help_parent, root);
          }
        }
        help_parent->color = BLACK;
      }
      else {
        sibling->color = RED;
        if (help_parent->color == BLACK)
          fixDoubleBlack(help_parent, root);
        else
          help_parent->color = BLACK;
      }
    }
  }
}

void deleteNode(struct RBTNode* v, struct RBTNode** root) {
  struct RBTNode* u = findBSTToReplace(v);

  // u i v są czarne
  bool uvBlack = ((u == NULL || u->color == BLACK) && v->color == BLACK);
  RBTNode *parent = v->parent;

  if (u == NULL) {
    // wtedy v jest liściem
    if (v == *root) {
      // jeśli v jest korzeniem, drzewo umiera
      *root = NULL;
    }
    else {
      if (uvBlack) {
        fixDoubleBlack(v, root);
      }
      else {
        if (getSibling(v) != NULL)
          getSibling(v)->color = RED;
      }
      if (v == left(parent)) {
        parent->left = NULL;
        noPointerAssigns++;
        noPointerReads++;
      }
      else {
        parent->right = NULL;
        noPointerAssigns++;
        noPointerReads++;
      }
    }
    delete v;
    return;
  }

  if (left(v) == NULL || right(v) == NULL) {
    if (v == *root) {
      v->key = u->key;
      v->left = v->right = NULL;
      noPointerAssigns += 2;
      noPointerReads += 2;
      delete u;
    }
    else {
      if (v == left(parent)) {
        parent->left = u;
        noPointerAssigns++;
        noPointerReads++;
      }
      else {
        parent->right = u;
        noPointerAssigns++;
        noPointerReads++;
      }
      delete v;
      u->parent = parent;
      if (uvBlack) {
        fixDoubleBlack(u, root);
      }
      else {
        u->color = BLACK;
      }
    }
    return;
  }
  
  // v ma dwójkę dzieci, podmiana wartości z następnikiem i rekurencja
  int temp;
  temp = u->key;
  u->key = v->key;
  v->key = temp;
  deleteNode(u, root);
}

void deleteElement(struct RBTNode** root, int key) {
  if (*root == NULL)
    return;

  struct RBTNode* v = *root;

  while(v != NULL) {
    if (compareKeys(v->key, key)) {
      v = left(v);
    }
    else if (compareKeys(key, v->key)) {
      v = right(v);
    }
    else
      break;
  }

  //nie znaleziono elementu
  if (v == NULL)
    return;

  deleteNode(v, root);
}

//nie badamy złożoności tej operacji, stąd tu nie używam funkcji
int treeHeight(struct RBTNode* root) {
  if (root == NULL)
    return 0;
  int leftHeight = treeHeight(root->left);
  int rightHeight = treeHeight(root->right);
  return ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
}

// global variables used in `print_BST`
char* left_trace; // needs to be allocaded with size
char* right_trace; // needs to be allocaded with size


void printRBT( struct RBTNode* root, int depth, char prefix) {
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
  char color = ' ';
  if (root->color == RED) 
    color = 'R';
  else
    color = 'B';
  printf("[%d|%c]\n", root->key, color);
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

  struct RBTNode* tree = NULL;
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


