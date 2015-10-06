#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


int lsh_exit(char **args);
int lsh_cd(char **args) ;

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_exit,
	&lsh_cd
};

int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int lsh_num_builtins() {
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
		printf("child has been created\n");
		if(strcmp(args[0], "ls") == 0){
			char *execArgs[] = {"ls", NULL};
			//execvp(args[0], args);
			execvp("ls", execArgs);
		}
		if(strcmp(args[0], "bash") == 0){
			char *execArgs[] = {"bash","mkdir", NULL};
			//execvp(args[0], args);
			execvp("bash", execArgs);
		}
	}
	else if(pid < 0){
		fprintf(stderr, "Fork Failed");
	}
	else {
		wait(NULL);
		printf("Child has completed\n");
	}

	return 1;

}

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



void commandLoop(void)
{
	char *line;
	char **args;
	int status;

	do {
		printf("Shell has started\n");
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
  printf("Exiting Shell");
	return EXIT_SUCCESS;
}

int lsh_exit(char **args)
{
  return 0;
}
