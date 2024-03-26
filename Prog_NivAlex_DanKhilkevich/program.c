#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH  30

//event structure
typedef struct
{
    char* p2title;  //event name  
    char* p2location;  //event location  
    int year;   //event year 
} event;

//sportsman structure
typedef struct {
    int id; // id of sportsman
    char Fname[10]; //first name 
    char Lname[15]; // last name
    char* p2club; //pointer to sportsman club
    int gen; //gender 
    event* p2events; //pointer to a dynamic arr of events
    int Nevents; //number of participated events
}sportsman;

//printSportmen function gets array of sportsmen and number of sportsmen
// the function prints the details(id,first name,last name, club, gender, Nevents, P2events) for each sportsmen
void printSportsmen(sportsman* sportsmen_array, int num_sportsmen)
{
    for (int i = 0; i < num_sportsmen; ++i) {
        printf("Sportsman %d:\n", sportsmen_array[i].id);
        printf("First Name: %s\n", sportsmen_array[i].Fname);
        printf("Last Name: %s\n", sportsmen_array[i].Lname);

        // Check if p2club is not NULL before printing
        if (sportsmen_array[i].p2club != NULL) {
            printf("Club: %s\n", sportsmen_array[i].p2club);
        }
        else {
            printf("Club: Not specified\n");
        }

        printf("Gender: %d\n", sportsmen_array[i].gen);
        printf("Number of Events: %d\n", sportsmen_array[i].Nevents);
        printf("Events:\n");

        // Check if p2events is not NULL before accessing
        if (sportsmen_array[i].p2events != NULL) {
            for (int j = 0; j < sportsmen_array[i].Nevents; ++j) {
                printf("- Event %d: %s, %s, %d\n", j + 1, sportsmen_array[i].p2events[j].p2title,
                    sportsmen_array[i].p2events[j].p2location, sportsmen_array[i].p2events[j].year);
            }
        }
        else {
            printf("No events registered\n");
        }
        printf("\n");
    }
} //

//print_Sportman_participated_events gets array of sportsmen and number of sportsmen
// the function prints for each sports men the events that he is participated. 
void print_Sportman_participated_events(sportsman* sportsmen_array, int num_sportsmen) 
{
    for (int sportman_index = 0; sportman_index < num_sportsmen; sportman_index++) {
        printf("%s %s events:\n", sportsmen_array[sportman_index].Fname, sportsmen_array[sportman_index].Lname);

        // Check if the sportsman has any events
        if (sportsmen_array[sportman_index].Nevents == 0 || sportsmen_array[sportman_index].p2events == NULL) {
            printf("No events participated.\n");
            continue; // Move to the next sportsman
        }

        // Print each event the sportsman participated in
        for (int event_index = 0; event_index < sportsmen_array[sportman_index].Nevents; event_index++) {
            printf("(-) %s, %s, %d\n",
                sportsmen_array[sportman_index].p2events[event_index].p2title,
                sportsmen_array[sportman_index].p2events[event_index].p2location,
                sportsmen_array[sportman_index].p2events[event_index].year);
        }
    }
}


//countLines function gets FileName and return the number of lines (without count the first line)
int countLines(const char* filename)
{
    int number_of_lines = 0; // we don't want to count the first line of the file. 
    int ch;
    char line[MAX_LINE_LENGTH];

    FILE* file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
    {
        printf("Error, can not opening the file\n");
        return -1;

    }
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

//FromFile2Sportsman reads data from "SportsmanData.txt" File and create new sportmen_array with this data.
//the function gets a pointer to sportsmen_array and the sportsmen_array_size and because it "void" function we worked by reference. 
void FromFile2Sportsman(const char* filename, sportsman** sportsmen_array, int sportsmen_array_size)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    *sportsmen_array = malloc(sportsmen_array_size * sizeof(sportsman)); // allocate memory for sportsmen array;
    if (*sportsmen_array == NULL) // allocate memory checking
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    char line[100];

    fgets(line, sizeof(line), file); // skip the format line (the first line)
    int sportsmen_count = 0;

    while (fgets(line, sizeof(line), file) != NULL && sportsmen_count < sportsmen_array_size) // read line by line from the file
    {
        int gen;
        char clubName[100]; // temporary buffer for club name, club name = maximum of 100 chars

        if (sscanf(line, "%d;%[^;];%[^;];%[^;];%d", &((*sportsmen_array)[sportsmen_count].id),
            (*sportsmen_array)[sportsmen_count].Fname,
            (*sportsmen_array)[sportsmen_count].Lname, clubName, &gen) != 5)
        {
            printf("Error reading data from file\n");
            exit(1);
        }
        (*sportsmen_array)[sportsmen_count].gen = gen;
        (*sportsmen_array)[sportsmen_count].Nevents = 0;
        (*sportsmen_array)[sportsmen_count].p2events = NULL;

        // Dynamically allocate memory to p2club based on the length of clubName
        (*sportsmen_array)[sportsmen_count].p2club = malloc((strlen(clubName) + 1) * sizeof(char));
        if ((*sportsmen_array)[sportsmen_count].p2club == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        strcpy((*sportsmen_array)[sportsmen_count].p2club, clubName);

        sportsmen_count++;
    }
    fclose(file);
}

//FromFile2Events reads data from "EeventData.txt" File and create new events_array with this data.
//the function gets a pointer to sportsmen_array and the sportsmen_array_size and because it "void" function we worked by reference. 
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

//check_if_sportsman_already_exists function gets pointer to sportsmen_array, size,new_sportman_id and check if this sportman
// already_exists in the sportsmen_array. if it exists the function return 0 and if not the function return 1; 
int check_if_sportsman_already_exists(sportsman* sportsmen_array[], int new_sportman_id, int sportsmen_array_size)
{
    // Check if the sportsman already exists in the array
    for (int i = 0; i < sportsmen_array_size; i++)
    {
        if ((*sportsmen_array)[i].id == new_sportman_id)
        {
            printf("Error: Sportsman with ID %d already exists.\n", new_sportman_id);
            return 0; // The sportsman already exists in the array
        }
    }
    return 1; // The sportsman does not exist in the array
}

//addSportman function gets pointer to sportmen_arr and pointer to sportsmen_array_size
// the function add new sportmen to the array and change his size. 
// note: sportsmen_array is a dynamic array and sportsmen_array_size is a dynamic size so we worked with pointers.  
int addSportsman(sportsman* sportsmen_array[], int* sportsmen_array_size)
{
    int new_sportman_id;
    char club_name[50];

    printf("Please enter the new sportsman details:\n");
    printf("Enter the new sportsman ID: ");
    scanf("%d", &new_sportman_id);
    if (check_if_sportsman_already_exists(sportsmen_array, new_sportman_id, *sportsmen_array_size) == 0) // Check if the sportsman already exists
        return 0; // The sportsman already exists in the array, return 0

    (*sportsmen_array_size)++; // add +1 to the size of the sportsmen array

    sportsman* temp = realloc(*sportsmen_array, (*sportsmen_array_size) * sizeof(sportsman)); // realloc memory for the new sportsmen array
    if (temp == NULL) 
    {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }
    *sportsmen_array = temp;

    // insert the details of the new sportsman
    printf("Enter the new sportsman First name: ");
    scanf("%s", (*sportsmen_array)[*sportsmen_array_size - 1].Fname);
    printf("Enter the new sportsman Last name: ");
    scanf("%s", (*sportsmen_array)[*sportsmen_array_size - 1].Lname);
    printf("Enter the new sportsman Club name: ");
    getchar();
    fgets(club_name, sizeof(club_name), stdin);
    club_name[strcspn(club_name, "\n")] = '\0'; // Remove trailing newline character

    // malloc memory for the club name and copy the string instead of address. 
    (*sportsmen_array)[*sportsmen_array_size - 1].p2club = malloc((strlen(club_name) + 1) * sizeof(char));
    if ((*sportsmen_array)[*sportsmen_array_size - 1].p2club == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }
    strcpy((*sportsmen_array)[*sportsmen_array_size - 1].p2club, club_name);
    printf("Enter the new sportsman Gender (1 for male or 0 for female): ");
    if (scanf("%d", &(*sportsmen_array)[*sportsmen_array_size - 1].gen) != 1) {
        printf("Error: Invalid input for gender.\n"); //the number is not 0/1
        return 0;
    }

    (*sportsmen_array)[*sportsmen_array_size - 1].id = new_sportman_id;
    (*sportsmen_array)[*sportsmen_array_size - 1].Nevents = 0;
    (*sportsmen_array)[*sportsmen_array_size - 1].p2events = NULL;

    return 1; // successed to add new sportman to the array
}


sportsman* find_Sportsman_ByID(int id, sportsman* sportsmen_array[], int* sportsmen_array_size)
{
    if (sportsmen_array == NULL || sportsmen_array_size == NULL) {
        return NULL;
    }

    for (int i = 0; i < *sportsmen_array_size; i++)
    {
        sportsman* current_sportsman = sportsmen_array[i];
        if (current_sportsman == NULL) {
            // Handle NULL pointer in the array
            continue;
        }

        if (current_sportsman->id == id)
        {
            return current_sportsman; // Found, return the sportsman
        }
    }

    return NULL; // Sportsman with the given ID not found
} // Function to find a sportsman by ID


void freeEventsMemory(sportsman* sportsmen_array, int sportsmen_array_size) 
{
    for (int i = 0; i < sportsmen_array_size; i++) {
        for (int j = 0; j < sportsmen_array[i].Nevents; j++) {
            free(sportsmen_array[i].p2events[j].p2title);
        }
        free(sportsmen_array[i].p2events);
    }
}


void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int isEventExist(event* events_array, int num_events, const char* event_name, const char* location, int year)
{
    char current_event_name[50];
    char current_event_location[50];
    int current_event_year;

    for (int i = 0; i < num_events; i++)
    {
        if (events_array[i].p2title == NULL || events_array[i].p2location == NULL)
        {
            printf("Error: Null pointer encountered.\n");
            continue; // move to the next event
        }

        strcpy(current_event_name, events_array[i].p2title);
        strcpy(current_event_location, events_array[i].p2location);
        current_event_year = events_array[i].year;
        if (strcmp(current_event_name, event_name) == 0 &&
            strcmp(current_event_location, location) == 0 &&
            current_event_year == year)
        {
            return 1; // event already exists
        }
    }
    return 0; // event does not exist
}


int addEvent(sportsman* sportsmen_array, int sportsman_id, int* sportmen_arr_size) {
    char event_name[30], location[30];
    int year;

    // Prompt user for event details
    clearInputBuffer();
    printf("Enter event name: ");
    fgets(event_name, sizeof(event_name), stdin);
    event_name[strcspn(event_name, "\n")] = '\0'; // Remove newline character

    printf("\nEnter event location: ");
    fgets(location, sizeof(location), stdin);
    location[strcspn(location, "\n")] = '\0'; // Remove newline character

    printf("\nEnter event year: ");
    scanf("%d", &year);
    clearInputBuffer(); // Clear input buffer to remove any leftover newline characters

    // Find the index of the sportsman in the sportsmen array
    int sportsman_index = -1;
    for (int i = 0; i < sportmen_arr_size; i++) {
        if (sportsmen_array[i].id == sportsman_id) {
            sportsman_index = i;
            break;
        }
    }

    // If the sportsman is not found, return with an error
    if (sportsman_index == -1) {
        printf("Error: Sportsman with ID %d not found.\n", sportsman_id);
        return 0;
    }

    // Check if the event already exists
    if (isEventExist(sportsmen_array[sportsman_index].p2events, sportsmen_array[sportsman_index].Nevents, event_name, location, year) == 1) {
        printf("Event already exists!!!\n");
        return 0; // Event already exists
    }

    // Reallocate memory for the events array of the sportsman
    sportsmen_array[sportsman_index].p2events = realloc(sportsmen_array[sportsman_index].p2events, (sportsmen_array[sportsman_index].Nevents + 1) * sizeof(event));
    if (sportsmen_array[sportsman_index].p2events == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 0; // Failed to allocate memory
    }

    // Allocate memory for the new event details
    sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2title = malloc((strlen(event_name) + 1) * sizeof(char));
    if (sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2title == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 0; // Failed to allocate memory
    }

    sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2location = malloc((strlen(location) + 1) * sizeof(char));
    if (sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2location == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2title);
        return 0; // Failed to allocate memory
    }

    // Copy event details
    strcpy(sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2title, event_name);
    strcpy(sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2location, location);
    sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].year = year;

    // Increment the number of events
    sportsmen_array[sportsman_index].Nevents++;

    return 1; // Event added successfully
}


int printMenu()
{
    int choice = 0; // Initialize choice variable

    // Display the menu options
    printf("Choose an option:\n");
    printf("(0) Exit\n");
    printf("(1) Add a new sportsman\n");
    printf("(2) Add a new event\n");
    printf("(4) print sportman array\n");
    printf("(5) print sportman particpated events\n");
    printf("(6) print  specific sportman events\n");
    printf("(7) print  the number os sportman's the particpated on specific event\n");
    // Prompt the user to enter their choice
    printf("Enter your choice:");
    scanf("%d", &choice);

    // Clear input buffer
    while (getchar() != '\n');

    return choice; // Return the user's choice
}


void Write_array_to_Sportsman_data(const char* filename, sportsman* sportsmen_array, int num_sportsmen)
{
    FILE* file_source = fopen("SportsmanData.txt", "r");
    FILE* file_dest = fopen("SportsmanData.txt", "w");
    if (file_source == NULL)
    {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }
    fprintf(file_dest, "format:sportsman_id;first_name;last_name;club_name;gender\n"); // Write the format line
    for (int i = 0; i < num_sportsmen; i++) // Write each sportsman's information to the file
    {
        fprintf(file_dest, "%d;%s;%s;%s;%d", sportsmen_array[i].id, sportsmen_array[i].Fname,
            sportsmen_array[i].Lname, sportsmen_array[i].p2club,
            sportsmen_array[i].gen);
        if (i != num_sportsmen - 1) //to avoid blank line in the end of the file 
        {
            fprintf(file_dest, "\n");
        }
    }
    file_source = file_dest;
    fclose(file_source);
    fclose(file_dest);
    printf("Array successfully written to %s.\n", filename);
}


void Write_array_to_Event_Data(const char* filename, sportsman* sportsmen_array, int num_sportsmen)
{
    FILE* file_source = fopen(filename, "r");
    FILE* file_dest = fopen(filename, "w");
    if (file_dest == NULL)
    {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    // Write each event information to the file
    for (int sportman_index = 0; sportman_index < num_sportsmen; sportman_index++)
    {
        for (int event_index = 0; event_index < sportsmen_array[sportman_index].Nevents; event_index++)
        {
            fprintf(file_dest, "%d,%s,%s,%d\n",
                sportsmen_array[sportman_index].id,
                sportsmen_array[sportman_index].p2events[event_index].p2title,
                sportsmen_array[sportman_index].p2events[event_index].p2location,
                sportsmen_array[sportman_index].p2events[event_index].year);
        }
    }
    fclose(file_source);
    fclose(file_dest);
    printf("Events successfully written to %s.\n", filename);
}


sportsman* getSportsmanByID(int sportsman_id, sportsman* sportsmen_array, int num_sportsmen)
{
    // Iterate through the sportsmen array to find the sportsman with the given ID
    for (int i = 0; i < num_sportsmen; i++) {
        if (sportsmen_array[i].id == sportsman_id) {
            // Return a pointer to the found sportsman
            return &sportsmen_array[i];
        }
    }
    // If the sportsman with the given ID is not found, return NULL
    return NULL;
}


// function to remove leading and trailing whitespace characters from a string
void trim(char* str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start])) {
        start++;
    }
    while (end > start && isspace(str[end])) {
        end--;
    }
    str[end + 1] = '\0'; // Null-terminate the trimmed string
    if (start > 0) {
        // Shift the trimmed string to the beginning of the original string
        memmove(str, str + start, end - start + 2);
    }
}

sportsman* get_sportsman_by_lastname(char sportsman_last_name[], sportsman* sportsmen_array, int* num_sportsmen)
{
    trim(sportsman_last_name);
    // Search for the sportsman with the given last name in the array
    for (int i = 0; i < num_sportsmen; i++) {
        // Compare the trimmed input last name with the last name of each sportsman in the array
        if (strcmp(sportsman_last_name, sportsmen_array[i].Lname) == 0) {
            return &sportsmen_array[i]; // Return a pointer to the found sportsman
        }
    }
    return NULL; // Return NULL if the sportsman with the given last name is not found
}

int printEvents(char sportsman_last_name[], sportsman* sportsmen_array, int* num_sportsmen)
{
    // Call helper function to get sportsman struct by last name
    sportsman* found_sportsman = get_sportsman_by_lastname(sportsman_last_name, sportsmen_array, &num_sportsmen);

    // If sportsman with the given last name is found
    if (found_sportsman != NULL)
    {
        if (found_sportsman->p2events == NULL) // Check if the sportsman has no events
        {
            printf("No events found for %s %s.\n", found_sportsman->Fname, found_sportsman->Lname);
            return 0; // Return 1 to indicate no events found
        }
        else
        {
            printf("Events for %s %s:\n", found_sportsman->Fname, found_sportsman->Lname);
            // Print each event for the found sportsman
            for (int j = 0; j < found_sportsman->Nevents; j++)
            {
                printf("(-) %s, %s, %d\n", found_sportsman->p2events[j].p2title,
                    found_sportsman->p2events[j].p2location, found_sportsman->p2events[j].year);
            }
        }
    }
    else
    {
        return -1; // Return -1 to indicate sportsman not found
    }
    return 1; // Return 0 to indicate success
}

// help function to trim leading and trailing spaces from a string this is for countEvent function
void trim1(char* str) {
    // Trim leading spaces
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // If the string is now empty, return
    if (*str == '\0') {
        return;
    }

    // Trim trailing spaces
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';
}

// function to count the number of sportsmen who participated in event E in the year Y
int countEvent(const char E[], int Y, sportsman* sportsmen_array, int* num_sportsmen) {
    int count = 0;

    // Trim event name
    char trimmed_event_name[50];
    strcpy(trimmed_event_name, E);
    trim1(trimmed_event_name);

    // Iterate over each sportsman in the array
    for (int i = 0; i < *(num_sportsmen); i++) {
        sportsman current_sportsman = sportsmen_array[i];

        // Check if the current sportsman has participated in the event in the specified year
        for (int j = 0; j < current_sportsman.Nevents; j++) {
            // Trim event title from the current sportsman's events
            char trimmed_event_title[50];
            strcpy(trimmed_event_title, current_sportsman.p2events[j].p2title);
            trim1(trimmed_event_title);

            if (strcmp(trimmed_event_title, trimmed_event_name) == 0 && current_sportsman.p2events[j].year == Y) {
                count++;
                break;  // No need to continue checking events for this specific sportsman
            }
        }
    }

    return count;
}

void main()
{
    char sportsman_last_name[MAX_NAME_LENGTH];
    char specific_event_name[MAX_NAME_LENGTH];
    int specific_event_year;
    int num_sportsmen = countLines("SportsmanData.txt"); //number of sportmens - taken from SportsmanData file. 
    sportsman* sportsmen_array = NULL; //pointer to sportsmen array
    FromFile2Sportsman("SportsmanData.txt", &sportsmen_array, num_sportsmen); //get the sportsman array 
    FromFile2Events("EventData.txt", sportsmen_array, num_sportsmen); //insert array of events for each sportsman
    int choice = 0; // Initialize choice variable
    while (1) // Infinite loop until the user chooses to exit
    {
        choice = printMenu(); // Print menu and get user choice
        if (choice == 0) // Exit if user chooses 0
        {
            printf("Exiting!\n");
            exit(0);
        }
        else if (choice == 1) // Add a new sportsman
        {
            if (addSportsman(&sportsmen_array, &num_sportsmen) == 1)
            {
                printf("Sportsman added successfully.\n");
                Write_array_to_Sportsman_data("SportsmanData.txt", sportsmen_array, num_sportsmen);
            }
            else
            {
                printf("Failed to add sportsman.\n");
            }
        }
        else if (choice == 2) // Add new event
        {
            printf("Enter the sportsman ID for whom you want to add an event: ");
            int selected_id;
            scanf("%d", &selected_id);
            if (addEvent(sportsmen_array,selected_id,&num_sportsmen) == 1)
            {
                printf("Event added successfully.\n");
                Write_array_to_Event_Data("EventData.txt", sportsmen_array, num_sportsmen);
            }
            else
            {
                printf("Failed to add event.\n");
            }
        }

        else if (choice == 4) // Handle other options
        {
            printSportsmen(sportsmen_array, num_sportsmen);
        }
        else if (choice == 5) // Handle other options
        {
            print_Sportman_participated_events(sportsmen_array, num_sportsmen);
        }
        else if (choice == 6) 
        {
            printf("Enter the last name of the sportsman:");
            scanf("%s", sportsman_last_name);
            while (getchar() != '\n'); // Clear the input buffer
            int result = printEvents(sportsman_last_name, sportsmen_array, &num_sportsmen);
            if (result == 0) 
            {
                printf("events list is empty for this sportsman.\n");
            }
            else if (result == -1) {
                printf("There is no sportman with this last name\n");
            }
            else {
                printf("Events printed successfully\n");
            }
        }
        else if (choice == 7) // Handle other options
        {
            printf("Enter the specific event name:");
            fgets(specific_event_name, sizeof(specific_event_name), stdin);
            specific_event_name[strcspn(specific_event_name, "\n")] = '\0'; // Remove the newline character
            printf("Enter the specific event year:");
            scanf("%d", &specific_event_year);
            while (getchar() != '\n'); // Clear the input buffer
            printf("The number of sportsman who participated in %s in %d is: %d\n ", specific_event_name, specific_event_year, countEvent(specific_event_name, specific_event_year, sportsmen_array, &num_sportsmen));
        }
        else // Invalid choice
        {
            printf("Invalid choice. Please enter a number between 0 and 7.\n");
        }

    }



    // Free allocated memory - At the end of the program!!
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
}
