#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


#define CMD_BUFF_SIZE 1024
#define DELIM " \t\n\r\a"
#define TOK_BUFF_SIZE 64

int tshCD(char **args);
int tshHelp(char **args);
int tshHistory(char **args);
int tshExit(char **args);

int (*builtinFunc[]) (char **) = {
    &tshCD,
    &tshHelp,
    &tshHistory,
    &tshExit 
};

char *builtinStr[] = {
    "cd",
    "help",
    "history",
    "exit"
};


int numOfBuiltins()
{
    return sizeof(builtinStr) / sizeof(char *);
}

int tshCD(char **args)
{
    if(args[1] == NULL){
        char *homeDir = getenv("HOME");
        if(chdir(homeDir) != 0){
            perror("lsh");
        }
    }else{
        if(chdir(args[1]) != 0){
            perror("lsh");
        }
    }
    return 1;
}

int tshExit(char **args)
{
    return 0;
}

int tshHelp(char **args)
{
    printf("Toy SHell - TSH\ntype the command ( program names ) and arguments, hit enter\n Here are some builtins\n");
    for(int i = 0; i < numOfBuiltins(); i++){
        printf("\t%s\n",builtinStr[i]);
    }
    return 1;
}

int printDir()
{
    char buf[1024];
    if ( getcwd(buf, sizeof(buf)) == NULL)  {
        fprintf(stderr,"Error getting current working directory\n");
    }
    
    printf("\033[0;32m[\033[34m%s\033[0;32m]\033[0m $ ",buf);
    return 1;
}

int tshHistory(char **args)
{
    FILE *fp = fopen("history_file.txt","r");
    char *buffer = malloc(sizeof(char *) * 1024);
    while( fgets(buffer,sizeof(buffer), fp) != NULL) {
        printf("%s",buffer);
        
    }
    return 1;
}

char *getCmd()
{
    int buffsize = CMD_BUFF_SIZE;
    int position = 0;
    int c;
    char *buff = malloc(sizeof(char) * buffsize);

    if(!buff){
        fprintf(stderr, "tsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    c = getchar();
    while(1){
        if(c == EOF || c == '\n'){
            buff[position] = '\0';
            return buff;
        }else{
            buff[position++] = c;
        }
        if(position >= buffsize){
            buffsize += CMD_BUFF_SIZE;
            buff = realloc(buff, buffsize * sizeof(char));
            if(!buff){
                fprintf(stderr, "tsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        c = getchar();
    }

}

char **parse(char *line)
{
    int buffsize = TOK_BUFF_SIZE; int position = 0;
    char **tokens = malloc(sizeof(char *) * buffsize);
    char *token;

    if(!tokens){
        fprintf(stderr, "tsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= buffsize){
            buffsize += TOK_BUFF_SIZE;
            tokens = realloc(tokens, buffsize * sizeof(char *));
            if(!tokens){
                fprintf(stderr, "tsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;

    return tokens;

}


int launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    
    if(pid == 0){
        if(execvp(args[0],args) == -1){
            perror("tsh");
        }
        exit(EXIT_FAILURE);
    }else if(pid < 0){
        perror("tsh");
    }else{
        do{
            waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int execute(char **args, FILE *file)
{
    if(args[0] == NULL){
        return 1;
    }
    char **temp = args;
    while (*temp != NULL) {
        fprintf(file,"%s\n",*temp);
        fflush(file);
        temp++;
    }


    for(int i = 0; i < numOfBuiltins(); i++){

        if(strcmp(args[0], builtinStr[i]) == 0){
            return (*builtinFunc[i])(args);
        }

    }
    return launch(args);
}



void mainloop()
{
    char *line;
    char **args;
    int status;
    FILE *history_file;
    do {
        history_file = fopen("history_file.txt","a");

        if (history_file == NULL) {
            history_file = fopen("history_file.txt","w");
        }
        
        printDir();

        line = getCmd();
        args = parse(line);
        status = execute(args, history_file);
        
        free(line);
        free(args);

    } while(status);
    fclose(history_file);

}


int main()
{
    mainloop();

}

