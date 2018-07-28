#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>

#define COPYMODE 0644

int execute(char* argv[]){
    int pid;
    int child_info = -1;
    int fd = 0;
    if(argv[0] == NULL)
        return 0;
    if((pid = fork()) == -1)
        perror("fork");
    else if(pid == 0){
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        //execvp(argv[0], argv);
        if(strcmp(argv[0], "pwd")==0){
            redirect(argv, &fd);
            execv("../pwd/pwd", NULL);
        }
        else if(strcmp(argv[0], "ls")==0){//bug
            redirect(argv, &fd);
            execv("../ls/ls", NULL);
        }
        else if(strcmp(argv[0], "cp")==0){
            execv("../cp/cp", argv);
        }
        else{
            printf("*\n");
            execvp(argv[0], argv);
        }
        perror("cannot execute command");
        exit(1);
    }
    else{
        if(wait(&child_info) == -1)
            perror("wait");
        //back file discriptor 1 to stdout if not
        if(fd){
            close(fd);
            if((fd = open(stdout, O_RDONLY))==-1){
                perror("Error: stdin redirection");
                child_info = -1;
            }
        }
        return child_info;
    }
}

void redirect(char** argv, int* fd){
    int count = 0;
    char** cptr = argv;
    while(*cptr++)
        ++count;
    if(count==3 && strcmp(argv[1], ">")==0){
        close(1);
        if((*fd = creat(argv[2], COPYMODE))==-1){
            fprintf(stderr, "Error: write error to ");
            perror(argv[2]);
            exit(1);
        }
    }
}