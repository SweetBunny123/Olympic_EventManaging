#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VENUES 100
#define MAX_NAME_LENGTH 100
#define MAX_LOCATION_LENGTH 100
#define MAX_DAYS 30
#define MAX_HOURS 24
#define MAX_EVENTS_PER_DAY 24
// so what can be done here? 
// before all that i neeed to make a struct for the venue and the event
typedef struct Event {
    char name[MAX_NAME_LENGTH];
    int startHour;
    int endHour;
    struct Event* next;
} Event;
typedef struct Venue {
    char name[MAX_NAME_LENGTH];
    char location[MAX_LOCATION_LENGTH];
    int capacity;
    Event* calendar[MAX_DAYS];
    struct Venue* next;
} Venue;
Venue* venueList = NULL;

void trim(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

int venueCount = 0;
// 1. add a venue
// lets talk about adding a venue 
int addVenue(char *name, char *location, int capacity)
{
    Venue *newVenue = (Venue*)malloc(sizeof(Venue));
    if(newVenue == NULL)
    {
        printf("-1\nError: Memory allocation failed for NewVenue. Unable to allocate required memory for the operation.\n");
        return -1;
    }
    // i will add the venue name and location and capacity first to inidiviual venue
    // then i will add the venue to the venue list if there is no venue with the same name
    strcpy(newVenue->name, name);
    strcpy(newVenue->location, location);
    newVenue->capacity = capacity;
    newVenue->next = NULL;

    //check if venue already exists
    Venue *current = venueList;
    if
        (!venueList) {
        newVenue->next = venueList;
        venueList = newVenue;// if venueList is null then newVenue is the first venue
    }
    else{
    while (current->next && strcmp(name, current->next->name) != 0) {
            current = current->next;
        }
        if (current && strcmp(name, current->name) == 0) {
            printf("-1\nError: Venue %s already exits \n", name);
            free(newVenue);
            return -1;// if venue already exists, i wont create a new venue and free the venue created and memory
        }
        else{
            while(current->next!=NULL)current = current->next;}
        newVenue->next = current->next;
        current->next = newVenue;
    }
        venueCount ++;// i dont know if this is the right way to do it but i will try it
        printf("0\n");
        return 0;//venue successfully added and the veneu list last venue is the new venue
}
Venue* findVenue(char* name) {
    Venue* current = venueList;
    while (current && strcmp(current->name, name) != 0) {
        current = current->next;
    }
    return current;
}

// 2. delete a venue
int deleteVenue(char *name)
{
    // to delete a venue i will need to find the venue if it exits and then make its pointer to null and free the venue
    Venue* current = venueList;
    Venue* prev = NULL;
    while(current && strcmp(current->name, name) != 0)
    {
        prev = current;
        current = current->next;
    }
    if(!current)
    {
        printf("-1\nError:Venue %s doesn't exist\n", name);
        return -1;
    }
    else
    {
        if(prev)
        {
            prev->next = current->next;
        }
        else
        {
            venueList = current->next;
        }
        free(current);
        venueCount --;
        printf("0\n");
        return 0;
    }
}
// 3. show venues
void showVenues(){
    // to show venues i will need to print the venue name and location and capacity
    // before that i am asked to print the total number of venues , the venuecount will come to play for this
    printf("%d\n", venueCount);
    Venue* current = venueList;
    while(current)
    {
        printf("%s %s %d\n", current->name, current->location, current->capacity);
        current = current->next;
    }
}
//--------------------------------------------/
// now how do i maintain the calender system for the venues
// 3. add event
int addEvent(char *venueName, int date, int fromHour, int toHour, char *eventName)
{
    //precaution invalid dates and hours
    if (date < 1 || date > MAX_DAYS || fromHour < 0 || toHour > MAX_HOURS || fromHour >= toHour) {
        printf("-1\nError: Invalid date / fromHour / toHour \n");
        return -1;
    }
    // to add an event i will need to find the venue if it exits and then add the event to the venue
    Venue* venue = findVenue(venueName);//findVenue should return the pointer to venue if it exits and null if it doesnt
    if (!venue) {
        printf("-1\nError:Venue Doesn't exist.\n");
        return -1;
    }
    Event* current = venue->calendar[date-1];
    while (current) {
        if ((toHour <= current->endHour && toHour > current->startHour)||(fromHour < current->endHour && fromHour >= current->startHour)||(fromHour<=current->startHour && toHour>current->endHour)) {
            printf("-1\nError:Time slot overlaps with already existing events.\n");
            return -1;
        }
        current = current->next;
    }
    Event* newEvent = (Event*)malloc(sizeof(Event));
    if (!newEvent) {
        printf("-1\nError:Memory allocation failed for NewEvent. Unable to allocate required memory for the operation.\n");// if the memory is not allocated then it will return -1
        return -1;
    }
    strcpy(newEvent->name, eventName);
    newEvent->startHour = fromHour;
    newEvent->endHour = toHour;
    newEvent->next = NULL;
    // again i will start from beginning of the list for a particular date of a venue
    if(!venue->calendar[date-1] || fromHour < venue->calendar[date-1]->startHour)
    {
        newEvent->next = venue->calendar[date-1];
        venue->calendar[date-1] = newEvent;
        // make this one first event of the list
    }
    else
    {
        // i will need to find the position of the event in the list
        current = venue->calendar[date-1];
        while (current->next && current->next->startHour < fromHour) {
            current = current->next;
        }
        newEvent->next = current->next;
        current->next = newEvent;
    }
    printf("0\n");
    return 0;
}
// 4. delete event
int deleteEvent(char *venueName, int date, int fromHour, char *eventName)
{
    if (date < 1 || date > MAX_DAYS || fromHour < 0 || fromHour >= MAX_HOURS) {
        printf("-1\nError: Invalid day / fromHour / toHour\n");
        return -1;
    }
    Venue* venue = findVenue(venueName);
    if(!venue)
    {
        printf("-1\nError: Venue doesn't exist\n");
        return -1;
    }
    Event* current = venue->calendar[date-1];
    Event* prev = NULL;
    while (current && (current->startHour != fromHour || strcmp(current->name, eventName) != 0)) {
        prev = current;
        current = current->next;
    }
    if (!current) {
        printf("-1\nError: Event doesn't exist.\n");
        return -1;
    }
    if(prev)
    {
        prev->next = current->next;// i make the prev point to current next

    }
    else
    {
        //this means this is the first event to be delted we simply switch venues's first point to its second;
        venue->calendar[date-1] = current->next;
    }
    free(current);//delete the required event
    printf("0\n");
    return 0;
}
// 5. show events
void showEvents(char* venueName, int date)
{
    Venue* venue = findVenue(venueName);
    if (!venue) {
        printf("-1\nError: Venue doesn't exist.\n");
        return;
    }
    int count = 0;
    Event* current = venue->calendar[date-1];
    while (current) {
        count++;
        current = current->next;
    }
    printf("%d\n", count);
    current = venue->calendar[date-1];
    while (current) {
        printf("%s %d %d\n", current->name, current->startHour, current->endHour);
        current = current->next;
    }
}
// 7. show calender
void showCalendar(char* venueName) {
    Venue* venue = findVenue(venueName);
    if (!venue) {
        printf("-1\nError: Venue doesn't exist.\n");
        return;
    }

    int totalEvents = 0;
    for (int d = 0; d < MAX_DAYS; d++) {
        Event* current = venue->calendar[d];
        while (current) {
            totalEvents++;
            current = current->next;
        }
    }

    printf("%d\n", totalEvents);

    for (int d = 0; d < MAX_DAYS; d++) {
        int dayEvents = 0;
        Event* current = venue->calendar[d];
        while (current) {
            dayEvents++;
            current = current->next;
        }

        printf("%d %d\n", d+1, dayEvents);
        current = venue->calendar[d];
        while (current) {
            printf("%s %d %d\n", current->name, current->startHour, current->endHour);
            current = current->next;
        }
    }
}



int main() {
    while (1) {
        char inp[100];
        if (scanf("%99s", inp) != 1) break;

        if (strcmp(inp, "End") == 0) {
            break;
        }
        else if (strcmp(inp, "addVenue") == 0) {
            char name[100], loc[100];
            int cap;

            if (scanf(" \"%99[^\"]\" \"%99[^\"]\" %d", name, loc, &cap) == 3) {
                addVenue(name, loc, cap);
            } else {
                printf("-1\nError: Invalid Input.\n");
            }
        }
        else if (strcmp(inp, "delVenue") == 0) {
            char name[100];
            if (scanf(" \"%99[^\"]\"", name) == 1) {
                deleteVenue(name);
            } else {
                printf("-1\nError: Invalid Input.\n");
            }
        }
        else if (strcmp(inp, "showVenues") == 0) {
            showVenues();
        }
        else if (strcmp(inp, "addEvent") == 0) {
            char name[100], eventN[100];
            int date, startH, toH;

            if (scanf(" \"%99[^\"]\" %d %d %d \"%99[^\"]\"", name, &date, &startH, &toH, eventN) == 5) {
                addEvent(name, date, startH, toH, eventN);
            } else {
                printf("-1\nError: Invalid Input.\n");
            }
        }
        else if (strcmp(inp, "delEvent") == 0) {
            char name[100], eventN[100];
            int date, startH;

            if (scanf(" \"%99[^\"]\" %d %d \"%99[^\"]\"", name, &date, &startH, eventN) == 4) {
                deleteEvent(name, date, startH, eventN);
            } else {
                printf("-1\nError: Invalid Input.\n");
            }
        }
        else if (strcmp(inp, "showEvents") == 0) {
            char name[100];
            int date;

            if (scanf(" \"%99[^\"]\" %d", name, &date) == 2) {
                showEvents(name, date);
            } else {
                printf("-1\nError: Invalid Input.\n");
            }
        }
        else if (strcmp(inp, "showCalendar") == 0) {
            char name[100];

            if (scanf(" \"%99[^\"]\"", name) == 1) {
                showCalendar(name);
            } else {
                printf("-1\nError: Invalid Input.\n");
            }
        }
        else {
            printf("-1\nError: Invalid Input.\n");
        }
    }

    // Free allocated memory before exiting
    Venue* currentVenue = venueList;
    while (currentVenue) {
        Venue* nextVenue = currentVenue->next;
        for (int i = 0; i < MAX_DAYS; i++) {
            Event* currentEvent = currentVenue->calendar[i];
            while (currentEvent) {
                Event* nextEvent = currentEvent->next;
                free(currentEvent);
                currentEvent = nextEvent;
            }
        }
        free(currentVenue);
        currentVenue = nextVenue;
    }

    return 0;
}
