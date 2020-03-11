#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define N 6

int main(int argc, char** argv) {
    //eens proberen met 1 pipe
    int pids[N];
    int fds_C_P[N][2];
    int i, biggest = 0;

    for(i = 0; i < N; i++) {
        //make pipe for communications between child and parent
        if(pipe(fds_C_P[i]) < 0) {
            perror(argv[0]);
            return 1;
        }
        //make child
        if((pids[i] = fork()) < 0) {
            perror(argv[0]);
            return 1;
        }
        if(pids[i] == 0) {
            int x = 0;
            //CHILD
            //create random number and write it to screen
            srand(getpid()); //use pid to seed rand to prevent children from generating same numbers
            unsigned int r = rand();
            //write result to pipe
            write(fds_C_P[i][1], &r, sizeof(r));
            close(fds_C_P[i][1]);
            printf("Child %d with pid %d generated: %u\n", i, getpid(), r);
            return 0;
        }
    }

    //read from all pipes
    for(i = 0; i < N; i++) {
        int n;
        read(fds_C_P[i][0], &n, sizeof(int));
        if(n > biggest) 
            biggest = n;
    }
    printf("Biggest number = %d\n", biggest);
    for(i = 0; i < N; i++) {
        waitpid(pids[i], NULL, 0);
    }
    return 0;
}