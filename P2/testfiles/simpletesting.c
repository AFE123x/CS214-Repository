#include <stdio.h>

int main() {
    FILE *file;
    char ch, prev_ch = '\0'; // Initialize prev_ch to null character
    int printNext = 0; // Flag to indicate whether to print the next character

    // Open the file in read mode
    file = fopen("kindaempty.txt", "r");

    // Check if file exists
    if (file == NULL) {
        printf("Unable to open file.\n");
        return 1;
    }

    // Read the file character by character
    while ((ch = fgetc(file)) != EOF) {
        // Check if the previous character was newline or space
        if (prev_ch == '\n' || prev_ch == ' ') {
            printNext = 1; // Set the flag to print the next character
        }

        if (printNext) {
            printf("%c\n", ch); // Print the current character
            printNext = 0; // Reset the flag
            break; // Stop reading after printing the character
        }

        prev_ch = ch; // Update the previous character
    }

    // Close the file
    fclose(file);

    return 0;
}
