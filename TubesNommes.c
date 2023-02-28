#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    pid_t P1 = 0;
    pid_t P2 = 0;
    pid_t P3 = 0;

    int f = mkfifo("pipe", 0600);

    if ((P2 = fork()) == 0) {            
        if ((P1 = fork()) == 0) {
            int fd = open("pipe", O_WRONLY);
            dup2(fd, 1);
            close(fd);
            execl("/usr/bin/rev", "rev", "In.txt", NULL);
        }
        else{
            int fd = open("pipe", O_RDONLY);
            dup2(fd, 0);
            close(fd);
            int out = open("Outn.txt", O_WRONLY | O_CREAT | O_TRUNC);
            dup2(out, 1);
            close(out);
        }
        execl("/usr/bin/rev", "rev", NULL);
        _exit(0);
    }
    waitpid(P2, NULL, 0);

    if ((P3 = fork()) == 0) {
        execl("/usr/bin/diff", "diff", "In.txt", "Outn.txt", "-s", NULL);
    }
}

/*
    pid_t P1 = 0;
    pid_t P2 = 0;
    pid_t P3 = 0;

    int f = mkfifo("pipe", 0660);

    if ((P2 = fork()) == 0) {            
        if ((P1 = fork()) == 0) {        
            int fd = open("pipe", O_WRONLY);
            dup2(fd, 1);
            close(fd);
            execl("/usr/bin/rev", "rev", "In.txt", NULL);
        }
        waitpid(P1, NULL, 0);

        int out = open("Outn.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660);
        int fd = open("pipe", O_RDONLY);
        dup2(fd, 0);
        dup2(out, 1);
        close(fd);
        close(out);

        execl("/usr/bin/rev", "rev", NULL);
    }
    waitpid(P2, NULL, 0);

    if ((P3 = fork()) == 0) {
        execl("/usr/bin/diff", "diff", "In.txt", "Outn.txt", "-s", NULL);
    }
*/
