#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct bst_node {
    int val;
	int size;
	int height;
	int update_size;
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

// Returns 1 if empty tree
static int is_empty_tree(NODE *r) {
	return r==NULL;
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
}

// Returns 1 if node is a leaf
static int is_leaf(NODE *r) {
	assert (!is_empty_tree(r));
	return ((r->left==NULL && r->right==NULL) ? 1 : 0);
}

// Return height of node
static int height(NODE *r) {
	if (is_empty_tree(r)) return -1;
	else return r->height;
}

static int size(NODE *r) {
	if (is_empty_tree(r)) return 0;
	else return r->size;
}

/******** AVL Helpers ********/


// Recalculates the height & size for node r
static void avl_update_node(NODE *r) {
	if (is_empty_tree(r)) return;
	r->size = 1 + size(r->left)  + size(r->right);
	r->height = 1 + ( (height(r->left) > height(r->right)) ? height(r->left) : height(r->right) );
}

// Handles promoting a leaf node to an inner node
static void avl_promote_leaf_to_parent(NODE *parent, NODE *leaf) {
	if (is_empty_tree(parent) || is_empty_tree(leaf)) return;
	leaf->parent = parent;
}

// Returns 1 if tree is valid AVL.
static int avl_validation(NODE *r) {
	int diff = height(r->left) - height(r->right);
	if (diff>=-1 && diff<=1) return 1;
	return 0;
}

// Cycle AVL tree left
static NODE *avl_cycle_left2(NODE *r) {
	struct bst tmp;
	tmp.root = r;
	printf ("\npre-cycle left\n");
	bst_preorder(&tmp);
	
	NODE *a = r->left;
	NODE *b = r;
	NODE *c = a->left;
	NODE *d = a->right;
	NODE *p = r->parent;
	
	// Left-Left case
	if (height(a->left) > height(a->right)) {
		if (is_not_root(b)) {
			if (is_left_child(b)) p->left = a;
			else p->right = a;
		}
	
		a->parent = p;
		b->left = d;
		a->right = b;
		b->parent = a;
		b->right = d;
		if (!is_empty_tree(d)) d->parent = b;
	}
	
	// Left-Right case
	else {
		if (is_not_root(b)) {
			if (is_left_child(b)) p->left = d;
			else p->right = d;
		}
	
		d->parent = p;
		b->left = d->right;
		d->right = b;
		a->parent = d;
		d->left = a;
		
		a->right = NULL;
		b->parent = d;
	}
	
	
	
	avl_update_node(b);
	avl_update_node(a);
	printf ("\npost-cycle left\n");
	bst_preorder(&tmp);
	return a;
}

static NODE *avl_cycle_left(NODE *r) {
	NODE *a = r->left;
	NODE *b = r;
	NODE *c = a->left;
	NODE *d = a->right;
	NODE *p = r->parent;
	
	// Left-Left case
	if (height(a->left) > height(a->right)) {
		if (is_not_root(b)) {
			if (is_left_child(b)) p->left = a;
			else p->right = a;
		}
		
		a->parent = p;
		a->left = c;
		a->right = b;
		
		b->left = NULL;
		b->parent = a;
		c->parent = a;
		avl_update_node(b);
		avl_update_node(c);
		avl_update_node(a);
		avl_update_node(p);
		return a;
	}
	
	// Left-Right case
	else {
		if (is_not_root(b)) {
			if (is_left_child(b)) p->left = d;
			else p->right = d;
		}
		
		d->parent = p;
		d->left = a;
		d->right = b;
		
		a->parent = d;
		b->parent = d;
		
		a->right = NULL;
		b->right = NULL;
		b->left = NULL;
		avl_update_node(a);
		avl_update_node(b);
		avl_update_node(d);
		avl_update_node(p);
		return d;
	}
}

// Cycle AVL tree right
static NODE *avl_cycle_right(NODE *r) {
	NODE *a = r->right;
	NODE *b = r;
	NODE *c = a->left;
	NODE *d = a->right;
	NODE *p = r->parent;
	
	// Right-Right case
	if (height(a->right) > height(a->left)) {
		if (is_not_root(b)) {
			if (is_right_child(b)) p->right = a;
			else p->left = a;
		}
	
		a->parent = p;
		a->left = b;
		a->right = d;
		
		b->parent = a;
		d->parent = a;
		b->right = NULL;
		avl_update_node(b);
		avl_update_node(d);
		avl_update_node(a);
		avl_update_node(p);
		return a;
	}
	
	// Right-Left case
	else {
		if (is_not_root(b)) {
			if (is_right_child(b)) p->right = c;
			else p->left = c;
		}
		
		c->parent = p;
		a->left = c->right;
		c->left = b;
		c->right = a;
		
		a->parent = c;
		b->parent = c;
		b->right = NULL;
		avl_update_node(b);
		avl_update_node(a);
		avl_update_node(c);
		avl_update_node(p);
	}
	return c;
}

static NODE *avl_cycle(NODE *r) {
	NODE *rt;
	struct bst tmp;
	tmp.root = r;
	if (height(r->left) > height(r->right)) {
		printf ("\npre-cycle left\n");
		bst_preorder(&tmp);
		tmp.root = avl_cycle_left(r);
		printf ("\npost-cycle left\n");
		bst_preorder(&tmp);
	}
	else {
		printf ("\npre-cycle right\n");
		bst_preorder(&tmp);
		tmp.root = avl_cycle_right(r);
		printf ("\npost-cycle right\n");
		bst_preorder(&tmp);
	}
	
	return tmp.root;
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
	  leaf->size = 1;
	  leaf->update_size = 0;
	  leaf->update_height = 0;
      leaf->val = x;
      return leaf;
    }

    if(r->val == x)
        return r;
    if(x < r->val){
		if (is_leaf(r)) {
			r->left = insert(r->left, x);
			avl_promote_leaf_to_parent(r, r->left);
			avl_update_node(r);
			return r;
		}
		
		else {
			r->left = insert(r->left, x);
			avl_update_node(r);
			if (avl_validation(r)) return r;
			return avl_cycle(r);
		}
    }
    else {
		if (is_leaf(r)) {
			r->right = insert(r->right, x);
			avl_promote_leaf_to_parent(r, r->right);
			avl_update_node(r);
			return r;
		}
		
		else {
			r->right = insert(r->right, x);
			avl_update_node(r);
			// return r;
			if (avl_validation(r)) return r;
			return avl_cycle(r);
		}
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
   return root;
}

BST_PTR bst_from_sorted_arr(int *a, int n){

  BST_PTR t = bst_create();

  t->root = from_arr(a, n);

  return t;
}
