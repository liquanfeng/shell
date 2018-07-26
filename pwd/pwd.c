#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

ino_t get_inode(char*);
void printpathto(ino_t);
void inum_to_name(ino_t, char*, int);

int main(int argc, char const *argv[])
{
    printpathto(get_inode("."));
    putchar('\n');
    return 0;
}

void printpathto(ino_t inode){
    ino_t myinode;
    char name[BUFSIZ];
    if(get_inode("..")!=inode){
        chdir("..");
        inum_to_name(inode, name, BUFSIZ);
        myinode = get_inode(".");
        printpathto(myinode);
        printf("/%s", name);
    }
}

void inum_to_name(ino_t inode, char* name, int buflen){
    DIR* dir_ptr = opendir(".");
    struct dirent* direntp;
    if(dir_ptr==NULL){
        perror(".");
        exit(1);
    }
    while((direntp = readdir(dir_ptr))!=NULL){
        if(direntp->d_ino==inode){
            strncpy(name, direntp->d_name, buflen);
            name[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
        }
    }
    fprintf(stderr, "error looking for inum %d\n", inode);
    exit(1);
}

ino_t get_inode(char* fname){
    struct stat info;
    if(stat(fname, &info) == -1){
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}