#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>

void show_info(struct utmp* utbufp){
    printf("%-8.8s %-8.8s %101d %s\n", utbufp->ut_name, utbufp->ut_line, utbufp->ut_time, utbufp->ut_host);
}
int main(int argc, char const *argv[]){
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    if((utmpfd = open("/var/run/utmp", O_RDONLY)==-1)){
        perror(UTMP_FILE);
        _exit(1);
    }
    while(read(utmpfd, &current_record, reclen) != 0)
        show_info(&current_record);
    close(utmpfd);
    return 0;
}