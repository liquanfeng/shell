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
    int count = 0;
    char** cptr = argv;
    while(*cptr++)
        ++count;
    if(argv[0] == NULL)
        return 0;
    if((pid = fork()) == -1) 
        perror("fork");
    else if(pid == 0){ 
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        //execvp(argv[0], argv);
        if(strcmp(argv[0], "who")==0){
            if(count>2 && strcmp(argv[1], ">")==0)
                redirect(argv, &fd);
            else if(count>2 && strcmp(argv[1], "|")==0)
                dopipe(argv);
            execv("../who/who", NULL);
        }
        else if(strcmp(argv[0], "pwd")==0){
            if(count>2 && strcmp(argv[1], ">")==0)
                redirect(argv, &fd);
            else if(count>2 && strcmp(argv[1], "|")==0)
                dopipe(argv);
            execv("../pwd/pwd", NULL);
        }
        else if(strcmp(argv[0], "ls")==0){//bug
            if(count>2 && strcmp(argv[1], ">")==0)
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
    close(1);
    if((*fd = creat(argv[2], COPYMODE))==-1)
        fprintf(stderr, "Error: write error to ", argv[2]);
}

void dopipe(char** argv){
    int thepipe[2], newfd, pid;
    if(pipe(thepipe)==-1)
        infobug("Error: Cannot get a pipe");
    if((pid=fork())==-1)
        infobug("Error: Cannot fork");
    if(pid == 0){
        close(thepipe[0]);
        if(dup2(thepipe[1],1)==-1)
            infobug("Error: Cannot redirect stdout");
        close(thepipe[1]);
        execlp(argv[0], argv[0], NULL);
        infobug(argv[0]);
    }
    close(thepipe[1]);
    if(dup2(thepipe[0], 0)==-1)
        infobug("Error: Cannot redirect stdin");
    close(thepipe[0]);
    execlp(argv[2], argv[2], NULL);
    infobug(argv[2]);
}

void infobug(char *s){
    fprintf(stderr, "Error: %s\n", s);
    exit(1);
}