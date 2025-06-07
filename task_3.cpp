#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>

const int MAX_FLIGHTS = 10;

class FlightBooking {
public:
    FlightBooking() : id(0), capacity(0), reserved(0) {}
    FlightBooking(int id, int capacity, int reserved);

    void printStatus() const;
    bool reserveSeats(int number_of_seats);
    bool cancelReservations(int number_of_seats);
    int getId() const { return id; }

private:
    int id;
    int capacity;
    int reserved;
    int getMaxReservable() const {
        return static_cast<int>(round((capacity > 0 ? capacity : 0) * 1.05));
    }
};

FlightBooking::FlightBooking(int id, int capacity, int reserved) : id(id) {
    this->capacity = (capacity >= 0) ? capacity : 0;

    if (reserved < 0) {
        this->reserved = 0;
    } else {
        int max_reservable = getMaxReservable();
        if (reserved > max_reservable) {
            this->reserved = max_reservable;
        } else {
            this->reserved = reserved;
        }
    }
     if (id <= 0) {
         this->id = 0;
         this->capacity = 0;
         this->reserved = 0;
         std::cerr << "Warning: Attempted to create flight with invalid ID <= 0. Flight not created." << std::endl;
     }
}

void FlightBooking::printStatus() const {
    if (id == 0) {
        return;
    }

    double percentage = 0.0;
    if (capacity > 0) {
        percentage = static_cast<double>(reserved) / capacity * 100.0;
    } else if (reserved > 0) {
        percentage = 100.0;
    }

    std::cout << "Flight " << id << ": " << reserved << "/" << capacity << " ("
              << std::fixed << std::setprecision(0) << percentage << "%) seats reserved" << std::endl;
}

bool FlightBooking::reserveSeats(int number_of_seats) {
    if (id == 0 || number_of_seats <= 0) {
        return false;
    }
    int max_reservable = getMaxReservable();
    if (reserved + number_of_seats > max_reservable) {
        return false;
    }
    reserved += number_of_seats;
    return true;
}

bool FlightBooking::cancelReservations(int number_of_seats) {
    if (id == 0 || number_of_seats <= 0) {
        return false;
    }
    if (reserved - number_of_seats < 0) {
        return false;
    }
    reserved -= number_of_seats;
    return true;
}

int findFlightIndexById(int targetId, const FlightBooking arr[], int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].getId() == targetId) {
            return i;
        }
    }
    return -1;
}

int findEmptySlot(const FlightBooking arr[], int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].getId() == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    FlightBooking bookings[MAX_FLIGHTS];
    int flight_count = 0;

    std::string line;
    std::string command;

    while (true) {
        bool has_flights = false;
        for (int i = 0; i < MAX_FLIGHTS; ++i) {
            if (bookings[i].getId() != 0) {
                bookings[i].printStatus();
                has_flights = true;
            }
        }
        if (!has_flights) {
            std::cout << "No flights in the system" << std::endl;
        }

        std::cout << "What would you like to do?: ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::stringstream ss(line);
        ss >> command;

        if (command == "quit") {
            break;
        } else if (command == "create") {
            int id = 0, capacity = 0;
            if (ss >> id >> capacity) {
                char remaining;
                if (!(ss >> remaining) && id > 0 && capacity >= 0) {
                    if (findFlightIndexById(id, bookings, MAX_FLIGHTS) != -1) {
                        std::cout << "Cannot perform this operation: Flight " << id << " already exists." << std::endl;
                    } else {
                        int emptySlot = findEmptySlot(bookings, MAX_FLIGHTS);
                        if (emptySlot == -1) {
                            std::cout << "Cannot perform this operation: Maximum number of flights reached." << std::endl;
                        } else {
                            bookings[emptySlot] = FlightBooking(id, capacity, 0);
                            if (bookings[emptySlot].getId() == id) {
                                flight_count++;
                            } else {
                                std::cout << "Cannot perform this operation: Invalid flight ID or capacity." << std::endl;
                            }
                        }
                    }
                } else {
                    std::cout << "Cannot perform this operation: Invalid format or values for 'create'. Use 'create [id>0] [capacity>=0]'." << std::endl;
                }
            } else {
                std::cout << "Cannot perform this operation: Invalid format for 'create'. Use 'create [id] [capacity]'." << std::endl;
            }
        } else if (command == "delete") {
            int id = 0;
            if (ss >> id) {
                char remaining;
                if (!(ss >> remaining) && id > 0) {
                    int index = findFlightIndexById(id, bookings, MAX_FLIGHTS);
                    if (index == -1) {
                        std::cout << "Cannot perform this operation: Flight " << id << " not found." << std::endl;
                    } else {
                        bookings[index] = FlightBooking();
                        flight_count--;
                    }
                } else {
                    std::cout << "Cannot perform this operation: Invalid format or non-positive ID for 'delete'. Use 'delete [id>0]'." << std::endl;
                }
            } else {
                std::cout << "Cannot perform this operation: Invalid format for 'delete'. Use 'delete [id]'." << std::endl;
            }
        } else if (command == "add") {
            int id = 0, n = 0;
            if (ss >> id >> n) {
                char remaining;
                if (!(ss >> remaining) && id > 0 && n > 0) {
                    int index = findFlightIndexById(id, bookings, MAX_FLIGHTS);
                    if (index == -1) {
                        std::cout << "Cannot perform this operation: Flight " << id << " not found." << std::endl;
                    } else {
                        if (!bookings[index].reserveSeats(n)) {
                            std::cout << "Cannot perform this operation" << std::endl;
                        }
                    }
                } else {
                    std::cout << "Cannot perform this operation: Invalid format or non-positive values for 'add'. Use 'add [id>0] [n>0]'." << std::endl;
                }
            } else {
                std::cout << "Cannot perform this operation: Invalid format for 'add'. Use 'add [id] [n]'." << std::endl;
            }
        } else if (command == "cancel") {
            int id = 0, n = 0;
            if (ss >> id >> n) {
                char remaining;
                if (!(ss >> remaining) && id > 0 && n > 0) {
                    int index = findFlightIndexById(id, bookings, MAX_FLIGHTS);
                    if (index == -1) {
                        std::cout << "Cannot perform this operation: Flight " << id << " not found." << std::endl;
                    } else {
                        if (!bookings[index].cancelReservations(n)) {
                            std::cout << "Cannot perform this operation" << std::endl;
                        }
                    }
                } else {
                    std::cout << "Cannot perform this operation: Invalid format or non-positive values for 'cancel'. Use 'cancel [id>0] [n>0]'." << std::endl;
                }
            } else {
                std::cout << "Cannot perform this operation: Invalid format for 'cancel'. Use 'cancel [id] [n]'." << std::endl;
            }
        } else {
            if (!command.empty()) {
               std::cout << "Unknown command: " << command << std::endl;
            }
        }

        command = "";
        line = "";
    }

    return 0;
}
