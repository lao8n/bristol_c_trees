/*********************************************************/
/* EXT.C *************************************************/
/*********************************************************/

#include "ext.h"

int main(void)
{
  int a[N], i;
  int std_worst, rb_worst;
  int std_sum = ZERO, rb_sum = ZERO;
  double std_avg, rb_avg;
  double std_worsttheo, rb_worsttheo;
  double std_avgcalc, N_d = N;
  double std_avgtheo;

  srand(time(NULL));
  make_array(a);

  /* WORST CASE ******************************************/
  std_worst = std_heightworst();
  rb_worst = RBTree_heightworst();
  /* Red-black tree with N internal nodes has height at
  most 2lg(n + 1) (i.e. log base 2) */
  std_worsttheo = (double) N;
  rb_worsttheo = TWO * my_log((double) N + ONE, TWO);

  /* AVERAGE CASE ****************************************/
  for(i = ZERO; i < SAMPLESIZE; i++){
    std_sum = std_sum + std_heightavg(a);
    rb_sum = rb_sum + RBTree_heightavg(a);
  }
  std_avg = (double) std_sum / SAMPLESIZE;
  rb_avg = (double) rb_sum / SAMPLESIZE;
  /* See word document for mathematical appendix */
  std_avgcalc = (N_d * N_d * N_d + SIX * N_d * N_d +
    ELEVEN * N_d + SIX) / TWENTYFOUR;
  std_avgtheo = (double) my_log((double) std_avgcalc, TWO);

  /* PRINT TABLE *****************************************/

  /* Note I didn't #define the print spans - these are
  arbitrary and just to make the table look nice */
  printf("\n  %-14s | %-14s | %-14s\n",
    "Basic BST", "Computed", "Theoretical");
  printf("  %-14s | %-14s | %-14s\n",
    "--------------", "--------------", "--------------");
  printf("  %-14s | %-14d | %-14.0f\n",
    "Worst", std_worst, std_worsttheo);
  printf("  %-14s | %-14.2f | %-14.2f\n",
    "Average", std_avg, std_avgtheo);

  printf("\n  %-14s | %-14s | %-14s\n",
    "Red-Black BST", "Computed", "Theoretical");
  printf("  %-14s | %-14s | %-14s\n",
    "--------------", "--------------", "--------------");
  printf("  %-14s | %-14d | %-14.2f\n",
    "Worst", rb_worst, rb_worsttheo);
  printf("  %-14s | %-14.2f | <%-13.2f\n\n",
    "Average", rb_avg, rb_worsttheo);

  return EXIT_SUCCESS;
}

/*********************************************************/
/* STANDARD BST ******************************************/
/*********************************************************/

Node* node_init(int data)
{
  Node* node = (Node*) gfmalloc(sizeof(Node));

  node->key = data;
  node->left = NULL;
  node->right = NULL;

  return node;
}

Node* node_insert(Node* node, int data)
{
  /* If the tree is empty return new node */
  if(node == NULL){
    return node_init(data);
  }
  /*O/w recur down tree */
  if(data < node->key){
    node->left = node_insert(node->left, data);
  }
  else if(data > node->key){
    node->right = node_insert(node->right, data);
  }

  return node;
}

void node_printinorder(Node* node)
{
  if(node != NULL){
    node_printinorder(node->left);
    printf("%d\n", node->key);
    node_printinorder(node->right);
  }
}

int node_height(Node *node)
{
  int l_height, r_height;

  if(node == NULL){
    return ZERO;
  }
  else {
    /*compute the height of each subtree */
    l_height = node_height(node->left);
    r_height = node_height(node->right);

    /*use the larger one */
    if(l_height > r_height){
      return l_height + ONE;
    }
    else {
      return r_height + ONE;
    }
  }
}

int std_heightworst(void)
{
  int i, height;
  Node *root = NULL;

  root = node_insert(root, ZERO);
  for(i = ONE; i < N; i++){
    node_insert(root, i);
  }

  height = node_height(root);
  std_free(root);

  return height;
}

int std_heightavg(int a[N])
{
  Node *root = NULL;
  int i, height;

  randomise(a);
  root = node_insert(root, a[ZERO]);
  for(i = ONE; i < N; i++){
    node_insert(root, a[i]);
  }
  height = node_height(root);
  std_free(root);

  return height;
}

void std_free(Node* node)
{
  if(node == NULL){
    return;
  }
  std_free(node->left);
  std_free(node->right);

  free(node);
}

/*********************************************************/
/* RED-BLACK BST *****************************************/
/*********************************************************/

RBTree* RBTree_init(void)
{
  RBTree *newtree;
  RBNode *tmp;

  /* init tree */
  newtree = (RBTree*) gfmalloc(sizeof(RBTree));

  /* init nil */
  tmp = newtree->nil = (RBNode*) gfmalloc(sizeof(RBNode));
  tmp->parent = tmp->left = tmp->right = tmp;
  tmp->colour = black;
  tmp->key = ZERO;

  /* init root */
  tmp = newtree->root = (RBNode*) gfmalloc(sizeof(RBNode));
  tmp->parent = tmp->left = tmp->right = newtree->nil;
  tmp->colour = black;
  tmp->key = ZERO;

  return(newtree);
}

void rotate_left(RBTree* tree, RBNode* x)
{
  RBNode *y, *nil = tree->nil;

  /* DIAGRAM **********************************************

       |          |         |\           |
       x          x         x |          y
      / \    =>  /|    =>  /|\|   =>    / \
     a   y      a | y     a | y        x   c
        / \       |/ \      |  \      / \
       b   c      b   c     b   c    a   b
    STEP 1      STEP 2    STEP 3     STEP 4

  ********************************************************/

  /* STEP 1 */
  y = x->right;
  /* STEP 2 */
  x->right = y->left;
  if(y->left != nil){
    y->left->parent = x;
  }
  /* STEP 3 */
  y->parent = x->parent;
  if(x == x->parent->left){
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  /* STEP 4 */
  y->left = x;
  x->parent = y;
}

void rotate_right(RBTree* tree, RBNode* y)
{
  RBNode *x, *nil = tree->nil;

  /* DIAGRAM **********************************************

       |          |          /|         |
       y          y         | y         x
      / \   =>    |\    =>  | |\   =>  / \
     x   c      x | c       x | c     a   y
    / \        / \|        / \|          / \
   a   b      a   b       a   b         b   c
   STEP 1     STEP 2      STEP 3      STEP 4

  ********************************************************/

  /* STEP 1 */
  x = y->left;
  /* STEP 2 */
  y->left = x->right;
  if(x->right != nil){
    x->right->parent = y;
  }
  /* STEP 3 */
  x->parent = y->parent;
  if(y == y->parent->left){
    y->parent->left = x;
  }
  else {
    y->parent->right = x;
  }
  /* STEP 4 */
  x->right = y;
  y->parent = x;
}

void RBTree_insertiter(RBTree* tree, RBNode* z)
{
  RBNode *x, *y, *nil = tree->nil;

  z->left = z->right = nil;
  y = tree->root;
  x = tree->root->left;

  /* loop through tree until hit nil */
  while(x != nil){
    y = x;
    if(z->key < x->key){
      x = x->left;
    }
    else if (z->key > x->key){
      x = x->right;
    }
  }
  /* y is then lagged version of x i.e. x's parent and
  then set z's parent to be y*/
  z->parent = y;
  /* which of y's two children left or right to place z */
  if((y == tree->root || z->key < y->key)){
    y->left = z;
  }
  else {
    y->right = z;
  }
}

RBNode* RBTree_insert(RBTree* tree, int key)
{
  RBNode *uncle, *z, *newnode;

  /* init z node and standard insert into bst, set colour
  = red */
  z = (RBNode*) gfmalloc(sizeof(RBNode));
  z->key = key;
  RBTree_insertiter(tree, z);
  newnode = z;
  z->colour = red;

  /* RB Insert Fixup */
  /* Property 1 = Every node is either red or black
     Property 2 = The root is black
     Property 3 = Every leaf (nil) is black
     Property 4 = If a node is red then both its children
                  are black
     Property 5 = For each node, all simple paths from the
                  node to descendant leaves contain the
                  same number of black nodes */

  /* Only properties 2 and 4 may be violated in the
  insertion process which we use the following code to
  fix */
  while(z->parent->colour == red){
    /* parent is grandparent's left child*/
    if(z->parent == z->parent->parent->left){
      /* therefore uncle is grandparent's right child */
      uncle = z->parent->parent->right;
      /* CASE OF THE RED UNCLE ***************************/
      /*
      # # = black   [ ] = red
      Case: z is right child
      This violates property 4 because A should have
      black children but it doesn't.

                #C#               z = [C]
                / \                   / \
              [A]  [D]=uncle =>     #A# #D#
                \                    \
              z=[B]                  [B]
              STEP 1                STEP 2

      Case: z is left child

                #C#               z = [C]
                / \                   / \
              [A]  [D]=uncle =>     #A# #D#
              /                     /
          z=[B]                   [B]
              STEP 1                STEP 2
      */
      /* STEP 1 */
      if(uncle->colour == red){
        /* STEP 2 */
        z->parent->colour = black;
        uncle->colour = black;
        z->parent->parent->colour = red;
        z = z->parent->parent;
      }
      /* CASE OF BLACK UNCLE *****************************/
      else {
        /* if on inside of tree rotate to outside */
        /* # # = black   [ ] = red

                #C#                    #C#
                / \                    / \
              [A]  #D# = uncle =>    [B] #D#
              /  \                  /  \
             a   [B] = z      z = [A]   c
                 / \             /   \
                b   c           a     b
        */
        if(z == z->parent->right){
          z = z->parent;
          rotate_left(tree, z);
        }
        /* now on right rotate tree right */
        /* # # = black   [ ] = red

               #C#              #B#
               /  \             /  \
             [B]  #D#         [A]   [C]
            /   \       =>   /  \   /  \
      z = [A]    c          a    b c    #D#
         /   \
        a     b

        */
        z->parent->colour = black;
        z->parent->parent->colour = red;
        rotate_right(tree, z->parent->parent);
      }
    }
    /* THIS IS JUST SYMMETRIC TO ABOVE CASE THEREFORE NO
    DIAGRAM INCLUDED */
    /* parent is grandparent's right child */
    else {
      /* therefore uncle is grandparent's left child */
      uncle = z->parent->parent->left;
      /* CASE OF THE RED UNCLE ***************************/
      if(uncle->colour == red){
        z->parent->colour = black;
        uncle->colour = black;
        z->parent->parent->colour = red;
        z = z->parent->parent;
      }
      /* CASE OF BLACK UNCLE *****************************/
      else {
        /* if on inside of tree rotate to outside */
        if(z == z->parent->left){
          z = z->parent;
          rotate_right(tree, z);
        }
        z->parent->colour = black;
        z->parent->parent->colour = red;
        rotate_left(tree, z->parent->parent);
      }
    }
  }
  tree->root->left->colour = black;
  return newnode;
}

int RBNode_height(RBTree *tree, RBNode* z)
{
  int l_height, r_height;

  if(z == tree->nil){
    return ZERO;
  }
  else {
    /*compute the height of each subtree */
    l_height = RBNode_height(tree, z->left);
    r_height = RBNode_height(tree, z->right);

    /*use the larger one */
    if(l_height > r_height){
      return l_height + ONE;
    }
    else {
      return r_height + ONE;
    }
  }
}

int RBTree_heightworst(void)
{
  RBTree *tree;
  int i, height;

  /* Create & print RED-BLACK BST */
  tree = RBTree_init();
  for(i = ZERO; i < N; i++){
    RBTree_insert(tree, i);
  }
  height = RBNode_height(tree, tree->root);
  RBTree_free(tree);

  return height;
}

int RBTree_heightavg(int a[N])
{
  RBTree *tree;
  int i, height;

  tree = RBTree_init();
  randomise(a);
  for(i = ZERO; i < N; i++){
    RBTree_insert(tree, a[i]);
  }
  height = RBNode_height(tree, tree->root);
  RBTree_free(tree);

  return height;
}

void RBTree_free(RBTree* tree)
{
  RBTree_recur(tree, tree->root->left);
  free(tree->root);
  free(tree->nil);
  free(tree);
}

void RBTree_recur(RBTree* tree, RBNode* x)
{
  RBNode* nil = tree->nil;

  if(x != nil){
    RBTree_recur(tree, x->left);
    RBTree_recur(tree, x->right);
    free(x);
  }
}

/*********************************************************/
/* MISC **************************************************/
/*********************************************************/

void* gfmalloc(size_t size)
{
  void *p;

  p = malloc(size);
  if(p == NULL){
    ON_ERROR("Malloc failed\n");
  }
  return p;
}

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void randomise(int a[N])
{
  int i;
  /* uses Fisher-Yates shuffle algorithm to get a 'random'
  list of unique numbers from 0 to N */
  for(i = N - ONE; i > ZERO; i--){
    /* Pick a random index from 0 to i */
    int j = rand() % (i + ONE);
    /* Swap arr[i] with the element at random index */
    swap(&a[i], &a[j]);
  }
}

void make_array(int a[N])
{
  int i;

  for(i = ZERO; i < N; i++){
    a[i] = i;
  }
}

double my_log(double x, int base)
{
  return log(x) / log(base);
}
