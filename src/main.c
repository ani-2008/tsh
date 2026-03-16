#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

void loop()
{
    char *line;
    char **args;
    int status;
    do {
        printf("$ ");
        line = getCmd();
        args = parse(line);
        status = execute(args);
        free(line);
        free(args);
    } while(status);

}


int main()
{
    loop();
    //help and exit

}

