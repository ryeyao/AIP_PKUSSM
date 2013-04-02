#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>


#ifndef LS_L_C
#define LS_L_C
#endif

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

char* get_path(char* name) {
    char* current_dir;
    int path_len;
    char* path; 

    current_dir = get_current_dir_name();
    path_len = strlen(current_dir) + strlen(name) + 1 + 1;
    path = malloc(path_len);
    path = strcpy(path, current_dir);
    path = strcat(path, "/");
    path = strcat(path, name);
//    printf("%s \r\n", path);

//   free(path);
    free(current_dir);
    return path;

}

void info_file(char* name) {
    struct stat sb;
    char* path;

    path = get_path(name);

    if (stat(path, &sb) == -1) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    char mode[9];
    int prop_1;
    char usr_name[128];
    char usr_name_2[128];
    long block_size;
    char last_mod[128];

    block_size = (long) sb.st_blksize;
    strcpy(last_mod, ctime(&sb.st_mtime));

    printf("%9s %1d %s %s    %ld  %s  %s \r\n", mode, prop_1, usr_name, usr_name_2, block_size, last_mod, name);


    free(path);

}

int main(int argc, char** argv) {
    
    char usage[] = "usage: ls_l -l";
    if (argc != 2) {
        printf("%s\r\n", usage);
        return -1;
    }

    if (strcmp("-l", argv[1])) {
        printf("%s\r\n", usage);
        return -1;
    }

    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, NULL, alphasort);
    if (n < 0) {
        perror("scandir");
    } else {
        while (n--) {
            //printf("%s\n", namelist[n]->d_name);
            info_file(namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }

}



