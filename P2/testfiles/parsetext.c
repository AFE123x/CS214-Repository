#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    // Open the input file
    int fd = open("./filedir/easiertest.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int row = 1, col = 1, bytesRead;

    // Read from the file
    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            // Check for word start
            if ((buffer[i] >= 'a' && buffer[i] <= 'z') || (buffer[i] >= 'A' && buffer[i] <= 'Z')) {
                printf("Word starts at row %d, col %d\n", row, col);
                // Skip to the end of the word
                while ((buffer[i] >= 'a' && buffer[i] <= 'z') || (buffer[i] >= 'A' && buffer[i] <= 'Z')) {
                    i++;
                    col++;
                }
            }
            // Handle newline characters
            if (buffer[i] == '\n') {
                row++;
                col = 1;
            } else {
                col++;
            }
        }
    }

    if (bytesRead == -1) {
        perror("Error reading file");
        close(fd);
        return 1;
    }

    // Close the file
    close(fd);
    return 0;
}
