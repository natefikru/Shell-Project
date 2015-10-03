#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main()
{
	commandLoop();

	return EXIT_SUCCESS;
}

void commandLoop(void)
{
	char *line;
	char **args;
	int status;

	do {
		printf("Shell has started");
		line = inputLine();
		args = parseLine(line);
		status = initiateArgs(args);

		free(line);
		free(args);
	} while(status);
}

char *inputLine(void)
{
	char *line = NULL;
	ssize_t bufferSize = 0;
	getline(&line, &bufferSize, stdin);
	return line;
}

#define tokenSize 64
#define tokenDivider = " \t\r\n\a"
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

//TODO

int initiateArgs(char **args)
{
	int i;
	if(args[0] == NULL){
		return 1;
	}
	for (i = 0; i < lsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) 
		{
			return (*builtin_func[i])(args);
    		}
  	}

  	return startCommands(args);
}

//tokens in an array
int startCommands(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0){
		execvp(args[0], args);
	}
	else if(pid < 0){
		fprintf(stderr, "Fork Failed");
	}
	else {
		wait(NULL);
		printf("Child has completed");
	}

	return 1;

}











