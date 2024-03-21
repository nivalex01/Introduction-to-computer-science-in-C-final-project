#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH  15

typedef struct
{
    char* p2title;  //event name  
    char* p2location;  //event location  
    int year;   //event year 
} event;

typedef struct {
    int id; // id of sportsman
    char Fname[10]; //first name 
    char Lname[15]; // last name
    char* p2club; //pointer to sportsman club
    int gen; //gender 
    event* p2events; //pointer to a dynamic arr of events
    int Nevents; //number of participated events
}sportsman;

int countLines(const char* filename)
{
    char line[100];
    FILE* file_ptr = fopen(filename, "r");
    
    if (file_ptr == NULL) {
        printf("Error, can not opening the file\n");
        return -1;
    
    }
    int number_of_lines = 0; // we don't want to count the first line of the file. 
   int ch;

    while ((ch = fgetc(file_ptr)) != EOF)
    {
        if (ch == '\n')
        {
            number_of_lines++;
        }
    }

    fclose(file_ptr);
    return number_of_lines;
}

void FromFile2Sportsman(const char* filename, sportsman* sportsmen_array[], int sportsmen_array_size) 
{

    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    *sportsmen_array = malloc(sportsmen_array_size * sizeof(sportsman));  // allocate memory for sportsmen array;

    if (*sportsmen_array == NULL) // allocate memory checking
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    char line[100];

    fgets(line, sizeof(line), file);  // skip the format line (the first line)
    int sportmen_count = 0;

    while (fgets(line, sizeof(line), file) != NULL && sportmen_count < sportsmen_array_size)  //read line by line from the file 
    {
        int gen;
        char clubName[100]; // temporary buffer for club name, club name = maximum of 100 chars

        if (sscanf(line, "%d;%[^;];%[^;];%[^;];%d", &((*sportsmen_array)[sportmen_count].id), (*sportsmen_array)[sportmen_count].Fname,
            (*sportsmen_array)[sportmen_count].Lname, clubName, &gen) != 5)
        {
            printf("Error reading data from file\n");
            exit(1);
        }
        (*sportsmen_array)[sportmen_count].gen = gen;

        // dynamically allocate memory to p2club based on the length of clubName
        (*sportsmen_array)[sportmen_count].p2club = malloc((strlen(clubName) + 1) * sizeof(char));
        if ((*sportsmen_array)[sportmen_count].p2club == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        strcpy((*sportsmen_array)[sportmen_count].p2club, clubName);

        /*printf("ID: %d; Name: %s %s; Club: %s; Gender: %s\n", (*sportsmen_array)[sportmen_count].id,
            (*sportsmen_array)[sportmen_count].Fname, (*sportsmen_array)[sportmen_count].Lname, (*sportsmen_array)[sportmen_count].p2club,
            ((*sportsmen_array)[sportmen_count].gen == 1) ? "Male" : "Female");*/

        free((*sportsmen_array)[sportmen_count].p2club);

        sportmen_count++;
    }

    fclose(file);
}

void main() {
    int numSportsmen = countLines("SportsmanData.txt");

    printf("number of sportsmen in the file %d\n", numSportsmen);

    sportsman* sportsmen = NULL; //pointer to sportsmen array

    FromFile2Sportsman("SportsmanData.txt", &sportsmen, numSportsmen);
   
    free(sportsmen);  // free allocated memory for sportsmen array
}