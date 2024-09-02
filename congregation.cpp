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


// time_t currdate = time(nullptr); // Get current time
// char currdate_1[11]; 

// // Format the current date as "YYYY-MM-DD"
// strftime(currdate_1, sizeof(currdate_1), "%Y-%m-%d", localtime(&currdate));
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



class Event {
public:
    string cong_name;
    string name;
    string venue;
    string venueLocation;
    string date;
    string startTime;
    string endTime;
    Event *next;

    Event(string& congName, string& eventName, string& venueName, 
        string& venueLocation, string& date, string& start, string& end)
        : cong_name(congName), name(eventName), venue(venueName), venueLocation(venueLocation),
          date(date), startTime(start), endTime(end), next(nullptr) {
    }
};



class Venue {
public:
    string name;
    string location;
    string city;
    string state;
    string postalCode;
    string country;
    int capacity;
    string congregation;
    Venue *next;
    Event *events;

    Venue(string& venueName, string& venueLocation, string& venueCity, 
        string& venueState, string& venuePostalCode, string& venueCountry, 
          int venueCapacity)
        : name(venueName), location(venueLocation), city(venueCity), state(venueState),
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
    Venue* reservedVenues;


    Congregation(string& congName, string& congType, string& start, string& end)
        : name(congName), type(congType), startDate(start), endDate(end), next(nullptr), reservedVenues(nullptr) {
    }
};



// now making a class for venueManager that will deal with all the query
class VenueManager {
public:

    Venue *venueList= nullptr;
    Congregation* congregationHead= nullptr;//for diffrent types of congregations
    // okay let me just make the findVenue thingy. 

    vector<string> findVenue_inReserved(string& name, string& country) {
        int flag = 0;
        vector<string>ans;
        Congregation* currentCong = congregationHead;
        while (currentCong != nullptr) {
            Venue* currentVenue = currentCong->reservedVenues;
            while (currentVenue != nullptr) {
                if (currentVenue->name==name && currentVenue->country==country) {
                    flag = 1;
                }
                currentVenue = currentVenue->next;
            }
            if(flag == 1) {
                ans.push_back(currentCong->name);
                flag = 0;
            }
            currentCong = currentCong->next;
        }
        return ans;
    }


    bool doesVenueExist(string& name, string& country){
        Venue* current = venueList;
        while (current != nullptr) {
            if (current->name==name && current->country==country) {
                return true;
            }
            current = current->next;
        }
        return false;
    }



    //assuming i have a linked list of congregation then i will try to find the congregation through it 
    Congregation* findCongregation( string& name) {

        Congregation* current = congregationHead;
        while (current != nullptr) {
            if (current->name==name) {
                return current; // Found the congregation
            }
            current = current->next;
        }
        return nullptr; // Congregation not found
    }



    void freeVenue(string& venueName, string& country, string& congregationName) {
        Congregation* cong = findCongregation(congregationName);
        if (cong == nullptr) {
            cout << "-1\nError\n";
            return;
        }

        Venue* prev = nullptr;
        Venue* current = cong->reservedVenues;

        // Locate the venue in the congregation's reserved venues
        while (current != nullptr && (current->name != venueName || current->country != country)) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "-1\nError\n";
            return;
        }
        // Delete all events in the venue
        Event* currentEvent = current->events;
        while (currentEvent != nullptr) {
            Event* nextEvent = currentEvent->next;
            delete currentEvent;
            currentEvent = nextEvent;
        }

        // Remove the venue from the congregation's list
        if (prev == nullptr) {
            cong->reservedVenues = current->next;
        } else {
            prev->next = current->next;
        }

        // Reset the venue's congregation and events
        current->congregation = "None";
        current->events = nullptr;

        cout << "0\n";
        return;
    }



    // if i need to add congregation i will add  if valid 
    void addCongregation(string& name, string& type, string& startDate, string& endDate) {
        if(!(Validator::isValidDate(startDate) && Validator::isValidDate(endDate) && endDate >= startDate )||(endDate <= today || startDate<today))
        {
            cout << "-1\nError\n";
            return;
        }
        // if congregation already exists , it should return nullptr
        if (findCongregation(name) != nullptr) {
            cout << "-1\nError\n";
            return;
        }
        // is DateTime // let this be a black box
        if (!(type == "Concert" || type=="Games" || type == "Convention" || type=="Conference")) {
            cout << "-1\nError\n";
            return;// we will try to restrict the types to the Concert Games Convention Conference 
        }
        // if no problems 
        Congregation* newCongregation = new Congregation(name, type, startDate, endDate);
        if(newCongregation==nullptr)
        {
            cout<<"-1\nError\n";
            return;
        }
        Congregation *current = congregationHead;
        if (congregationHead == nullptr) {
            congregationHead = newCongregation;
            cout << "0\n";
            return;
        }
        while(current->next!= nullptr)
        {
            current = current->next;
        }
        current->next = newCongregation;
        // i will append the congregation  in the end, in the congregation
        cout << "0\n";//added successfully
        return;
    }



    void deleteCongregation(string& name) {

        if (congregationHead == nullptr) {
            cout << "-1\nError\n";
            return;
        }
        Congregation* prev = nullptr;
        Congregation* current = congregationHead;
        while (current != nullptr && current->name != name) {
            prev = current;
            current = current->next;
        }
        if (current == nullptr) {
            cout << "-1\nError\n";
            return;
        }
        // Delete all venues from the venue list
        Venue* currentVenue = current->reservedVenues;
        while (currentVenue != nullptr) {
            Venue* nextVenue = currentVenue->next;
            freeVenue(currentVenue->name, currentVenue->country, current->name);
            currentVenue = nextVenue;
        }
        // Update the linked list
        if (prev == nullptr) {
            // If the congregation to delete is the head
            congregationHead = current->next;
        } else {
            prev->next = current->next;
        }
        if (current->next != nullptr) {
            current->next= nullptr;
        }
        // Free the congregation
        delete current;
        cout << "0\n";
        return;
    }



    void showCongregations() {
        int count = 0;
        Congregation* current = congregationHead;
        // Count the number of congregations
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        // Print the count
        cout << count << "\n";
        // Reset current to the head to print details
        current = congregationHead;

        while (current != nullptr) {
            cout << current->name << " " << current->type << " "
                << current->startDate<<" "<< current->endDate<<"\n";
            current = current->next;
        }
    }
        


    void addVenue(string& name,string& location,string& city,string& state,string& postalCode,string& country,int capacity)
    {
        if(venueList==nullptr)
        {
            venueList = new Venue(name, location, city, state, postalCode, country, capacity);
            cout << "0\n";
            return;
        }
        if(doesVenueExist(name, country)) {
            cout << "-1\nError\n";
            return;
        }

        Venue* newVenue = new  Venue(name, location, city, state, postalCode, country, capacity);
        Venue *current = venueList;
        
        while(current->next!=nullptr){
            current = current->next;
        }
        current->next = newVenue;
        cout << "0\n";
        return;
    }

    void deleteVenue( string& name,  string& country) {
        Venue* prev = nullptr;
        Venue* current = venueList;
        if(doesVenueExist(name, country))
        {
            vector<string>temp = findVenue_inReserved(name, country);
            if(temp.size()==0)
            {//aah it isn't reserved
                while(current!= nullptr && current->name!= name && current->country!= country)
                {
                    prev = current;
                    current = current->next;
                }
                if(current == nullptr)
                {
                    cout << "-1\nError\n";
                    return;
                }
                if(prev == nullptr)
                {
                    venueList = current->next;
                }
                else{
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            else 
            {
                cout << "-1\nError\n";
                return;
            }
        }
    }


    void showVenues(string& city, string& state, string& country, string& postal)
    {
        Venue* current = venueList;
        if(city=="" && state== "" && postal=="" && country==""){
            cout<<"-1\nError\n";
        }
        else if(country=="")
        {
            cout<<"-1\nError\n";
        }
        if(state=="" && !(city==""))
        {

            cout<<"-1\nError\n";
        }

        vector<Venue*> temp;
        if(city=="" && state=="" && postal=="")
        {
            while(current!= nullptr)
            {
                if(current->country == country)
                {
                    temp.push_back(current);
                }
                current = current->next;
            }
        }
        else if(postal=="")
        {
            current = venueList;
            while(current!= nullptr)
            {
                if(current->city == city && current->country == country && current->state == state)
                {
                    temp.push_back(current);
                }
                current = current->next;
            }
        }
        else if(city=="" && state=="")
        {
            current = venueList;
            while(current!= nullptr)
                {
                    if(current->postalCode == postal && current->country == country)
                    {
                        temp.push_back(current);
                    }
                    current = current->next;
                }
        }
        else{
            current = venueList;
            while(current!=nullptr)
            {
                if(current->city == city && current->state == state && current->country == country && current->postalCode == postal)
                {
                    temp.push_back(current);
                }
                current = current->next;
            }
        }

        cout << temp.size() << "\n";
            for(size_t i = 0; i < temp.size(); i++)
            {
                cout << temp[i]->name << " " << temp[i]->location << " " << temp[i]->capacity << "\n";
            }

    }


    void reserveVenue(string& venueName, string& country, string& congregationName) {
    // Step 1: Check if the venue exists in the venue list.
        Venue* venue = nullptr;
        Venue* current = venueList;
        while (current != nullptr) {
            if (current->name == venueName && current->country == country) {
                venue = current;                
            }
            current = current->next;
        }
        if (venue == nullptr) {
            cout << "-1\nError\n";
            return;
        }

        // Step 2: Check if the congregation actually exists
        Congregation* congregation = findCongregation(congregationName);
        if (congregation == nullptr) {
            cout << "-1\nError\n";
            return;
        }

        // Step 3: Check if the venue is already reserved in any other congregation.
        vector<string> congregationNames = findVenue_inReserved(venueName, country);
        bool overlap = false;
        for (auto& otherCongName : congregationNames) {
            Congregation* otherCong = findCongregation(otherCongName);
            if (otherCong && otherCong != congregation) {
                if (!(congregation->endDate < otherCong->startDate ||  otherCong->endDate < congregation->startDate)) {
                    overlap = true;
                    break;
                }
            }
        }

        if (overlap) {
            cout << "-1\nError\n";
            return;
        }


        Venue *copied = new Venue(venue->name, venue->location, venue->city, venue->state, venue->postalCode, venue->country, venue->capacity);
        // Step 4: Reserve the venue
        if (congregation->reservedVenues == nullptr) {
            congregation->reservedVenues = copied;
        } else {
            Venue* lastVenue = congregation->reservedVenues;
            while (lastVenue->next != nullptr) {
                lastVenue = lastVenue->next;
            }
            lastVenue->next = copied;
        }
        copied->next = nullptr;
        cout << "0\n";
        return;
    }


    void showReserved(string& congregationName) {
        Congregation* cong = findCongregation(congregationName);
        if (cong == nullptr) {
            cout << "-1\nError\n";
            return;
        }

        int count = 0;
        Venue* current = cong->reservedVenues;
        while (current != nullptr) {
            count++;
            current = current->next;
        }

        cout << count << "\n";

        current = cong->reservedVenues;
        while (current != nullptr) {
            cout << current->name << " " << current->location << " " << current->capacity << "\n";
            current = current->next;
        }
    }

    void addEvent(string& congregationName, string& venueName, string& venueCountry,string& date, string& fromTime, string& toTime, string& eventName) {
            
            // Check if the venue exists
            Venue* venue = nullptr;
            Venue* currentVenueList = venueList;
            while (currentVenueList != nullptr) {
                if (currentVenueList->name == venueName && currentVenueList->country == venueCountry) {
                    venue = currentVenueList;
                    break;
                }
                currentVenueList = currentVenueList->next;
            }

            if (venue == nullptr) {
                cout << "-1\nError\n";
                return;
            }

            // Check if the congregation exists
            Congregation* cong = findCongregation(congregationName);
            if (cong == nullptr) {
                cout << "-1\nError\n";
                return;
            }

            // Check if the venue is reserved for this congregation
            bool isReserved = false;
            Venue* currentVenue = cong->reservedVenues;
            while (currentVenue != nullptr) {
                if (currentVenue->name == venueName && currentVenue->country == venueCountry) {
                    isReserved = true;
                    break;
                }
                currentVenue = currentVenue->next;
            }

            if (!isReserved) {
                cout << "-1\nError\n";//not reserved
                return;
            }
            else{
            // Parse and validate date and times
                // Check if event is within congregation dates
                if (date < cong->startDate || date > cong->endDate) {
                    cout << "-1\nError\n"; //date out of range
                    return;
                }
        
                //checking if the event is already present in the event list
                Event* currentEvent = currentVenue->events;
                while (currentEvent != nullptr) {
                    if (currentEvent->date == date && currentEvent->name == eventName &&  currentEvent->startTime == fromTime && currentEvent->endTime == toTime) {
                        cout << "-1\nError\n";
                        return;
                    }
                    currentEvent = currentEvent->next;
                }


                //checking that the start time and end time are valid
                if (Validator::isValidTime(fromTime, toTime) == false){
                    cout << "-1\nError" << endl;
                    return;
                }
                //checking if the date is valid
                if (Validator::isValidDate(date) == false){
                    cout << "-1\nError " << endl;
                    return;
                }
                //checking if endtime <= starttime, which should be an error
                if (fromTime >= toTime){
                    cout << "-1\nError" << endl; 
                    return;
                }
                //checking that the times are aligned with the 15 minute boundary
                if (Validator::timebound(fromTime, toTime) == false){
                    cout << "-1\nError" << endl;
                    return;
                }
                //checking that the duration of the event is min 30 minutes and max 24 hrs (1440 minutes)
                int gaptime = Validator::timediff(date, toTime, date, fromTime);
                if (gaptime < 30 || gaptime > 1440){
                    cout << "-1\nError" << endl; 
                    return;
                } 

                // Check for conflicts with existing events
                currentEvent = currentVenue->events;
                while (currentEvent != nullptr) {
                    if (currentEvent->date == date) {
                            if ((fromTime >= currentEvent->startTime && fromTime < currentEvent->endTime) || 
                                (toTime > currentEvent->startTime && toTime <= currentEvent->endTime) ||
                                (fromTime <= currentEvent->startTime && toTime >= currentEvent->endTime)){
                                cout << "-1\nError" << endl;
                                return;
                            }
                    }
                    currentEvent = currentEvent->next;
                }
                //checking that the gap between this event and all existing events is >= 30 minutes
                //also this makes sure that for events on different days, the time difference is at least 30 minutes
                currentEvent = currentVenue->events;
                while(currentEvent!=nullptr)
                {
                                    //the time gap should be 30 mins between the end of an existing event, and start of this event (timegap_EtoS)
                    //and between the start of an existing event and end of this event (timegap_StoE)

                    int timegap_StoE, timegap_EtoS;
                    //timediff calculates timeStr1 - timeStr2, so timeStr1 should be after timeStr2 for positive value
                    //so, checking the dates first, and then based on that, passing the arguments

                    //if date of new event is after the date of existing event
                    if (date > currentEvent->date){
                        timegap_StoE = Validator::timediff(date, toTime, currentEvent->date, currentEvent->startTime);
                        timegap_EtoS = Validator::timediff(date, fromTime, currentEvent->date, currentEvent->endTime);
                        if (timegap_EtoS < 30 || timegap_StoE < 30){
                            cout << "-1\nError" << endl;
                            return;
                        }
                    }

                    //if date of new event is before the date of existing event
                    else if (date < currentEvent->date){
                        timegap_StoE = Validator::timediff(currentEvent->date, currentEvent->startTime, date, toTime);
                        timegap_EtoS = Validator::timediff(currentEvent->date, currentEvent->endTime,date, fromTime);

                        if (timegap_EtoS < 30 || timegap_StoE < 30){
                            cout << "-1\nError" << endl;
                            return;
                        }
                    }

                    //if the dates are the same
                    else{
                        //if they are on the same day, the time difference must be based on which is greater

                        //for timegap_StoE
                        //if the start time of the existing event is after the end time of the new event, then the timegap is the difference between the start time of the existing event and the end time of the new event
                        if (currentEvent->startTime > toTime){
                            timegap_EtoS = Validator::timediff(currentEvent->date, currentEvent->startTime, date, toTime);
                        }
                        //else, the timegap is the difference between the end time of the new event and the start time of the existing event
                        else{
                            timegap_EtoS = Validator::timediff(date, toTime, currentEvent->date, currentEvent->startTime);
                        }

                        //for timegap_EtoS
                        //if the end time of the existing event is after the start time of the new event, then the timegap is the difference between the end time of the existing event and the start time of the new event
                        if (currentEvent->endTime > fromTime){
                            timegap_StoE = Validator::timediff(currentEvent->date, currentEvent->endTime, date, fromTime);
                        }
                        //else, the timegap is the difference between the start time of the new event and the end time of the existing event
                        else{
                            timegap_StoE = Validator::timediff(date, fromTime, currentEvent->date, currentEvent->endTime);
                        }
                        if (timegap_EtoS < 30 || timegap_StoE < 30){
                            cout << "-1\nError" << endl;
                            // cout<<"here is the error "<<endl;
                            return;
                        }
                    }
                    currentEvent = currentEvent->next;
                }
                // Add the new event
                Event* newEvent = new Event(congregationName, eventName, venueName, venue->location, date, fromTime, toTime);
                // Find the correct position to insert the new event
                Event* current = currentVenue->events;
                if(currentVenue->events == nullptr)
                {
                    currentVenue->events = newEvent;
                    cout<<"0\n";
                    return;
                }
                Event* prev = nullptr;
                while (current != nullptr && (current->date < date || 
                    (current->date == date && current->startTime < fromTime))) {
                    prev = current;
                    current = current->next;
                }

                // Insert the new event in the correct position
                newEvent->next = current;
                if (prev == nullptr) {
                    currentVenue->events = newEvent;
                } else {
                    prev->next = newEvent;
                }
                cout << "0\n";
                return;
            }
        return;
    }


    void deleteEvent(string& congregationName, string& venueName, string& venueCountry, string& date, string& startTime, string& eventName) {
        // Find the congregation
        if(!(Validator::isValidDate(date))) {
            cout << "-1\nError\n";
            return;
        }
        Congregation* cong = findCongregation(congregationName);
        if (cong == nullptr) {
            cout << "-1\nError\n";
            return;
        }

        // Find the venue in the congregation's reserved venues
        Venue* venue = nullptr;
        Venue* currentVenue = cong->reservedVenues;
        while (currentVenue != nullptr) {
            if (currentVenue->name == venueName && currentVenue->country == venueCountry) {
                venue = currentVenue;
                break;
            }
            currentVenue = currentVenue->next;
        }

        if (venue == nullptr) {
            cout << "-1\nError\n";
            return;
        }
        
        // Find and delete the event
        Event* prevEvent = nullptr;
        Event* currentEvent = venue->events;
        while (currentEvent != nullptr) {
            if (currentEvent->date == date && currentEvent->startTime == startTime && currentEvent->name == eventName) {
                // Found the event, delete it
                if (prevEvent == nullptr) {
                    venue->events = currentEvent->next;
                } else {
                    prevEvent->next = currentEvent->next;
                }
                delete currentEvent;
                cout << "0\n";
                return;
            }
            prevEvent = currentEvent;
            currentEvent = currentEvent->next;
        }

        // Event not found
        cout << "-1\nError\n";
        return;
    }
void showEvents(string& venueName, string& venueCountry, string& date) {
    vector<string> congregations = findVenue_inReserved(venueName, venueCountry);
    vector<Event*> eventsOnDate;
    bool validCongregationFound = false;

    for (auto& congName : congregations) {
        Congregation* cong = findCongregation(congName);
        if (cong && date >= cong->startDate && date <= cong->endDate) {
            validCongregationFound = true;
            Venue* venue = nullptr;
            Venue* currentVenue = cong->reservedVenues;
            while (currentVenue != nullptr) {
                if (currentVenue->name == venueName && currentVenue->country == venueCountry) {
                    venue = currentVenue;
                    break;
                }
                currentVenue = currentVenue->next;
            }

            if (venue) {
                Event* currentEvent = venue->events;
                while (currentEvent != nullptr) {
                    if (currentEvent->date == date) {
                        eventsOnDate.push_back(currentEvent);
                    }
                    currentEvent = currentEvent->next;
                }
            }
        }
    }
    if (!validCongregationFound) {
        cout << "-1\nError\n";
        return;
    }

    sort(eventsOnDate.begin(), eventsOnDate.end(),
        [](const Event* a, const Event* b) {
            if (a->startTime == b->startTime) {
                return a->name < b->name;
            }
            return a->startTime < b->startTime;
        });

    cout << eventsOnDate.size() << "\n";
    for (const Event* event : eventsOnDate) {
        cout << event->name << " " << event->startTime << " " << event->endTime << "\n";
    }
}


void showCalendar(string& congregationName, string& venueName, string& venueCountry) {
        Congregation* cong = findCongregation(congregationName);
        if (!cong) {
            cout << "-1\nError\n";
            return;
        }
        Venue* venue = nullptr;
        Venue* currentVenue = cong->reservedVenues;
        while (currentVenue != nullptr) {
            if (currentVenue->name == venueName && currentVenue->country == venueCountry) {
                venue = currentVenue;
                break;
            }
            currentVenue = currentVenue->next;
        }

        if (!venue) {
            cout << "-1\nError\n";
            return;
        }

        vector<Event*> congregationEvents;
        Event* currentEvent = venue->events;
        while (currentEvent != nullptr) {
                congregationEvents.push_back(currentEvent);
            currentEvent = currentEvent->next;
        }

        cout << congregationEvents.size() << "\n";

        string currentDate = cong->startDate;
        while (currentDate <= cong->endDate) {
            int eventsOnDate = 0;
            for (Event* event : congregationEvents) {
                if (event->date == currentDate) {
                    eventsOnDate++;
                }
            }

            cout << currentDate << " " << eventsOnDate << "\n";

            for (Event* event : congregationEvents) {
                if (event->date == currentDate) {
                    cout << event->name << " " << event->startTime << " " << event->endTime << "\n";
                }
            }

            // Move to next day
            currentDate = incrementDate(currentDate);
        }
    }

    string incrementDate(const string& date) {
        int year, month, day;
        sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
        
        day++;
        if (day > daysInMonth(year, month)) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
        
        char newDate[11];
        sprintf(newDate, "%04d-%02d-%02d", year, month, day);
        return string(newDate);
    }

    int daysInMonth(int year, int month) {
        static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
            return 29;
        return days[month - 1];
    }
};


int main()
{
    //inp is the whole line containing command, and the arguments
    string inp;
    Extractor E;
    VenueManager M;
    while (getline(cin, inp))
    {
        istringstream iss(inp);
        
        //cmd is the command 
        string cmd;
        iss >> cmd;
        
        if (cmd == "End")
        {
            break;
        } 

        else if (cmd == "addVenue")
        {
            //name, location, capacity  
            string name, loc, addr, city, state, postal, country;
            int cap = -1;

            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, loc, '\"');
            getline(iss, loc, '\"');
            iss >> cap;
            //getting the address, city, state, postal, country from the location
            E.getaddr(loc, &addr, &city, &state, &postal, &country);

            //error if input format is wrong
            if (iss.fail() || name.empty() || loc.empty() || addr.empty() || city.empty() || state.empty() || postal.empty() || country.empty()){
                cout << "-1\nError" << endl;
                continue;
            }

            M.addVenue(name, loc,city,state, postal, country,cap);
        }

        else if (cmd == "deleteVenue")
        {
            string name;
            string country;

            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');

            //error if input format is wrong
            if (iss.fail() || name.empty() || country.empty()){
                cout << "-1\nError" << endl;
                continue;
            }

            M.deleteVenue(name, country);
        } 

        else if (cmd == "showVenues")
        {
            string loc, city, state, postal, country;
            getline(iss, loc, '\"');
            getline(iss, loc, '\"');

            //getting the city, state, postal, country from the location
            E.getaddrshowvenues(loc, &city, &state, &postal, &country);

            //error if input format is wrong
            if (iss.fail() || loc.empty()){
                cout << "-1\nError" << endl;
                continue;
            }
            M.showVenues(city, state, country, postal);
        } 

        else if (cmd == "reserveVenue")
        {
            string name, cong, country;
            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            //error if input format is wrong
            if (iss.fail() || name.empty() || country.empty() || cong.empty()){
                cout << "-1\nError" << endl;
                continue;
            }

            M.reserveVenue(name, country, cong);

        }

        else if (cmd == "freeVenue")
        {
            string name, cong, country;
            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            //error if input format is wrong
            if (iss.fail() || name.empty() || country.empty() || cong.empty()){
                cout << "-1\nError" << endl;
                continue;
            }

            M.freeVenue(name, country, cong);

        }

        else if (cmd == "showReserved")
        {
            string cong;
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            //error if input format is wrong
            if (iss.fail() || cong.empty()){
                cout << "-1\nError" << endl;
                continue;
            }

            M.showReserved(cong);
        }

        else if (cmd == "addEvent")
        {
            string cong, name, eventN, date, stTime, endTime, country;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');
            getline(iss, date, '\"');
            getline(iss, date, '\"');
            getline(iss, stTime, '\"');
            getline(iss, stTime, '\"');
            getline(iss, endTime, '\"');
            getline(iss, endTime, '\"');
            getline(iss, eventN, '\"');
            getline(iss, eventN, '\"');

            //getting the value of today's date
            time_t currdate = time(nullptr); // Get current time
            char currdate_1[11]; 

            // Format the current date as "YYYY-MM-DD"
            strftime(currdate_1, sizeof(currdate_1), "%Y-%m-%d", localtime(&currdate));
            string today = currdate_1;
            
            ///error if input format is wrong
            if (iss.fail() || cong.empty() || name.empty() || eventN.empty() || date.empty() || stTime.empty() || endTime.empty() || country.empty()){
                cout << "-1\nError" << endl;
                continue;
            }
            
            //event is in the past
            if (date < today){
                cout << "-1\nError" << endl;
                continue;
            }

            M.addEvent(cong, name, country, date, stTime, endTime, eventN);
        } 

        else if (cmd == "deleteEvent")
        {
            string cong, name, eventN, date, stTime, country;
            
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');
            getline(iss, date, '\"');
            getline(iss, date, '\"');
            getline(iss, stTime, '\"');
            getline(iss, stTime, '\"');
            getline(iss, eventN, '\"');
            getline(iss, eventN, '\"');

            time_t currdate = time(nullptr); // Get current time
            char currdate_1[11]; 

            // Format the current date as "YYYY-MM-DD"
            strftime(currdate_1, sizeof(currdate_1), "%Y-%m-%d", localtime(&currdate));
            string today = currdate_1;
            
            //error if input format is wrong
            if (iss.fail() || cong.empty() || name.empty() || eventN.empty() || date.empty() || stTime.empty() || country.empty()){
                cout << "-1\nError" << endl;
                continue;
            }
            
            //event is in the past
            if (date < today){
                cout << "-1\nError" << endl;
                continue;
            }

            M.deleteEvent(cong, name, country, date, stTime, eventN);
            
        } 

        else if (cmd == "showEvents")
        {
            string name, date, country;

            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');
            getline(iss, date, '\"');
            getline(iss, date, '\"');

            time_t currdate = time(nullptr); // Get current time
            char currdate_1[11]; 

            // Format the current date as "YYYY-MM-DD"
            strftime(currdate_1, sizeof(currdate_1), "%Y-%m-%d", localtime(&currdate));
            string today = currdate_1;

            //error if input format is wrong
            if (iss.fail() || name.empty() || date.empty() || country.empty()){
                cout << "-1\nError" << endl;
                continue;
            }
            
            //event is in the past
            if (date < today){
                cout << "-1\nError" << endl;
                continue;
            }

            M.showEvents(name, country, date);

        } 

        else if (cmd == "showCalendar")
        {
            string name, cong, country;
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, name, '\"');
            getline(iss, name, '\"');
            getline(iss, country, '\"');
            getline(iss, country, '\"');

            //error if input format is wrong
            if (iss.fail() || name.empty() || cong.empty() || country.empty()){
                cout << "input fail" << endl;
                cout << "-1\nError" << endl;
                continue;
            }

            M.showCalendar(cong, name, country);
        } 

        else if (cmd == "addCongregation")
        {
            string cong, type, stDate, enDate;
            
            time_t currdate = time(nullptr); // Get current time
            char currdate_1[11]; 
            // Format the current date as "YYYY-MM-DD"
            strftime(currdate_1, sizeof(currdate_1), "%Y-%m-%d", localtime(&currdate));
            string today = currdate_1;
            
            getline(iss, cong, '\"');
            getline(iss, cong, '\"');
            getline(iss, type, '\"');
            getline(iss, type, '\"');
            getline(iss, stDate, '\"');
            getline(iss, stDate, '\"');
            getline(iss, enDate, '\"');
            getline(iss, enDate, '\"');

            //error if start date is after end date or end date is before today
            if (stDate > enDate) {
                cout << "-1\nError" << endl;
                continue;
            }
            if (enDate < today){
                cout << "-1\nError" << endl;
                continue;
            }

            //error if input format is wrong
            if (iss.fail() || cong.empty() || type.empty() || stDate.empty() || enDate.empty()){
                cout << "-1\nError" << endl;
                continue;
            }

            M.addCongregation(cong, type, stDate, enDate);
        }

        else if (cmd == "deleteCongregation")
        {
            string cong;

            getline(iss, cong, '\"');
            getline(iss, cong, '\"');

            //error if input format is wrong
            if (iss.fail() || cong.empty()){
                cout << "-1\nError" << endl;
                continue;
            }
            M.deleteCongregation(cong);
        }

        else if (cmd == "showCongregations")
        {
            M.showCongregations();
        }

        else
        {
            //error if no such command
            cout << "-1\nError" << endl;
        }
    }
    return 0;
}
