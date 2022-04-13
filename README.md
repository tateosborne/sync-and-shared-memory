## sync-and-shared-memory
---
#### This project uses C and Linux system calls to create a program that creates two processes which communicate and act upon some shared memory 
---
There is a parent process, which creates creates a `shmget`, which creates a piece of shared memory. The parent process writes to the shared memory,
and then attemps a `fork()`. If the fork is successful, the child process is created. The two processes take turns writing and reading whatever is in
shared memory. The parent writes "done" to shared memory and then waits for the child process to terminate. The child processs terminates once it reads "done"). Then, the parent process de-allocates the shared memory before the program is completed.
