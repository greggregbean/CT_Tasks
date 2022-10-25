#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv []) {
    char dir[100] = {'\0'};

    if(argv[1] == NULL) {
        strcpy(dir, "./" );
        printf("По умолчанию выбрали текущю директорию: \"./\". \n");
    }
    else {
        strcpy(dir, argv[1]);
        printf("Наша директория: \"%s\". \n", dir);
    }
    
    int fd[2];
    if (pipe(fd) < 0) {
        printf("Can't create a pipe\n");
        exit(-1);
    }
    
    pid_t process_id = fork();

    if(process_id < 0) {
        printf("Failed to create child procces! \n");
        exit(-1);
    }

    else if(process_id > 0) {
        printf("Parent proccess. PID = %d. \n", getpid());

        pid_t p_id = fork();

        int parent_child_status;
        wait(&parent_child_status);

        if (p_id < 0) {
            printf("Failed to create parent-child process! \n");
            exit(-1);
        } 

        else if (p_id > 0) {
            printf("Parent - parent proccess. PID = %d. \n", getpid());
            close(fd[0]);

            int file = open("file.txt", O_RDONLY);
            if(file == -1) {
                printf("Failed to open file! \n");
                exit(-1);
            }
            char string[1000] = {'\0'};
            size_t nbytes = read(file, string, 1000);
            write(fd[1], string, nbytes);
        }

        else {
            char ls_command[100] = "ls ";
            strcat(ls_command, dir);
            strcat(ls_command, " > ");
            strcat(ls_command, "file.txt"); 
            printf("Parent-child proccess. PID = %d. \n", getpid());
            execl("/bin/bash", "/bin/bash", "-c",  ls_command, NULL);
        }
    }

    else {
        sleep(2);
        printf("Child proccess. PID = %d. \n", getpid());
        close(fd[1]);

        char str[1000] = {'\0'};
        read(fd[0], str, 1000);
        printf("%s \n", str);
        exit(0);
    }

    return 0;
}