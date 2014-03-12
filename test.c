#include <assert.h>
#include "bst.h"

void t_num_elem() {
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	BST_PTR t2;
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);

    t2 = t;
	
	while (t2 != NULL) {
		assert (t2->numElem = );
	}
}

void t_height() {
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	BST_PTR t2;
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);

    t2 = t;
	
	while (t2 != NULL) {
		assert (t2->numElem = );
	}
}

void t_avl() {
	int a[] = {8, 2, 6, 9, 11, 3, 7};
	
	BST_PTR t = bst_create();
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);
}


int t_bst_insert() {
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

    

    /* PART 2 */
    
    bst_inorder(t);

    bst_preorder(t);

    bst_postorder(t);

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
