#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

const int MAX_DAYS = 30;
const int MAX_HOURS = 24;

class Event {
public:
    Event(const std::string& name, int startHour, int endHour)
        : name(name), startHour(startHour), endHour(endHour) {}

    std::string getName() const { return name; }
    int getStartHour() const { return startHour; }
    int getEndHour() const { return endHour; }

private:
    std::string name;
    int startHour;
    int endHour;
};

class Venue {
public:
    Venue(const std::string& name, const std::string& location, int capacity)
        : name(name), location(location), capacity(capacity), calendar(MAX_DAYS) {}

    std::string getName() const { return name; }
    std::string getLocation() const { return location; }
    int getCapacity() const { return capacity; }

    int addEvent(int date, int fromHour, int toHour, const std::string& eventName) {
        if (date < 1 || date > MAX_DAYS || fromHour < 0 || toHour > MAX_HOURS || fromHour >= toHour) {
            return -1;
        }

        for (const auto& event : calendar[date - 1]) {
            if ((toHour <= event.getEndHour() && toHour > event.getStartHour()) ||
                (fromHour < event.getEndHour() && fromHour >= event.getStartHour())) {
                return -1;
            }
        }

        calendar[date - 1].emplace_back(eventName, fromHour, toHour);
        std::sort(calendar[date - 1].begin(), calendar[date - 1].end(),
                  [](const Event& a, const Event& b) { return a.getStartHour() < b.getStartHour(); });
        return 0;
    }

    int deleteEvent(int date, int fromHour, const std::string& eventName) {
        if (date < 1 || date > MAX_DAYS || fromHour < 0 || fromHour >= MAX_HOURS) {
            return -1;
        }

        auto& events = calendar[date - 1];
        auto it = std::find_if(events.begin(), events.end(),
                               [&](const Event& e) { return e.getStartHour() == fromHour && e.getName() == eventName; });

        if (it == events.end()) {
            return -1;
        }

        events.erase(it);
        return 0;
    }

    void showEvents(int date) const {
        if (date < 1 || date > MAX_DAYS) {
            std::cout << "-1\nError\n";
            return;
        }

        const auto& events = calendar[date - 1];
        std::cout << events.size() << std::endl;
        for (const auto& event : events) {
            std::cout << event.getName() << " " << event.getStartHour() << " " << event.getEndHour() << std::endl;
        }
    }

    void showCalendar() const {
        int totalEvents = 0;
        for (const auto& dayEvents : calendar) {
            totalEvents += dayEvents.size();
        }

        std::cout << totalEvents << std::endl;

        for (int d = 0; d < MAX_DAYS; ++d) {
            const auto& events = calendar[d];
            std::cout << d + 1 << " " << events.size() << std::endl;
            for (const auto& event : events) {
                std::cout << event.getName() << " " << event.getStartHour() << " " << event.getEndHour() << std::endl;
            }
        }
    }

private:
    std::string name;
    std::string location;
    int capacity;
    std::vector<std::vector<Event>> calendar;
};

class VenueManager {
public:
    int addVenue(const std::string& name, const std::string& location, int capacity) {
        auto it = std::find_if(venues.begin(), venues.end(),
                               [&](const Venue& v) { return v.getName() == name; });
        if (it != venues.end()) {
            return -1;
        }

        venues.emplace_back(name, location, capacity);
        std::sort(venues.begin(), venues.end(),
                  [](const Venue& a, const Venue& b) { return a.getName() < b.getName(); });
        return 0;
    }

    int deleteVenue(const std::string& name) {
        auto it = std::find_if(venues.begin(), venues.end(),
                               [&](const Venue& v) { return v.getName() == name; });
        if (it == venues.end()) {
            return -1;
        }

        venues.erase(it);
        return 0;
    }

    void showVenues() const {
        std::cout << venues.size() << std::endl;
        for (const auto& venue : venues) {
            std::cout << venue.getName() << " " << venue.getLocation() << " " << venue.getCapacity() << std::endl;
        }
    }

    int addEvent(const std::string& venueName, int date, int fromHour, int toHour, const std::string& eventName) {
        auto it = findVenue(venueName);
        if (it == venues.end()) {
            return -1;
        }
        return it->addEvent(date, fromHour, toHour, eventName);
    }

    int deleteEvent(const std::string& venueName, int date, int fromHour, const std::string& eventName) {
        auto it = findVenue(venueName);
        if (it == venues.end()) {
            return -1;
        }
        return it->deleteEvent(date, fromHour, eventName);
    }

    void showEvents(const std::string& venueName, int date) const {
        auto it = findVenue(venueName);
        if (it == venues.end()) {
            std::cout << "-1\nError\n";
            return;
        }
        it->showEvents(date);
    }

    void showCalendar(const std::string& venueName) const {
        auto it = findVenue(venueName);
        if (it == venues.end()) {
            std::cout << "-1\nError\n";
            return;
        }
        it->showCalendar();
    }

private:
    std::vector<Venue> venues;

    std::vector<Venue>::iterator findVenue(const std::string& name) {
        return std::find_if(venues.begin(), venues.end(),
                            [&](const Venue& v) { return v.getName() == name; });
    }

    std::vector<Venue>::const_iterator findVenue(const std::string& name) const {
        return std::find_if(venues.begin(), venues.end(),
                            [&](const Venue& v) { return v.getName() == name; });
    }
};

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

int main() {
    VenueManager manager;
    std::string input;

    while (true) {
        std::getline(std::cin, input);
        input = trim(input);

        if (input == "End") break;

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "addVenue") {
            std::string name, location;
            int capacity;
            iss.ignore();
            std::getline(iss, name, '"');
            iss.ignore();
            std::getline(iss, location, '"');
            iss >> capacity;

            int result = manager.addVenue(name, location, capacity);
            std::cout << result << std::endl;
            if (result == -1) std::cout << "Error\n";
        }
        else if (command == "delVenue") {
            std::string name;
            iss.ignore();
            std::getline(iss, name, '"');

            int result = manager.deleteVenue(name);
            std::cout << result << std::endl;
            if (result == -1) std::cout << "Error\n";
        }
        else if (command == "showVenues") {
            manager.showVenues();
        }
        else if (command == "addEvent") {
            std::string venueName, eventName;
            int date, fromHour, toHour;
            iss.ignore();
            std::getline(iss, venueName, '"');
            iss >> date >> fromHour >> toHour;
            iss.ignore();
            std::getline(iss, eventName, '"');

            int result = manager.addEvent(venueName, date, fromHour, toHour, eventName);
            std::cout << result << std::endl;
            if (result == -1) std::cout << "Error\n";
        }
        else if (command == "delEvent") {
            std::string venueName, eventName;
            int date, fromHour;
            iss.ignore();
            std::getline(iss, venueName, '"');
            iss >> date >> fromHour;
            iss.ignore();
            std::getline(iss, eventName, '"');

            int result = manager.deleteEvent(venueName, date, fromHour, eventName);
            std::cout << result << std::endl;
            if (result == -1) std::cout << "Error\n";
        }
        else if (command == "showEvents") {
            std::string venueName;
            int date;
            iss.ignore();
            std::getline(iss, venueName, '"');
            iss >> date;

            manager.showEvents(venueName, date);
        }
        else if (command == "showCalendar") {
            std::string venueName;
            iss.ignore();
            std::getline(iss, venueName, '"');

            manager.showCalendar(venueName);
        }
        else {
            std::cout << "-1\nError\n";
        }
    }

    return 0;
}
