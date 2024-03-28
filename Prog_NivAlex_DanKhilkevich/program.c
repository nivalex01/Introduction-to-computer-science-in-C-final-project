#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH  30
#define MAX_CLUB_NUMBER 100
#define MAX_EVENT_NUMBER 100
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

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
        printf("Sportsman ID %d:\n", sportsmen_array[i].id);
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
    char line[MAX_LINE_LENGTH];
    int sportsmen_count = 0;
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
    fgets(line, sizeof(line), file); // skip the format line (the first line)
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

    char line[MAX_LINE_LENGTH]; // assuming maximum line length is 100 characters
    int sportsman_id, year;
    char event_name[MAX_NAME_LENGTH], location[MAX_NAME_LENGTH];
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

//capitalize_each_First_Letter get a string and capitalize the first letter of each word
void capitalize_each_First_Letter(char* str) {
    int i;

    // Capitalize first letter of the string
    if (str[0] != '\0') {
        str[0] = toupper(str[0]);
    }

    // Capitalize first letter of each word
    for (i = 1; str[i] != '\0'; i++) {
        if (isspace(str[i - 1]) && isalpha(str[i])) {
            str[i] = toupper(str[i]);
        }
    }
}


int is_number(const char* str) {
    // Check if each character is a digit
    for (int i = 0; str[i] != '\0'; i++) {
        // Check if the character is not between '0' and '9'
        if (str[i] < '0' || str[i] > '9') 
        {
            return 1; //the str is not a number;
        }
    }
    return 0; //the str is number 
}

//addSportman function gets pointer to sportmen_arr and pointer to sportsmen_array_size
// the function add new sportmen to the array and change his size. 
// note: sportsmen_array is a dynamic array and sportsmen_array_size is a dynamic size so we worked with pointers.  
int addSportsman(sportsman* sportsmen_array[], int* sportsmen_array_size)
{
    int new_sportman_id;
    char club_name[MAX_NAME_LENGTH];

    printf("Please enter the new sportsman details:\n");
    while (1) 
    {
        printf("Enter the new sportsman ID: ");
        if (scanf("%d", &new_sportman_id) != 1) 
        {
            printf("Error: Invalid input for ID. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
        }
        else 
        {
            break; // Exit the loop if a valid ID is provided
        }
    }

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
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    //char club_name[MAX_NAME_LENGTH];
    // insert the details of the new sportsman
    while (1) 
    {
        printf("Enter the new sportsman First name: ");
        scanf("%s", first_name);

        if (is_number(first_name)==0) 
        {
            printf("You entered a number instead of a name. Please try again.\n");
        }
        else 
        {
            // Copy the first name to the Fname field of the last sportsman in the array
            strcpy((*sportsmen_array)[*sportsmen_array_size - 1].Fname, first_name);
            break;
        }
    }
    capitalize_each_First_Letter((*sportsmen_array)[*sportsmen_array_size - 1].Fname); // capitalize first letter of each word


    while (1)
    {
        printf("Enter the new sportsman Last name: ");
        scanf("%s", last_name);

        if (is_number(last_name) == 0)
        {
            printf("You entered a number instead of a name. Please try again.\n");
        }
        else
        {
            // Copy the first name to the Fname field of the last sportsman in the array
            strcpy((*sportsmen_array)[*sportsmen_array_size - 1].Lname, last_name);
            break;
        }
    }

    capitalize_each_First_Letter((*sportsmen_array)[*sportsmen_array_size - 1].Lname); // capitalize first letter of each word
    printf("Enter the new sportsman Club name: ");
    getchar();
    fgets(club_name, sizeof(club_name), stdin);
    club_name[strcspn(club_name, "\n")] = '\0'; // Remove trailing newline character
    capitalize_each_First_Letter(club_name);
    // malloc memory for the club name and copy the string instead of address. 
    (*sportsmen_array)[*sportsmen_array_size - 1].p2club = malloc((strlen(club_name) + 1) * sizeof(char));
    if ((*sportsmen_array)[*sportsmen_array_size - 1].p2club == NULL) 
    {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }
    strcpy((*sportsmen_array)[*sportsmen_array_size - 1].p2club, club_name);
    int gender_input;
    do 
    {
        printf("Enter the new sportsman Gender (1 for male or 0 for female): ");
        if (scanf("%d", &gender_input) != 1) //check if the input is not a number (the input is string)
        {
            printf("Error: Invalid input for gender.\n"); //the user entered string instead of a number
            while (getchar() != '\n'); // Clear input buffer
        }
        //the user enterd number, we need to check if the number is only 1 or 0
        else if (gender_input != 0 && gender_input != 1)
        {
            printf("Error: Gender should be 0 (female) or 1 (male).\n"); //the number is not 1 or 0
        }
        else //valid input
        {
            (*sportsmen_array)[*sportsmen_array_size - 1].gen = gender_input;
            break; // good input,  now we can exit the loop
        }
    } while (1); // running until we get good input

    (*sportsmen_array)[*sportsmen_array_size - 1].id = new_sportman_id;
    (*sportsmen_array)[*sportsmen_array_size - 1].Nevents = 0;
    (*sportsmen_array)[*sportsmen_array_size - 1].p2events = NULL;

    return 1; // successed to add the new sportman to the array
}

//find_Sportsman_ByID gets id number, sportsmen_array and sportsmen_array_size
//the function return sportman struct with the specific id it get. 
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

//freeEventsMemory function release memory for each event in the event arrary

//////// !!!!!!!!!!need to check if i use it!!!!!!!!!!!!!! /////////
void freeEventsMemory(sportsman* sportsmen_array, int sportsmen_array_size) 
{
    for (int i = 0; i < sportsmen_array_size; i++) {
        for (int j = 0; j < sportsmen_array[i].Nevents; j++) {
            free(sportsmen_array[i].p2events[j].p2title);
        }
        free(sportsmen_array[i].p2events);
    }
}

//clearInputBuffer clear the buffers for each word (spaces)
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//isEventExist function get and array of events, number of events, event name,location and year
//the function return 1 if the event is already exists on the events list or 0 if its not already exists
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


int does_string_has_number(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) 
    {
        if (str[i] > 48 && str[i] < 57) 
        {
            return 1; // Number found
        }
    }
    return 0; // No number found
}
//addEvent function gets sportman_array, sportman_id and sportman_array size
//the function add new event to this sportsman
int addEvent(sportsman* sportsmen_array, int sportsman_id, int* sportmen_arr_size) {
    char event_name[MAX_NAME_LENGTH], location[MAX_NAME_LENGTH];
    int year;

    // Prompt user for event details
    clearInputBuffer();
    while (1) {
        printf("Enter the new event name: ");
        fgets(event_name, sizeof(event_name), stdin);

        // Remove newline character if present
        if (event_name[strlen(event_name) - 1] == '\n') {
            event_name[strlen(event_name) - 1] = '\0';
        }

        if (does_string_has_number(event_name) == 1) {
            printf("You entered a number instead of a name. Please try again.\n");
        }
        else {
            break;
        }
    }
    capitalize_each_First_Letter(event_name);
    event_name[strcspn(event_name, "\n")] = '\0'; // Remove newline character

    while (1) 
    {
        printf("Enter the new event location: ");
        fgets(location, sizeof(location), stdin);

        // Remove newline character if present
        if (location[strlen(location) - 1] == '\n') {
            event_name[strlen(location) - 1] = '\0';
        }

        if (does_string_has_number(location) == 1) {
            printf("You entered a number instead of a name. Please try again.\n");
        }
        else {
            break;
        }
    }
    capitalize_each_First_Letter(location);
    location[strcspn(location, "\n")] = '\0'; // Remove newline character


    while (1)
    {
        printf("enter the event year:");
        if (scanf("%d", &year) != 1)
        {
            printf("Error: Invalid input for event year. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
        }
        else
        {
            break; // Exit the loop if a valid ID is provided
        }
    }
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
        printf("Event already exists!!!\n\n");
        return 0; // Event already exists
    }

    // Reallocate memory for the events array of the sportsman
    sportsmen_array[sportsman_index].p2events = realloc(sportsmen_array[sportsman_index].p2events, (sportsmen_array[sportsman_index].Nevents + 1) * sizeof(event));
    if (sportsmen_array[sportsman_index].p2events == NULL) {
        printf("Error: Memory allocation failed.\n\n");
        return 0; // Failed to allocate memory
    }

    // Allocate memory for the new event details
    sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2title = malloc((strlen(event_name) + 1) * sizeof(char));
    if (sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2title == NULL) {
        printf("Error: Memory allocation failed.\n\n");
        return 0; // Failed to allocate memory
    }

    sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2location = malloc((strlen(location) + 1) * sizeof(char));
    if (sportsmen_array[sportsman_index].p2events[sportsmen_array[sportsman_index].Nevents].p2location == NULL) {
        printf("Error: Memory allocation failed.\n\n");
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

//print menu function
int printMenu()
{
    int choice = 0; // Initialize choice variable

    // Display the menu options
    printf("Choose an option:\n");
    printf("(0) Exit\n");
    printf("(1) Add a new sportsman\n");
    printf("(2) Add a new event\n");
    printf("(3) print sportman array\n");
    printf("(4) print sportman particpated events\n");
    printf("(5) print specific sportman events\n");
    printf("(6) print the number of sportman's that particpated on specific event\n");
    printf("(7) print the best club\n");
    printf("(8) Check the same event\n");
    printf("(9) Print sorted events\n");
    printf("(10) Delete an event\n");
    // Prompt the user to enter their choice
    printf("Enter your choice:");
    scanf("%d", &choice);

    // Clear input buffer
    while (getchar() != '\n');

    return choice; // Return the user's choice
}

//Write_array_to_Sportsman_data function gets file_name, sportsmen_array, and num_sportsmen
//the function writes the updated array of sportsmen to the file "SportsmanData.txt"
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
        if (i != num_sportsmen - 1) //to avoid blank line in the end of the file. we want to write the last one without new line
        {
            fprintf(file_dest, "\n");
        }
    }
    file_source = file_dest;
    fclose(file_source);
    fclose(file_dest);
    printf("Array successfully written to %s.\n", filename);
}

//Write_array_to_event_data function gets file_name, sportsmen_array, and num_sportsmen
//the function writes the updated array of events to the file "eventData.txt"
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


char* getSportClubName(char* p2club) {
    // Find the length of the club name
    size_t length = strlen(p2club);

    // Allocate memory for the club name without the first and last spaces
    char* club_name = (char*)malloc(length + 1); // Plus one for null terminator
    if (club_name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Find the index of the first non-space character
    size_t start = 0;
    while (p2club[start] == ' ') {
        start++;
    }

    // Find the index of the last non-space character
    size_t end = length - 1;
    while (end > 0 && p2club[end] == ' ') {
        end--;
    }

    // Copy the club name without the leading and trailing spaces
    size_t j = 0;
    for (size_t i = start; i <= end; i++) {
        club_name[j++] = p2club[i];
    }
    club_name[j] = '\0'; // Null-terminate the string

    return club_name;
}

void bestClub(sportsman* sportsmen_array, int* num_sportsmen) 
{
    typedef struct {
        char* Club_Name;
        int number_of_events;
    } ClubEvents;

    // Array to store club names and their corresponding event counts
    ClubEvents array_of_clubs_and_their_events[MAX_CLUB_NUMBER];
    for (int i = 0; i < MAX_CLUB_NUMBER; i++) {
        array_of_clubs_and_their_events[i].Club_Name = NULL;
        array_of_clubs_and_their_events[i].number_of_events = 0;
    }

    // Iterate through each sportsman and count their events for each club
    for (int i = 0; i < *num_sportsmen; i++) {
        sportsman current_sportsman = sportsmen_array[i];
        char* club_name = getSportClubName(current_sportsman.p2club);
        int found = 0;
        for (int j = 0; j < MAX_CLUB_NUMBER; j++) {
            if (array_of_clubs_and_their_events[j].Club_Name != NULL &&
                strcmp(array_of_clubs_and_their_events[j].Club_Name, club_name) == 0) {
                array_of_clubs_and_their_events[j].number_of_events += current_sportsman.Nevents;
                found = 1;
                break;
            }
            else if (array_of_clubs_and_their_events[j].Club_Name == NULL) {
                array_of_clubs_and_their_events[j].Club_Name = club_name;
                array_of_clubs_and_their_events[j].number_of_events += current_sportsman.Nevents;
                found = 1;
                break;
            }
        }
        if (!found) {
            free(club_name); // Free allocated memory if not used
        }
    }

    // Find the club with the maximum number of events
    char* best_club_name = NULL;
    int max_events = 0;
    for (int i = 0; i < MAX_CLUB_NUMBER; i++) {
        if (array_of_clubs_and_their_events[i].Club_Name != NULL &&
            array_of_clubs_and_their_events[i].number_of_events > max_events) {
            best_club_name = array_of_clubs_and_their_events[i].Club_Name;
            max_events = array_of_clubs_and_their_events[i].number_of_events;
        }
    }
    printf("The club which has the biggest number of sportsmen who participated in the events is %s and the number of events is: %d\n", best_club_name, max_events);
}

void correct_formatLastName(char* lastName) {
    int i;

    // Convert all letters to lowercase
    for (i = 0; lastName[i] != '\0'; i++) {
        lastName[i] = tolower(lastName[i]);
    }

    // Capitalize the first letter
    if (i > 0) {
        lastName[0] = toupper(lastName[0]);
    }
}

void capitalizeFirstLetter(char* str) {
    int i;

    // Capitalize first letter of the string
    if (strlen(str) > 0) {
        str[0] = toupper(str[0]);
    }

    // Capitalize first letter of each word
    for (i = 1; str[i] != '\0'; i++) {
        if (isspace(str[i - 1]) && isalpha(str[i])) {
            str[i] = toupper(str[i]);
        }
        else {
            str[i] = tolower(str[i]);
        }
    }
}


int CheckSameEvent(int I, sportsman* sportsmen_array, int* num_sportsmen) {
    int flag = 0;
    sportsman* current_sportsman = NULL;
    event current_event;
    for (int i = 0; i < *num_sportsmen; i++) {
        if (sportsmen_array[i].id == I) {
            current_sportsman = &sportsmen_array[i];
            flag = 1;
            break; 
        }
    }
    // If sportsman with given ID is not found, print message and return
    if (current_sportsman == NULL) {
        printf("Sportsman with ID %d not found.\n", I);
        return 0; // Return 0 indicating failure
    }
    if (strcmp(current_sportsman->p2events, "No events registered") == 0) {
        printf("Sportsman with ID %d has no events.\n", I);
        return 0; // Return 0 indicating failure
    }
    // Iterate over the events of the current sportsman
    for (int i = 0; i < current_sportsman->Nevents; i++) {
        current_event = current_sportsman->p2events[i];

        int participated_with_others = 0; // flag that checks if the current sportsman participated with other sportsmen in this event

        // Iterate over other sportsmen to find those who participated in the same event
        for (int j = 0; j < *num_sportsmen; j++) 
        {
            if (sportsmen_array[j].id == I) 
            {
                continue; // Skip the current sportsman
            }

            // Iterate over the events of the other sportsman
            for (int k = 0; k < sportsmen_array[j].Nevents; k++) 
            {
                event other_event = sportsmen_array[j].p2events[k];
                if (strcmp(current_event.p2title, other_event.p2title) == 0 &&
                    strcmp(current_event.p2location, other_event.p2location) == 0 &&
                    current_event.year == other_event.year) 
                {
                    // Print the first name and last name of the other sportsman who participated in the same event
                    printf("Event: %s, Location: %s, Year: %d\n", current_event.p2title, current_event.p2location, current_event.year);
                    printf("Participant: %s %s\n", sportsmen_array[j].Fname, sportsmen_array[j].Lname);
                    flag = 1; // Set flag to indicate success
                    participated_with_others = 1;
                }
            }
        }
        // If the current sportsman participated with others in this event, print a separator line
        if (participated_with_others) {
            printf("--------\n");
        }
    }
    // If flag is 0, no other sportsmen participated in events with the specified sportsman
    if (flag == 0) 
    {
        printf("No other sportsmen participated in events with sportsman ID %d.\n", I);
    }
    return flag; // return flag indicating success or failure
}

void printSortedEvents(const char* C, sportsman* sportsmen_array, int* num_sportsmen) {
    // flag to check if the club exists
    int club_exists = 0;

    // running on the sportsmen_array
    for (int i = 0; i < *num_sportsmen; i++) 
    {
        // Check if the current sportsman belongs to the club C
        if (strcmp(sportsmen_array[i].p2club, C) == 0) 
        {
            club_exists = 1; // found a much club
            event unique_events[MAX_EVENT_NUMBER]; //array of events for this club
            int num_unique_events = 0; // Number of events for this club

            // running on the events array of the current sportsman
            for (int j = 0; j < sportsmen_array[i].Nevents; j++) 
            {
                // Check if the current event is already in the set of events of this club
                int event_exists = 0;
                for (int k = 0; k < num_unique_events; k++) 
                {
                    if (strcmp(unique_events[k].p2title, sportsmen_array[i].p2events[j].p2title) == 0 &&
                        strcmp(unique_events[k].p2location, sportsmen_array[i].p2events[j].p2location) == 0 &&
                        unique_events[k].year == sportsmen_array[i].p2events[j].year) {
                        event_exists = 1; 
                        break;
                    }
                }
                // If the event is not already existst in the array events we add it
                if (!event_exists) 
                {
                    unique_events[num_unique_events] = sportsmen_array[i].p2events[j];
                    num_unique_events++;
                }
            }

            // this part just sort the events by the year. 
            // I used simple Bubble sort (i know this from high school) to sort the array
            for (int x = 0; x < num_unique_events - 1; x++) 
            {
                for (int y = 0; y < num_unique_events - x - 1; y++) 
                {
                    if (unique_events[y].year > unique_events[y + 1].year) //check for each event if he is bigger form the next one
                    {

                        //if it is bigger we swap the events case we want the event will be order by year (e.g: 2001,2005,2006,2007,2024)
                        event temp = unique_events[y];
                        unique_events[y] = unique_events[y + 1];
                        unique_events[y + 1] = temp;
                    }
                }
            }

            // Print the sorted unique events
            for (int m = 0; m < num_unique_events; m++) 
            {
                printf("Event: %s, Year: %d\n", unique_events[m].p2title, unique_events[m].year);
            }
        }
    }

    // If the club does not exist, print a message to the user
    if (!club_exists) 
    {
        printf("Club '%s' does not exist.\n", C);
    }
}

void deleteEvent(const char* E, int Y, sportsman* sportsmen_array, int* num_sportsmen) 
{
    for (int i = 0; i < *num_sportsmen; i++) 
    {
        for (int j = 0; j < sportsmen_array[i].Nevents; j++) 
        {
            // Check if the event matches the given name (E) and year (Y)
            if (strcmp(sportsmen_array[i].p2events[j].p2title, E) == 0 && sportsmen_array[i].p2events[j].year == Y) 
            {
                // Free memory allocated for the event's title and location
                free(sportsmen_array[i].p2events[j].p2title);
                free(sportsmen_array[i].p2events[j].p2location);

                // Remove the event from the events list
                for (int k = j; k < sportsmen_array[i].Nevents - 1; k++) 
                {
                    sportsmen_array[i].p2events[k] = sportsmen_array[i].p2events[k + 1];
                }
                sportsmen_array[i].Nevents--; // Decrement the number of events
                sportsmen_array[i].p2events = realloc(sportsmen_array[i].p2events, sportsmen_array[i].Nevents * sizeof(event));

                // Adjust index after removal
                j--;
            }
        }
    }
}

void NewClub(const char* C1, const char* C2, sportsman* sportsmen_array, int* num_sportsmen) {
    //// Open a new file named "Club.txt" for writing
    //FILE* fp = fopen("Club.txt", "w");
    //if (fp == NULL) {
    //    printf("Error opening file.\n");
    //    return;
    //}

    //// Iterate through the sportsmen array to identify sportsmen belonging to clubs C1 and C2
    //for (int i = 0; i < *num_sportsmen; i++) {
    //    if (strcmp(sportsmen_array[i].club_name, C1) == 0) {
    //        // Store events for club C1 sportsmen
    //        for (int j = 0; j < sportsmen_array[i].Nevents; j++) {
    //            fprintf(fp, "%s,%s,%d\n", sportsmen_array[i].p2events[j].p2title, sportsmen_array[i].p2events[j].p2location, sportsmen_array[i].p2events[j].year);
    //        }
    //    }
    //    else if (strcmp(sportsmen_array[i].club_name, C2) == 0) {
    //        // Store events for club C2 sportsmen
    //        for (int j = 0; j < sportsmen_array[i].Nevents; j++) {
    //            fprintf(fp, "%s,%s,%d\n", sportsmen_array[i].p2events[j].p2title, sportsmen_array[i].p2events[j].p2location, sportsmen_array[i].p2events[j].year);
    //        }
    //    }
    //}

    //// Close the file
    //fclose(fp);

    //printf("Club.txt created and events written successfully.\n");
}


void main()
{
    char sportsman_last_name[MAX_NAME_LENGTH];
    char specific_event_name[MAX_NAME_LENGTH];
    char club_name[MAX_NAME_LENGTH];
    char E[MAX_NAME_LENGTH];
    int Y;
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
            int selected_id;
            while (1)
            {
                printf("Enter the sportsman ID for whom you want to add an event: ");
                if (scanf("%d", &selected_id) != 1)
                {
                    printf("Error: Invalid input for ID. Please enter a number.\n");
                    while (getchar() != '\n'); // Clear input buffer
                }
                else
                {
                    break; // Exit the loop if a valid ID is provided
                }
            }
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

        else if (choice == 3) // Handle other options
        {
            printSportsmen(sportsmen_array, num_sportsmen);
        }
        else if (choice == 4) // Handle other options
        {
            print_Sportman_participated_events(sportsmen_array, num_sportsmen);
        }
        else if (choice == 5) 
        {
            printf("Enter the last name of the sportsman:");
            scanf("%s", sportsman_last_name);
            if (atoi(sportsman_last_name) != 0) // if the user enter int
            {
                printf("Invalid input: Last name cannot be an integer.\n");
                return 1; // Exit the program due to invalid input
            }
            correct_formatLastName(sportsman_last_name); //if the user enter 'gefen' instead 'Gefen' 
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
        else if (choice == 6) 
        {
            printf("Enter the specific event name:");
            fgets(specific_event_name, sizeof(specific_event_name), stdin);
            capitalizeFirstLetter(specific_event_name); //If the user enter WoRlD CUP instead of World Cup
            specific_event_name[strcspn(specific_event_name, "\n")] = '\0'; // Remove the newline character
            printf("Enter the specific event year:");
            scanf("%d", &specific_event_year);
            while (getchar() != '\n'); // Clear the input buffer
            printf("The number of sportsman who participated in %s in %d is: %d\n ", specific_event_name, specific_event_year, countEvent(specific_event_name, specific_event_year, sportsmen_array, &num_sportsmen));
        }
        else if (choice == 7) // Handle other options
        {
            bestClub(sportsmen_array, &num_sportsmen);
        }
        else if (choice == 8) // Handle other options
        {
            int I;
            printf("Enter the sportsman ID for whom you want check: ");
            scanf("%d", &I);
            printf("here is the list of sportsman that particpated in the same event with this sportman:\n");
            CheckSameEvent(I, sportsmen_array, &num_sportsmen);
        }
        else if (choice == 9) // Handle other options
        {
            printf("Enter the club name:");
            fgets(club_name, sizeof(club_name), stdin);
            capitalize_each_First_Letter(club_name);
            club_name[strcspn(club_name, "\n")] = '\0'; // Remove the newline character
            printSortedEvents(club_name, sportsmen_array, &num_sportsmen);
        }
        else if (choice == 10) // Handle other options
        {
            printf("Enter the event name that you want to delete:");
            fgets(E, sizeof(E), stdin);
            capitalize_each_First_Letter(E);
            E[strcspn(E, "\n")] = '\0'; // Remove the newline character
            printf("Enter the event year that you want to delete:");
            scanf("%d", &Y);
            while (getchar() != '\n'); // Clear the input buffer
            deleteEvent(E, Y, sportsmen_array, &num_sportsmen);
            Write_array_to_Event_Data("EventData.txt", sportsmen_array, num_sportsmen); //update the file

        }
        else // Invalid choice
        {
            printf("Invalid choice. Please enter a number between 0 and 10.\n");
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
