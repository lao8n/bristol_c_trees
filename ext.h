/*********************************************************/
/* EXT.H *************************************************/
/*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#define ON_ERROR(STR) fprintf(stderr, STR); \
        exit(EXIT_FAILURE);
#define N 10000
#define SAMPLESIZE 100
#define ZERO 0
#define ONE 1
#define TWO 2
#define SIX 6
#define ELEVEN 11
#define TWENTYFOUR 24

/*********************************************************/
/* STANDARD BST ******************************************/
/*********************************************************/

struct node {
  int            key;
  struct node*   left;
  struct node*   right;
};
typedef struct node Node;

Node*     node_init(int data);
Node*     node_insert(Node* node, int data);
void      node_printinorder(Node* node);
int       node_height(Node *node);
int       std_heightworst(void);
int       std_heightavg(int a[N]);
void      std_free(Node* node);

/*********************************************************/
/* RED-BLACK BST *****************************************/
/*********************************************************/

enum rdblk {black, red};
typedef enum rdblk rdblk;

struct rbnode {
  rdblk          colour;
  int            key;
  struct rbnode* parent;
  struct rbnode* left;
  struct rbnode* right;
};
typedef struct rbnode RBNode;

struct rbtree {
  RBNode*        nil;
  RBNode*        root;
};
typedef struct rbtree RBTree;

RBTree*   RBTree_init(void);
void      rotate_left(RBTree* tree, RBNode* x);
void      rotate_right(RBTree* tree, RBNode* y);
void      RBTree_insertiter(RBTree* tree, RBNode* z);
RBNode*   RBTree_insert(RBTree* tree, int key);
int       RBNode_height(RBTree *tree, RBNode* z);
int       RBTree_heightworst(void);
int       RBTree_heightavg(int a[N]);
void      RBTree_free(RBTree* tree);
void      RBTree_recur(RBTree* tree, RBNode* x);

/*********************************************************/
/* MISC **************************************************/
/*********************************************************/

void*     gfmalloc(size_t size);
void      swap(int *a, int *b);
void      randomise(int a[N]);
void      make_array(int a[N]);
double    my_log(double x, int base);
