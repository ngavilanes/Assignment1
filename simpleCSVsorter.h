/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/
#ifndef SIMPLECSVSORTER_H
#define SIMPLECSVSORTER_H
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct Node{
//	struct Node * next= NULL;
	char * str;
	char * key;
	int index;
	struct Node * next;
}Node;

#endif

//Suggestion: define a struct that mirrors a record (row) of the data set


//Suggestion: prototype a mergesort function

