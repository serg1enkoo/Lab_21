#include <iostream> // Підключення бібліотеки для введення/виведення даних (cout, cin)
#include <iomanip>  // Підключення бібліотеки для маніпуляцій з виведенням (setprecision)
#include <string>   // Підключення бібліотеки для роботи з рядками (string)
#include <sstream>  // Підключення бібліотеки для роботи з рядковими потоками (stringstream, для парсингу команд)
#include <cmath>    // Підключення бібліотеки для математичних функцій (round для коректного розрахунку ліміту)
#include <limits>   // Підключення для numeric_limits (для очищення буфера вводу)

// Оголошення класу FlightBooking
class FlightBooking {
public:
    // Оголошення конструктора класу
    FlightBooking(int id, int capacity, int reserved);

    // Оголошення методу для виведення статусу рейсу
    void printStatus();

    // Оголошення методу для додавання бронювань
    // Повертає true, якщо бронювання успішне, false - якщо ні
    bool reserveSeats(int number_of_seats);

    // Оголошення методу для скасування бронювань
    // Повертає true, якщо скасування успішне, false - якщо ні
    bool cancelReservations(int number_of_seats);

    // Додамо гетер для ID, якщо він знадобиться (хоча в завданні не вимагається)
    int getId() const { return id; }

private:
    // Приватні члени класу (змінні)
    int id;         // Ідентифікатор рейсу
    int capacity;   // Загальна місткість літака
    int reserved;   // Кількість заброньованих місць

    // Приватний метод для перевірки ліміту (105%)
    // Використовує round для коректного порівняння з цілим числом
    int getMaxReservable() const {
        return static_cast<int>(round(capacity * 1.05));
    }
};

// Реалізація конструктора класу FlightBooking
FlightBooking::FlightBooking(int id, int capacity, int reserved) : id(id), capacity(capacity) {
    // Використання списку ініціалізації для id та capacity
    // this->id = id;
    // this->capacity = capacity > 0 ? capacity : 0; // Переконуємось, що місткість не від'ємна

    // Обробка початкової кількості заброньованих місць згідно з правилами
    if (reserved < 0) {
        this->reserved = 0; // Якщо передано від'ємне число, ставимо 0
    } else {
        // Перевіряємо, чи не перевищує початкове бронювання 105% ліміт
        int max_reservable = getMaxReservable();
        if (reserved > max_reservable) {
            this->reserved = max_reservable; // Якщо перевищує, встановлюємо максимум
        } else {
            this->reserved = reserved; // Інакше зберігаємо передане значення
        }
    }
     // Виправляємо помилку з оригінального PDF: переконуємось, що capacity не нульова/від'ємна
     if (this->capacity < 0) {
         this->capacity = 0;
     }
}


// Реалізація методу printStatus
void FlightBooking::printStatus() {
    // Розрахунок відсотка заповненості
    double percentage = 0.0;
    if (capacity > 0) {
        percentage = static_cast<double>(reserved) / capacity * 100.0;
    } else if (reserved > 0) {
         // Якщо capacity = 0, а reserved > 0, то відсоток невизначений або нескінченний.
         // Виведемо 100% або інше значення за логікою. Тут виведемо 100%.
         percentage = 100.0; // Або можна вивести спеціальне повідомлення
    }

    // Виведення звіту про статус рейсу
    std::cout << "Flight " << id << ": " << reserved << "/" << capacity << " ("
              << std::fixed << std::setprecision(0) << percentage << "%) seats reserved" << std::endl;
}

// Реалізація методу для додавання бронювань
bool FlightBooking::reserveSeats(int number_of_seats) {
    // Перевірка на некоректне значення (не можна додати 0 або від'ємну кількість)
    if (number_of_seats <= 0) {
        return false; // Операція неможлива
    }

    // Перевірка, чи не призведе додавання до перевищення ліміту 105%
    int max_reservable = getMaxReservable();
    if (reserved + number_of_seats > max_reservable) {
        return false; // Операція неможлива, бо перевищить ліміт
    }

    // Якщо всі перевірки пройдені, додаємо місця
    reserved += number_of_seats;
    return true; // Операція успішна
}

// Реалізація методу для скасування бронювань
bool FlightBooking::cancelReservations(int number_of_seats) {
    // Перевірка на некоректне значення (не можна скасувати 0 або від'ємну кількість)
    if (number_of_seats <= 0) {
        return false; // Операція неможлива
    }

    // Перевірка, чи не намагаємося скасувати більше місць, ніж заброньовано
    if (reserved - number_of_seats < 0) {
        return false; // Операція неможлива, не можна мати від'ємну кількість броней
    }

    // Якщо всі перевірки пройдені, скасовуємо місця
    reserved -= number_of_seats;
    return true; // Операція успішна
}


// Головна функція програми
int main() {
    int capacity = 0; // Змінна для зберігання місткості
    int reserved = 0; // Змінна для зберігання початкової кількості заброньованих місць

    // --- Введення початкових даних ---
    std::cout << "Provide flight capacity: ";
    // Перевірка на коректність вводу місткості
    while (!(std::cin >> capacity) || capacity < 0) {
        std::cout << "Invalid input. Please enter a non-negative integer for capacity: ";
        std::cin.clear(); // Скидання прапорців помилок потоку
        // Пропускаємо некоректний ввід до кінця рядка
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Provide number of reserved seats: ";
    // Перевірка на коректність вводу заброньованих місць
    while (!(std::cin >> reserved)) {
         std::cout << "Invalid input. Please enter an integer for reserved seats: ";
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Створення об'єкта booking класу FlightBooking з початковими даними та обмеженнями конструктора
    FlightBooking booking(1, capacity, reserved);

    std::string line; // Змінна для зчитування цілого рядка команди
    std::string command; // Змінна для зберігання самої команди ("add", "cancel", "quit")
    int value; // Змінна для зберігання числового значення команди (кількість місць)

    // Очищення буфера вводу перед getline, щоб прибрати '\n', що залишився після std::cin >> reserved
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // --- Головний цикл обробки команд ---
    while (true) { // Нескінченний цикл, вихід з якого відбувається по команді "quit"
        booking.printStatus(); // Виведення поточного статусу рейсу
        std::cout << "What would you like to do? (add [n], cancel [n], quit): "; // Запрошення для користувача

        // Зчитування цілого рядка вводу користувача
        if (!std::getline(std::cin, line)) {
             // Обробка помилки читання або кінця файлу
             break;
        }

        // Використання stringstream для "розбору" рядка команди
        std::stringstream ss(line);
        ss >> command; // Спроба зчитати перше слово (команду)

        if (command == "quit") {
            break; // Вихід з циклу, якщо команда "quit"
        } else if (command == "add") {
            // Спроба зчитати число після команди "add"
            if (ss >> value) {
                 // Перевіряємо, чи немає зайвих символів після числа
                 char remaining;
                 if (!(ss >> remaining)) { // Якщо більше нічого не зчитується, команда коректна
                    // Виклик методу резервування
                    if (!booking.reserveSeats(value)) {
                        // Якщо метод повернув false (операція не вдалася)
                        std::cout << "Cannot perform this operation" << std::endl;
                    }
                 } else {
                      std::cout << "Invalid command format." << std::endl;
                 }
            } else {
                // Якщо не вдалося зчитати число після "add"
                std::cout << "Invalid command format. Use 'add [number]'." << std::endl;
            }
        } else if (command == "cancel") {
            // Спроба зчитати число після команди "cancel"
            if (ss >> value) {
                 char remaining;
                 if (!(ss >> remaining)) { // Перевірка на зайві символи
                    // Виклик методу скасування
                    if (!booking.cancelReservations(value)) {
                        // Якщо метод повернув false (операція не вдалася)
                        std::cout << "Cannot perform this operation" << std::endl;
                    }
                 } else {
                     std::cout << "Invalid command format." << std::endl;
                 }
            } else {
                // Якщо не вдалося зчитати число після "cancel"
                std::cout << "Invalid command format. Use 'cancel [number]'." << std::endl;
            }
        } else {
            // Якщо введена команда не "add", "cancel" або "quit"
            std::cout << "Unknown command." << std::endl;
        }

        // Очищення змінних для наступної ітерації (не обов'язково, але для ясності)
        command = "";
        value = 0;
    }

    // Повернення 0 означає, що програма завершилася успішно
    return 0;
}
