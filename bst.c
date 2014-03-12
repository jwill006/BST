#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct bst_node {
    int val;
	int count;
	int height;
	int update_count;
	int update_height;
	struct bst_node *parent;
    struct bst_node *left;
    struct bst_node *right;

};
typedef struct bst_node NODE;


struct bst {
    NODE *root;
};

/******** BST Helpers ********/

// Returns 1 if node is the root of a tree.
static int is_root(NODE *r) {
	if (r==NULL) return 1;
	return (r->parent==NULL ? 1 : 0);
}

// Returns 1 if empty tree
static int is_empty(NODE *r) {
	return r==NULL;
}

// Returns 1 if node is a leaf
static int is_leaf(NODE *r) {
	assert (r!=NULL);
	return ((r->left==NULL && r->right==NULL) ? 1 : 0);
}

// Return height of node
static int height(NODE *r) {
	if (r==NULL) return -1;
	else return r->height;
}

/******** AVL Helpers ********/

// Handles attching node onto a leaf
static void avl_update_leaf_node(NODE *r) {
	// int branch = 0;  #TODO refactor for dynamic branch selection
	r->count++;
	r->height++;
	r->update_height = 1;
	r->update_count = 1;
}

static void avl_update_inner_node(NODE *r) {
	if ((r->left != NULL) && (r->left->update_count)) {
		r->count++;
		r->update_count = 1;
		
		if (r->left->update_height) {
			r->height++;
			r->update_height = 1;
		}
		
		// Reset update flags
		r->left->update_count = 0;
		r->left->update_height = 0;
			
	}
	
	if ((r->right != NULL) && (r->right->update_count)) {
		r->count++;
		r->update_count = 1;
		r->right->update_count = 0;
		
		if (r->right->update_height) {
			r->height++;
			r->update_height = 1;
		}
		
		// Reset update flags
		r->right->update_count = 0;
		r->right->update_height = 0;
	}
}

// Returns 1 if tree is valid AVL.
static int avl_check(NODE *r) {
	int diff = height(t->left) - height(t->right);
	if (diff>=-1 && diff<=1) return 1;
	return 0;
}

static void assign_parent(NODE *parent, NODE *child) {
	if (parent==NULL || child==NULL) return;
	child->parent = parent;
	assert(parent->left==child || parent->right==child);
}

//######### TODO: AVL restructure


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
	int leaf_check;
	
    if(r == NULL){
      leaf = malloc(sizeof(NODE));
	  leaf->parent = NULL;
      leaf->left = NULL;
      leaf->right = NULL;
	  leaf->height = 0;
	  leaf->update_count = 0;
	  leaf->update_height = 0;
      leaf->val = x;
      return leaf;
    }

    if(r->val == x)
        return r;
    if(x < r->val){
		leaf_check = is_leaf(r);
        r->left = insert(r->left, x);
		
		// Something will be added to tree
		if (leaf_check) {
			r->height++;
			assign_parent(r, r->left);
			avl_update_leaf_node(r);
		}
		
		else {
			avl_update_inner_node(r);
			if (avl_check(r)) return r;
			else return (avl_cycle(r));
		}
		
		
		
		
		
		
		
		
		
		
    }
    else {
        r->right = insert(r->right, x);
		
		
		
		
		assign_parent(r, r->right);
		if (avl_check(r)) return r;
		else return (avl_cycle(r));
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
static int size(NODE *r){

    if(r==NULL) return 0;
    return size(r->left) + size(r->right) + 1;
}
int bst_size(BST_PTR t){

    return size(t->root);
}

// static int height(NODE *r){
//     int l_h, r_h;
// 
//     if(r==NULL) return -1;
//     l_h = height(r->left);
//     r_h = height(r->right);
//     return 1 + (l_h > r_h ? l_h : r_h);
// 
// }
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
   
   if (root->left != NULL) root->numElem++;
   if (root->right != NULL) root->numElem++;
   return root;
}

BST_PTR bst_from_sorted_arr(int *a, int n){

  BST_PTR t = bst_create();

  t->root = from_arr(a, n);

  return t;
}
