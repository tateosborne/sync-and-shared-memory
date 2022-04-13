## sync-and-shared-memory
---
#### This project uses C and Linux system calls to create a program that creates two processes which communicate and act upon some shared memory 
---
There is a parent process, which creates creates a `shmget`, which creates a piece of shared memory. The parent process writes to the shared memory,
and then attemps a `fork()`. If the fork is successful, the child process is created. The two processes take turns writing and reading whatever is in
shared memory. The child process is terminated when the parent writes "done" to shared memory (and thus the child reads "done"). 
