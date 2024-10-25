#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <glob.h>


#define BUFFER_SIZE 4096

int main() {
    
    glob_t globbuf;

    glob("*.o", 0, NULL, &globbuf);

    //print the glob count

    printf("\nglob count is: %ld\n", globbuf.gl_pathc);
    // Print the matched files
    for (int i = 0; i < globbuf.gl_pathc; i++) {
        printf("%s\n", globbuf.gl_pathv[i]);
    }


    return 0;
}