#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXBUFSIZE 1000
#define MAXNAMESIZE 20

void run_compilation (const char* programm_name) {
    pid_t child_process_id = fork();
    
    int child_child_status;
    wait(&child_child_status);

    if(child_process_id < 0) printf("Failed to create child-child process! \n");

    else if(child_process_id > 0) printf("Child-parent process. PID = %d. Just go back to parent process. \n", getpid());

    else {
        printf("Child-child process. PID = %d. \n", getpid());

        char object_name[MAXNAMESIZE];
        strcpy(object_name, programm_name);
        object_name[strlen(object_name) - 1] = 'o';
        printf("Programm name: \"%s\". Object name: \"%s\". \n", programm_name, object_name); 

        int res_of_compile = execl("/usr/bin/gcc", "/usr/bin/gcc", programm_name, "-o", object_name, (char *)NULL);
        if(res_of_compile == -1) printf("Failed to compile %s! \n", programm_name);

        exit(0);
    }
}

void run_process (const char* programm_name) {

    pid_t process_id = fork();
    
    //For debugging:
    int child_status;
    wait(&child_status);

    if(process_id < 0) printf("Failed to create child procces! \n");

    else if(process_id > 0) {
        printf("Parent proccess. PID = %d. Just go back to the loop. \n", getpid());
    }

    else {
        printf("Child proccess. PID = %d. \n", getpid());

        run_compilation(programm_name);

        char object_name[MAXNAMESIZE];
        strcpy(object_name, programm_name);
        object_name[strlen(object_name) - 1] = 'o';
        char src_obj_name[MAXNAMESIZE] = "./";
        strcat(src_obj_name, object_name);
        printf("Source object name: \"%s\". \n", src_obj_name);

        int res_of_run = execl(src_obj_name, src_obj_name, NULL);
        if(res_of_run == -1) printf("Failed to run %s! \n", src_obj_name);  

        exit(0);
    }
}

int main(int argc, char* argv[]) {
    run_process("helloworld.c");
}
