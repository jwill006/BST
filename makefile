a.out : test.c bst.c bst.h
	gcc -g -o a.out test.c bst.c
clear :
	rm *.o *.out
