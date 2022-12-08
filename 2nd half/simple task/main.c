#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <dirent.h>

#define FILE_NAME 1000
#define FILE_SIZE 1000000

int main(int argc, char* argv[]) {
    char file[FILE_NAME] = {'\0'};
    char dir[FILE_NAME] = {'\0'};
    char path[FILE_NAME * 2] = {'\0'};

    if(argv[1] != NULL) {
        strcpy(file, argv[1]);
    }
    else {
        printf("No name of file in command line. \n");
        exit(-1);
    }
    printf("%s \n", file);

    if(argv[2] != NULL) {
        strcpy(dir, argv[2]);
    }
    else {
        strcpy(dir, "./");
    }
    printf("%s \n", dir);

    strcpy(path, dir);
    strcat(path, file);
    printf("Full pass: \"%s\".\n", path);

    struct stat info;

    if(stat(path, &info) == 0) {
        printf("File is already existing. \n");
        exit(0);
    }

    char cur_dir_file [FILE_NAME] = {"./"};
    strcat(cur_dir_file, file);

    if(stat(cur_dir_file, &info) == 0) {
        remove(cur_dir_file);
    }

    int fd = open(file, O_CREAT | O_RDWR | O_EXCL, 0666);
    if (fd < 0 ) { 
        printf("Failed to open file. \n");
        exit(-1);
    }

    if (ftruncate(fd, FILE_SIZE) < 0) {
        printf("Failed to ftruncate. \n");
        exit(-1);
    };

    char* ptr = (char*) mmap(NULL, FILE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Failed to map. \n");
        exit(-1);
    }

    close (fd);

    char* tmpptr = ptr;

    DIR* cur_dir;
    if((cur_dir = opendir(dir)) == NULL) {
        printf("Failed to open \"%s\". \n", dir);
        exit(-1);
    }

    struct dirent* next_dir;

    while((next_dir = readdir(cur_dir)) != NULL) {
        printf("\"%s\" \n", next_dir -> d_name);
        strcat(tmpptr, next_dir -> d_name);
        strcat(tmpptr, "\n");
    }

    closedir(cur_dir);

    if(munmap(ptr, FILE_SIZE) < 0) {
        printf("Failed to munmap.\n");
        exit(-1);
    }

}