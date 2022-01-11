#include "shell.h"

int parse_line(char *s, char **argv[]){ 
    if(s == NULL || *s == 0 || strcmp(s," ") == 0){
        return 1;
    }
    int j = 0;
    int k = 0;
    char buffer[MAX_CMD_LENGTH];
    for(size_t i = 0; i < strlen(s); i++){
        if(isspace(s[i])){
            buffer[i] = '\0';
            (*argv)[j] = malloc(sizeof(strlen(buffer)));
            strcpy((*argv)[j],buffer);
            for(size_t k = 0; k < MAX_CMD_LENGTH; k++){
                buffer[k] = '\0';
            }
            k = -1;
            j++;
        }
        if(j >= 16){
            return 2;
        }
        buffer[k] = s[i];
        if(s[i+1] == '\0'){
            buffer[i+1] = '\0';
            (*argv)[j] = malloc(sizeof(strlen(buffer)));
            strcpy((*argv)[j],buffer);
        }
        k++;
    }
    (*argv)[j+1] = NULL;
    return 0;
}

void free_parse(char **argv){
    int i = 0;
    while(argv[i] != NULL){
        free(argv[i]);
        argv[i] = NULL;
        i++;  
    }
    argv[i+1] = NULL;
}

void waiting(){
    while(1){
        pid_t waiting = waitpid(p, NULL, WNOHANG);
        if(waiting == p || waiting == -1){
            break;
        }
    }
}

int excute_cmd(char **cmd){
    p = fork();
    int e;        
    if(p == 0){
        e = execvp(cmd[0], cmd);
        if(e == -1){
            if((write(STDOUT_FILENO, cmd[0], strlen(*cmd))) < -1){
                perror("error write of execvp 1");
                return 1;
            }
            if((write(STDOUT_FILENO,": command not find\n", strlen(": command not find\n"))) < -1){
                perror("error write of execvp 2");
                return 1;
            }
            exit(1);
        }            
        exit(1);
    }
    waiting();
    return 0;
}

void new_line(){ 
    printf("\n");
}

/* int redir_IO(char **argv, char* f){  // don't work for the moment
    if(argv[2] == ">"){
        strcpy(strcpy(argv[1], f));
    }
    return 0;
}*/

int main(){
    char cmd[MAX_CMD_LENGTH];
    char **argv = malloc(sizeof(char*)*MAX_CMD_LENGTH);
    char *curdir = malloc(sizeof(char)*1024);
    
    /* char* file; // file to redirect 
    int fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0666); // for the redirection
    if(fd < -1){
        perror("error of open");
    } */

    if(sigfillset(&new_sa.sa_mask) == -1){
        perror("error sigfillset");
        return 1;
    };
    new_sa.sa_handler = SIG_IGN;
    new_sa.sa_flags = 0;
   
    while(1){
        curdir = getcwd(curdir,200);
        if((write(STDOUT_FILENO, curdir, strlen(curdir))) < -1){
            return 1;
        }
        if((write(STDOUT_FILENO, "$ ", strlen("$ "))) < -1){
            return 1;
        }
        // ignore CTRL-C signal and skip a line 
        if (sigaction(SIGINT, &new_sa, &old_sa) == 0 && old_sa.sa_handler != SIG_IGN){
            new_sa.sa_handler = new_line;
            sigaction(SIGINT, &new_sa, 0);
        }

        int nb = read(0, cmd, MAX_CMD_LENGTH);
        if(nb == 0){
            new_line();
            break;
        }
        cmd[nb-1] = '\0';
        if(!strcmp(cmd, "exit")){
            exit(1);
        }
        if(parse_line(cmd, &argv) == 2){
            printf("max number of arguments exceeded\n");
        }else{
            excute_cmd(argv); 
        }
    }
    free_parse(argv);
    free(argv);
    free(curdir);
    return 0;
}