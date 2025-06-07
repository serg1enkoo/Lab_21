#include <iostream> // Підключення бібліотеки для введення/виведення даних (cout, cin)
#include <iomanip>  // Підключення бібліотеки для маніпуляцій з виведенням (setprecision)
#include <string>   // Підключення бібліотеки для роботи з рядками (string)
#include <sstream>  // Підключення бібліотеки для роботи з рядковими потоками (stringstream, для парсингу команд)
#include <vector>   // Хоча в завданні вказано масив С-стилю, вектор був би гнучкішим. Але дотримуємось завдання.
#include <cmath>    // Підключення бібліотеки для математичних функцій (round)
#include <limits>   // Підключення для numeric_limits (для очищення буфера вводу)

const int MAX_FLIGHTS = 10; // Максимальна кількість рейсів, яку система може обробляти

// Оголошення класу FlightBooking
class FlightBooking {
public:
    // Конструктор за замовчуванням: створює "порожній" або неіснуючий рейс
    FlightBooking() : id(0), capacity(0), reserved(0) {}

    // Параметризований конструктор: створює рейс з заданими параметрами та обмеженнями
    FlightBooking(int id, int capacity, int reserved);

    // Метод для виведення статусу рейсу
    void printStatus() const; // Додаємо const, бо метод не змінює об'єкт

    // Метод для додавання бронювань
    bool reserveSeats(int number_of_seats);

    // Метод для скасування бронювань
    bool cancelReservations(int number_of_seats);

    // Метод для отримання ID рейсу (гетер)
    // Виправлено помилку з PDF (retutn -> return), додано const
    int getId() const { return id; }

private:
    // Приватні члени класу
    int id;
    int capacity;
    int reserved;

    // Приватний метод для перевірки ліміту (105%)
    int getMaxReservable() const {
        // Переконуємось, що capacity не від'ємна перед множенням
        return static_cast<int>(round((capacity > 0 ? capacity : 0) * 1.05));
    }
};

// Реалізація параметризованого конструктора класу FlightBooking
FlightBooking::FlightBooking(int id, int capacity, int reserved) : id(id) {
    // Ініціалізуємо id
    // Перевіряємо валідність capacity
    this->capacity = (capacity >= 0) ? capacity : 0;

    // Обробка початкової кількості заброньованих місць згідно з правилами
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
    // Важливо: якщо переданий ID <= 0, рейс вважається невалідним/не створеним
    // Конструктор не повинен сам змінювати id на 0, це логіка вищого рівня (в main)
    // Але ми можемо додати перевірку, щоб не створювати рейс з ID=0 через цей конструктор
     if (id <= 0) {
         // Можна викинути виняток, вивести помилку або встановити ID в 0,
         // показуючи, що створення не вдалося. Згідно з умовою (ID 0 - невалідний),
         // ми не повинні дозволяти створювати рейс з ID 0 через цей конструктор.
         // Встановимо ID в 0, щоб позначити його як невалідний/пустий слот.
         this->id = 0;
         this->capacity = 0;
         this->reserved = 0;
         std::cerr << "Warning: Attempted to create flight with invalid ID <= 0. Flight not created." << std::endl;
     }
}


// Реалізація методу printStatus
// Додано const, бо метод не змінює стан об'єкта
void FlightBooking::printStatus() const {
    // Перевіряємо, чи рейс існує (чи ID не 0)
    if (id == 0) {
        // Не виводимо нічого для неіснуючих рейсів
        return;
    }

    double percentage = 0.0;
    if (capacity > 0) {
        percentage = static_cast<double>(reserved) / capacity * 100.0;
    } else if (reserved > 0) {
        percentage = 100.0; // Або інша логіка для випадку capacity = 0
    }

    std::cout << "Flight " << id << ": " << reserved << "/" << capacity << " ("
              << std::fixed << std::setprecision(0) << percentage << "%) seats reserved" << std::endl;
}

// Реалізація методу для додавання бронювань
bool FlightBooking::reserveSeats(int number_of_seats) {
     // Не можна додавати місця до неіснуючого рейсу
    if (id == 0 || number_of_seats <= 0) {
        return false;
    }
    int max_reservable = getMaxReservable();
    if (reserved + number_of_seats > max_reservable) {
        return false; // Перевищення ліміту 105%
    }
    reserved += number_of_seats;
    return true;
}

// Реалізація методу для скасування бронювань
bool FlightBooking::cancelReservations(int number_of_seats) {
     // Не можна скасовувати місця у неіснуючого рейсу
    if (id == 0 || number_of_seats <= 0) {
        return false;
    }
    if (reserved - number_of_seats < 0) {
        return false; // Не можна мати від'ємну кількість броней
    }
    reserved -= number_of_seats;
    return true;
}

// --- Допоміжні функції для роботи з масивом рейсів ---

// Функція для пошуку індексу рейсу за його ID
// Повертає індекс знайденого рейсу або -1, якщо рейс не знайдено
int findFlightIndexById(int targetId, const FlightBooking arr[], int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].getId() == targetId) {
            return i; // Знайдено рейс, повертаємо індекс
        }
    }
    return -1; // Рейс не знайдено
}

// Функція для пошуку першого вільного слоту в масиві
// Вільний слот позначається ID = 0
// Повертає індекс вільного слоту або -1, якщо вільних слотів немає
int findEmptySlot(const FlightBooking arr[], int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].getId() == 0) {
            return i; // Знайдено вільний слот
        }
    }
    return -1; // Вільних слотів немає
}


// Головна функція програми
int main() {
    // Створення масиву для зберігання об'єктів FlightBooking
    // Всі елементи автоматично ініціалізуються конструктором за замовчуванням (id=0)
    FlightBooking bookings[MAX_FLIGHTS];
    int flight_count = 0; // Лічильник активних рейсів

    std::string line;    // Для читання рядка команди
    std::string command; // Для зберігання команди ("create", "delete", "add", "cancel", "quit")

    // Очищення буфера перед першим getline (якщо були попередні вводи)
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Головний цикл обробки команд
    while (true) {
        // --- Виведення статусу всіх активних рейсів ---
        bool has_flights = false; // Прапорець, чи є хоч один активний рейс
        for (int i = 0; i < MAX_FLIGHTS; ++i) {
            if (bookings[i].getId() != 0) {
                bookings[i].printStatus();
                has_flights = true;
            }
        }
        // Якщо активних рейсів немає, виводимо повідомлення
        if (!has_flights) {
            std::cout << "No flights in the system" << std::endl;
        }

        // --- Запит та зчитування команди ---
        std::cout << "What would you like to do?: ";
        if (!std::getline(std::cin, line)) {
            break; // Вихід при помилці читання або кінці файлу
        }

        // Використання stringstream для розбору команди
        std::stringstream ss(line);
        ss >> command; // Зчитуємо перше слово - команду

        // --- Обробка команд ---
        if (command == "quit") {
            break; // Вихід з циклу
        } else if (command == "create") {
            int id = 0, capacity = 0;
            // Спробуємо зчитати id та capacity
            if (ss >> id >> capacity) {
                 char remaining;
                 // Перевіряємо, чи немає зайвих символів та чи валідні id та capacity
                 if (!(ss >> remaining) && id > 0 && capacity >= 0) {
                     // Перевірка, чи рейс з таким ID вже існує
                     if (findFlightIndexById(id, bookings, MAX_FLIGHTS) != -1) {
                         std::cout << "Cannot perform this operation: Flight " << id << " already exists." << std::endl;
                     } else {
                         // Пошук вільного слоту
                         int emptySlot = findEmptySlot(bookings, MAX_FLIGHTS);
                         if (emptySlot == -1) {
                             std::cout << "Cannot perform this operation: Maximum number of flights reached." << std::endl;
                         } else {
                             // Створення нового рейсу у вільному слоті
                             bookings[emptySlot] = FlightBooking(id, capacity, 0);
                             // Перевірка, чи конструктор не скинув ID (якщо ID був <= 0)
                             if (bookings[emptySlot].getId() == id) {
                                 flight_count++; // Збільшуємо лічильник активних рейсів
                                 // Можна вивести підтвердження: std::cout << "Flight " << id << " created." << std::endl;
                             } else {
                                 // Конструктор скинув ID, бо він був невалідний
                                 std::cout << "Cannot perform this operation: Invalid flight ID or capacity." << std::endl;
                             }
                         }
                     }
                 } else {
                     // Помилка формату або невалідні значення id/capacity
                     std::cout << "Cannot perform this operation: Invalid format or values for 'create'. Use 'create [id>0] [capacity>=0]'." << std::endl;
                 }
            } else {
                // Не вдалося зчитати id та capacity
                std::cout << "Cannot perform this operation: Invalid format for 'create'. Use 'create [id] [capacity]'." << std::endl;
            }
        } else if (command == "delete") {
            int id = 0;
            if (ss >> id) { // Спробуємо зчитати id
                 char remaining;
                 if (!(ss >> remaining) && id > 0) { // Перевірка на зайві символи та валідний ID
                    int index = findFlightIndexById(id, bookings, MAX_FLIGHTS);
                    if (index == -1) {
                        // Рейс не знайдено
                        std::cout << "Cannot perform this operation: Flight " << id << " not found." << std::endl;
                    } else {
                        // Видалення рейсу (скидання до стану за замовчуванням)
                        bookings[index] = FlightBooking(); // Викликаємо конструктор за замовчуванням
                        flight_count--; // Зменшуємо лічильник активних рейсів
                         // Можна вивести підтвердження: std::cout << "Flight " << id << " deleted." << std::endl;
                    }
                 } else {
                     std::cout << "Cannot perform this operation: Invalid format or non-positive ID for 'delete'. Use 'delete [id>0]'." << std::endl;
                 }
            } else {
                 std::cout << "Cannot perform this operation: Invalid format for 'delete'. Use 'delete [id]'." << std::endl;
            }
        } else if (command == "add") {
            int id = 0, n = 0;
            if (ss >> id >> n) { // Зчитуємо id та кількість місць n
                 char remaining;
                 if (!(ss >> remaining) && id > 0 && n > 0) { // Перевірка формату та валідності значень
                     int index = findFlightIndexById(id, bookings, MAX_FLIGHTS);
                     if (index == -1) {
                         std::cout << "Cannot perform this operation: Flight " << id << " not found." << std::endl;
                     } else {
                         // Спроба додати місця
                         if (!bookings[index].reserveSeats(n)) {
                             // Якщо додавання не вдалося (наприклад, перевищено ліміт)
                             std::cout << "Cannot perform this operation" /* << ": Capacity limit reached for flight " << id */ << std::endl;
                         }
                         // Якщо вдалося, статус оновиться на наступній ітерації циклу
                     }
                 } else {
                      std::cout << "Cannot perform this operation: Invalid format or non-positive values for 'add'. Use 'add [id>0] [n>0]'." << std::endl;
                 }
            } else {
                std::cout << "Cannot perform this operation: Invalid format for 'add'. Use 'add [id] [n]'." << std::endl;
            }
        } else if (command == "cancel") {
            int id = 0, n = 0;
            if (ss >> id >> n) { // Зчитуємо id та кількість місць n
                 char remaining;
                  if (!(ss >> remaining) && id > 0 && n > 0) { // Перевірка формату та валідності значень
                    int index = findFlightIndexById(id, bookings, MAX_FLIGHTS);
                    if (index == -1) {
                        std::cout << "Cannot perform this operation: Flight " << id << " not found." << std::endl;
                    } else {
                        // Спроба скасувати місця
                        if (!bookings[index].cancelReservations(n)) {
                            // Якщо скасування не вдалося (наприклад, недостатньо броней)
                            std::cout << "Cannot perform this operation" /* << ": Not enough reservations to cancel for flight " << id */ << std::endl;
                        }
                         // Якщо вдалося, статус оновиться на наступній ітерації циклу
                    }
                  } else {
                      std::cout << "Cannot perform this operation: Invalid format or non-positive values for 'cancel'. Use 'cancel [id>0] [n>0]'." << std::endl;
                  }
            } else {
                 std::cout << "Cannot perform this operation: Invalid format for 'cancel'. Use 'cancel [id] [n]'." << std::endl;
            }
        } else {
            // Якщо команда не розпізнана
            if (!command.empty()) { // Не виводити помилку, якщо користувач просто натиснув Enter
               std::cout << "Unknown command: " << command << std::endl;
            }
        }

        // Очищення змінних для наступної ітерації (опціонально)
        command = "";
        line = "";
    }

    return 0; // Успішне завершення програми
}
