#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sched.h>
#include<sys/wait.h>
#define STACK_SIZE 4068 

//forward functiona initiation
int shell_exit(char **args);
int help(char **args);
int cd(char **args) ;
int shell_clone(char **args);

//list of possible built in funtions commands
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "clone"
};


//list of possible built in functions
int (*builtin_func[]) (char **) = {
	&cd,
	&help,
	&shell_exit,
	&shell_clone
};

//manual function for opening a folder, needs a path
int cd(char **args)
{
	printf("calling cd\n");
  if (args[1] == NULL) {
    fprintf(stderr, "expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return 1;
}

//function that is ran with 'clone net' call in command line
static int cloneFunction(){
	printf("Clone ip link has been called\n");

	char *iplinkArgs[] = {"ip","link", NULL};
	execvp(*iplinkArgs, iplinkArgs);
	return 0;
}

//function that prints out the help documentation
int help(char **args)
{
	printf("Help Options\n");
	printf("1. Type in a bash command\n");
	printf("2. Run an ip link process in a new network with 'clone net'\n");
	printf("3. Type 'exit' to quit the program.\n");
	return 1;
}

//clone function that runs clone command with CLONE_NEWNET as a flag
int shell_clone(char **args){
	
	printf("calling clone\n");
	//initiates the child stack
	void * child_stack;
	child_stack = malloc(STACK_SIZE);
	if(strcmp(args[0], "clone") == 0){
		if(args[1] == NULL){
		fprintf(stderr, "Must have the argument 'net' with the clone function.\n");
		}
		if(strcmp(args[1], "net") == 0){
			clone(&cloneFunction,(char *)child_stack+STACK_SIZE, CLONE_NEWNET , NULL);
			sleep(1);
		}
	}	
	
}

//function that returns the number of built in functions
int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

//function that reads line from input
char *inputLine(void)
{
	char *line = NULL;
	ssize_t bufferSize = 0;
	getline(&line, &bufferSize, stdin);
	return line;
}


//defines the byte size of each token and the delimiter that breaks up the tokens.
#define tokenSize 64
#define tokenDivider " \t\r\n\a"
char **parseLine(char *line)
{
	//initializes the buffer size, token size and position to equal 0
	int bufferSize = tokenSize, position = 0;
	char **tokens = malloc(bufferSize * sizeof(char*));
	char *token;

	//if there are no tokens then error out.
	if (!tokens){
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	//create the first token by character.
	token = strtok(line, tokenDivider);

	//while loop that goes through the line and breaks up the line into multiple tokens
	while(token != NULL){
		//goes down the line, character by character,  using position as a reference
		tokens[position] = token;
		position++;

		if(position >= bufferSize){
			bufferSize += tokenSize;
			tokens = realloc(tokens, bufferSize * sizeof(char*));
			if(!tokens){
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, tokenDivider);
	}
	//returns a list of the tokens
	tokens[position] = NULL;
	return tokens;
}

//tokens in an array
int startCommands(char **args)
{
	pid_t pid, wpid;
	int status;
	
	//sets child pid
	pid = fork();
	if(pid == 0){
		//possible input commands.
		if(strcmp(args[0], "ls") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "mkdir") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "touch") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "pwd") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "rm") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "rmdir") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "grep") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "sh") == 0){
			execvp(*args, args);
		}
		else if(strcmp(args[0], "bash") == 0){
			char *execArgs[] = {"/bin/bash", NULL};
			execvp(*args, args);
		}
		else {
		//this line encompasses everything possible
		execvp(*args, args);
		}
	}
	else if(pid < 0){
		//fail if theres an error.
		fprintf(stderr, "Fork Failed");
	}
	else {
		//parent function waits for the child to finish.
		wait(NULL);
	}

	return 1;

}

//takes list of tokens and runs them.
int initiateArgs(char **args)
{
	int i;
	if(args[0] == NULL){
		return 1;
	}
	//loops through the built in list first to see if the command is already built into the program.
	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0)
		{
			return (*builtin_func[i])(args);
    		}
  	}
	//if it isn't already built in then the fork/exec process is ran.
  	return startCommands(args);
}



void commandLoop(void)
{
	char *line;
	char **args;
	int status;

//this loop run continuously waiting for a new input to be entered.
	do {
		//line input
		line = inputLine();

		//parse line into tokens
		args = parseLine(line);

		//run tokens as commands
		status = initiateArgs(args);

		free(line);
		free(args);
	} while(status);
}

int main()
{
	//main function that runs the shell.
	commandLoop();
  printf("Exiting Shell\n");
	return EXIT_SUCCESS;
}

int shell_exit(char **args)
{
  return 0;
}



