#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

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
    FILE* file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("Error, can not opening the file\n");
        return -1;
    }
    int number_of_lines = -1; // we don't want to count the first line of the file. 
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
void main()
{
    int sportsmanLines = countLines("SportsmanData.txt");
    int eventLines = countLines("EventData.txt");
    printf("Number of lines in SportsmanData.txt: %d\n", sportsmanLines);
    printf("Number of lines in EventData.txt: %d\n", eventLines);
    printf("hapoel zona");
}