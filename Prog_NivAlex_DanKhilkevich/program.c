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

//void printSportsmen(sportsman* sportsmen_array, int num_sportsmen)  //פונקצית עזר הדפסת מערך ספורטאים
//{
//    for (int i = 0; i < num_sportsmen; ++i) {
//        printf("Sportsman %d:\n", sportsmen_array[i].id);
//        printf("First Name: %s\n", sportsmen_array[i].Fname);
//        printf("Last Name: %s\n", sportsmen_array[i].Lname);
//        printf("Club: %s\n", sportsmen_array[i].p2club);
//        printf("Gender: %d\n", sportsmen_array[i].gen);
//        printf("Number of Events: %d\n", sportsmen_array[i].Nevents);
//        printf("Events:\n");
//        for (int j = 0; j < sportsmen_array[i].Nevents; ++j) {
//            printf("- %s, %s, %d\n", sportsmen_array[i].p2events[j].p2title, sportsmen_array[i].p2events[j].p2location, sportsmen_array[i].p2events[j].year);
//        }
//        printf("\n");
//    }
//}


int countLines(const char* filename)
{
    char line[100];
    FILE* file_ptr = fopen(filename, "r");

    if (file_ptr == NULL)
    {
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
        (*sportsmen_array)[sportmen_count].Nevents = 0;
        (*sportsmen_array)[sportmen_count].p2events = NULL;
        // dynamically allocate memory to p2club based on the length of clubName
        (*sportsmen_array)[sportmen_count].p2club = malloc((strlen(clubName) + 1) * sizeof(char));
        if ((*sportsmen_array)[sportmen_count].p2club == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        strcpy((*sportsmen_array)[sportmen_count].p2club, clubName);
        free((*sportsmen_array)[sportmen_count].p2club);
        sportmen_count++;
    }
    fclose(file);
}


void FromFile2Events(const char* filename, sportsman* sportsmen_array, int num_sportsmen)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error can not opening the file %s\n", filename);
        return;
    }

    char line[100]; // assuming maximum line length is 100 characters
    int sportsman_id, year;
    char event_name[30], location[30];
    while (fgets(line, sizeof(line), file) != NULL)  // read each line to get [sportsman id, event name, location, and year]
    {
        sscanf(line, "%d,%[^,],%[^,],%d", &sportsman_id, event_name, location, &year);
        sportsman* current_sportsman = NULL;
        for (int i = 0; i < num_sportsmen; i++)
        {
            if (sportsmen_array[i].id == sportsman_id)  // Found the matching sportman in the sportman array
            {
                current_sportsman = &sportsmen_array[i];
                break;
            }
        }

        // we know that sportsman is found, now we add the event to their list of events
        if (current_sportsman != NULL)
        {
            // Reallocate memory for the events array (event arr is daynamic array)
            current_sportsman->p2events = realloc(current_sportsman->p2events, (current_sportsman->Nevents + 1) * sizeof(event));
            if (current_sportsman->p2events == NULL)
            {
                printf("Memory allocation failed.\n");
                exit(1);
            }

            // Assign event details
            event* current_event = &(current_sportsman->p2events[current_sportsman->Nevents]);
            current_event->p2title = (char*)malloc(strlen(event_name) + 1); //the title of the event is dymamic
            if (current_event->p2title == NULL)
            {
                printf("Memory allocation failed.\n");
                exit(1);
            }
            strcpy(current_event->p2title, event_name); //replace the address of p2title with value (the title)

            current_event->p2location = (char*)malloc(strlen(location) + 1); //the location of the event is dymamic
            if (current_event->p2location == NULL)
            {
                printf("Memory allocation failed.\n");
                exit(1);
            }
            strcpy(current_event->p2location, location); //replace address of p2location with value (the location)
            current_event->year = year; // set value
            current_sportsman->Nevents++;  // add 1 to the number of events for this specific sportsman. 
        }
    }
    fclose(file); //close EventData.txt file
}

int main()
{
    int num_sportsmen = countLines("SportsmanData.txt"); //number of sportmens - taken from SportsmanData file. 
    sportsman* sportsmen_array = NULL; //pointer to sportsmen array right now its pointer to null
    FromFile2Sportsman("SportsmanData.txt", &sportsmen_array, num_sportsmen); //get the sportsman array. 
    FromFile2Events("EventData.txt", sportsmen_array, num_sportsmen); //insert array of events for each sportsman

    for (int sportman_index = 0; sportman_index < num_sportsmen; sportman_index++)  //print for each sportsman his participated events. //running on sportsman array
    {
        printf("%s %s events:\n", sportsmen_array[sportman_index].Fname, sportsmen_array[sportman_index].Lname);
        for (int event_index = 0; event_index < sportsmen_array[sportman_index].Nevents; event_index++) // running on events array 
        {
            printf("(-) %s, %s, %d\n", sportsmen_array[sportman_index].p2events[event_index].p2title, sportsmen_array[sportman_index].p2events[event_index].p2location, sportsmen_array[sportman_index].p2events[event_index].year);
        }
    }
    // Free allocated memory
    for (int i = 0; i < num_sportsmen; i++)
    {
        for (int j = 0; j < sportsmen_array[i].Nevents; j++)
        {
            free(sportsmen_array[i].p2events[j].p2title);
            free(sportsmen_array[i].p2events[j].p2location);
        }
        free(sportsmen_array[i].p2events);
    }
    free(sportsmen_array);
    printf("hey");
    return 0;
}
