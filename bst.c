#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct bst_node {
    int val;
	int size;
	int height;
	struct bst_node *parent;
    struct bst_node *left;
    struct bst_node *right;

};
typedef struct bst_node NODE;


struct bst {
    NODE *root;
};

static void preorder(NODE *, int);
static NODE* avl_cycle_tree_right(NODE *);
static NODE* avl_cycle_tree_left(NODE *);
static void inorder(NODE *);

/*****************************/
/******** BST Helpers ********/

// Returns 1 if empty tree
static int is_empty_tree(NODE *r) {
	return r==NULL;
}

// Returns 1 if not empty tree
static int is_not_empty_tree(NODE *r) {
	return is_empty_tree(r)==0;
}

// Returns 1 if node is the root of a tree.
static int is_root(NODE *r) {
	if (is_empty_tree(r)) return 0;
	return (r->parent==NULL ? 1 : 0);
}

// Returns 1 if node is not the root of a tree.
static int is_not_root(NODE *r) {
	return is_root(r)==0;
}

// Returns 1 if node is a left child
static int is_left_child(NODE *r) {
	if (is_empty_tree(r)) return 0;
	if (is_not_root(r)) {
		if (r->parent->left == r) return 1;
	}
}

// Returns 1 if node is a right child
static int is_right_child(NODE *r) {
	if (is_empty_tree(r)) return 0;
	if (is_not_root(r)) {
		if (r->parent->right == r) return 1;
	}
	return 0;
}

// Returns 1 if node is a leaf
static int is_leaf(NODE *r) {
	assert (is_not_empty_tree(r));
	return ((r->left==NULL && r->right==NULL) ? 1 : 0);
}

// Return height of sub-tree with root r 
//  NOTE: the empty tree has height -1
static int height(NODE *r) {
	if (is_empty_tree(r)) return -1;
	else return r->height;
}

// Returns number of nodes in sub-tree with root r
static int size(NODE *r) {
	if (is_empty_tree(r)) return 0;
	else return r->size;
}


/*****************************/
/******** AVL Helpers ********/

// Recalculates the height & size for node r
static void avl_update_node(NODE *r) {
	if (is_empty_tree(r)) return;
	r->size = 1 + size(r->left)  + size(r->right);
	r->height = 1 + ( (height(r->left) > height(r->right)) ? height(r->left) : height(r->right) );
}

static int avl_balance_factor(NODE *r) {
	if (is_empty_tree(r)) return 0;
	return height(r->left) - height(r->right);
}

// Handles promoting a leaf node to an inner node
static void avl_promote_leaf_to_parent(NODE *parent, NODE *leaf) {
	if (is_empty_tree(parent) || is_empty_tree(leaf)) return;
	leaf->parent = parent;
}



// Cycle AVL tree to the right
static NODE* avl_cycle_tree_right(NODE *r) {
	NODE *a = r;
	NODE *b = r->left;
	NODE *d = b->right;
	
	a->left = d;
	b->right = a;
	
	if (is_not_empty_tree(d)) d->parent = a;
	if (is_not_empty_tree(a)) a->parent = b;
	avl_update_node(d);
	avl_update_node(a);
	avl_update_node(b);
	return b;
}

// Cycle AVL tree to the left
static NODE* avl_cycle_tree_left(NODE *r) {
	NODE *a = r;
	NODE *b = r->right;
	NODE *c = b->left;
	
	a->right = c;
	b->left = a;
	if (is_not_empty_tree(c)) c->parent = a;
	if (is_not_empty_tree(a)) a->parent = b;
	avl_update_node(c);
	avl_update_node(a);
	avl_update_node(b);
	return b;
}

// Cycle switch
static NODE* avl_cycle(NODE *r, int x) {
	int balance = avl_balance_factor(r);
			
	// Left Left Case
    if (balance > 1 && x < r->left->val) {
		return avl_cycle_tree_right(r);
    }
	
    // Left Right Case
    if (balance > 1 && x > r->left->val) {
		r->left = avl_cycle_tree_left(r->left);
		return avl_cycle_tree_right(r);
    }

    // Right Right Case
    if (balance < -1 && x > r->right->val) {
        return avl_cycle_tree_left(r);
	}

    // Right Left Case
    if (balance < -1 && x < r->right->val)
    {
		r->right = avl_cycle_tree_right(r->right);
		return avl_cycle_tree_left(r);
    }
}

// Flag is set to 1 if sub-tree with root r is NOT AVL
static int avl_validation_flag(NODE *r) {
	int balance = avl_balance_factor(r);	
	if (balance>1 || balance<-1) return 1;
	return 0;
}

// Validates if subtree with root r and additional node with val x is AVL
//  Returns the subtree if AVL, else cycles and returns the resulting sub-tree
static NODE * avl_validation(NODE *r, int x) {
	if (avl_validation_flag(r)) {
		printf("AVL violation (%d) Insert (%d)\n", r->val, x);
		preorder(r,0);
		return avl_cycle(r,x);
	}
	
	return r;
}

BST_PTR bst_create(){
  BST_PTR t = malloc(sizeof(struct bst));
  t->root = NULL;
  return t;
}


static void free_r(NODE *r){
    if(r==NULL) return;
    free_r(r->left);
    free_r(r->right);
    free(r);
}
void bst_free(BST_PTR t){
    free_r(t->root);
    free(t);
}

static void t_avl_parent_helper(NODE* r) {
	if (is_empty_tree(r)) return;
	printf("TESTING PARENTS N(%d) P(%d) :   PL(%d)  PR(%d)\n", r->val, r->parent->val, (is_not_empty_tree(r->parent->left) ? r->parent->left->val : 0), (is_not_empty_tree(r->parent->right) ? r->parent->right->val : 0) );
	assert(r->parent->left==r || r->parent->right==r);
	t_avl_parent_helper(r->left);
	t_avl_parent_helper(r->right);
}

int t_avl_parent(BST_PTR t) {
	t_avl_parent_helper(t->root->left);
	t_avl_parent_helper(t->root->right);
	return 1;
}

static NODE * insert(NODE *r, int x){
    NODE *leaf;
	
    if(r == NULL){
      leaf = malloc(sizeof(NODE));
	  leaf->parent = NULL;
      leaf->left = NULL;
      leaf->right = NULL;
	  leaf->height = 0;
	  leaf->size = 1;
      leaf->val = x;
      return leaf;
    }

    if(r->val == x)
        return r;
    if(x < r->val){
		r->left = avl_validation(insert(r->left, x),x);
		if (is_not_empty_tree(r->left)) r->left->parent = r;
		avl_update_node(r);
		return r;
    }
	
    else {
		r->right = avl_validation(insert(r->right,x),x);
		if (is_not_empty_tree(r->right)) r->right->parent = r;
		avl_update_node(r);
		return r;
    }
}

// how about an iterative version?
static NODE *insert_i(NODE *r, int x){

  return NULL;

}


void bst_insert(BST_PTR t, int x){
    t->root = insert(t->root, x);
}

int bst_contains(BST_PTR t, int x){
    NODE *p = t->root;

    while(p != NULL){

        if(p->val == x)
            return 1;
        if(x < p->val){
            p = p->left;
        }
        else
            p = p->right;
    }
    return 0;  
}

static int min_h(NODE *r){
  if(r==NULL)
    return -1; // should never happen!
  while(r->left != NULL)
      r = r->left;
  return r->val;
}

static int max_h(NODE *r){
  if(r==NULL)
    return -1; // should never happen!
  while(r->right != NULL)
      r = r->right;
  return r->val;
}

static NODE *remove_r(NODE *r, int x, int *success){
NODE   *tmp;
int sanity;

  if(r==NULL){
    *success = 0;
    return NULL;
  }
  if(r->val == x){
    *success = 1;

    if(r->left == NULL){
        tmp = r->right;
        free(r);
        return tmp;
    }
    if(r->right == NULL){
        tmp = r->left;
        free(r);
        return tmp;
    }
    // if we get here, r has two children
    r->val = min_h(r->right);
    r->right = remove_r(r->right, r->val, &sanity);
    if(!sanity)
        printf("ERROR:  remove() failed to delete promoted value?\n");
    return r;
  }
  if(x < r->val){
    r->left = remove_r(r->left, x, success);
  }
  else {
    r->right = remove_r(r->right, x, success);
  }
  return r;

}


int bst_remove(BST_PTR t, int x){
    int success;
    t->root = remove_r(t->root, x, &success);
    return success;
}

int bst_size(BST_PTR t){

    return size(t->root);
}


int bst_height(BST_PTR t){
    return height(t->root);

}

int bst_min(BST_PTR t){
    return min_h(t->root);
}

int bst_max(BST_PTR t){
    return max_h(t->root);
}

static void indent(int m){
    int i;
    for(i=0; i<m; i++)
        printf("-");
}

static void inorder(NODE *r){
  if(r==NULL) return;
  inorder(r->left);
  printf("[%d]\n", r->val);
  inorder(r->right);

}
void bst_inorder(BST_PTR t){

  printf("========BEGIN INORDER============\n");
  inorder(t->root);
  printf("=========END INORDER============\n");

}

static void preorder(NODE *r, int margin){
  if(r==NULL) {
    indent(margin);
    printf("NULL \n");
  } else {
    indent(margin);
    printf("%d\n", r->val);
    preorder(r->left, margin+3);
    preorder(r->right, margin+3);
  }
}

void bst_preorder(BST_PTR t){

  printf("========BEGIN PREORDER============\n");
  preorder(t->root, 0);
  printf("=========END PREORDER============\n");

}

/* TODO
 * Complete the (recursive) helper function for the post-order traversal.
 * Remember: the indentation needs to be proportional to the height of the node!
 */
static void postorder(NODE *r, int margin){
  if(r==NULL) {
    indent(margin);
    printf("NULL \n");
  } else {
    indent(margin);
    postorder(r->left, margin+3);
    postorder(r->right, margin+3);
	printf("%d\n", r->val);
  }
}

// indentation is proportional to depth of node being printed
//   depth is #hops from root.
void bst_postorder(BST_PTR t){

  printf("========BEGIN POSTORDER============\n");
  postorder(t->root, 0);
  printf("=========END POSTORDER============\n");

}

/* TODO:
 * Write the (recursive) helper function from_arr, used by
 * bst_from_sorted_arr(...). The function must return a sub-tree that is
 * perfectly balanced, given a sorted array of elements a.
 */
static NODE * from_arr(int *a, int n){
	int m;
	NODE *root;

   if(n <= 0) return NULL;
   m = n/2;
   root = malloc(sizeof(NODE));
   root->val = a[m];
   root->left = from_arr(a, m);
   root->right = from_arr(&(a[m+1]), n-(m+1));
   return root;
}

BST_PTR bst_from_sorted_arr(int *a, int n){

  BST_PTR t = bst_create();

  t->root = from_arr(a, n);

  return t;
}