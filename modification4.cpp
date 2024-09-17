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

string today = "2024-09-02";
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

    static int timediff(const string dateStr1, const string timeStr1, const string dateStr2, const string timeStr2){
        // Parse date and time strings
        istringstream dateStream1(dateStr1);
        istringstream dateStream2(dateStr2);
        istringstream timeStream1(timeStr1);
        istringstream timeStream2(timeStr2);

        int y1, m1, d1, h1, mi1, y2, m2, d2, h2, mi2;

        tm tm1 = {};
        tm tm2 = {};

        char de1 = '-';
        dateStream1 >> y1 >> de1 >> m1 >> de1 >> d1;
        dateStream2 >> y2 >> de1 >> m2 >> de1 >> d2;

        char de2 = ':';

        timeStream1 >> h1 >> de2 >> mi1;
        timeStream2 >> h2 >> de2 >> mi2;

        tm1.tm_year = y1 - 1900;
        tm1.tm_mon = m1 - 1;
        tm1.tm_mday = d1;
        tm1.tm_hour = h1;
        tm1.tm_min = mi1;
        tm2.tm_year = y2 - 1900;
        tm2.tm_mon = m2 - 1;
        tm2.tm_mday = d2;
        tm2.tm_hour = h2;
        tm2.tm_min = mi2;

        // Convert to chrono::time_point
        auto timePoint1 = chrono::system_clock::from_time_t(mktime(&tm1));
        auto timePoint2 = chrono::system_clock::from_time_t(mktime(&tm2));

        // Compute difference in minutes
        auto duration = timePoint1 - timePoint2;
        auto diffInMinutes = chrono::duration_cast<chrono::minutes>(duration).count();

        return diffInMinutes;
    }

    static bool timebound(const string timeStr1, const string timeStr2){
        istringstream timeStream1(timeStr1);
        istringstream timeStream2(timeStr2);

        int h1, mi1, h2, mi2;

        tm tm1 = {};
        tm tm2 = {};

        char del = ':';

        timeStream1 >> h1 >> del >> mi1;
        timeStream2 >> h2 >> del >> mi2;

        tm1.tm_hour = h1;
        tm1.tm_min = mi1;
        tm2.tm_hour = h2;
        tm2.tm_min = mi2;

        if (tm1.tm_hour < 0 || tm1.tm_hour > 24 || tm2.tm_hour < 0 || tm2.tm_hour > 24 || tm1.tm_min < 0 || tm1.tm_min > 59 || tm2.tm_min < 0 || tm2.tm_min > 59 || 
            tm1.tm_min % 15 != 0 || tm2.tm_min % 15 != 0){
                return false;
            }
        return true;
    }

    static bool isValidTime(const string startTimeStr, const string endTimeStr){
        istringstream timeStream1(startTimeStr);
        istringstream timeStream2(endTimeStr);

        int h1, mi1, h2, mi2;

        //tm1 is the tm object for start time, and tm2 is for end time
        tm tm1 = {};
        tm tm2 = {};

        char del = ':';

        timeStream1 >> h1 >> del >> mi1;
        timeStream2 >> h2 >> del >> mi2;

        tm1.tm_hour = h1;
        tm1.tm_min = mi1;
        tm2.tm_hour = h2;
        tm2.tm_min = mi2;

        //end time's hour can be 24, but only if minute is 00
        if (tm2.tm_hour == 24){
            //if minute is not 0, it is not valid time
            if (tm2.tm_min != 0){
                return false;
            }

            //if minute is 00, check if start time is valid
            if (tm2.tm_min == 0){
                if (tm1.tm_hour < 0 || tm1.tm_min > 59 || tm1.tm_hour > 24 || tm1.tm_min < 0){
                    return false;
                }
            }
        }

        //if end time's hour is not 24, verify both start and end time the same way
        if (tm2.tm_hour != 24){
            if (tm1.tm_hour < 0 || tm1.tm_min > 59 || tm1.tm_hour > 24 || tm1.tm_min < 0 || tm2.tm_hour < 0 || tm2.tm_min > 59 || tm2.tm_hour > 24 || tm2.tm_min < 0){
                return false;
            }
        }

        return true;
    }
};



class Program {
public:
    string cong_name;
    string name;
    string type;
    string venue;
    string venueLocation;
    string date;
    string startTime;
    string endTime;
    Program *next;
    Venue *venueList;

    Program(string& congName, string& eventName,string& type, string& venueName, 
        string& venueLocation, string& date, string& start, string& end)
        : cong_name(congName), name(eventName), type(type),venue(venueName), venueLocation(venueLocation),
          date(date), startTime(start), endTime(end), next(nullptr) {
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
    string congregation;
    Venue *next;

    Venue(string& venueName, string& venueLocation,string& venue_type, string& venueCity, 
        string& venueState, string& venuePostalCode, string& venueCountry, 
          int venueCapacity)
        : name(venueName), location(venueLocation),type(venue_type), city(venueCity), state(venueState),
          postalCode(venuePostalCode), country(venueCountry), capacity(venueCapacity),
          congregation("None"), next(nullptr), events(nullptr) {
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


    Congregation(string& congName, string& congType, string& start, string& end)
        : name(congName), type(congType), startDate(start), endDate(end), next(nullptr), reservedVenues(nullptr) {
    }
};

Venue *venueList= nullptr;
Congregation* congregationHead= nullptr;
Program *programHead = nullptr;

    bool doesVenueExists(const string& name, const string& country) {
        Venue* current = venueList;
        while (current != nullptr) {
            if (current->name == name && current->country == country) {
                return true;
            }
            current = current->next;
        }
        return false;
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
        if(doesVenueExist(name, country)) {
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
    void showVenues(string& city, string& state, string& country, string& postal, string& type = "NONE")
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
    bool congExists(const string& congName) {
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
    if(!(Validator::isValidDate(startDate) && Validator::isValidDate(endDate) && endDate >= startDate) || (endDate <= today || startDate < today))
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

    void deleteCongregation(const string& congName) {
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
                venueCurrent->congregation = "None";
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







    void addProgram(const string& cong, const string& prog_name, const string& prog_type, const string& startDate, const string& endDate) {
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

        Program* newProgram = new Program(cong, prog_name, prog_type, "", "", startDate, startDate, endDate);
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
        Congregation* current = congHead;
        
        // Count the number of congregations
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        cout << count << endl;
        
        // Reset current to the head of the list
        current = congHead;
        
        // Print details of each congregation
        while (current != nullptr) {
            cout << current->cong << " " << current->type << " " << current->startDate << " " << current->endDate << endl;
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
    for (Program* reserved_prog = venue->events; reserved_prog != nullptr; reserved_prog = reserved_prog->next) {
        if ((prog_ptr->date >= reserved_prog->date && prog_ptr->date < reserved_prog->endTime) ||
            (prog_ptr->endTime > reserved_prog->date && prog_ptr->endTime <= reserved_prog->endTime) ||
            (prog_ptr->date <= reserved_prog->date && prog_ptr->endTime >= reserved_prog->endTime)) {
            cout << "-1\nError" << endl;
            return;
        }
    }


    // Reserve the venue
    prog_ptr->venue = venue->name;
    prog_ptr->venueLocation = venue->location;
    
    // Add program to venue's events
    if (venue->events == nullptr) {
        venue->events = prog_ptr;
    } else {
        Program* current = venue->events;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = prog_ptr;
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
    if (prog_ptr->venue != venue->name) {
        cout << "-1\nError" << endl;
        return;
    }

    // Remove the program from the venue's events
    Program* prev_event = nullptr;
    for (Program* event = venue->events; event != nullptr; prev_event = event, event = event->next) {
        if (event == prog_ptr) {
            if (prev_event == nullptr) {
                venue->events = event->next;
            } else {
                prev_event->next = event->next;
            }
            break;
        }
    }

    // Clear the venue information from the program
    prog_ptr->venue = "";
    prog_ptr->venueLocation = "";

    // If the venue has no more events, mark it as not reserved
    if (venue->events == nullptr) {
        venue->congregation = "None";
    }

    cout << "0" << endl;
}

    void showReservedVenues(const string cong)
    {
        //check if congregation exists
        bool exists2;
        int foundc;
        exists2 = Congregations::congExists(cong, &foundc, congList);
        //if congregation doesn't exist
        if (!exists2){
            cout << "-1\nError" << endl;
            return;
        }
        //if congregation exists, then printing the venues
        if (exists2){
            cout << congList[foundc].progList.size() << endl;
            
            //printing the programs at that congregation and its reservations
            for (int i=0; i<congList[foundc].progList.size(); i++){
                cout << congList[foundc].progList[i].prog << " " << congList[foundc].progList[i].type << " ";

                //go throught list of venues, count how many reserved for this prog and print the number and thier details
                int count_of_reservations = 0;
                for (int j=0; j<venList.size(); j++){
                    if (venList[j].reserved){
                        for (int k=0; k<venList[j].reservedProg.size(); k++){
                            if (venList[j].reservedProg[k].prog == congList[foundc].progList[i].prog && venList[j].reservedProg[k].cong == cong){
                                count_of_reservations++;
                            }
                        }
                    }
                }

                cout << count_of_reservations << endl;

                if (count_of_reservations > 0){
                    for (int j=0; j<venList.size(); j++){
                        if (venList[j].reserved){
                            for (int k=0; k<venList[j].reservedProg.size(); k++){
                                if (venList[j].reservedProg[k].prog == congList[foundc].progList[i].prog && venList[j].reservedProg[k].cong == cong){
                                    cout << venList[j].name << " " << venList[j].location << " " << venList[j].type << " " << venList[j].capacity << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        return;
    }

   void showPrograms(const string cong){
        //finding congregation
        int foundc = -1;
        for (int i = 0; i < congList.size(); i++){
            if (congList[i].cong == cong) {
                foundc = i;
                break;
            }
        }
        if (foundc == -1){
            cout << "-1\nError" << endl;
            return;
        }

        //number of programs
        cout << congList[foundc].progList.size() << endl;
        //printing the programs
        for (int i = 0; i < congList[foundc].progList.size(); i++){
            cout << congList[foundc].progList[i].type << " " << congList[foundc].progList[i].prog << " " << congList[foundc].progList[i].startDate << " " << congList[foundc].progList[i].endDate << endl;
        }
        return;
    }

};


int main() {
    //for the whole line of input
    string input;
    Extractor E;
    ScheduleManager SM;
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

            SM.callAddCong(cong, type, startDate, endDate);
        }

        else if (cmd == "deleteCongregation"){
            string cong;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            if (iss.fail() || cong.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            SM.callDelCong(cong);

        }
        
        else if (cmd == "showCongregations"){
            SM.showCongregation();
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

            SM.callAddProgram(cong, type, prog, startDate, endDate);
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

            SM.deleteProgram(cong, prog);
        }

        else if (cmd == "showPrograms"){
            string cong;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            if (iss.fail() || cong.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            SM.showPrograms(cong);
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

            SM.callAddVenue(name, loc, type, cap, addr, city, state, postal, country);
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

            SM.deleteVenue(name, country);

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

            SM.callShowVenues(city, state, postal, country, type);
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

            SM.reserveVenue(name, country, cong, prog);
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

            SM.freeVenue(name, country, cong, prog);
        }

        else if (cmd == "showReserved"){
            string cong;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            if (iss.fail() || cong.empty()) {
                cout << "-1\nError" << endl;
                continue;
            }

            SM.showReservedVenues(cong);
        }
    
        else{
            cout << "-1\nError" << endl;
        }
    }
    

    return 0;
}
