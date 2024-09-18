#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime> // Include ctime for struct tm
#include <iomanip> // Include iomanip for setfill and setw
#include <sstream> // Include stringstream
#include<chrono>
using namespace std;

class Extractor
{
public:
    Extractor() {}
    static void getaddr(string loc, string *addr, string *city, string *state, string *postal, string *country)
    {
        istringstream iss1(loc);
        getline(iss1, *addr, ':');
        getline(iss1, *city, ':');
        getline(iss1, *state, ':');
        getline(iss1, *postal, ':');
        getline(iss1, *country, ':');
    }

    static void getaddrshowvenues(string loc, string *city, string *state, string *postal, string *country)
    {
        istringstream iss1(loc);
        getline(iss1, *city, ':');
        getline(iss1, *state, ':');
        getline(iss1, *postal, ':');
        getline(iss1, *country, ':');
    }

};
class Validator
{
public:
    Validator() {}

    static bool isLeap(int year)
    {
        if (year % 400 == 0)
            return true;
        if (year % 100 == 0)
            return false;
        if (year % 4 == 0)
            return true;
        return false;
    }

    static bool isValidDate(string dateString)
    {
        tm date = {};
        istringstream ss(dateString);
        char delim = '-';

        // Parse year, month, and day
        if (ss >> date.tm_year >> delim >> date.tm_mon >> delim >> date.tm_mday) {
            date.tm_year -= 1900; // Adjust year (since 1900)
            date.tm_mon -= 1; // Adjust month (0-based), so it's also index in daysInMonth

            // Validate components
            if (date.tm_mon >= 0 && date.tm_mon < 12 &&
                date.tm_mday >= 1 && date.tm_mday <= 31) {
                // Validate day based on month and year
                int feb;
                if (isLeap(date.tm_year + 1900)) {
                    // Leap year
                    feb = 29;
                }
                else {
                    // Non-leap year
                    feb = 28;
                }
                vector<int> daysInMonth = {31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                if (date.tm_mday <= daysInMonth[date.tm_mon]) {
                    return true;
                }
            }
        }

        return false; // Invalid date
    }

};




class Venue {
public:
    string name;
    string location;
    string type;
    string city;
    string state;
    string postalCode;
    string country;
    int capacity;
    Venue *next;

    Venue(string& venueName, string& venueLocation, string& venue_type, string& venueCity, 
          string& venueState, string& venuePostalCode, string& venueCountry, 
          int venueCapacity)
        : name(venueName), location(venueLocation), type(venue_type), city(venueCity), 
          state(venueState), postalCode(venuePostalCode), country(venueCountry), capacity(venueCapacity), next(nullptr) {
    }
};


class Program {
public:
    string name;
    string type;
    string startDate;
    string endDate;
    Program *next;
    Venue *venueList;  // Add this line

    Program(string& eventName, string& eventType, string& start, string& end)
        : name(eventName), type(eventType), startDate(start), endDate(end), next(nullptr), venueList(nullptr) {  // Initialize venueList
    }
};

//first my intention will be to create a congregation class that will have all the venues stored in it
class Congregation {
public:
    string name;
    string type;
    string startDate;
    string endDate;
    Congregation *next;
    Program* programList;
    Venue* reservedVenueList;

    Congregation(string& congName, string& congType, string& start, string& end)
        : name(congName), type(congType), startDate(start), endDate(end), next(nullptr), programList(nullptr), reservedVenueList(nullptr) {
    }
};




Venue *venueList= nullptr;
Congregation* congregationHead= nullptr;
Program *programHead = nullptr;

    bool doesVenueExists( string& name,  string& country) {
        Venue* current = venueList;
        while (current != nullptr) {
            if (current->name == name && current->country == country) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    Congregation* findCongregation( string& name) {
        Congregation* current = congregationHead;
        while (current != nullptr) {
            if (current->name == name) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    // i am creating a add venue this 
    void addVenue(string& name, string& location, string& type, string& city, string& state, string& postalCode, string& country, int capacity)
    {
        if(venueList == nullptr)
        {
            venueList = new Venue(name, location, type, city, state, postalCode, country, capacity);
            cout << "0\n";
            return;
        }
        if(doesVenueExists(name, country)) {
            cout << "-1\nError\n";
            return;
        }
        if (!(type == "Outdoor Stadium" || type == "Indoor Stadium" || type == "Swimming Pool" || 
            type == "Hotel" || type == "Convention Center" || type == "Concert Hall")) {
            cout << "-1\nError\n";
            return;
        }

        Venue* newVenue = new Venue(name, location, type, city, state, postalCode, country, capacity);
        Venue *current = venueList;
        
        while(current->next != nullptr){
            current = current->next;
        }

        current->next = newVenue;
        cout << "0\n";
        return;
    }

    // for show venues
    void showVenues(string& city, string& state, string& country, string& postal, string type = "NONE")
    {
        Venue* current = venueList;
        vector<Venue*> temp;

        if(city.empty() && state.empty() && postal.empty() && country.empty()) {
            cout << "-1\nError\n";
            return;
        }
        if(country.empty()) {
            cout << "-1\nError\n";
            return;
        }
        if(state.empty() && !city.empty()) {
            cout << "-1\nError\n";
            return;
        }
        while(current != nullptr) {
            bool match = false;
            if(city.empty() && state.empty() && postal.empty()) {
                match = (current->country == country);
            } else if(postal.empty()) {
                match = (current->city == city && current->state == state && current->country == country);
            } else if(city.empty() && state.empty()) {
                match = (current->postalCode == postal && current->country == country);
            } else {
                match = (current->city == city && current->state == state && current->country == country && current->postalCode == postal);
            }
            if(match && (type == "NONE" || current->type == type)) {
                temp.push_back(current);
            }
            current = current->next;
        }

        cout << temp.size() << "\n";
        for(Venue* venue : temp) {
            cout << venue->name << " " << venue->location << " " <<venue->type<<" "<< venue->capacity << "\n";
        }
    }

    // if cong exists
    bool congExists( string& congName) {
        Congregation* current = congregationHead;
        while (current != nullptr) {
            if (current->name == congName) {
                return true;
            }
            current = current->next;
        }
        return false;
    }


    void addCongregation(string& name, string& type, string& startDate, string& endDate) {
    if(!(Validator::isValidDate(startDate) && Validator::isValidDate(endDate)))
    {
        cout << "-1\nError\n";
        return;
    }
    if (findCongregation(name) != nullptr) {
        cout << "-1\nError\n";
        return;
    }
    if (!(type == "Concert" || type == "Games" || type == "Convention" || type == "Conference")) {
        cout << "-1\nError\n";
        return;
    }
    
    Congregation* newCongregation = new Congregation(name, type, startDate, endDate);
    if(newCongregation == nullptr)
    {
        cout << "-1\nError\n";
        return;
    }
    
    if (congregationHead == nullptr) {
        congregationHead = newCongregation;
    } else {
        Congregation* current = congregationHead;
        while(current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newCongregation;
    }
    
    cout << "0\n";
    }

    void deleteCongregation( string& congName) {
        if (congregationHead == nullptr) {
            cout << "-1\nError\n";
            return;
        }

        Congregation* current = congregationHead;
        Congregation* prev = nullptr;

        // Find the congregation to delete
        while (current != nullptr && current->name != congName) {
            prev = current;
            current = current->next;
        }

        // If congregation not found
        if (current == nullptr) {
            cout << "-1\nError\n";
            return;
        }

        // Remove the congregation from the list
        if (prev == nullptr) {
            congregationHead = current->next;
        } else {
            prev->next = current->next;
        }

        // Free all venues of all programs for the found congregation
        Program* programCurrent = current->programList;
        while (programCurrent != nullptr) {
            // Remove this program's venues
            Venue* venueCurrent = programCurrent->venueList;
            while (venueCurrent != nullptr) {
                Venue* toDeleteVenue = venueCurrent;
                venueCurrent = venueCurrent->next;
                delete toDeleteVenue;
            }
            Program* toDelete = programCurrent;
            programCurrent = programCurrent->next;
            delete toDelete;
        }

        // Delete the congregation
        delete current;

        cout << "0\n";
    }
    void addProgram(string& cong,  string& prog_name,  string& prog_type,  string& startDate,  string& endDate) {
        Congregation* cong_ptr = findCongregation(cong);
        if(cong_ptr == nullptr) {
            cout << "-1\nError" << endl;
            return;
        }
        
        string cong_type = cong_ptr->type;
        vector<string> valid_prog_types;

        if(cong_type == "Conference") {
            valid_prog_types = {"Workshop", "Main Conference", "Banquet"};
        } else if(cong_type == "Convention") {
            valid_prog_types = {"Workshop", "Main Conference", "Banquet", "Food Court", "Exhibition"};
        } else if(cong_type == "Games") {
            valid_prog_types = {"Ceremony", "Track and field", "Indoor games", "Water sports"};
        } else if(cong_type == "Concert") {
            valid_prog_types = {"Main Concert", "Pre-concert"};
        }

        if (!Validator::isValidDate(startDate) || !Validator::isValidDate(endDate)) {
            cout << "-1\nError" << endl;
            return;
        }


        if (find(valid_prog_types.begin(), valid_prog_types.end(), prog_type) == valid_prog_types.end()) {
            cout << "-1\nError" << endl;
            return;
        }

        if (startDate < cong_ptr->startDate || endDate > cong_ptr->endDate) {
            cout << "-1\nError" << endl;
            return;
        }

        Program* newProgram = new Program(prog_name, prog_type,startDate,endDate);
        if (cong_ptr->programList == nullptr) {
            cong_ptr->programList = newProgram;
        } else {
            Program* current = cong_ptr->programList;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newProgram;
        }

        cout << "0" << endl;
    }


    void showCongregation() {
        int count = 0;
        Congregation* current = congregationHead;
        
        // Count the number of congregations
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        cout << count << endl;
        
        // Reset current to the head of the list
        current = congregationHead;
        
        // Print details of each congregation
        while (current != nullptr) {
            cout << current->name << " " << current->type << " " << current->startDate << " " << current->endDate << endl;
            current = current->next;
        }
        
        return;
    }

    
    void reserveVenue(string &name, string &country, string& congregation, string& prog)
    {
    // Check if venue exists
    Venue *venue = nullptr;
    for (Venue *current = venueList; current != nullptr; current = current->next) {
        if (current->name == name && current->country == country) {
            venue = current;
            break;
        }
    }
    if (venue == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // i will have to create a copy of the venue
    Venue* venueCopy = new Venue(venue->name, venue->location, venue->type, venue->city, venue->state, venue->postalCode, venue->country, venue->capacity);
    // Check if congregation exists

    Congregation* cong_ptr = findCongregation(congregation);
    if (cong_ptr == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Check if program exists
    Program* prog_ptr = nullptr;
    for (Program* current = cong_ptr->programList; current != nullptr; current = current->next) {
        if (current->name == prog) {
            prog_ptr = current;
            break;
        }
    }
    if (prog_ptr == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Check if venue's type supports that program
    vector<string> supported_programs;
    if (venue->type == "Outdoor Stadium") {
        supported_programs = {"Opening Ceremony", "Closing Ceremony", "Track and field", "Ceremony", "Pre-concert", "Main Concert", "Workshop", "Main Conference", "Banquet", "Food Court", "Exhibition"};
    } else if (venue->type == "Indoor Stadium") {
        supported_programs = {"Indoor games"};
    } else if (venue->type == "Swimming Pool") {
        supported_programs = {"Water Sports"};
    } else if (venue->type == "Hotel") {
        supported_programs = {"Workshop", "Main Conference", "Banquet", "Food Court", "Exhibition"};
    } else if (venue->type == "Convention Center") {
        supported_programs = {"Pre-concert", "Main Concert", "Workshop", "Main Conference", "Banquet", "Food Court", "Exhibition"};
    } else if (venue->type == "Concert Hall") {
        supported_programs =  {"Pre-concert", "Main Concert"};
    }

    if (find(supported_programs.begin(), supported_programs.end(), prog_ptr->type) == supported_programs.end()) {
        cout << "-1\nError" << endl;
        return;
    }



    // Check for time conflicts
    //I will have to go through every program and its venue if the venue matches it shouldn't overlap.
    // Check for time conflicts
    Program* current = cong_ptr->programList;
    while (current != nullptr) {
        Venue* currentVenue = current->venueList;
        while (currentVenue != nullptr) {
            if (currentVenue->name == venue->name) {
                if ((prog_ptr->startDate >= current->startDate && prog_ptr->startDate < current->endDate) ||
                    (prog_ptr->endDate > current->startDate && prog_ptr->endDate <= current->endDate) ||
                    (prog_ptr->startDate <= current->startDate && prog_ptr->endDate >= current->endDate)) {
                    cout << "-1\nError" << endl;
                    return;
                }
            }
            currentVenue = currentVenue->next;
        }
        current = current->next;
    }


    // Reserve the venue
    if (prog_ptr->venueList == nullptr) {
        prog_ptr->venueList = venueCopy;
    } else {
        Venue* current = prog_ptr->venueList;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = venueCopy;
    }

    // i will also have to add the venue to the venue list of the congregation
    Venue* congVenueCopy = new Venue(venue->name, venue->location, venue->type, venue->city, venue->state, venue->postalCode, venue->country, venue->capacity);
    if (cong_ptr->reservedVenueList == nullptr) {
        cong_ptr->reservedVenueList = congVenueCopy;
    } else {
        Venue* current = cong_ptr->reservedVenueList;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = congVenueCopy;
    }

    cout << "0" << endl;
}


void freeVenue(string &name, string &country, string& congregation, string& prog)
{
    // Check if venue exists
    Venue *venue = nullptr;
    for (Venue *current = venueList; current != nullptr; current = current->next) {
        if (current->name == name && current->country == country) {
            venue = current;
            break;
        }
    }
    if (venue == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Check if congregation exists
    Congregation* cong_ptr = findCongregation(congregation);
    if (cong_ptr == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Check if program exists
    Program* prog_ptr = nullptr;
    Program* prev_prog = nullptr;
    for (Program* current = cong_ptr->programList; current != nullptr; prev_prog = current, current = current->next) {
        if (current->name == prog) {
            prog_ptr = current;
            break;
        }
    }
    if (prog_ptr == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Check if the program is actually reserved at this venue
    bool venueFound = false;
    for (Venue* currentVenue = prog_ptr->venueList; currentVenue != nullptr; currentVenue = currentVenue->next) {
        if (currentVenue->name == venue->name && currentVenue->country == venue->country) {
            venueFound = true;
            break;
        }
    }

    if (!venueFound) {
        cout << "-1\nError" << endl;
        return;
    }


    // Remove the venue from the program's venueList
    Venue* prev = nullptr;
    Venue* current = prog_ptr->venueList;
    while (current != nullptr) {
        if (current->name == name && current->country == country) {
            if (prev == nullptr) {
                prog_ptr->venueList = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }

    // Check if the venue is still reserved by any other program
    bool venueReserved = false;
    for (Program* currentProg = cong_ptr->programList; currentProg != nullptr; currentProg = currentProg->next) {
        if (currentProg != prog_ptr) {
            for (Venue* currentVenue = currentProg->venueList; currentVenue != nullptr; currentVenue = currentVenue->next) {
                if (currentVenue->name == name && currentVenue->country == country) {
                    venueReserved = true;
                    break;
                }
            }
            if (venueReserved) break;
        }
    }

    // Remove the venue from the congregation's reservedVenueList only if it's not reserved by any other program
    if (!venueReserved) {
        prev = nullptr;
        current = cong_ptr->reservedVenueList;
        while (current != nullptr) {
            if (current->name == name && current->country == country) {
                if (prev == nullptr) {
                    cong_ptr->reservedVenueList = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                break;
            }
            prev = current;
            current = current->next;
        }
    }

    cout << "0" << endl;
}

void deleteVenue(string &name, string &country)
{
    // Check if venue exists in venueList
    Venue* prev = nullptr;
    Venue* current = venueList;
    while (current != nullptr) {
        if (current->name == name && current->country == country) {
            break;
        }
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Check if venue is present in any congregation's programs
    for (Congregation* cong = congregationHead; cong != nullptr; cong = cong->next) {
        for (Program* prog = cong->programList; prog != nullptr; prog = prog->next) {
            for (Venue* venue = prog->venueList; venue != nullptr; venue = venue->next) {
                if (venue->name == name && venue->country == country) {
                    cout << "-1\nError" << endl;
                    return;
                }
            }
        }
    }

    // Remove venue from venueList
    if (prev == nullptr) {
        venueList = current->next;
    } else {
        prev->next = current->next;
    }
    delete current;

    cout << "0" << endl;
}

void showReservedVenues(string cong)
{
    // Check if congregation exists
    Congregation* cong_ptr = findCongregation(cong);
    if (cong_ptr == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Count and print the number of programs
    int program_count = 0;
    Program* current_program = cong_ptr->programList;
    while (current_program != nullptr) {
        program_count++;
        current_program = current_program->next;
    }
    cout << program_count << endl;
    // Print details of each program and its venues
    current_program = cong_ptr->programList;
    while (current_program != nullptr) {
        // Count and print the number of venues for this program
        int venue_count = 0;
        Venue* current_venue = current_program->venueList;
        while (current_venue != nullptr) {
            venue_count++;
            current_venue = current_venue->next;
        }
        cout << current_program->name << " " << current_program->type << " " << venue_count << endl;

        // Print details of each venue for this program
        current_venue = current_program->venueList;
        while (current_venue != nullptr) {
            cout << current_venue->name << " " << current_venue->location << " " << current_venue->type << " " << current_venue->capacity << endl;
            current_venue = current_venue->next;
        }

        current_program = current_program->next;
    }
}



void deleteProgram(string cong, string prog) {
    // Check if congregation exists
    Congregation* cong_ptr = findCongregation(cong);
    if (cong_ptr == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    Program* prev = nullptr;
    Program* current = cong_ptr->programList;

    while (current != nullptr) {
        if (current->name == prog) {
            if (current->venueList == nullptr) {
                // Program found and its venueList is empty, delete it
                if (prev == nullptr) {
                    cong_ptr->programList = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                cout << "0" << endl;
                return;
            } else {
                // Program found but its venueList is not empty
                cout << "-1\nError" << endl;
                return;
            }
        }
        prev = current;
        current = current->next;
    }

    // Program not found
    cout << "-1\nError" << endl;
}

void showPrograms( string cong) {
    // Check if congregation exists
    Congregation* cong_ptr = findCongregation(cong);
    if (cong_ptr == nullptr) {
        cout << "-1\nError" << endl;
        return;
    }

    // Count and print the number of programs
    int count = 0;
    Program* current = cong_ptr->programList;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    cout << count << endl;

    // Print details of each program
    current = cong_ptr->programList;
    while (current != nullptr) {
        cout << current->type << " " << current->name << " " << current->startDate << " " << current->endDate << endl;
        current = current->next;
    }
}


int main() {
    //for the whole line of input
    string input;
    Extractor E;
    while (getline(cin, input)) {
        //for the individual words
        string cmd;
        istringstream iss(input);
        
        iss>>cmd;

        if (cmd == "End"){
            break;
        }

        else if (cmd == "addCongregation"){
            string cong, type, startDate, endDate;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, type, '\"');
            getline(iss, type, '\"');
            getline(iss, startDate, '\"');
            getline(iss, startDate, '\"');
            getline(iss, endDate, '\"');
            getline(iss, endDate, '\"');

            if (iss.fail() || cong.empty() || type.empty() || startDate.empty() || endDate.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            time_t currdate = time(nullptr); // Get current time
            char currdate_1[11]; 

            // Format the current date as "YYYY-MM-DD"
            strftime(currdate_1, sizeof(currdate_1), "%Y-%m-%d", localtime(&currdate));
            string today = currdate_1;
            
            //checking that start date <= ennd date
            if (startDate > endDate) {
                cout << "-1\nError" << endl;
                continue;
            }

            // Check if the start date and end date are in the future
            if (startDate < today || endDate < today) {
                cout << "-1\nError" << endl;
                continue;
            }

            addCongregation(cong, type, startDate, endDate);
        }

        else if (cmd == "deleteCongregation"){
            string cong;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            if (iss.fail() || cong.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            deleteCongregation(cong);

        }
        
        else if (cmd == "showCongregations"){
            showCongregation();
        }

        else if (cmd == "addProgram"){
            string cong, type, prog, startDate, endDate;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, type, '\"');
            getline(iss, type, '\"');
            getline(iss, prog, '\"');
            getline(iss, prog, '\"');
            getline(iss, startDate, '\"');
            getline(iss, startDate, '\"');
            getline(iss, endDate, '\"');
            getline(iss, endDate, '\"');

            if (iss.fail() || cong.empty() || type.empty() || prog.empty() || startDate.empty() || endDate.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            time_t currdate = time(nullptr); // Get current time
            char currdate_1[11]; 

            // Format the current date as "YYYY-MM-DD"
            strftime(currdate_1, sizeof(currdate_1), "%Y-%m-%d", localtime(&currdate));
            string today = currdate_1;
            
            //checking that start date <= ennd date
            if (startDate > endDate) {
                cout << "-1\nError" << endl;
                continue;
            }

            // Check if the start date and end date are in the future
            if (startDate < today || endDate < today) {
                cout << "-1\nError" << endl;
                continue;
            }

            addProgram(cong, prog,type, startDate, endDate);
        }

        else if (cmd == "deleteProgram"){
            string cong, prog;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, prog, '\"');
            getline(iss, prog, '\"');

            if (iss.fail() || cong.empty() || prog.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            deleteProgram(cong, prog);
        }

        else if (cmd == "showPrograms"){
            string cong;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            if (iss.fail() || cong.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            showPrograms(cong);
        }

        else if (cmd == "addVenue"){
            string name, loc, type;
            string addr, city, state, postal, country;
            int cap;
            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, loc, '\"');
            getline(iss, loc, '\"');
            getline(iss, type, '\"');
            getline(iss, type, '\"');
            iss >> cap;

            E.getaddr(loc, &addr, &city, &state, &postal, &country);
            if (iss.fail() || name.empty() || loc.empty() || type.empty() || addr.empty() || city.empty() || state.empty() || postal.empty() || country.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            addVenue(name, loc, type, city, state, postal, country, cap);
        }

        else if (cmd == "deleteVenue"){
            string name, country;

            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');

            if (iss.fail() || name.empty() || country.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            deleteVenue(name, country);

        }

        else if (cmd == "showVenues"){
            string location, type;
            string city, state, postal, country;

            getline(iss, location, '\"');
            getline(iss, location, '\"');

            getline(iss, type, '\"');
            getline(iss, type, '\"');

            if ((iss.fail() || location.empty()) && (!type.empty())) {
                cout << "-1\nError" << endl;
                continue;
            }

            E.getaddrshowvenues(location, &city, &state, &postal, &country);
            if(type=="")showVenues(city, state, country,postal);
            else showVenues(city, state, country, postal, type);
        }

        else if (cmd == "reserveVenue"){
            string name, country, cong, prog;

            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, prog, '\"');
            getline(iss, prog, '\"');

            if (iss.fail() || name.empty() || country.empty() || cong.empty() || prog.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            reserveVenue(name, country, cong, prog);
        }

        else if (cmd == "freeVenue"){
            string name, country, cong, prog;

            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, prog, '\"');
            getline(iss, prog, '\"');

            if (iss.fail() || name.empty() || country.empty() || cong.empty() || prog.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            freeVenue(name, country, cong, prog);
        }

        else if (cmd == "showReserved"){
            string cong;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            if (iss.fail() || cong.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            showReservedVenues(cong);
        }
    
        else{
            cout << "-1\nError" << endl;
        }
    }
    

    return 0;
}
