//Netids: Arun ajf227 , Kareem kkj47
//No idea what includes we'll need so im just including everything
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "./parser.h"
#include "./tries.h"

//if a word is incorrect in ANY FILE, return EXIT_FAILURE
// if 1 that means to exit with failure, if 0 that means exit with success
short failExit = 0;

//Error handling for cases with directory
//Might include some for different file types
enum ERROR_CODES {
    NOT_DIRECTORY,
    NOT_FILE,
    ARG_EMPTY,
    DNE,
};

    

void PRINTERR(enum ERROR_CODES error, char *value, char *argv[]) {
    switch (error) {
        case NOT_DIRECTORY:
            printf("Error: %s is not a directory. Make sure your path is correct\n", value);
            break;
        case NOT_FILE:
            printf("Error: %s is not a file. Make sure your path is correct\n", value);
            break;
        case ARG_EMPTY:
            printf("Error: The given argument(%s) is empty. Therefor nothing will be printed for this file\n", value);
            break;
        case DNE:
            printf("Error: The given argument(%s) does not exist. Make sure your path is correct\n", value);
            break;
    }
}

//call functions ahead of time
void DTE(char *arg);
void isEmpty(char *arg);
void spellCheck(char *file);



//function Does This Exist
//checks if the given file or directory exists
void DTE(char *arg) {
    struct stat buffer;
    int exist = stat(arg, &buffer);
    if (exist == 0) {
        //printf("The file exists\n");
        isEmpty(arg);
    } else {
        PRINTERR(DNE, arg, NULL);
        exit(EXIT_FAILURE);
    }
}

//function that checks if given directory is empty
void isEmpty(char *arg) {
    struct stat buffer;
    stat(arg, &buffer);
        //check if the passed in argument is a file or a directory
        if (S_ISDIR(buffer.st_mode)) {
            if (buffer.st_size == 2) {
                PRINTERR(ARG_EMPTY, arg, NULL);
               //printf("the dir is empty\n");
                
                //exit(EXIT_FAILURE); // uncomment if you want the program to exit after finding an empty directory
                //printf("The directory %s is empty\n", arg);
            }
            //printf("The Directory is not empty\n");

            //check if the files within the directory are empty or something I would want to read.
        }

        if (S_ISREG(buffer.st_mode)) {
            if (buffer.st_size == 0) {
                PRINTERR(ARG_EMPTY, arg, NULL);
                //printf("the file is empty\n");
                //exit(EXIT_FAILURE); // uncomment if you want the program to exit after finding an empty file
                //printf("The file %s is empty\n", arg);
            }
            //printf("The File is not empty\n");
        }
}

void spellCheck(char *file) {
    struct stat buffer;
    stat(file, &buffer);

    //if the given file is a directory, run the spell check on all the files within the directory
    if (S_ISDIR(buffer.st_mode)) {
        //open the directory
        //kind of redundant to do this since we already checked if it existed but
        //I want to make sure it is a directory
        DIR *dir = opendir(file);
        if (dir == NULL) {
            perror("Unable to open directory");
            //print the current file
            exit(EXIT_FAILURE);
        }
        // Read each entry in the directory
        struct dirent *entry = readdir(dir);
        
        while (entry != NULL) {
            printf("%s\n",entry->d_name);
            // Ignore special entries "." and ".."
            if (entry->d_name[0] != '.') {
                //spell check only the text files or no extension files
                if ((strrchr(entry->d_name, '.') == NULL || strcmp(strrchr(entry->d_name, '.'), ".txt" ) == 0) && entry->d_name != NULL) {
                    //printf("%s\n", entry->d_name);
                    
                    //NOW we will loop through this directory and compare each "entry->d_name" to the dictionary
                    size_t file_name = strlen(entry->d_name);
                    size_t directory_path = strlen(file);
                    char* fullname = malloc(sizeof(char) * (file_name + directory_path + 10));
                    strcpy(fullname,file);
                    strcat(fullname,entry->d_name);
                    //printf("fullname: %s\n",fullname);
                    //is this function now defunct????
                    //I think so
                    
                    //checking if passed in argument is a directory
                    struct stat buffer2;
                    stat(fullname, &buffer2);
                    if (S_ISDIR(buffer2.st_mode)) {
                        //add a forward slash to the end of the directory
                        strcat(fullname,"/");
                        //check if the directory is empty (just to print to the terminal)
                        isEmpty(fullname);
                        spellCheck(fullname);
                        
                    }

                    if (S_ISREG(buffer2.st_mode)) {
                        //printf("The current file is: %s\n", fullname);
                        failExit = parsefile(fullname);
                    }

                    
                    free(fullname);
                }
            }
            entry = readdir(dir);
        }
        // Close the directory
        closedir(dir);


    }

    //if the given file is a regular file, run the spell check on the file
    //I will not error handle for the possibility of the file being not a file
    if (S_ISREG(buffer.st_mode)) {
        //printf("The current file is: %s\n", file);
        failExit = parsefile(file);
    }

}
//do we have to check and make sure if they are anything other than directory or file? probably


int main(int argc, char** argv) {

    //if argc is less than 2, print error message and return 1
    if (argc <= 2) {
        printf("Usage: spchk <Dictionary Path> <Directory1 or File1 Path> <Directory2 or File2 Path> ... <DirectoryN or FileN Path>\n");
        return 1;
    }

    //argv[0] is the program
    //argv[1] is the dictionary
    //argv[2+] is a directory or file
    if (argc > 2) {

        //iterate through the arguments passed in
        for (int i = 0; i < argc-1; i++) {
            //check if the given arguments are valid directories or files AND if they are empty
            //as of this moment I took off checking if the given argument is empty.
            //technically if they are empty it shouldnt actually interfere with the running of the program
            //but maybe ill add a print or warning that the given argument is empty
            DTE(argv[i+1]);
        }
        //print how many files there are
        //printf("There were %d files passed into the program. (counting the dictionary)\n", argc-1);

        //parse the dictionary
        parsedict(argv[1]);
        //the dictionary is now parsed and the trie is built
        //print();

        //now we compare each file or directory to the dictionary
        for (int i = 0; i < argc-2; i++) {
            //printf("in main for loop: %s\n",argv[i+2]);
            //compare the dictionary to the file or directory
            spellCheck(argv[i+2]);
        }
        destroy();

        if (failExit) {
            exit(EXIT_FAILURE);

        } else if (!failExit) {
            exit(EXIT_SUCCESS);
        }
        
    }




    return 0;
}   

// ⢳⠀⠀⠀⡴⠀⠀⢀⠀⠀⣠⣴⣿⣿⣯⠄⠀⢀⠀⠀⠀⠀⡟⠻⠿⠟⠛⠢⡜⠓⠠⢤⠞⠻⣶⣤⡀⠀⢒⢶⠀⢠⠂⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⣰⠁⣴⠞⢿⠹⠏⠡⠞⣿⣿⣿⠟⠉⠉⠑⠦⠤⠊⠁⠀⠀⠀⠀⠀⠱⡄⠀⢨⣄⠀⢈⣿⣝⢷⣄⡸⢀⡝⠁⠀⠀⠀⠀⠀⠀⠤
// ⠀⠀⠠⠃⠀⣧⠖⠋⠑⡶⣤⡞⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠘⣆⠀⠀⠀⠀⠀⠘⠒⠉⠉⠳⣾⣿⣿⠷⣽⢃⡞⠁⠀⠀⠀⠀⠀⠀⠀⠐
// ⠀⢀⠆⠀⢸⡧⢄⡠⠔⠁⡎⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣦⠀⠀⠀⠀⠀⠀⢢⡀⠀⠈⢻⡅⢠⣿⠟⢻⠶⣄⠘⡷⠀⠀⠀⠀⠀
// ⠶⠛⠋⣰⡿⠀⢀⡟⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣟⢷⡄⠀⠀⠀⠀⠀⠱⡄⠀⠀⣹⣿⢺⠀⢀⣄⣾⡈⠁⢠⣄⡀⠀⠀
// ⡀⠀⢠⣿⣿⣷⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣠⡹⣦⣤⣤⣤⣤⣶⣿⡟⠚⠁⠘⣄⣹⣧⣄⠈⣷⠀⠃⠈⠂⠀⠀
// ⠀⠀⣼⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣧⣁⡀⢹⣿⡟⠋⠉⠙⠻⡏⠀⠀⠘⣿⣿⡿⠿⣿⣧⡀⠀⠀⠈⠒
// ⡄⠀⣟⢀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣯⠙⠛⠛⠛⠓⢶⡦⠽⣄⣳⠀⠀⠠⣿⣿⣧⡀⣿⣯⣿⣄⠉⠉⠉
// ⡶⠛⣉⡻⠀⠀⠀⠀⠐⠁⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠙⢦⡈⢻⡇⠀⠀⢿⣿⢿⣷⣿⡟⣻⣿⣆⠀⣄
// ⠳⣎⠁⡇⠀⠀⠀⠀⢰⠀⠀⠀⠀⠀⠀⠀⠀⢸⡆⠀⠀⠀⠀⠀⠀⣾⠀⠀⡠⠀⣀⣀⣀⣀⣈⣓⣷⠀⠀⠈⢿⣦⣿⣿⣿⣿⣿⡜⣆⠃
// ⠉⠛⢻⡀⠀⠀⠀⠀⢸⠀⠀⠀⣇⠀⠀⠀⠀⠘⣧⠀⠀⠀⠀⠀⠀⢹⡇⢴⣿⣿⣛⣿⣿⣿⣿⣿⣿⡇⡀⠀⢸⣽⣿⣿⣿⣮⣿⣿⣹⣶
// ⠀⠀⡾⠀⠀⠀⠀⠀⠀⡇⠀⠀⢹⡀⠀⠀⠀⠀⢿⡄⡀⠀⠀⠀⠀⣸⡇⠉⠙⠁⠀⠀⠉⠁⠁⠀⠈⡇⡇⠀⠘⣿⢿⣿⣿⠟⠛⢿⣿⣿
// ⠀⠀⡇⠀⠀⠀⠀⠀⠀⡇⠀⠀⠈⣧⠀⠀⠀⠀⢼⠻⣷⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⡇⠀⣼⡇⣀⣽⢿⣆⠀⢸⠀⡿
// ⠂⠀⢸⠀⠀⡆⠀⠀⠀⢹⠀⠀⠀⢹⣆⠀⠀⠀⢸⣇⢻⣷⡀⣠⠞⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⠇⠀⣿⡷⠛⡿⢸⠈⠑⣶⣄⠀
// ⠀⠀⢸⡀⠀⣧⠀⠀⠀⢘⣇⠀⠀⠀⢟⢦⢦⠀⠀⢻⡄⠹⡄⠙⠗⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⢰⡯⠤⠚⢁⡿⢳⠀⠈⣿⣷
// ⠀⠀⠀⡇⠀⢸⡀⡆⠀⠘⣿⢦⡄⠀⠘⡄⠙⢷⡤⠤⠽⣦⠃⠀⠀⠀⣀⣀⣄⡀⠀⠀⠀⠀⠀⠀⣼⡏⢠⢿⠁⢀⠞⠁⠀⢸⠀⢀⠈⠋
// ⡀⠀⠀⢹⡀⢸⣧⣰⡀⠀⠹⣆⠙⢦⣀⠙⣄⠀⠉⠀⠀⠀⢀⣤⣾⣽⣾⣿⣿⡏⣷⡄⠀⠀⠀⢰⢛⡷⠃⣼⡟⢹⠀⠀⠀⢸⡆⢨⣤⡴
// ⠈⠁⠀⢸⢳⠀⣿⡷⣷⣄⠀⠙⣄⠀⠉⠉⠉⠒⠀⠀⠀⠀⣾⣿⠿⠛⠉⠉⠉⠃⠙⢿⠀⠀⠀⢃⡜⠁⣰⣿⣇⡎⠀⠀⢣⣿⢀⠟⠉⠀
// ⠀⠀⠀⢸⢀⣧⢹⡇⠈⢻⣷⠤⣈⢦⡀⠀⠀⠀⠀⠀⠀⠀⢻⡏⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠘⠁⠀⣰⣿⣿⣾⣷⠒⢦⣿⠟⠁⠀⠀⠀
// ⠠⢤⣤⣼⢸⣀⡙⢳⡄⠀⠹⡇⠀⠙⣿⠦⠀⠀⠀⠀⠀⠀⠘⣧⡀⠀⠀⠀⠀⠀⣠⠇⠀⠀⠀⢀⣾⣿⣿⣿⣿⣾⠀⣀⢳⣤⠊⠀⠀⠠
// ⠀⠀⠀⠉⢿⢛⣿⣿⣿⣧⣀⢻⣤⠀⢹⣦⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠉⠀⠀⢀⡴⢛⣿⣿⣿⣻⡿⠙⡆⡼⠀⠙⣄⠀⠀⠀
// ⠀⠀⠀⠀⡸⠋⢱⠀⠙⢿⡏⠳⢿⣷⣄⠻⣿⣦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠾⠋⢀⣾⡄⠠⠈⡇⡇⠀⢹⠁⠀⠀⠈⠣⢄⠀
// ⠀⠀⢀⠜⠁⠀⣼⡇⠀⢀⠟⢆⠀⠹⡟⠛⢾⣛⠉⠹⡙⠛⠛⠿⣶⣲⠶⢦⡤⠴⣿⠋⠀⠀⣿⠗⢺⡀⢧⡇⠣⣀⣈⣹⣾⡄⠀⠀⠀⠑
// ⠀⠀⠀⠀⠀⢀⠟⢳⠀⠪⣀⡤⣱⣄⣽⡄⠀⠈⠙⠒⠒⠲⣶⣄⡀⠈⠛⢾⣷⣼⢃⣀⣴⠾⠋⠓⠢⢷⣈⡟⢲⡈⠉⠀⠘⡇⠀⠀⠀⠀
// ⠀⠀⠀⠀⢠⠊⠀⡯⠀⠀⠀⡇⠀⠀⠹⣰⠀⠀⠀⠀⠀⠀⢀⣹⣿⠿⠿⢿⣿⣿⡏⠛⢿⣦⡀⠀⠀⠀⠉⠳⣄⡱⡄⠀⠀⠳⡀⠀⠀⠀
