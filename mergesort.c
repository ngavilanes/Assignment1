#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simpleCSVsorter.h"

 Node * GetMiddleNode( Node *ptr){
	
	 Node *fast = ptr->next;
	
	while((fast != NULL) && (fast->next != NULL)){
		ptr = ptr->next;
		fast = fast->next->next;
	}

	return ptr;

}
 Node * Merge(struct Node *head, struct Node *headTwo){
//	printf("we in int\n");	
	if(!head){

		return headTwo;
	}
	if(!headTwo){
		return head;
	}
	long headKey ;
	if(strlen(head->key) == 0){
		headKey = 0;
		
	}else{
		headKey = atol(head->key);
	}

	
	long headTwoKey ;

	if(strlen(headTwo->key) == 0){
		headTwoKey = -1;
	}else{
		headTwoKey = atol(headTwo->key);
	}

	if(headKey < headTwoKey){
		head->next = Merge(head->next, headTwo);
		return head;
	}else{
		headTwo ->next = Merge(head, headTwo->next);
		return headTwo;
	}

}

 Node * MergeDouble(struct Node *head, struct Node *headTwo){
//	printf("we in double\n");
	if(!head){
		return headTwo;
	}
	if(!headTwo){
		return head;
	}
	double headKey;
	if(strlen(head->key) == 0){
		headKey = -1;
	}else{
		sscanf(head->key, "%lf", &headKey);
	}
	
	double headTwoKey;
	
	if(strlen(headTwo->key) == 0){
		headTwoKey = -1;
	}else{
		sscanf(headTwo->key, "%lf", &headTwoKey);
	}
	
	
	if(headKey < headTwoKey){
		head->next = MergeDouble(head->next, headTwo);
		return head;
	}else{
		headTwo->next = MergeDouble(head, headTwo->next);
		return headTwo;
	}


}
 Node * MergeStrings(struct Node *head, struct Node *headTwo){
//	printf("we in strings\n");
	if(!head){
		return headTwo;
	}
	if(!headTwo){
		return head;
	}
	int val = strcmp(head->key, headTwo->key);
	 
	if(val < 0){
		head->next = MergeStrings(head->next, headTwo);
		return head;
	}else{
		headTwo->next = MergeStrings(head, headTwo->next);
		return headTwo;
	
	}

}
void printList1( Node *head){
	while(head != NULL){
		printf("string %s key  [%s] -> ",head->str, head->key);
		head = head->next;
	}
	printf("\n");
}




 Node * divide(struct Node *head, int dataType){
	
	 if(head->next == NULL){ 
		return head;
	}
	 Node *middleNode = GetMiddleNode(head);
	
	
	 Node *headTwo = middleNode->next;
	middleNode->next = NULL;	
	head = divide(head, dataType);

	headTwo = divide(headTwo, dataType);	
	if(dataType == 2){	
		head =  Merge(head, headTwo);
	}else if(dataType == 1){
		head = MergeDouble(head, headTwo);
	}else if(dataType == 0){
		head = MergeStrings(head, headTwo);
	}
	

	return head;


}




int isNumber(char *a){ //THIS FUNCTION HAS BEEN TESTED WITH 8 TEST CASES AND IT WORKS
	//"123"  "12.3"   "1."   "aa"   "a.a"   ".a"    "..a"    "1.."  
	//Will return 0 (FALSE) if the string is NOT a number
	//Will return 1 (TRUE) if the string is a number
	
	int isNumber = 1; //BOOLEAN VALUE
	int isDouble = 0; //BOOLEAN VALUE

	int i = 0;
	int length = strlen(a);
	
	int numberOfPeriods = 0;
	
	for(;i < length ;i++){
		char c = a[i];
		//printf("%c \n",c);
		if(c == '.'){
			if(numberOfPeriods == 0){

				numberOfPeriods++;
				isDouble = 1;
			}else{
				//printf("Not a digit because of two periods");
				isNumber = 0;
				break;
			}
		}else if(isdigit(c) == 0){
			//printf("Not a digit %c at index %d \n",c ,i);
			isNumber = 0;
			break;
		}
	}

	//printf("%s \n", a);
	if(isNumber == 0){
	//	printf("Not a number\n");
		isNumber = 0; // 0 MEANS IT IS NOT A NUMBER
	}else{
		if(isDouble == 1){
	//		printf("String is a number and a double\n");
			isNumber = 1; //1 MEANS IT IS A double
		}else{
	//		printf("String is a number but not a double\n");
			isNumber = 2; //2 MEANS NUMBER IS A INT
		}
	}	
	return isNumber;	

}
void OutputData( Node *head,FILE * sortedFilePtr, char * sortedFileName, char *firstRow){
		printf("INSIDE OutputData\n");
		//sortedFilePtr = fopen(sortedFileName,"w");
		fprintf( sortedFilePtr,firstRow);
	while(head != NULL){
		fprintf( sortedFilePtr,"%s",head->str);
		//printf("%s\n", head->str );
		head = head->next;
	}
	//close(sortedFilePtr);

}

