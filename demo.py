import sys

class Venue:
    def __init__(self, name, location, capacity):
        self.name = name
        self.location = location
        self.capacity = capacity
        self.calendar = {day: {} for day in range(1, 31)}

class OlympicsScheduler:
    def __init__(self):
        self.venues = {}

    def add_venue(self, name, location, capacity):
        if name in self.venues:
            return -1, "Error: venue already exists"
        try:
            self.venues[name] = Venue(name, location, int(capacity))
            return 0, None
        except ValueError:
            return -1, "Error: invalid capacity"

    def delete_venue(self, name):
        if name not in self.venues:
            return -1, "Error: venue does not exist"
        del self.venues[name]
        return 0, None

    def show_venues(self):
        if not self.venues:
            return "0"
        result = f"{len(self.venues)}\n"
        for venue in self.venues.values():
            result += f"{venue.name} {venue.location} {venue.capacity}\n"
        return result.strip()

    def add_event(self, venue_name, date, from_hour, to_hour, event_name):
        if venue_name not in self.venues:
            return -1, "Error: venue does not exist"
        venue = self.venues[venue_name]
        try:
            date = int(date)
            from_hour = int(from_hour)
            to_hour = int(to_hour)
        except ValueError:
            return -1, "Error: invalid date or hour"
        
        if date < 1 or date > 30 or from_hour < 0 or to_hour > 23 or from_hour >= to_hour:
            return -1, "Error: invalid date or hour range"
        
        for hour in range(from_hour, to_hour):
            if hour in venue.calendar[date]:
                return -1, f"Error: time slot {hour} already occupied"
        
        for hour in range(from_hour, to_hour):
            venue.calendar[date][hour] = event_name
        return 0, None

    def delete_event(self, venue_name, date, from_hour, event_name):
        if venue_name not in self.venues:
            return -1, "Error: venue does not exist"
        venue = self.venues[venue_name]
        try:
            date = int(date)
            from_hour = int(from_hour)
        except ValueError:
            return -1, "Error: invalid date or hour"
        
        if date < 1 or date > 30 or from_hour < 0 or from_hour > 23:
            return -1, "Error: invalid date or hour"
        
        if from_hour not in venue.calendar[date] or venue.calendar[date][from_hour] != event_name:
            return -1, f"Error: no matching event (on date {date} starting at hour {from_hour})"
        
        to_hour = from_hour + 1
        while to_hour in venue.calendar[date] and venue.calendar[date][to_hour] == event_name:
            to_hour += 1
        
        for hour in range(from_hour, to_hour):
            del venue.calendar[date][hour]
        return 0, None

    def show_events(self, venue_name, date):
        if venue_name not in self.venues:
            return -1, "Error: venue does not exist"
        venue = self.venues[venue_name]
        try:
            date = int(date)
        except ValueError:
            return -1, "Error: invalid date"
        
        if date < 1 or date > 30:
            return -1, "Error: invalid date"
        
        events = []
        for hour in sorted(venue.calendar[date].keys()):
            event_name = venue.calendar[date][hour]
            if not events or events[-1][0] != event_name:
                events.append([event_name, hour, hour + 1])
            else:
                events[-1][2] = hour + 1
        
        result = f"{len(events)}\n"
        for event in events:
            result += f"{event[0]} {event[1]} {event[2]}\n"
        return result.strip()

    def show_calendar(self, venue_name):
        if venue_name not in self.venues:
            return -1, "Error: venue does not exist"
        venue = self.venues[venue_name]
        
        total_events = sum(len(day_events) for day_events in venue.calendar.values())
        result = f"{total_events}\n"
        
        for date in range(1, 31):
            events = []
            for hour in sorted(venue.calendar[date].keys()):
                event_name = venue.calendar[date][hour]
                if not events or events[-1][0] != event_name:
                    events.append([event_name, hour, hour + 1])
                else:
                    events[-1][2] = hour + 1
            
            result += f"{date} {len(events)}\n"
            for event in events:
                result += f"{event[0]} {event[1]} {event[2]}\n"
        
        return result.strip()

def main():
    scheduler = OlympicsScheduler()
    
    print("Please enter a command...")
    for line in sys.stdin:
        line = line.strip()
        if line.lower() == "end":
            break
        
        parts = line.split(maxsplit=1)
        command = parts[0]
        args = parts[1] if len(parts) > 1 else ""
        
        if command == "addVenue":
            try:
                name, location, capacity = args.rsplit(maxsplit=2)
                name = name.strip('"')
                location = location.strip('"')
                result, error = scheduler.add_venue(name, location, capacity)
            except ValueError:
                result, error = -1, "Error: invalid arguments"
        elif command == "deleteVenue":
            result, error = scheduler.delete_venue(args.strip('"'))
        elif command == "showVenues":
            print(scheduler.show_venues())
            print("Please enter a command...")
            continue
        elif command == "addEvent":
            try:
                venue_name, date, from_hour, to_hour, event_name = args.rsplit(maxsplit=4)
                venue_name = venue_name.strip('"')
                event_name = event_name.strip('"')
                result, error = scheduler.add_event(venue_name, date, from_hour, to_hour, event_name)
            except ValueError:
                result, error = -1, "Error: invalid arguments"
        elif command == "deleteEvent":
            try:
                venue_name, date, from_hour, event_name = args.rsplit(maxsplit=3)
                venue_name = venue_name.strip('"')
                event_name = event_name.strip('"')
                result, error = scheduler.delete_event(venue_name, date, from_hour, event_name)
            except ValueError:
                result, error = -1, "Error: invalid arguments"
        elif command == "showEvents":
            try:
                venue_name, date = args.rsplit(maxsplit=1)
                venue_name = venue_name.strip('"')
                print(scheduler.show_events(venue_name, date))
            except ValueError:
                print(-1)
                print("Error: invalid arguments")
            print("Please enter a command...")
            continue
        elif command == "showCalendar":
            result = scheduler.show_calendar(args.strip('"'))
            if isinstance(result, tuple):
                print(-1)
                print(result[1])
            else:
                print(result)
            print("Please enter a command...")
            continue
        else:
            result, error = -1, "Error: unknown command"
        
        print(result)
        if error:
            print(error)
        print("Please enter a command...")

if __name__ == "__main__":
    main()