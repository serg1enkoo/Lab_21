#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>

class FlightBooking {
public:
    FlightBooking(int id, int capacity, int reserved);
    void printStatus();
    bool reserveSeats(int number_of_seats);
    bool cancelReservations(int number_of_seats);
    int getId() const { return id; }

private:
    int id;
    int capacity;
    int reserved;
    int getMaxReservable() const {
        return static_cast<int>(round(capacity * 1.05));
    }
};

FlightBooking::FlightBooking(int id, int capacity, int reserved) : id(id), capacity(capacity) {
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
    if (this->capacity < 0) {
        this->capacity = 0;
    }
}

void FlightBooking::printStatus() {
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
    if (number_of_seats <= 0) {
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
    if (number_of_seats <= 0) {
        return false;
    }
    if (reserved - number_of_seats < 0) {
        return false;
    }
    reserved -= number_of_seats;
    return true;
}

int main() {
    int capacity = 0;
    int reserved = 0;

    std::cout << "Provide flight capacity: ";
    while (!(std::cin >> capacity) || capacity < 0) {
        std::cout << "Invalid input. Please enter a non-negative integer for capacity: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Provide number of reserved seats: ";
    while (!(std::cin >> reserved)) {
        std::cout << "Invalid input. Please enter an integer for reserved seats: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    FlightBooking booking(1, capacity, reserved);

    std::string line;
    std::string command;
    int value;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        booking.printStatus();
        std::cout << "What would you like to do? (add [n], cancel [n], quit): ";

        if (!std::getline(std::cin, line)) {
            break;
        }

        std::stringstream ss(line);
        ss >> command;

        if (command == "quit") {
            break;
        } else if (command == "add") {
            if (ss >> value) {
                char remaining;
                if (!(ss >> remaining)) {
                    if (!booking.reserveSeats(value)) {
                        std::cout << "Cannot perform this operation" << std::endl;
                    }
                } else {
                    std::cout << "Invalid command format." << std::endl;
                }
            } else {
                std::cout << "Invalid command format. Use 'add [number]'." << std::endl;
            }
        } else if (command == "cancel") {
            if (ss >> value) {
                char remaining;
                if (!(ss >> remaining)) {
                    if (!booking.cancelReservations(value)) {
                        std::cout << "Cannot perform this operation" << std::endl;
                    }
                } else {
                    std::cout << "Invalid command format." << std::endl;
                }
            } else {
                std::cout << "Invalid command format. Use 'cancel [number]'." << std::endl;
            }
        } else {
            std::cout << "Unknown command." << std::endl;
        }

        command = "";
        value = 0;
    }

    return 0;
}
