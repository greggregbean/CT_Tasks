#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
    char file[100] = {'\0'};
    char dir[100] = {'\0'};
    char path[200] = {'\0'};

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
    int res_of_stat = stat(path, &info);

    if(res_of_stat == 0) {
        printf("File is already existing. \n");
        exit(0);
    }

    int fd = open(file, O_CREAT | O_RDWR | O_EXCL, 0666);
    if (fd < 0 ) { 
        printf("Failed to open file. \n");
        exit(-1);
    }

    if (ftruncate(fd, 100000) < 0) {
        printf("Failed to ftruncate. \n");
        exit(-1);
    };

    char* ptr = (char*) mmap(NULL, 100000, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Failed to map. \n");
        exit(-1);
    }

    close (fd);

    char* tmpptr = ptr;

    DIR* cur_dir = opendir(dir);

    struct dirent* next_dir;

    while((next_dir = readdir(cur_dir)) != NULL) {
        printf("\"%s\" \n", next_dir -> d_name);
        strcat(tmpptr, next_dir -> d_name);
        strcat(tmpptr, "\n");
        tmpptr += (strlen(next_dir -> d_name) + 1);
    }

    closedir(cur_dir);

    if(munmap(ptr, 100000) < 0) {
        printf("Failed to munmap.\n");
        exit(-1);
    }

}