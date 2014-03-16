#include <assert.h>
#include "bst.h"

void t_num_elem() {
	int i;
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	BST_PTR t2;
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);
	
	bst_free(t);
}

void t_height() {
	int i;
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	BST_PTR t2;
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);
	
	bst_free(t);
	
}

int *gen_random_arr(int size) {
	int *elements = malloc(sizeof(int)*size);
	int i;

	// inizialize
	for (i = 0; i < size; ++i)
	  elements[i] = i;

	for (i = size - 1; i > 0; --i) {
	  // generate random index
	  int w = rand()%i;
	  // swap items
	  int t = elements[i];
	  elements[i] = elements[w];
	  elements[w] = t;
	}
	
	return elements;
}


void t_avl() {
	int i;
	int size = 1024;
	BST_PTR t = bst_create();
	int *a = gen_random_arr(size);
	
    for(i=0; i<size; i++)
        bst_insert(t, i);
	
	bst_inorder(t);
	    	
	
	
	printf("Height of root: %d\n", bst_height(t));
	printf("Size of tree: %d\n", bst_size(t));
	
	for(i=0; i<1023; i++) {
		bst_remove(t,a[i]);
		printf ("Delete %d\n", a[i]);
	}
	
	bst_inorder(t);
	
	printf("Height of root: %d\n", bst_height(t));
	printf("Size of tree: %d\n", bst_size(t));
	free(a);
	bst_free(t);
}


int t_bst_insert() {
	int i;
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);

    assert(bst_size(t) == 7);
	bst_free(t);
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
	
	printf("Height of root: %d\n", bst_height(t));
	printf("Size of tree: %d\n", bst_size(t));
    
    int *b = bst_to_array(t);
		
	printf("A: ");
	for (i=0; i<7; i++) {
		printf("%d ", sorted_a[i]);
	}
	printf("\n");
	
	printf("B: ");
	for (i=0; i<7; i++) {
		printf("%d ", b[i]);
	}
	printf("\n");
	free(b);

    bst_free(t);
}
