#include<stddef.h>
#define YES 1
#define NO 0

char* next_cmd();
char** splitline(char*);
void freelist(char**);
void* emalloc(size_t);
void* erealloc(void*, size_t);
int execute(char**);
void fatal(char*, char*, int);

//process.c
int process(char**);

//controlflow.c
int ok_to_execute();
int is_control_command(char*);
int do_control_command(char**);

//builtin.c
int builtin_command(char** args, int* resultp);
