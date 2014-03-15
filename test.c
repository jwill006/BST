#include <assert.h>
#include "bst.h"

void t_num_elem() {
	int i;
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	BST_PTR t2;
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);
}

void t_height() {
	int i;
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	BST_PTR t2;
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);
	
}


void t_avl() {
	int i;
	
	BST_PTR t = bst_create();
	
    // for(i=0; i<1024; i++)
    //     bst_insert(t, i);
    	
	    for(i=63; i>=32; i--)
	        bst_insert(t, i);
	
	    for(i=0; i<32; i++)
	        bst_insert(t, i);
	
	for(i=128; i<256; i++)
		bst_insert(t,i);
	
	for(i=127; i>63; i--)
		bst_insert(t,i);
	
	// t_avl_parent(t);
	
	bst_inorder(t);
	
	// for(i=bst_height(t); i>=0; i--, t->root=t->root->left)
	// 	assert(t->root->height==i);
	
	printf("Height of root: %d\n", bst_height(t));
	printf("Size of tree: %d\n", bst_size(t));
}


int t_bst_insert() {
	int i;
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);

    assert(bst_size(t) == 7);
}


int main(){
    int i;

    /* PART 1 */

    int a[] = {8, 2, 6, 9, 11, 3, 7};

    BST_PTR t = bst_create();

	t_avl();
    

    /* PART 2 */
    
    // bst_inorder(t);
    // 
    // bst_preorder(t);
    // 
    // bst_postorder(t);

    bst_free(t);
    
    /* PART 3 (extra) */
    
    int sorted_a[] = {2, 3, 6, 7, 8, 9, 11};
    
    t = bst_from_sorted_arr(sorted_a, 7);
    
    /*
    bst_inorder(t);

    bst_preorder(t);

    bst_postorder(t);

    bst_free(t);
    */
}
