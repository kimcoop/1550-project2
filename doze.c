#define _XOPEN_SOURCE 600

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

void my_handler(int);
void displayUsage(FILE *, const char *);

int main(int argc, char **argv)
{
    char c[BUFSIZ];
    int fd;
    int n;
    long int nBytes;
    pid_t pid;
    int child_status;

    if(argc != 3){
        displayUsage(stderr, argv[0]);
        exit(1);
    }

    nBytes = strtol(argv[1], NULL, 10);
    if((nBytes <= 0) || (nBytes > BUFSIZ)){
        fprintf(stderr, "Buffer size %s is not a positive integer between 1 and %d\n",
                argv[1], BUFSIZ);
        exit(2);
    }

    if((fd = open(argv[2], O_RDONLY, 0)) < 0){
        perror(argv[2]);
        exit(3);
    }

    if(signal(SIGUSR1, my_handler) == SIG_ERR){
        perror("Could not set a handler for SIGUSR1");
        exit(4);
    }

    pid = fork();
    if(pid < 0){
        perror("fork");
        exit(6);
    }

    if(pid == 0){
        printf("Child (%d) about to pause()\n", (int)getpid());
        pause();
        printf("Child's turn %d!\n", (int)getpid());
        lseek(fd, 0L, SEEK_SET);
        while((n = read(fd, c, nBytes)) != 0){
            if(write(STDOUT_FILENO, c, n) < 0){
                perror("Write Error!\n");
                exit(7);
            }
        }
        int rc = 37;
        printf("Child exiting (status = %d (0x%.2X))\n", rc, rc);
        exit(rc);
    }
    else{
        printf("Parent's turn! (pid = %d, kid = %d)\n", (int)getpid(), (int)pid);
        while((n = read(fd, c, nBytes)) != 0){
            if(write(STDOUT_FILENO, c, n) < 0){
                perror("Write Error!\n");
                exit(7);
            }
        }
        printf("Parent sleeping\n");
        sleep(1);
        printf("Parent sending signal to child\n");
        kill(pid, SIGUSR1);
        printf("Parent waiting for child\n");
        int corpse = wait(&child_status);
        printf("waiting over: pid = %d, status = 0x%.4X\n", corpse, child_status);
    }

    if(close(fd) < 0){
        perror(argv[2]);
        exit(8);
    }

    printf("%d exiting\n", (int)getpid());
    return(0);
}

void displayUsage(FILE *fp, const char *arg)
{
        fprintf(fp, "Usage: %s n filename\n", arg);
        fprintf(fp, "where\n");
        fprintf(fp, "\tn\tNumber of bytes to read and write in each \"block\".\n");
        fprintf(fp, "\tfilename\tName of file to be catted.\n");
}

void my_handler(int sig)
{
    if(signal(sig, my_handler) == SIG_ERR){
        perror("Could not set a handler for SIGUSR1");
        exit(4);
    }
    printf("In signal handler (pid %d)\n", (int)getpid());
}