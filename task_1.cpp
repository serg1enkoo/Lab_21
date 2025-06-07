#include <iostream> // Підключення бібліотеки для введення/виведення даних (наприклад, std::cout, std::cin)
#include <iomanip>  // Підключення бібліотеки для маніпуляцій з виведенням (наприклад, для форматування відсотків)

// Оголошення класу FlightBooking
class FlightBooking {
public:
    // Оголошення конструктора класу
    // Він приймає id рейсу, загальну місткість та кількість заброньованих місць
    FlightBooking(int id, int capacity, int reserved);

    // Оголошення методу для виведення статусу рейсу
    void printStatus();

private:
    // Приватні члени класу (змінні)
    int id;         // Ідентифікатор рейсу
    int capacity;   // Загальна місткість літака
    int reserved;   // Кількість заброньованих місць
};

// Реалізація методу printStatus поза класом (але він належить класу FlightBooking)
void FlightBooking::printStatus()
{
    // Розрахунок відсотка заповненості
    // Важливо привести одне з цілих чисел (reserved або capacity) до double,
    // щоб результат ділення був дробовим числом, а не цілим.
    double percentage = 0.0;
    if (capacity > 0) { // Перевірка, щоб уникнути ділення на нуль
        percentage = static_cast<double>(reserved) / capacity * 100.0;
    } else if (reserved > 0) {
        break;
    }


    // Виведення звіту про статус рейсу у вказаному форматі
    // std::fixed та std::setprecision(0) використовуються для виведення відсотка як цілого числа
    std::cout << "Flight " << id << ": " << reserved << "/" << capacity << " ("
              << std::fixed << std::setprecision(0) << percentage << "%) seats taken" << std::endl;
}

// Реалізація конструктора класу FlightBooking
FlightBooking::FlightBooking(int id, int capacity, int reserved)
{
    // Збереження переданих значень у відповідні приватні члени класу
    // this-> вказує, що ми звертаємося саме до членів поточного об'єкта класу
    this->id = id;
    this->capacity = capacity;
    this->reserved = reserved;

    // Завдання не обмежує кількість заброньованих місць,
    // але в реальній системі тут могла б бути перевірка:
    // if (reserved > capacity) { /* обробка помилки або логування */ }
    // if (reserved < 0) { reserved = 0; /* або обробка помилки */ }
    // if (capacity < 0) { capacity = 0; /* або обробка помилки */ }
}

// Головна функція програми
int main() {
    int capacity = 0; // Змінна для зберігання місткості
    int reserved = 0; // Змінна для зберігання кількості заброньованих місць

    // Запит у користувача ввести місткість рейсу
    std::cout << "Provide flight capacity: "; // Виведення повідомлення
    std::cin >> capacity; // Зчитування значення, введеного користувачем, у змінну capacity

    // Запит у користувача ввести кількість заброньованих місць
    std::cout << "Provide number of reserved seats: "; // Виведення повідомлення
    std::cin >> reserved; // Зчитування значення, введеного користувачем, у змінну reserved

    // Створення об'єкта booking класу FlightBooking
    // Викликається конструктор FlightBooking(1, capacity, reserved)
    // Рейсу присвоюється id = 1, місткість та заброньовані місця беруться з введених користувачем значень
    FlightBooking booking(1, capacity, reserved);

    // Виклик методу printStatus для створеного об'єкта booking
    // Цей метод виведе інформацію про рейс на екран
    booking.printStatus();

    // Повернення 0 означає, що програма завершилася успішно
    return 0;
}
