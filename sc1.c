//Gav's stuff
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleCSVsorter.h"
#include "mergesort.c"


//B's stuff
#include <signal.h>
#include <unistd.h>
#include <dirent.h>


Node *head = NULL;
Node * nullhead = NULL;
char * column;
Node * append(Node * head, Node * nullhead){
	Node* ptr = nullhead;
	while(nullhead->next != NULL){
		nullhead = nullhead->next;
	}
	nullhead->next = head;
	return ptr;
}



char * trim(char * str){
//	printf("this is str %s\n",str);
	int i;
	
	while(isspace(*str)){
		str++;
	}	
	for(i = strlen(str)-1; isspace(str[i]); i--){
	}
	str[i+1] = '\0';
	return str;	
}

int findCategoryColumnNumber(char* categories, char* column, int* matchPtr){
		
		char* token;	
		int col = 0;
		int match = 0;
		while((token = strsep(&categories,","))!=NULL){
		
		if(token[strlen(token)-1]=='\n'){
		
			token[strlen(token)-1] = '\0';
			token = trim(token);
		}		

		if(strcmp(token,column)==0){

			*matchPtr = 1;
			break;
			}
		else{
			col++;
		}
	
	}//
	return col;
}

void makeLinkedList(int col, FILE *unsortedFilePtr){
	//Using getline on UNSORTED FILE
	//COL IS NUMBER OF COLUMNS NEEDED TO SKIP UNTIL REACH RELEVANT DATA
	
	char * str= NULL; //buffer to hold records
	size_t size=0;
	int i;                                                      //Nick: I is the index for the string
	int currcol = 0;
	char buff2[512];                                            //Holds the relevant data
	int buffcount = 0;                                          //Index for buff2
	Node * insert = (Node *) malloc(sizeof(Node));
	insert->next = NULL;
	Node* nullinsert = (Node* ) malloc(sizeof(Node));
	nullinsert->next = NULL;
	printf("______________________________________________78\n");
	nullhead = nullinsert;
	head = insert;
	Node * ptr = head;
	Node * nullptr = nullhead;


//-----------------------------BEGIN READING UNSORTED FILE----------------------------------------------

	while(getline(&str,&size,unsortedFilePtr)>0){                          //Guuch
	//	printf("this is str %s\n",str);
	//	insert->str = strdup(str);
		currcol=0;                                               //Guuch
		i =0;                  
     
//--------------------------LOOP TO SKIP COLUMNS UNTIL RELEVANT DATA IS FOUND-----------------------------
		while(currcol != col){                                    //Guch
		//	printf("this is char %c\n",str[i]);
			if(str[i] != ',' && str[i]!='"'){
				
				i++;
			}
			else if(str[i] =='"'){ //quote- must reach end of quote
				i++;
				while(str[i]!='"'){
					i++;
				}
				i++;
			
			}
			else{ //comma
				currcol++;
				i++;	
				
			}
	
			
		}


//-----------------------------DONE SKIPPING GARBAGE NOW GETTING RELEVANT DATA--------------------------------
		
		while(str[i]!=','){
			if(str[i] =='\n'){
				break;
			}
			if(str[i] =='"'){ //WE HAVE FOUND A QUOTE IN RELEVANT DATA
				i++;
				while(str[i]!='"'){
					buff2[buffcount++] = str[i];
					i++;
				}
				i++;
			}
			else{ //WE HAVE JUST FOUND A CHARACTER THATS NOT \n or "
				buff2[buffcount++] = str[i];
				i++;		
			}
		
	
		}
		
		buff2[buffcount+1] ='\0';	
	
//------------------------------WE GOT OUR RELEVANT DATA NOW TO INSERT IT INTO A NODE----------------------------
//
		//DATA IS NOT EMPTY
		if(strlen(buff2)!=0){
			insert->str = strdup(str);
			insert->key = strdup(buff2);
			memset(&buff2,'\0',sizeof(buff2));
			buffcount = 0;
			Node * Next = (Node*) malloc(sizeof(Node));
			Next->next = NULL;
			ptr = insert;
			insert->next = Next;
			insert = insert->next;
		}

		//EMPTY VALUE ,,
		else{
			nullinsert->str =strdup(str);
			nullinsert->key = strdup(buff2);
			memset(&buff2,'\0',sizeof(buff2));
			buffcount = 0;
			Node * Next = (Node*) malloc(sizeof(Node));
			Next->next = NULL;
			nullptr = nullinsert;
			nullinsert->next = Next;
			nullinsert = nullinsert->next;
		}


	}


//-----------------------------LINKED LIST IS READY->TRIM VALUES->MERGESORT--------------------------------------

	nullptr->next = NULL;
	free(nullinsert);
	ptr->next = NULL;
	free(insert);
	ptr = head;
	while(ptr!=NULL){
		ptr->key = trim(ptr->key);
	 
		ptr = ptr->next;
	}

}
void startSorter(DIR* dr, char * path, char * outputPath){
	//This method is used to "jump start" the traversal of the current directory	
	//When it finds a directory it will call recursiveRunner
	struct dirent *de;
	//dr = opendir(path);

	while( (de = readdir(dr)) != NULL){
		char* currentName = de->d_name; //getting name of directory/file
		char *dot = strrchr(currentName, '.');
		printf("this is dot %s\n", dot);
		printf("Checking   %s\n", currentName);
		if(dot && !strcmp(dot,".csv")){
			printf("Found a csv file %s \n", currentName);
			char * sortedAlready = strstr(currentName,"sorted");
			if(sortedAlready!=NULL){
			continue;
			}
			char * ActualName = strdup(currentName); //copy the current name
			*dot = '\0'; //cuts off current name at the dot to set up sorted final name



	
			int pid = fork();
			if(pid==0){	
			char* sortedFileName;
			if((sortedFileName = malloc ( strlen(currentName ) + strlen("sorted-") +1))   != NULL){
				sortedFileName[0] = '\0';
				strcat(sortedFileName,outputPath);
				strcat(sortedFileName,"/");
				strcat(sortedFileName, currentName);
				strcat(sortedFileName, "-sorted-");
				strcat(sortedFileName, column);	
				strcat(sortedFileName, ".csv");
					
				printf("_________Current Name %s \n", currentName);
				printf("__________sorted file name %s \n", sortedFileName);

				//inside of this if is where the sorting method should be called
					//

					char  *filePAName = (char*)malloc(2 + strlen(path) + strlen(currentName));
            		strcpy(filePAName, path);
            		strcat(filePAName, "/");
            		strcat(filePAName, ActualName);
					//
			printf("this is filePAN name %s \n",filePAName);
								//Test Code
				FILE *unsortedFilePtr;
				unsortedFilePtr = fopen(filePAName,"r");
				if(unsortedFilePtr == NULL){
					printf("Could Not open %s for %s \n", currentName,sortedFileName);
				}
				//first category
				char * categories=NULL; //buffer to hold the first row which are the column names
				char * categoriesdup = NULL;   //Idk wtf this is for ask Nick
				int col=0;                      //col where key are held
				int match = -1;                      // might not need
				size_t size = 0;
				printf("_________________217 SFN %s Categoriess: %s CurName: %s \n", sortedFileName, categories, currentName);
				getline(&categories,&size,unsortedFilePtr); //Gets the first line in csv file to be sorted
				printf("_________________219 SFN %s Cats: %s CurName: %s \n", sortedFileName, categories, currentName);
				categoriesdup= strdup(categories);
				col = findCategoryColumnNumber(categories, column, &match);   //Finds the index of the category
				
				printf("First Row %s  Column Search %s Column Index %d\n", categoriesdup,column, col );
				
				if(match==-1){
					printf("NOOOOOOOOOOOOOOOOOOOOOOOO no match %s \n", filePAName);
					continue;
				}
				else{
					printf("CDUP: %s",categoriesdup);
				}

				makeLinkedList(col, unsortedFilePtr);

				int isNum = isNumber(head->key);
				
				
				//CALLING MERGESORT 
				head = divide(head, isNum);	

				if(nullhead->str!=NULL){
					head = append(head,nullhead);
				}
				FILE *sortedFilePtr;
				sortedFilePtr = fopen(sortedFileName,"w");
				

				OutputData(head, sortedFilePtr, sortedFileName, categoriesdup); //writing data to sorted file
//				free(head);
				//End of test

				//At this point we should pass the filePtr to the sorting algorithm as well as the csv file
				//fprintf(sortedFilePtr,"TestCSV");
				fclose(sortedFilePtr);
				}
		
			
				
				//kill(getpid(), SIGKILL);
			}
			else{
			//pid not equal to 0 -- parent - add shit to array and keep going
				continue;

			}
	


	
		}else{
			//not a csv -- might be directory

			printf("XXX %s\n", currentName);
			if(strcmp(currentName,".") == 0 || strcmp(currentName,"..") == 0){
				printf("Skipping this %s \n",currentName);
			}else{

					 char buf[PATH_MAX + 1]; /* not sure about the "+ 1" */
				    char *res = realpath(currentName, buf);
				    if (res) {
				        printf("This source is at %s.\n", buf);
				    } else {

				        perror("realpath");
				        exit(EXIT_FAILURE);
				    }
		char  *filePathAndName = (char*)malloc(1 + strlen(buf));
            	strcpy(filePathAndName, buf);
            	//strcat(filePathAndName, "/");
            	//strcat(filePathAndName, currentName);
            	printf("File Path %s\n", filePathAndName);
				DIR* subDirectoryPtr = opendir(filePathAndName);
				if(subDirectoryPtr != NULL){       


				int pid = fork();

				if(pid == 0){
				
					startSorter(subDirectoryPtr, filePathAndName,outputPath);
					printf("Done sorting file %s  PID %d is getting killed \n", currentName , getpid());
					kill(getpid(), SIGKILL); //exit instead  
					
				}
			
				
				
				}
			//	closedir(subDirectoryPtr);
			}
		}
	}
	//closedir(dr);
}
int main (int argc, char * argv[]){
	char startDir[] = ".";
	char outputDir[] = ".";
	int cflag =-1; //find cflag 
	//char buff[1024];
	if(argc < 3){
		printf("There is an input missing, try again\n");
		return -1;	
	}

	int i;
	for(i =0; i<argc; i++){
		printf("this is argv %s\n", argv[i]);
		if(strcmp(argv[i],"-c")==0){
			if(i == argc){
				//no more args ==ERROR
			}
		//	printf("HELLLOOO\n");
			column = argv[i + 1];

		}
		else if(strcmp(argv[i],"-d")==0){
			strcat(startDir,"/");
			strcat(startDir, argv[i+1]);
			DIR * dir= opendir(startDir);
			if(dir){
				closedir(dir);
			}
			else{
				//directory doesn't exist
				perror("Error Start Directory does not exist ");
				return -1;
			}
			
			

		}
		else if(strcmp(argv[i],"-o")==0){
			strcat(outputDir,"/");
			strcat(outputDir,argv[i+1]);
			DIR * dir= opendir(outputDir);
			if(dir){
				closedir(dir);
			}
			else{
				//directory doesn't exist
				perror("Error output directory does not exist ");
				return -1;
			}
			

		}
	}

	printf("this is column %s\n", column);
	printf("this is startdir %s\n", startDir);
	printf("this is outputdir %s\n", outputDir);
	size_t size = 0;	

	//column = argv[2]; //user inputted value that will be sorted on - global var
	struct dirent *de;
	
	DIR* dr;// = opendir(".");
	//dr = opendir(".");
	//startSorter(dr, ".");	
	dr = opendir(startDir);
	startSorter(dr,startDir,outputDir);
		
	return 0;
	
	
}


