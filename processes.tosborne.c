#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define BUFFER_SIZE 32

// declare parent and child flags as global variables
int parentGo;
int childGo;

// parent process signal handler
void parentHandler() {
    childGo = 1;
}

// child process signal handler
void childHandler() {
    parentGo = 1;

}

int main() {
    int pid;
    int ppid;
    int memid;
    int key = IPC_PRIVATE;
    char *ptr;
    char buffer[BUFFER_SIZE];

    // initialize the flags to 0
    parentGo = 0;
    childGo = 0;

    // register the signals
    signal(SIGUSR1, parentHandler);
    signal(SIGUSR2, childHandler);

    // create new shared memory segment:
    //   returns the shared memory key (a positive integer) if shmget() is successful
    //   returns -1 if shmget() fails
    memid = shmget(key, BUFFER_SIZE, IPC_EXCL | 0666);
    if (memid < 0) {
        printf("shmget() failed\n");
        return 1;
    }

    // ptr is assigned the shared memory segment
    ptr = (char *) shmat(memid, 0, 0);

    // confirm ptr was assigned the memory segment
    if (ptr == NULL) {
        printf("shmat() failed\n");
        return 1;
    }

    // fork():
    //   returns 0 to the child process
    //   returns the child's pid to the parent process
    //   returns -1 to if the fork failed
    pid = fork();
    if (pid < 0) {
        printf("the fork() failed");
    }

    // parent process
    if (pid > 0) {

        // parent process prints its pid
        printf("I am the parent, and my pid is %d\n", getpid());

        //  parent process sends signal to child process via parentHandler
        kill(pid, SIGUSR1);

        // parent process enters spin loop until child process signals it to proceed
        while (!parentGo)
            ;

        // parent process writes buffer to ptr
        strcpy(buffer, "hello");
        printf("parent is writing '%s' to the shared memory\n", buffer);
        strcpy(ptr, buffer);

        // parent process sends signal to child process via parentHandler and sets parent flag to 0
        kill(pid, SIGUSR1);
        parentGo = 0;

        // parent process enters spin loop until child process signals it to proceed
        while (!parentGo)
            ;

        // parent process writes buffer to ptr
        strcpy(buffer, "from");
        printf("parent is writing '%s' to the shared memory\n", buffer);
        strcpy(ptr, buffer);

        // parent process sends signal to child process via parentHandler and sets parent flag to 0
        kill(pid, SIGUSR1);
        parentGo = 0;

        // parent process enters spin loop until child process signals it to proceed
        while (!parentGo)
            ;

        // parent process writes buffer to ptr
        strcpy(buffer, "tate");
        printf("parent is writing '%s' to the shared memory\n", buffer);
        strcpy(ptr, buffer);

        // parent process sends signal to child process via parentHandler and sets parent flag to 0
        kill(pid, SIGUSR1);
        parentGo = 0;

        // parent process enters spin loop until child process signals it to proceed
        while (!parentGo)
            ;

        // parent process writes buffer to ptr, sends signal to child process via parentHandler
        strcpy(buffer, "done");
        printf("parent is writing '%s' to the shared memory\n", buffer);
        strcpy(ptr, buffer);

        // parent process sends signal to child process via parentHandler
        kill(pid, SIGUSR1);

        // parent process waits for child process to finish
        wait(NULL);

        // parent process prints that the child has completed its work
        printf("child is done\n");
    }

    // child process
    else {
        // assign pid the child's pid and ppid the parent's pid
        pid = getpid();
        ppid = getppid();

        // child process enters spin loop to wait until parent process prompts child to continue
        while (!childGo)
            ;

        // child process prints the child's pid
        printf("I am the child, and my pid is %d\n", pid);

        // child process send a signal to parent process via childHandler and sets child flag to 0
        kill(ppid, SIGUSR2);
        childGo = 0;

        // child process enters spin loop until parent process signals it to proceed
        while (!childGo)
            ;

        // child process enters a while loop that runs until parent process writes 'done' to the shared memory
        while (strcmp(ptr, "done") != 0) {
            // child process prints what is in shared memory
            printf("I am the child, and I read this from the shared memory: '%s'\n", ptr);

            // child process sends a signal to parent process via childHandler, sets child flag to 0
            kill(ppid, SIGUSR2);
            childGo = 0;

            // child process enters spin loop until parent process signals it to proceed
            while (!childGo)
                ;
        }

        // child process prints ptr (which will be 'done' now the while loop has been exited)
        printf("I am the child, and I read this from the shared memory: '%s'\n", ptr);
    }

    // detach the shared memory segment
    shmdt(ptr);
    shmctl(memid, IPC_RMID, NULL);

    return 0;
}
