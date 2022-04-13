cmake_minimum_required(VERSION 3.21)
project(Synchronization_and_Shared_Memory C)

set(CMAKE_C_STANDARD 11)

add_executable(process-synchronization
        processes.tosborne.c)
add_executable(sighandler-example
        sighandler-example.c)