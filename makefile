a.out : test.c bst.c bst.h
	gcc -o a.out test.c bst.c
clear :
	rm *.o *.out
