#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>


#ifndef LS_L_C
#define LS_L_C
#endif

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

char* uid_to_name(uid_t uid) {
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pw_ptr->pw_name;
    }
}

char* gid_to_name(gid_t gid) {
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return grp_ptr->gr_name;
    }
}

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

// make the permission string
char* perm_str(mode_t mode) {

    char* mode_str = malloc(9);
    strcpy(mode_str, "rwxrwxrwx");
    mode_t usr_perm, grp_perm, oth_perm;

    usr_perm = mode & S_IRWXU;
    grp_perm = mode & S_IRWXG;
    oth_perm = mode & S_IROTH;

    // User perm
    if (!(usr_perm & S_IRUSR)) {
        mode_str[0] = '-';
    }

    if (!(usr_perm & S_IWUSR)) {
        mode_str[1] = '-';
    }

    if (!(usr_perm & S_IXUSR)) {
        mode_str[2] = '-';
    }

    // Group
    if (!(grp_perm & S_IRGRP)) {
        mode_str[3] = '-';
    }

    if (!(grp_perm & S_IRGRP)) {
        mode_str[4] = '-';
    }

    if (!(grp_perm & S_IRGRP)) {
        mode_str[5] = '-';
    }

    // Other
    if (!(oth_perm & S_IROTH)) {
        mode_str[6] = '-';
    }

    if (!(oth_perm & S_IWOTH)) {
        mode_str[7] = '-';
    }

    if (!(oth_perm & S_IXOTH)) {
        mode_str[8] = '-';
    }

    return mode_str;

}

// Print the file information the way "ls -l" does.
void info_file(char* name) {
    struct stat sb;
    char* path;

    if (!strcmp(name, ".") || !strcmp(name, "..")) {
        return;
    }
    path = get_path(name);

    if (stat(path, &sb) == -1) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    char type;
    char *perm;
    int nlink;
    char* usr_name;
    char* usr_group_name;
    long size;
    char mtime[128];

    type = '-';
    perm = perm_str(sb.st_mode);
    nlink = sb.st_nlink;
    usr_name = uid_to_name(sb.st_uid);
    usr_group_name = gid_to_name(sb.st_gid);
    size = (long) sb.st_blksize;
    strcpy(mtime, ctime(&sb.st_mtime));
    int end = strlen(mtime);
    mtime[end - 2] = '\0'; // remove the '\n'

    switch (sb.st_mode & S_IFMT) {
       case S_IFBLK:  
           type = 'b';
           break;
       case S_IFCHR:  
           type = 'c';
           break;
       case S_IFDIR:  
           type = 'd';
           break;
       case S_IFIFO:
           type = 'f';
           break;
       case S_IFLNK:
           type = 'l';
           break;
       case S_IFREG:
           type = 'r';
           break;
       case S_IFSOCK:
           type = 's';
           break;
       default:
           break;
    }

    printf("%c%9s %1d %s %s %ld %s %s\n", type, perm, nlink, usr_name, usr_group_name, size, mtime, name);

    free(path);
    free(perm);
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
            info_file(namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }
}
