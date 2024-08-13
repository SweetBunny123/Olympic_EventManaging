#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 100
#define MAX_LOCATION_LENGTH 100
#define MAX_DAYS 30
#define MAX_HOURS 24

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

int addVenue(char* name, char* location, int capacity) {
    Venue* newVenue = (Venue*)malloc(sizeof(Venue));
    if (!newVenue) {
        printf("-1\nError: out of memory\n");
        return -1;
    }

    strcpy(newVenue->name, name);
    strcpy(newVenue->location, location);
    newVenue->capacity = capacity;
    for (int i = 0; i < MAX_DAYS; i++) {
        newVenue->calendar[i] = NULL;
    }
    newVenue->next = NULL;

    if (!venueList || strcmp(name, venueList->name) < 0) {
        newVenue->next = venueList;
        venueList = newVenue;
    } else {
        Venue* current = venueList;
        while (current->next && strcmp(name, current->next->name) > 0) {
            current = current->next;
        }
        if (current->next && strcmp(name, current->next->name) == 0) {
            printf("-1\nError: venue already exists\n");
            free(newVenue);
            return -1;
        }
        newVenue->next = current->next;
        current->next = newVenue;
    }

    printf("0\n");
    return 0;
}

int deleteVenue(char* name) {
    Venue* current = venueList;
    Venue* prev = NULL;

    while (current && strcmp(current->name, name) != 0) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        printf("-1\nError: venue does not exist\n");
        return -1;
    }

    if (prev) {
        prev->next = current->next;
    } else {
        venueList = current->next;
    }

    for (int i = 0; i < MAX_DAYS; i++) {
        Event* event = current->calendar[i];
        while (event) {
            Event* nextEvent = event->next;
            free(event);
            event = nextEvent;
        }
    }

    free(current);
    printf("0\n");
    return 0;
}

void showVenues() {
    int count = 0;
    Venue* current = venueList;
    while (current) {
        count++;
        current = current->next;
    }

    printf("%d\n", count);
    current = venueList;
    while (current) {
        printf("%s %s %d\n", current->name, current->location, current->capacity);
        current = current->next;
    }
}

Venue* findVenue(char* name) {
    Venue* current = venueList;
    while (current && strcmp(current->name, name) != 0) {
        current = current->next;
    }
    return current;
}

int addEvent(char* venueName, int date, int fromHour, int toHour, char* eventName) {
    if (date < 1 || date > MAX_DAYS || fromHour < 0 || toHour > MAX_HOURS || fromHour >= toHour) {
        printf("-1\nError: invalid date or hour range\n");
        return -1;
    }

    Venue* venue = findVenue(venueName);
    if (!venue) {
        printf("-1\nError: venue does not exist\n");
        return -1;
    }

    Event* current = venue->calendar[date-1];
    while (current) {
        if ((fromHour < current->endHour && toHour > current->startHour) ||
            (current->startHour < toHour && current->endHour > fromHour)) {
            printf("-1\nError: time slot already occupied\n");
            return -1;
        }
        current = current->next;
    }

    Event* newEvent = (Event*)malloc(sizeof(Event));
    if (!newEvent) {
        printf("-1\nError: out of memory\n");
        return -1;
    }

    strcpy(newEvent->name, eventName);
    newEvent->startHour = fromHour;
    newEvent->endHour = toHour;
    newEvent->next = NULL;

    if (!venue->calendar[date-1] || fromHour < venue->calendar[date-1]->startHour) {
        newEvent->next = venue->calendar[date-1];
        venue->calendar[date-1] = newEvent;
    } else {
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

int deleteEvent(char* venueName, int date, int fromHour, char* eventName) {
    if (date < 1 || date > MAX_DAYS || fromHour < 0 || fromHour >= MAX_HOURS) {
        printf("-1\nError: invalid date or hour\n");
        return -1;
    }

    Venue* venue = findVenue(venueName);
    if (!venue) {
        printf("-1\nError: venue does not exist\n");
        return -1;
    }

    Event* current = venue->calendar[date-1];
    Event* prev = NULL;

    while (current && (current->startHour != fromHour || strcmp(current->name, eventName) != 0)) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        printf("-1\nError: no matching event (on date %d starting at hour %d)\n", date, fromHour);
        return -1;
    }

    if (prev) {
        prev->next = current->next;
    } else {
        venue->calendar[date-1] = current->next;
    }

    free(current);
    printf("0\n");
    return 0;
}

void showEvents(char* venueName, int date) {
    if (date < 1 || date > MAX_DAYS) {
        printf("-1\nError: invalid date\n");
        return;
    }

    Venue* venue = findVenue(venueName);
    if (!venue) {
        printf("-1\nError: venue does not exist\n");
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

void showCalendar(char* venueName) {
    Venue* venue = findVenue(venueName);
    if (!venue) {
        printf("-1\nError: venue does not exist\n");
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
    char command[20];
    char input[256];
    char name[MAX_NAME_LENGTH], location[MAX_LOCATION_LENGTH], eventName[MAX_NAME_LENGTH];
    int capacity, date, fromHour, toHour;

    while (1) {
        printf("Please enter a command...\n");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        trim(input);

        if (strcmp(input, "End") == 0) break;

        sscanf(input, "%s", command);

        if (strcmp(command, "addVenue") == 0) {
            if (sscanf(input, "%*s \"%[^\"]\" \"%[^\"]\" %d", name, location, &capacity) == 3) {
                addVenue(name, location, capacity);
            } else {
                printf("-1\nError: invalid arguments\n");
            }
        } else if (strcmp(command, "deleteVenue") == 0) {
            if (sscanf(input, "%*s \"%[^\"]\"", name) == 1) {
                deleteVenue(name);
            } else {
                printf("-1\nError: invalid arguments\n");
            }
        } else if (strcmp(command, "showVenues") == 0) {
            showVenues();
        } else if (strcmp(command, "addEvent") == 0) {
            if (sscanf(input, "%*s \"%[^\"]\" %d %d %d \"%[^\"]\"", name, &date, &fromHour, &toHour, eventName) == 5) {
                addEvent(name, date, fromHour, toHour, eventName);
            } else {
                printf("-1\nError: invalid arguments\n");
            }
        } else if (strcmp(command, "deleteEvent") == 0) {
            if (sscanf(input, "%*s \"%[^\"]\" %d %d \"%[^\"]\"", name, &date, &fromHour, eventName) == 4) {
                deleteEvent(name, date, fromHour, eventName);
            } else {
                printf("-1\nError: invalid arguments\n");
            }
        } else if (strcmp(command, "showEvents") == 0) {
            if (sscanf(input, "%*s \"%[^\"]\" %d", name, &date) == 2) {
                showEvents(name, date);
            } else {
                printf("-1\nError: invalid arguments\n");
            }
        } else if (strcmp(command, "showCalendar") == 0) {
            if (sscanf(input, "%*s \"%[^\"]\"", name) == 1) {
                showCalendar(name);
            } else {
                printf("-1\nError: invalid arguments\n");
            }
        } else {
            printf("-1\nError: unknown command\n");
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