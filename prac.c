#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main(){
	printf("MAIN PID %d\n", getpid());
	printf("CHILD PID: ");
	fflush(stdout);
	int status = 0;
	int values  =0;
	int i;
	while(i<50){
	i++;	
	int pid = fork();
	if(pid==0){
		int fuck = getpid();
		printf("%d, ",fuck);
		return 1;
	}
	else{
		wait(&status);
		values += WEXITSTATUS(status);
	}
	}
	printf("\n");

	
	printf("this is values : %d \n",values);
	return 1;


}
