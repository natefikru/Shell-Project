#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sched.h>


int shell_exit(char **args);
int help(char **args);
int cd(char **args) ;
int shell_clone(char **args);

#define STACK_SIZE 4068 
char *clone_net = "CLONE_NET";

//bool * isBackground = false;

char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "clone"
};

int (*builtin_func[]) (char **) = {
	&cd,
	&help,
	&shell_exit,
	&shell_clone
};

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

int cloneCalled(){
	char *ifconfigArgs[] = {"ifconfig", NULL};
	execvp(*ifconfigArgs, ifconfigArgs);
	printf("Clone_NET has been called\n");
}

int help(char **args)
{
	printf("calling help\n");
	printf("this is where the help status will go\n");
	return 1;
}

int shell_clone(char **args){
	
	printf("calling clone\n");
	void *child_stack = malloc(STACK_SIZE);
	if(strcmp(args[0], "clone") == 0){
		if(args[1] == NULL){
		fprintf(stderr, "expected argument CLONE_NET or clone_fs\n");
		}
		if(strcmp(args[1], "net") == 0){
			clone(&cloneCalled, child_stack+STACK_SIZE, *clone_net, NULL);
		}
	}	
	
}


int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

char *inputLine(void)
{
	char *line = NULL;
	ssize_t bufferSize = 0;
	getline(&line, &bufferSize, stdin);
	return line;
}



#define tokenSize 64
#define tokenDivider " \t\r\n\a"
char **parseLine(char *line)
{

	int bufferSize = tokenSize, position = 0;
	char **tokens = malloc(bufferSize * sizeof(char*));
	char *token;

	if (!tokens){
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, tokenDivider);

	while(token != NULL){
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
	tokens[position] = NULL;
	return tokens;
}

//tokens in an array
int startCommands(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0){
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
		execvp(*args, args);
		}
	}
	else if(pid < 0){
		fprintf(stderr, "Fork Failed");
	}
	else {
		wait(NULL);
	}

	return 1;

}

int initiateArgs(char **args)
{
	int i;
	if(args[0] == NULL){
		return 1;
	}
	printf("built in arg call\n");
	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0)
		{
			return (*builtin_func[i])(args);
    		}
  	}

  	return startCommands(args);
}



void commandLoop(void)
{
	char *line;
	char **args;
	int status;

	do {
		line = inputLine();
		args = parseLine(line);
		status = initiateArgs(args);

		free(line);
		free(args);
	} while(status);
}

int main()
{
	commandLoop();
  printf("Exiting Shell\n");
	return EXIT_SUCCESS;
}

int shell_exit(char **args)
{
  return 0;
}



