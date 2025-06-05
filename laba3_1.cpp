#include <iostream>
#include <cmath>    // для sqrt()
#include <iomanip>  // для форматирования вывода

using namespace std;

double calculate_function(double x) {
    if (x <= -8.0) {
        return -3.0;
    } else if (x > -8.0 && x <= -3.0) {
        return (3.0 / 5.0) * x + (9.0 / 5.0);
    } else if (x > -3.0 && x <= 3.0) {
        return -sqrt(9.0 - x * x);
    } else if (x > 3.0 && x <= 5.0) {
        return x - 3.0;
    } else {
        return 3.0;
    }
}

int main() {
    // Установка локали, чтобы в выводе использовалась точка как десятичный разделитель
    setlocale(LC_ALL, "C");

    const double x_start = -10.0; // Начало интервала X
    const double x_end = 8.0;     // Конец интервала X
    const double dx = 1.0;        // Шаг изменения X

    // Настраиваем формат вывода чисел с плавающей точкой
    cout << fixed << setprecision(4);

    // Вывод "шапки" таблицы
    cout << "+-----------------------+" << endl;
    cout << "|    x    |    f(x)   |" << endl;
    cout << "+-----------------------+" << endl;

    // Циклический вычислительный процесс
    for (double x = x_start; x <= x_end; x += dx) {
        double y = calculate_function(x);

        // Вывод строки таблицы с результатами
        cout << "| " << setw(7) << x << " | " << setw(9) << y << " |" << endl;
    }

    // Вывод "подвала" таблицы
    cout << "+-----------------------+" << endl;

    return 0;
}
