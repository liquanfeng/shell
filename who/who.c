#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>

void show_time(long timeval){
    char* cp;
    cp = ctime(&timeval);
    printf("%12.12s", cp+4);
}

void show_info(struct utmp* utbufp){
    if(utbufp->ut_type != USER_PROCESS)
        return;
    printf("%-8.8s ", utbufp->ut_name);
    printf("%-8.8s ", utbufp->ut_line);
    show_time(utbufp->ut_time);
    printf("%s\n", utbufp->ut_host);
}

int main(int argc, char const *argv[]){
    struct utmp utbuf;
    int utmpfd;

    if((utmpfd = open(UTMP_FILE, O_RDONLY))==-1){
        perror(UTMP_FILE);
        _exit(1);
    }
    while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
        show_info(&utbuf);
    close(utmpfd);
    return 0;
}