#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

