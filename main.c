#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	//creates a pointer for 'status'
	//this tells loop to keep going until 'quit' is inputted
	char * status = "ongoing";
	
	//array for the input command
	char str[100];

	
	printf("Shell has started\n");
	
	//while loop that repeats and waits for next command
	while(strcmp(status, "ongoing") == 0){

		printf("\n status is equal to %s", status);
		printf("\n");
		//reads input command
		scanf("%s", str);

	//TODO child should be created at the start of the shell.
	//TODO environment commands

		//'strcmp' compares strings against eachother
		if(strcmp(str, "showenv") == 0){
		//print out properties.txt line by line
		}
		if(strcmp(str, "mysetenv") == 0){
		//sets input as line in properties.txt
		}
		if(strcmp(str, "myexport") == 0){
		//sets input as property of child
		}
		if(strcmp(str, "showchild") == 0){
		//prints out the child directory
		}
		if(strcmp(str, "quit") == 0){
			printf("\n you have entered: %s ", str);
			printf("\n");
			status = "quit";
		}
	}
	exit(0);
}
