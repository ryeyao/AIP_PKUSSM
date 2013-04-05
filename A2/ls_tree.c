#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>

#ifndef LS_TREE_C
#define LS_TREE_C
#endif

#define EXIT_FAILURE -1

// Get the ablolute path of the file specified in the current directory
// Note: the returned value must be freed after calling this function
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

// Get the mode of the file
long get_mode(char* name) {
    struct stat sb;
    char* path = get_path(name);
    
    if (stat(path, &sb) == -1) {
        perror(path);
        exit(EXIT_FAILURE);
    }
    free(path);
    return sb.st_mode;
}

// Tell if the filetype in the mode is S_IFDIR
int IS_DIR(mode_t mode) {
    return !((mode & S_IFMT) ^ S_IFDIR);
}

// FIXME: This function doesn't work...
int compare(struct dirent ** ent1, struct dirent ** ent2) {
    if (IS_DIR((*ent1)->d_name) && IS_DIR((*ent2)->d_name)) {
        return 0;
    }
    if (IS_DIR((*ent1)->d_name) && !IS_DIR((*ent2)->d_name)) {
        return -1;
    }
    if (!IS_DIR((*ent1)->d_name) && IS_DIR((*ent2)->d_name)) {
        return 1;
    }
}

// Print the file tree.
void print_file(int ident, char * name) {

    int i = 0;
    if (ident != 0) {
        printf("|");
    }
    for (i = 0; i < ident; i++) {
        printf("   ");
    }
    printf("|-- %s\n", name);

    // Print directory recursively
    if (IS_DIR(get_mode(name))) { 

        struct dirent **namelist;
        int n = 0;

        char* path = get_path(name);
        chdir(path);

        n = scandir(".", &namelist, NULL, compare);
        free(path);
        if (n < 0) {
            perror("scandir");
        } else {
            while (n--) {
                // Dismiss the two special files
                if (!strcmp(namelist[n]->d_name, ".") || !strcmp(namelist[n]->d_name, "..")) {
                } else {
                    // Just print all files include the hidden files
                    print_file(ident + 1, namelist[n]->d_name);
                }
                free(namelist[n]);
            }
            free(namelist);
        }
        chdir("..");
    } 
    return;
}


int main (int argc, char** argv) {
    struct dirent **namelist;
    int n = 0;

    // Start to print file tree from current directory
    n = scandir(".", &namelist, NULL, compare);
    if (n < 0) {
        perror("scandir");
    } else {
        while (n--) {
            if (!strcmp(namelist[n]->d_name, ".") || !strcmp(namelist[n]->d_name, "..")) {
            } else {
                print_file(0, namelist[n]->d_name);
            }
            free(namelist[n]);
        }
        free(namelist);
    }

}
