#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <time.h>

#define ONID "brierej"
#define PREFIX "movies_"
#define SUFFIX ".csv"

int getIntRange(int, int);
int randomNum(int, int);
char* createDir();
void processFile();
char* findLargest();
char* findSmallest();
void subMenu();
void mainMenu();

int main(void) {

    //seed random generator for later use
    unsigned seed = time(0);
    srand(seed);

    mainMenu();

	return 0;
}

/*******************************************************************
* Description: validates that user input an integer within
 * 	the allowable range. Takes two integers as parameters, the
 * 	first for the allowable minimum and the second for the allowable
 * 	maximum. Returns validated int to calling function.
 * *****************************************************************/
int getIntRange(int minIn, int maxIn) {

    int isValid = 0;
    int input;
    int min = minIn;
    int max = maxIn;

    scanf("%d", &input);

    if (input < min || input > max)
    {
        printf("You entered an incorrect choice. Try again.\n\n");
        input = 0;
    }

    return input;
}

/******************************************************************
 * Description: function to generate a random number. Will receive
 * 	two ints as parameters for the minimum and maximum for the
 * 	range
 * ***************************************************************/
int randomNum(int min, int max) {
    int num = 0;

    num = (rand() % (max - min + 1)) + min;

    return num;
}

char* createDir() {
    // generate a random number, convert to string, determine length
    int number = randomNum(0, 99999);
    //printf("your random number is %d\n", number);
    char numString[6];
    sprintf(numString, "%d", number);
    size_t lenNum = strlen(numString);

    // determine length of ONID
    size_t lenONID = strlen(ONID);

    char* movies = ".movies.";
    size_t lenMovies = strlen(movies);

    // create newDir pathname as string and strcat values
    char* newDir = (char*)malloc(sizeof(char) * (lenNum + lenONID + lenMovies));

    strcpy(newDir, ONID);
    strcat(newDir, movies);
    strcat(newDir, numString);
    //printf("the name of the new dir is %s\n", newDir);

    // mkdir and set permissions (rwxr-x---)
    mkdir(newDir, 0750);
    printf("Created directory with name %s\n", newDir);

    return newDir;
}


void processFile() {

    char* dir = createDir();
    printf("the name of the new dir, returned from createDir is %s\n", dir);

    // parse file by year

    // In the new directory, create one file for each year in which at least one movie was released
    // see assignment for more...

    free(dir);
}


char* findLargest() {
    // code adapted from 3_5_stat_example.c in Exploration: Directories
    
    // open current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
    off_t fileSize = 0;
    struct stat dirStat;
    int i = 0;
    char* fileName = malloc(100);

    // go through all the files
    while ((aDir = readdir(currDir)) != NULL) {

        // code to compare end of string adapted from
        // https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
        size_t lenstr = strlen(aDir->d_name);
        size_t lensuffix = strlen(SUFFIX);

        if ((strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) && (strncmp(aDir->d_name + lenstr - lensuffix, SUFFIX, lensuffix) == 0)) {
            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);
            // compare the current value of fileSize and the st_size value of the directory entry
            if (i == 0 || dirStat.st_size > fileSize) {
                fileSize = dirStat.st_size;
                memset(fileName, '\0', sizeof(fileName));
                strcpy(fileName, aDir->d_name);
            }
            i++;
        }
    }

    closedir(currDir);

    return fileName;
}

char* findSmallest() {
    // code adapted from 3_5_stat_example.c in Exploration: Directories

    // open current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
    off_t fileSize = 0;
    struct stat dirStat;
    int i = 0;
    char* fileName = malloc(100);

    // go through all the files
    while ((aDir = readdir(currDir)) != NULL) {

        // code to compare end of string adapted from
        // https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
        size_t lenstr = strlen(aDir->d_name);
        size_t lensuffix = strlen(SUFFIX);

        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0  && (strncmp(aDir->d_name + lenstr - lensuffix, SUFFIX, lensuffix) == 0)) {
            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);
            // compare the current value of fileSize and the st_size value of the directory entry
            if (i == 0 || dirStat.st_size < fileSize) {
                fileSize = dirStat.st_size;
                memset(fileName, '\0', sizeof(fileName));
                strcpy(fileName, aDir->d_name);
            }
            i++;
        }
    }

    closedir(currDir);

    return fileName;
}


void subMenu() {
    int choice = 0;
    while (choice == 0) {
        printf("Which file do you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");
        printf("Enter a choice from 1 to 3: ");
        choice = getIntRange(1, 3);
    }

    switch (choice)
    {
    case 1:
        // for testing
        printf("You want to process the largest file\n");
        char* largestFile = findLargest();
        printf("Now processing the chosen file named %s\n", largestFile);
        free(largestFile);

        processFile();
        // should I call mainMenu again here or just return?
        mainMenu();
        break;
    case 2:
        // for testing
        printf("You want to process the smallest file\n");
        char* smallestFile = findSmallest();
        printf("Now processing the chosen file named %s\n", smallestFile);
        free(smallestFile);
        mainMenu();
        break;
    case 3:
        // for testing
        printf("You want to specify the name of a file\n");
        mainMenu();
        break;
    }
}

void mainMenu() {
    int choice = 0;
    while (choice == 0) {
        printf("1. Select file to process\n");
        printf("2. Exit the program\n\n");
        printf("Enter a choice 1 or 2: ");
        choice = getIntRange(1, 2);
    }

    switch (choice)
    {
    case 1:
        subMenu();
        break;
    case 2:
        printf("Bye!\n\n");
        break;
    }
}