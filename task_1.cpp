#include <iostream>
#include <iomanip>

class FlightBooking {
public:
    FlightBooking(int id, int capacity, int reserved);
    void printStatus();

private:
    int id;
    int capacity;
    int reserved;
};

void FlightBooking::printStatus()
{
    double percentage = 0.0;
    if (capacity > 0) {
        percentage = static_cast<double>(reserved) / capacity * 100.0;
    }

    std::cout << "Flight " << id << ": " << reserved << "/" << capacity << " ("
              << std::fixed << std::setprecision(0) << percentage << "%) seats taken" << std::endl;
}

FlightBooking::FlightBooking(int id, int capacity, int reserved)
{
    this->id = id;
    this->capacity = capacity;
    this->reserved = reserved;
}

int main() {
    int capacity = 0;
    int reserved = 0;

    std::cout << "Provide flight capacity: ";
    std::cin >> capacity;

    std::cout << "Provide number of reserved seats: ";
    std::cin >> reserved;

    FlightBooking booking(1, capacity, reserved);

    booking.printStatus();

    return 0;
}
