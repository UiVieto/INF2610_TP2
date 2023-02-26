#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

#define READ 0
#define WRITE 1

int main() {
    pid_t P1 = 0;
    pid_t P2 = 0;
    pid_t P3 = 0;

    int fd[2];
    pipe(fd);

    if ((P2 = fork()) == 0) {
        if ((P1 = fork()) == 0) {
            dup2(fd[WRITE], 1);              
            close(fd[READ]);
            close(fd[WRITE]);

            execl("/usr/bin/rev", "rev", "In.txt", NULL);
        }        
        waitpid(P1, NULL, 0);    

        int out = open("Out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660);
        dup2(fd[READ], 0);
        dup2(out, 1);
        close(fd[WRITE]);
        close(fd[READ]);
        close(out); 

        execl("/usr/bin/rev", "rev", NULL);
    }
    close(fd[WRITE]);
    close(fd[READ]);   
    waitpid(0, NULL, 0);
    
    if ((P3 = fork()) == 0) {
        execl("/usr/bin/diff", "diff", "In.txt", "Out.txt", "-s", NULL);
    }
}
