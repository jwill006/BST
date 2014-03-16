#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct bst_node {
    int val;
	int size;
	int height;
	int root;
    struct bst_node *left;
    struct bst_node *right;

};
typedef struct bst_node NODE;


struct bst {
    NODE *root;
};

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
	return r->root;
}

// Returns 1 if node is not the root of a tree.
static int is_not_root(NODE *r) {
	return is_root(r)==0;
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

// Cycle AVL tree to the left
static NODE* avl_cycle_tree_left(NODE *r) {
	NODE *a = r;
	NODE *b = r->right;
	NODE *c = (is_not_empty_tree(b) ? b->left : NULL);
	
	a->right = c;
	if (is_not_empty_tree(b)) b->left = a;
	avl_update_node(c);
	avl_update_node(a);
	avl_update_node(b);
	return b;
}

// Cycle AVL tree to the right
static NODE* avl_cycle_tree_right(NODE *r) {
	NODE *a = r;
	NODE *b = r->left;
	NODE *d = (is_not_empty_tree(b) ? b->right : NULL);
	
	a->left = d;
	if (is_not_empty_tree(b)) b->right = a;
	avl_update_node(d);
	avl_update_node(a);
	avl_update_node(b);
	return b;
}

// Cycle switch for insertion
static NODE* avl_insert_cycle(NODE *r, int x) {
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

// Cycle switch for deletion
static NODE* avl_delete_cycle(NODE *r) {
	int balance = avl_balance_factor(r);
	int l_balance = avl_balance_factor(r->left);
	int r_balance = avl_balance_factor(r->right);
			
	// Left Left Case
    if (balance > 1 && l_balance >= 0) {
		return avl_cycle_tree_right(r);
    }
	
    // Left Right Case
    if (balance > 1 && l_balance < 0) {
		r->left = avl_cycle_tree_left(r->left);
		avl_update_node(r);
		return avl_cycle_tree_right(r);
		
    }

    // Right Right Case
    if (balance < -1 && r_balance <= 0) {
        return avl_cycle_tree_left(r);
	}

    // Right Left Case
    if (balance < -1 && r_balance > 0)
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
static NODE * avl_validation(NODE *r, int x, int insert) {
	avl_update_node(r);
	if (avl_validation_flag(r)) {
		if (insert) return avl_insert_cycle(r,x);
		return avl_delete_cycle(r);
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

static NODE * insert(NODE *r, int x){
    NODE *leaf;
	
    if(r == NULL){
      leaf = malloc(sizeof(NODE));
	  leaf->root = 1;
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
		r->left = avl_validation(insert(r->left,x),x,1);
		if(is_not_empty_tree(r->left) && r->left->root) r->left->root = 0;
    }
	
    else {
		r->right = avl_validation(insert(r->right,x),x,1);
		if(is_not_empty_tree(r->right) && r->right->root) r->right->root = 0;
    }
	return avl_validation(r,x,1);
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

static NODE * min_h(NODE *r){
  if(r==NULL)
    return NULL; // should never happen!
  while(r->left != NULL)
      r = r->left;
  return r;
}

static NODE * max_h(NODE *r){
  if(r==NULL)
    return NULL; // should never happen!
  while(r->right != NULL)
      r = r->right;
  return r;
}

static NODE *remove_r(NODE *r, int x, int *success){
	NODE *tmp;
	
	if (is_empty_tree(r))
	        return r;
 
    // Target node is in left sub-tree
    if ( x < r->val )
        r->left = avl_validation(remove_r(r->left, x, success),x,0);

    // Target node is in right sub-tree
    else if( x > r->val )
        r->right = avl_validation(remove_r(r->right, x, success),x,0);

    // Found node with val x
    else
    {
        // node has 0 or 1 child
        if( (is_empty_tree(r->left)) || (is_empty_tree(r->right)) )
        {
            tmp = (is_not_empty_tree(r->left) ? r->left : r->right);

            // No children
            if(is_empty_tree(tmp))
            {
                tmp = r;
                r = NULL;
            }
			
			// One child
            else {
				*r = *tmp;
			}
            free(tmp);
        }
		
		// node has 2 children
        else
        {
            // Copy the node with the smallest val from right sub-tree and delete it
            tmp = min_h(r->right);
            r->val = tmp->val;
            r->right = avl_validation(remove_r(r->right, tmp->val, success), tmp->val, 0);
        }
    }	
	return avl_validation(r,x,0);
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
    return min_h(t->root)->val;
}

int bst_max(BST_PTR t){
    return max_h(t->root)->val;
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
   avl_update_node(root);
   return root;
}

BST_PTR bst_from_sorted_arr(int *a, int n){

  BST_PTR t = bst_create();

  t->root = from_arr(a, n);

  return t;
}

//
static void inorder_val(NODE *r, int *a, int *pos){
  if(r==NULL) return;
  inorder_val(r->left, a, pos);
  a[*pos] = r->val;
  (*pos)++;
  inorder_val(r->right, a, pos);
}

int* bst_to_array(BST_PTR t) {
	int *result = malloc(sizeof(int) * t->root->size);
	int pos = 0;
	inorder_val(t->root, result, &pos);
	return result;
}