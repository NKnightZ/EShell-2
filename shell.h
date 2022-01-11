#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_CMD_LENGTH 1024

struct sigaction new_sa;
struct sigaction old_sa;
pid_t p;

// function which parses the string which for each word separated by a space stores it in the array argv[] and sets the last cell to NULL
int parse_line(char *s, char **argv[]); 

// frees the allocations made in parse_line
void free_parse(char **argv); 

// function allowing to call waitpid to suspend the execution of a process until the state of p changes 
void waiting(); 

// function to execute a command by passing the parse word array as a parameter using parse_line 
int excute_cmd(char **cmd); 

// function to skip a line after ignoring the CTRL-C signal 
void new_line(); 

// handles redirection from standard output to a file 
// int redir_IO(char **argv, char *f);