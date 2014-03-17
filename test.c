#include <assert.h>
#include "bst.h"

#define SAMPLE_SIZE 1024

int *gen_random_arr(int size) {
	int *result = malloc(sizeof(int)*size);
	int i;

	for (i=0; i<size; i++)
		result[i] = i;
	for (i=size-1; i>0; i--) {
		int x = rand()%i;
		int y = result[i];
		result[i] = result[x];
		result[x] = y;
	}
	
	return result;
}

void t_height() {
	int i;
    int a[] = {8, 2, 6, 9, 11, 3, 7};
	
    BST_PTR t = bst_create();
	BST_PTR t2;
	
    for(i=0; i<7; i++)
        bst_insert(t, a[i]);
	
	
	
	bst_remove(t,11);
	bst_remove(t,2);
	bst_preorder(t);
	printf("Min elem: %d\n", bst_min(t));
	printf("Max elem: %d\n", bst_max(t));
	
	printf("Nearest elem: %d\n", bst_get_nearest(t,500));
	printf("Num LEQ: %d\n", bst_num_leq(t,10));
	
	bst_free(t);
	
}


void t_avl() {
	int i;
	BST_PTR t = bst_create();
	int *a = gen_random_arr(SAMPLE_SIZE);
	
    for(i=0; i<SAMPLE_SIZE; i++)
        bst_insert(t, i);	    	
	
	printf("Height of root: %d\n", bst_height(t));
	printf("Size of tree: %d\n", bst_size(t));
	printf("Min elem: %d\n", bst_min(t));
	printf("Max elem: %d\n", bst_max(t));
	printf("Nearest elem: %d\n", bst_get_nearest(t,500));
	printf("Num LEQ: %d\n", bst_num_leq(t,10));
	
	for(i=0; i<SAMPLE_SIZE-1; i++) {
		bst_remove(t,a[i]);
		printf ("Delete %d\n", a[i]);
	}
	
	assert(bst_to_array(t)[0]==a[SAMPLE_SIZE-1]);
	
	printf("Height of root: %d\n", bst_height(t));
	printf("Size of tree: %d\n", bst_size(t));
	printf("Min elem: %d\n", bst_min(t));
	printf("Max elem: %d\n", bst_max(t));
	printf("Nearest elem: %d\n", bst_get_nearest(t,500));
	printf("Num LEQ: %d\n", bst_num_leq(t,10));
	
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
	
	// t_height();
    

    /* PART 2 */
    
    // bst_inorder(t);
    // 
    // bst_preorder(t);
    // 
    // bst_postorder(t);

    bst_free(t);
    
    
    int sorted_a[] = {2, 3, 6, 7, 8, 9, 11};
    
    t = bst_from_sorted_arr(sorted_a, 7);
	int *b = bst_to_array(t);
	
	printf("Height of root: %d\n", bst_height(t));
	printf("Size of tree: %d\n", bst_size(t));
	
	printf("ith smallest element: %d\n", bst_get_ith(t,10));
	printf("Num LEQ: %d\n", bst_num_leq(t,3));
    
    
		
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
