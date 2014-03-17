#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <assert.h>


struct bst_node {
    int val;
	int size;
	int height;
	int min;
	int max;
	int root;
    struct bst_node *left;
    struct bst_node *right;

};
typedef struct bst_node NODE;


struct bst {
    NODE *root;
};

/********************************************************/
/********************** BST Helpers *********************/

// Returns 1 if empty tree
static int is_empty_tree(BST_PTR t) {
	return t==NULL;
}

// Returns 1 if tree is not empty
static int is_not_empty_tree(BST_PTR t) {
	return is_empty_tree(t)==0;
}

// Returns 1 if empty subtree
static int is_empty_subtree(NODE *r) {
	return r==NULL;
}

// Returns 1 if subtree is not empty
static int is_not_empty_subtree(NODE *r) {
	return is_empty_subtree(r)==0;
}

// Returns 1 if node is a leaf
static int is_leaf(NODE *r) {
	assert (is_not_empty_subtree(r));
	return ((r->left==NULL && r->right==NULL) ? 1 : 0);
}

// Return height of subtree with root r 
//  NOTE: the empty subtree has height -1
static int height(NODE *r) {
	if (is_empty_subtree(r)) return -1;
	else return r->height;
}

// Returns number of nodes in subtree with root r
static int size(NODE *r) {
	if (is_empty_subtree(r)) return 0;
	else return r->size;
}


/*************************************************************/
/************************ AVL Helpers ************************/

// Recalculates the height, size, min & max of subtree with root r
static void avl_update_node(NODE *r) {
	if (is_empty_subtree(r)) return;
	r->size = 1 + size(r->left)  + size(r->right);
	r->height = 1 + ( (height(r->left) > height(r->right)) ? height(r->left) : height(r->right) );
	r->min = is_not_empty_subtree(r->left) ? r->left->min : r->min;
	r->max = is_not_empty_subtree(r->right) ? r->right->max : r->max;
}

// Returns the balance factor of subtree with root r
static int avl_balance_factor(NODE *r) {
	if (is_empty_subtree(r)) return 0;
	return height(r->left) - height(r->right);
}

// Cycle AVL tree to the left
static NODE* avl_cycle_tree_left(NODE *r) {
	NODE *a = r;
	NODE *b = r->right;
	NODE *c = (is_not_empty_subtree(b) ? b->left : NULL);
	
	a->right = c;
	if (is_not_empty_subtree(b)) b->left = a;
	avl_update_node(c);
	avl_update_node(a);
	avl_update_node(b);
	return b;
}

// Cycle AVL tree to the right
static NODE* avl_cycle_tree_right(NODE *r) {
	NODE *a = r;
	NODE *b = r->left;
	NODE *d = (is_not_empty_subtree(b) ? b->right : NULL);
	
	a->left = d;
	if (is_not_empty_subtree(b)) b->right = a;
	avl_update_node(d);
	avl_update_node(a);
	avl_update_node(b);
	return b;
}

// Cycle switch for insertion
static NODE* avl_insert_cycle(NODE *r, int x) {
	int balance = avl_balance_factor(r);

	// Left Left Case
	if (balance > 1 && x < r->left->val) return avl_cycle_tree_right(r);

	// Left Right Case
	if (balance > 1 && x > r->left->val) {
		r->left = avl_cycle_tree_left(r->left);
		return avl_cycle_tree_right(r);
	}

	// Right Right Case
	if (balance < -1 && x > r->right->val) return avl_cycle_tree_left(r);

	// Right Left Case
	if (balance < -1 && x < r->right->val) {
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
	if (balance > 1 && l_balance >= 0) return avl_cycle_tree_right(r);

	// Left Right Case
	if (balance > 1 && l_balance < 0) {
		r->left = avl_cycle_tree_left(r->left);
		avl_update_node(r);
		return avl_cycle_tree_right(r);
	}

	// Right Right Case
	if (balance < -1 && r_balance <= 0) return avl_cycle_tree_left(r);

	// Right Left Case
	if (balance < -1 && r_balance > 0) {
		r->right = avl_cycle_tree_right(r->right);
		return avl_cycle_tree_left(r);
	}
}

// Returns 1 if subtree with root r is NOT AVL
static int avl_validation_flag(NODE *r) {
	int balance = avl_balance_factor(r);	
	if (balance>1 || balance<-1) return 1;
	return 0;
}

// Validates if subtree with root r is AVL
//  Returns the subtree if AVL, 
//  else cycles and returns resulting subtree
static NODE * avl_validation(NODE *r, int x, int insert) {
	avl_update_node(r);
	if (avl_validation_flag(r)) {
		if (insert) return avl_insert_cycle(r,x);
		return avl_delete_cycle(r);
	}
	return r;
}


/*************************************************************/
/************************ BST Helpers ************************/

static void free_r(NODE *r){
	if(r==NULL) return;
	free_r(r->left);
	free_r(r->right);
	r->min = 0;
	r->max = 0;
	r->val = 0;
	r->root = 0;
	r->size = 0;
	r->height = 0;
	r->left = NULL;
	r->right = NULL;
	free(r);
}

// Recursive Helper for bst_insert
static NODE * insert(NODE *r, int x){
    NODE *leaf;
	
    if(r == NULL){
      leaf = malloc(sizeof(NODE));
	  leaf->root = 1;
      leaf->left = NULL;
      leaf->right = NULL;
	  leaf->height = 0;
	  leaf->max = x;
	  leaf->min = x;
	  leaf->size = 1;
      leaf->val = x;
      return leaf;
    }

    if(r->val == x)
        return r;
    if(x < r->val){
		r->left = avl_validation(insert(r->left,x),x,1);
		if(is_not_empty_subtree(r->left) && r->left->root) r->left->root = 0;
    }
	
    else {
		r->right = avl_validation(insert(r->right,x),x,1);
		if(is_not_empty_subtree(r->right) && r->right->root) r->right->root = 0;
    }
	return avl_validation(r,x,1);
}

// how about an iterative version?
static NODE *insert_i(NODE *r, int x){

  return NULL;

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

// Recursive Helper for bst_remove
static NODE *remove_r(NODE *r, int x, int *success){
	NODE *tmp;
	
	if (is_empty_subtree(r)) return r;

	// Target node is in left subtree
	if ( x < r->val ) r->left = avl_validation(remove_r(r->left, x, success),x,0);

	// Target node is in right subtree
	else if( x > r->val ) r->right = avl_validation(remove_r(r->right, x, success),x,0);

	// Found node with val x
	else {
		
		// node has 0 or 1 child
		if( (is_empty_subtree(r->left)) || (is_empty_subtree(r->right)) ) {
			tmp = (is_not_empty_subtree(r->left) ? r->left : r->right);
			
			// No children
			if(is_empty_subtree(tmp)) {
				tmp = r;
				r = NULL;
			}
			
			// One child
			else *r = *tmp;
	    	free(tmp);
		}
	
		// node has 2 children
		else {
			// Copy the node with the smallest val from right subtree and delete it
			tmp = min_h(r->right);
			r->val = tmp->val;
			r->right = avl_validation(remove_r(r->right, tmp->val, success),tmp->val,0);
		}
	}
	return avl_validation(r,x,0);
}

/*************************************************************/
/************************ BST functions ************************/

// Returns a new bst
BST_PTR bst_create(){
	BST_PTR t = malloc(sizeof(struct bst));
	t->root = NULL;
	return t;
}

int bst_contains(BST_PTR t, int x){
    NODE *p = t->root;

    while(p != NULL){
        if(p->val == x) return 1;
        if(x < p->val) p = p->left;
        else p = p->right;
    }
	
    return 0;  
}

void bst_free(BST_PTR t){
    free_r(t->root);
	t->root = NULL;
    free(t);
}

// Inserts x into tree t
void bst_insert(BST_PTR t, int x){
    t->root = insert(t->root, x);
}

// Removes x from tree t and returns whether deletion was successful
int bst_remove(BST_PTR t, int x){
	if (is_empty_tree(t)) return 0;
	
	int success;
    t->root = remove_r(t->root, x, &success);
	
	if (is_not_empty_subtree(t->root)) {
		if (t->root->min==x) t->root->min = is_not_empty_subtree(min_h(t->root)) ? min_h(t->root)->val : t->root->val;
		if (t->root->max==x) t->root->max = is_not_empty_subtree(max_h(t->root)) ? max_h(t->root)->val : t->root->val;
	}
	
    return success;
}

// Returns the size of tree t
int bst_size(BST_PTR t){
	if (is_empty_tree(t)) return 0;
    return size(t->root);
}

// Returns height of tree t or -1 if tree is empty
int bst_height(BST_PTR t){
	if (is_empty_tree(t) || is_empty_subtree(t->root)) return -1;
    return height(t->root);
}

// Returns minimum elem of tree t or -1 if tree is empty
int bst_min(BST_PTR t){
	if (is_empty_tree(t) || is_empty_subtree(t->root)) return -1;
    return t->root->min;
}

// Returns maximum elem of tree t or -1 if tree is empty
int bst_max(BST_PTR t){
	if (is_empty_tree(t) || is_empty_subtree(t->root)) return -1;
    return t->root->max;
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
 * bst_from_sorted_arr(...). The function must return a subtree that is
 * perfectly balanced, given a sorted array of elements a.
 */
static NODE * from_arr(int *a, int n){
	int m;
	NODE *root;

	if(n <= 0) return NULL;
	m = n/2;
	root = malloc(sizeof(NODE));
	root->root = 1;
	root->min = a[m];
	root->max = a[m];
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

/**********************************************************************/
/************************ Additional Functions ************************/

// Recursive helper for bst_num_leq
static int bst_num_leq_helper(NODE *r, int x) {
	if (is_empty_subtree(r)) return 0;
	if (r->val < x) return 1 + size(r->left) + bst_num_leq_helper(r->right, x);
	if (r->val == x) return 1 + size(r->left);
	return bst_num_leq_helper(r->left, x);
}

// Returns the number of elem less than or equal to x
int bst_num_leq(BST_PTR t,int x) {
	if (is_empty_tree(t) || is_empty_subtree(t->root)) return 0;	
	return bst_num_leq_helper(t->root,x);
}

// Recursive helper for bst_num_geq
static int bst_num_geq_helper(NODE *r, int x) {
	if (is_empty_subtree(r)) return 0;
	if (r->val > x) return 1 + size(r->right) + bst_num_geq_helper(r->left, x);
	if (r->val == x) return 1 + size(r->right);
	return bst_num_geq_helper(r->right, x);
}

// Returns the number of elem greater or equal to x
int bst_num_geq(BST_PTR t,int x) {
	if (is_empty_tree(t) || is_empty_subtree(t->root)) return 0;	
	return bst_num_geq_helper(t->root, x);
}

// Recursive helper for bst_get_nearest
static void bst_get_nearest_helper(NODE *r, int x, int *closest) {
	if (is_empty_subtree(r)) return;
	NODE *b = r->left;
	NODE *c = r->right;
	
	// Compare previous closest elem with current elem
	if (abs(x - r->val) < abs(x - *closest)) *closest = r->val;
	
	// Closest elem MUST be current
	if (r->val==x) {
		*closest = x;
		return;
	}
	
	// Closest elem is current OR in left subtree
	else if (x<r->val) {
		
		// Current elem or left child
		if (is_not_empty_subtree(b)) {
			if (b->val > x) {
				if (abs(x - b->val) < abs(x - *closest)) *closest = b->val;
				return;
			}
			bst_get_nearest_helper(b, x, closest);
		}
		bst_get_nearest_helper(c, x, closest);
	}
	
	// Closest elem is current OR in right subtree
	else {
		
		// Current elem or right child
		if (is_not_empty_subtree(c)) {
			if (c->val > x) {
				if (abs(x - c->val) < abs(x - *closest)) *closest = c->val;
				return;
			}
			bst_get_nearest_helper(c, x, closest);
		}
		bst_get_nearest_helper(b, x, closest);
	}
}

// Returns the elem with closest proximity to x
int bst_get_nearest(BST_PTR t, int x) {
	if (is_empty_tree(t) || is_empty_subtree(t->root)) {
		fprintf(stderr, "Tree is empty!\n", size(t->root));
		return -1;
	}
	
	int closest = t->root->val;
	bst_get_nearest_helper(t->root, x, &closest);
	return closest;
}

// Recursive helper for bst_get_ith
void bst_get_ith_helper(NODE *r, int i, int *result) {
	if (is_empty_subtree(r)) return;
	
	if (size(r->left) + 1 == i) {
		*result = r->val;
		return;
	}
	else if (is_not_empty_subtree(r->left) && size(r->left) > i) {
		bst_get_ith_helper(r->left, i, result);
	}
	else if (is_not_empty_subtree(r->left) && size(r->left) < i) {
		bst_get_ith_helper(r->right, i-(size(r->left)+1), result);
	}
}

// Returns the ith smallest element in t or -1 if i > size(t)
int bst_get_ith(BST_PTR t, int i) {
	if (is_empty_tree(t) || is_empty_subtree(t->root) || i<1 || i>size(t->root)) {
		fprintf(stderr, "Tree is empty or i is out of bounds i: %d  tree size: %d\n", i, size(t->root));
		return -1;
	}
	
	int result = -1;
	bst_get_ith_helper(t->root, i, &result);
	return result;
}

// Recursively helper for bst_to_array
static void bst_to_array_helper(NODE *r, int *a, int *pos){
	if(is_empty_subtree(r)) return;
	bst_to_array_helper(r->left, a, pos);
	a[*pos] = r->val;
	(*pos)++;
	bst_to_array_helper(r->right, a, pos);
}

// Returns a sorted array of all elems in t
int* bst_to_array(BST_PTR t) {
	if (is_empty_tree(t) || is_empty_subtree(t->root)) return NULL;
	int *result = malloc(sizeof(int) * t->root->size);
	int pos = 0;
	bst_to_array_helper(t->root, result, &pos);
	return result;
}