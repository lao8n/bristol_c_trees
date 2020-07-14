/*********************************************************/
/* BST.C *************************************************/
/*********************************************************/

#include "bst.h"

#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3

/* BST.H NODE-VERSION PROTOTYPES  ************************/
bstnode*  bstnode_init(int sz, void* v);
void      bstnode_insert(bst* b, bstnode** node_ptr,
            void* v);
int       bstnode_size(bstnode* node);
int       bstnode_maxdepth(bstnode* node);
bool      bstnode_isin(bst* b, bstnode* node, void* v);
void      bstnode_insertarray(bstnode* node, void* v,
            int n);
void      bstnode_free(bstnode** node_ptr);
char*     bstnode_print(bst* b, bstnode* node);
void      bstnode_getordered(bst* b, bstnode* node,
            void** v_ptr);
void      bstnode_insertsortedarray(bst* b,
            bstnode** node_ptr, void** v_ptr, int start,
            int end);

/* BST HELPER PROTOTYPES *********************************/
void      bst_printinorder(bst* b);
void      bst_printpreorder(bst* b);
void      bst_printpostorder(bst* b);
void      bst_printlevelorder(bst* b);
void      print_voidarray(bst* b, void* v, int n);

/* BST NODE HELPER PROTOTYPES ****************************/
void*     gfmalloc(size_t size);
void*     gfcalloc(size_t n, size_t el_size);
void*     value_init(int sz, void* v);
void**    bstnode_getdataaddress(bstnode** node_ptr);
bstnode** bstnode_getleftaddress(bstnode** node_ptr);
bstnode** bstnode_getrightaddress(bstnode** node_ptr);
void      value_free(void** v_ptr);
void      bstnode_printinorder(bst* b, bstnode* node);
void      bstnode_printpreorder(bst* b, bstnode* node);
void      bstnode_printpostorder(bst* b, bstnode* node);
void      bstnode_printlevelorder(bst* b, bstnode* node);
void      bstnode_printgivenlevel(bst* b, bstnode* node,
            int depth);

/*********************************************************/
/* BST.H FUNCTIONS ***************************************/
/*********************************************************/

/* Initialise binary search tree */
bst* bst_init(int sz,
              int(*comp)(const void* a, const void* b),
              char*(*prnt)(const void* a))
{
  bst* b;

  if(sz <= ZERO){
    ON_ERROR("Size of BST element to bst_init <= 0\n");
  }

  b = (bst *) gfmalloc(sizeof(bst));

  b->top = NULL;
  b->elsz = sz;
  b->compare = comp;
  b->prntnode = prnt;

  return b;
}

/* Insert 1 item into the tree */
void bst_insert(bst* b, void* v)
{
  if(b == NULL){
    ON_ERROR("BST to bst_insert is NULL\n");
  }
  if(v == NULL){
    ON_ERROR("V to bst_insert is NULL\n");
  }

  bstnode_insert(b, &b->top, v);
}

/* Number of nodes in tree */
int bst_size(bst* b)
{
  if(b == NULL){
    ON_ERROR("BST to bst_size is NULL\n");
  }

  return bstnode_size(b->top);
}

/* Longest path from root to any leaf */
int bst_maxdepth(bst* b)
{
  if(b == NULL){
    ON_ERROR("BST to bst_maxdepth is NULL\n");
  }

  return bstnode_maxdepth(b->top);
}

/* Bulk insert n items from an array v into an initialised
tree */
void bst_insertarray(bst* b, void* v, int n)
{
  int i;

  if(b == NULL){
    ON_ERROR("BST to bst_insertarray is NULL\n");
  }
  if(v == NULL){
    ON_ERROR("V to bst_insertarray is NULL\n");
  }
  if(n <= 0){
    ON_ERROR("Size of array to bst_insertarray is <= 0\n");
  }
  /* No way to make sure that n is correct and that the
  pointer arithmetic doesn't go out of bounds */
  for(i = ZERO; i < n; i++){
    bst_insert(b, v);
    /* force in cast to char* to do pointer arithmetic */
    v = (void*)((char*) v + (size_t) b->elsz);
  }
}

/* Whether the data in v, is stored in the tree */
bool bst_isin(bst* b, void* v)
{
  if(b == NULL){
    ON_ERROR("BST to bst_isin is NULL\n");
  }
  if(v == NULL){
    ON_ERROR("V to bst_isin is NULL\n");
  }

  return bstnode_isin(b, b->top, v);
}

/* Clear all memory associated with tree, & set pointer to
NULL */
void bst_free(bst** p)
{
  bst* tempb_ptr;

  if(*p == NULL){
    ON_ERROR("BST to bst_free is NULL\n");
  }

  if(*p != NULL){
    tempb_ptr = *p;
    bstnode_free(&(*p)->top);
    free(tempb_ptr);
  }
  *p = NULL;
}

/* Return a string displaying the tree in a textual form
(head(left)(right)) recursively */
char* bst_print(bst* b)
{
  if(b == NULL){
    ON_ERROR("BST to bst_print is NULL\n");
  }

  return bstnode_print(b, b->top);
}

/* Fill an array with a copy of the sorted tree data */
void bst_getordered(bst* b, void* v)
{
  if(b == NULL){
    ON_ERROR("BST to bst_getordered is NULL\n");
  }
  if(v == NULL){
    ON_ERROR("V to bst_getordered is NULL\n");
  }
  /* Unfortunately as your prototype does not include the
  size of the array v we can do overflow checks */

  bstnode_getordered(b, b->top, &v);
}

bst* bst_rebalance(bst* b)
{
  bst* b_balanced;
  void* v;
  void *temp;

  if(b == NULL){
    ON_ERROR("BST to bst_rebalance is NULL\n");
  }

  v = gfcalloc((size_t) bst_size(b) + ONE,
    (size_t) b->elsz);
  temp = v;

  bstnode_getordered(b, b->top, &temp);
  b_balanced = bst_init(b->elsz,
    b->compare, b->prntnode);

  bstnode_insertsortedarray(b_balanced,
    &b_balanced->top, &v, ZERO, bst_size(b) - ONE);

  free(v);

  return b_balanced;
}

/*********************************************************/
/* BST HELPER FUNCTIONS **********************************/
/*********************************************************/

void bst_printinorder(bst *b)
{
  if(b == NULL){
    ON_ERROR("BST to bst_printinorder is NULL\n");
  }

  bstnode_printinorder(b, b->top);
}

void bst_printpreorder(bst *b)
{
  if(b == NULL){
    ON_ERROR("BST to bst_printpreorder is NULL\n");
  }

  bstnode_printpreorder(b, b->top);
}

void bst_printpostorder(bst *b)
{
  if(b == NULL){
    ON_ERROR("BST to bst_printpostorder is NULL\n");
  }

  bstnode_printpostorder(b, b->top);
}

void bst_printlevelorder(bst *b)
{
  if(b == NULL){
    ON_ERROR("BST to bst_printlevelorder is NULL\n");
  }

  bstnode_printlevelorder(b, b->top);
}

void print_voidarray(bst* b, void* v, int n)
{
  int i;
  void* temp = v;

  if(b == NULL){
    ON_ERROR("BST to print_voidarray is NULL\n");
  }
  if(v == NULL){
    ON_ERROR("V to print_voidarray is NULL\n");
  }
  if(n <= ZERO){
    ON_ERROR("Size of array to print_voidarray is <= 0\n");
  }

  for(i = ZERO; i < n; i++){
    printf("%s\n", b->prntnode(temp));
    temp = (void*)((char*) temp +
      (size_t) b->elsz);
  }
}

/*********************************************************/
/* BST.H NODE-VERSION FUNCTIONS **************************/
/*********************************************************/

bstnode* bstnode_init(int sz, void* v)
{
  bstnode *node;

  if(sz <= ZERO){
    ON_ERROR("Size of BST element to bstnode_init <= 0\n");
  }
  if(v == NULL){
    ON_ERROR("V to bstnode_init is NULL\n");
  }

  node = (bstnode *) gfmalloc(sizeof(bstnode));

  /* need to malloc space for v */
  node->data = value_init(sz, v);
  node->left = NULL;
  node->right = NULL;

  return node;
}

void bstnode_insert(bst* b, bstnode** node_ptr, void* v)
{
  if(b == NULL){
    ON_ERROR("BST to bstnode_insert is NULL\n");
  }
  if(v == NULL){
    ON_ERROR("V to bstnode_insert is NULL\n");
  }

  if(*node_ptr == NULL){
    *node_ptr = bstnode_init(b->elsz, v);
  }
  else if(b->compare(v,
    (*node_ptr)->data) < ZERO){
    bstnode_insert(b,
      bstnode_getleftaddress(node_ptr), v);
  }
  else if(b->compare(v,
    (*node_ptr)->data) > ZERO) {
    bstnode_insert(b,
      bstnode_getrightaddress(node_ptr), v);
  }
  /* if b->compare(v, node->data) == 0) i.e.
  v already in tree then do nothing as don't want
  replication*/
}

int bstnode_size(bstnode* node)
{
  /* branch ends */
  if(node == NULL){
    return ZERO;
  }
  /* sum sub-trees */
  else {
    return bstnode_size(node->left) +
      bstnode_size(node->right) + ONE;
  }
}

int bstnode_maxdepth(bstnode* node)
{
  int l_depth, r_depth;

  /* branch ends */
  if(node == NULL){
    return ZERO;
  }
  else {
    /* compute the depth of each subtree */
    l_depth = bstnode_maxdepth(node->left);
    r_depth = bstnode_maxdepth(node->right);

    /* use the larger one */
    if(l_depth > r_depth){
      return l_depth + ONE;
    }
    else {
      return r_depth + ONE;
    }
  }
}

bool bstnode_isin(bst* b, bstnode* node, void* v)
{
  if(b == NULL){
    ON_ERROR("BST to bstnode_isin is NULL\n");
  }
  if(v == NULL){
    ON_ERROR("V to bstnode_isin is NULL\n");
  }

  if(node == NULL){
    return false;
  }
  else if(b->compare(v, node->data) == ZERO){
    return true;
  }
  else if(b->compare(v, node->data) < ZERO){
    return bstnode_isin(b, node->left, v);
  }
  else if(b->compare(v, node->data) > ZERO){
    return bstnode_isin(b, node->right, v);
  }
  else {
    ON_ERROR("bstnode_isin failed\n");
  }
}

void bstnode_free(bstnode** node_ptr)
{
  if(*node_ptr != NULL){
    bstnode* tempnode_ptr = *node_ptr;
    value_free(bstnode_getdataaddress(node_ptr));
    bstnode_free(bstnode_getleftaddress(node_ptr));
    bstnode_free(bstnode_getrightaddress(node_ptr));
    free(tempnode_ptr);
  }
  *node_ptr = NULL;
}

char* bstnode_print(bst* b, bstnode* node)
{
  char *str_l, *str_r, *str_node, *str;
  int len_l, len_r, len_node;

  if(b == NULL){
    ON_ERROR("BST to bstnode_print is NULL");
  }

  if(node == NULL){
    return str = (char*) gfcalloc((size_t) ONE,
      sizeof(char));
  }
  str_l = bstnode_print(b, node->left);
  str_r = bstnode_print(b, node->right);
  str_node = b->prntnode(node->data);
  len_l = strlen(str_l);
  len_r = strlen(str_r);
  len_node = strlen(b->prntnode(node->data));

  /* THREE because two brackets and null character */
  str = (char*) gfmalloc((size_t) len_l + len_r + len_node
    + THREE);
  sprintf(str, "(%s%s%s)", str_node, str_l, str_r);

  free(str_l);
  free(str_r);

  return str;
}

void bstnode_getordered(bst* b, bstnode* node,
  void** v_ptr)
{
  if(b == NULL){
    ON_ERROR("BST to bstnode_getordered is NULL\n");
  }
  if(*v_ptr == NULL){
    ON_ERROR("V to bstnode_getordered is NULL\n");
  }

  if(node != NULL){
    bstnode_getordered(b, node->left, v_ptr);
    sprintf(*v_ptr, "%s",
      b->prntnode(node->data));
    *v_ptr = (void*)((char*) *v_ptr +
      (size_t) b->elsz);
    bstnode_getordered(b, node->right, v_ptr);
  }
}

void bstnode_insertsortedarray(bst* b_balanced,
  bstnode** node_ptr, void** v_ptr, int start, int end)
{
  int mid;
  void* mid_elem;

  if(b_balanced == NULL){
    ON_ERROR("BST to bstnode_getordered is NULL\n");
  }
  if(*v_ptr == NULL){
    ON_ERROR("V to bstnode_getordered is NULL\n");
  }

  /* Base case */
  if(start > end){
    return;
  }

  /* Get middle element and make root */
  mid = (start + end)/TWO;
  mid_elem = (void*)((char *) *v_ptr +
    (size_t) mid * b_balanced->elsz);
  if(*node_ptr == NULL){
    *node_ptr = bstnode_init(b_balanced->elsz,
    mid_elem);
  }

  /* Recursively construct the left subtree */
  bstnode_insertsortedarray(b_balanced,
    bstnode_getleftaddress(node_ptr),
    v_ptr, start, mid - ONE);

  /* Recursively construct the right subtree */
  bstnode_insertsortedarray(b_balanced,
    bstnode_getrightaddress(node_ptr),
    v_ptr, mid + ONE, end);
}


/*********************************************************/
/* BST NODE HELPER FUNCTIONS *****************************/
/*********************************************************/

/* Note less need for error messages here because these
functions are called in other functions that already
check for NULL etc. */

void* gfmalloc(size_t size)
{
  void *p;

  p = malloc(size);
  if(p == NULL){
    ON_ERROR("Malloc failed\n");
  }
  return p;
}

void* gfcalloc(size_t n, size_t el_size)
{
  void *p;

  p = calloc(n, el_size);
  if(p == NULL){
    ON_ERROR("Calloc failed\n");
  }
  return p;
}

void* value_init(int sz, void* v)
{
  void *value;
  /* need to malloc space for *v, as bstnode just has space
  for pointer */
  value = gfmalloc((size_t) sz);
  value = memcpy(value, v, (size_t) sz);

  return value;
}

void** bstnode_getdataaddress(bstnode** node_ptr)
{
  return &(*node_ptr)->data;
}

bstnode** bstnode_getleftaddress(bstnode** node_ptr)
{
  return &(*node_ptr)->left;
}

bstnode** bstnode_getrightaddress(bstnode** node_ptr)
{
  return &(*node_ptr)->right;
}

void value_free(void** v_ptr)
{
  void* tempv_ptr = *v_ptr;
  free(tempv_ptr);
  *v_ptr = NULL;
}

void bstnode_printinorder(bst* b, bstnode* node)
{
  if(b == NULL){
    ON_ERROR("BST to bstnode_printinorder is NULL\n");
  }

  if(node != NULL){
    bstnode_printinorder(b, node->left);
    printf("%s\n", b->prntnode(node->data));
    bstnode_printinorder(b, node->left);
  }
}

void bstnode_printpreorder(bst* b, bstnode* node)
{
  if(b == NULL){
    ON_ERROR("BST to bstnode_printpreorder is NULL\n");
  }

  if(node != NULL){
    printf("%s\n", b->prntnode(node->data));
    bstnode_printpreorder(b, node->left);
    bstnode_printpreorder(b, node->right);
  }
}

void bstnode_printpostorder(bst* b, bstnode* node)
{
  if(b == NULL){
    ON_ERROR("BST to bstnode_printinorder is NULL\n");
  }

  if(node != NULL){
    bstnode_printpostorder(b, node->left);
    bstnode_printpostorder(b, node->right);
    printf("%s\n", b->prntnode(node->data));
  }
}

void bstnode_printlevelorder(bst* b, bstnode* node)
{
  int i;
  int maxdepth;

  if(b == NULL){
    ON_ERROR("BST to bstnode_printlevelorder is NULL\n");
  }

  maxdepth = bst_maxdepth(b);
  for(i = ONE; i <= maxdepth; i++){
    bstnode_printgivenlevel(b, node, i);
  }
}

void bstnode_printgivenlevel(bst* b, bstnode* node,
  int depth)
{
  if(b == NULL){
    ON_ERROR("BST to bstnode_printgivenlevel is NULL\n");
  }
  if(depth <= ZERO){
    ON_ERROR("Depth to bstnode_printgivenlevel is <= 0\n");
  }

  if(node == NULL){
    return;
  }
  if(depth == ONE){
    printf("%s\n", b->prntnode(node->data));
  }
  else if(depth > ONE){
    bstnode_printgivenlevel(b, node->right,
      depth - ONE);
    bstnode_printgivenlevel(b, node->left,
      depth - ONE);
  }
}
