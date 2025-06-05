#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <random>
#include <set>

using namespace std;

// Функция позволяет красиво выводить векторы в табличном формате с разделительными линиям (элементы вектора в строчку)
template <typename T>
ostream& operator<<(ostream& os, vector<T>& result){
    for (auto item : result){
        os << "\t" << item << "\t║";
    }
    os << "\n╠═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n";
    return os;
}

// Функция для возведения числа в степень по модулю (для тестов простоты)
int powMod(int base, int degree, int mod) {
    int result = base % mod;
    for (int i = 2; i <= degree; i++) {
        result = (result * base) % mod; //берем остаток от деления для предотвращения переполнения
    }
    return result;
}

// Функция для нахождения простых чисел с использованием решета Эратосфена
vector<int> sieveEratos(int N) {
    vector<bool> boolPrime(N + 1, true);  // Массив для пометки простых чисел от 2 до N
    for (int p = 2; p * p <= N; p++) {
        if (boolPrime[p]) {
            for (int i = p * p; i <= N; i += p) {  // Помечаем все его кратные как составные и вычеркиваем их
                boolPrime[i] = false;
            }
        }
    }

    vector<int> primes;//список простых чисел
    for (int p = 2; p <= N; p++) {
        if (boolPrime[p]) {  // Если число простое, добавляем его в список
            primes.push_back(p);
        }
    }

    return primes;
}

// Функция для генерации случайного числа в заданном диапазоне
int randDist(int min, int max) {
    random_device rd;
    mt19937 rand(rd());
    uniform_int_distribution<int> dist(min, max);
    return dist(rand);
}

// Функция для выполнения теста Миллера-Рабина
bool rabin(int num, int k) {
    if (num == 2 || num == 3) {
        return true;  // 2 и 3 всегда простые
    }
    if (num < 2 || num % 2 == 0) {
        return false;  // Четные числа, кроме 2, не простые
    }

    int s = 0;
    int d = num - 1;
    while (d % 2 == 0) {  // Разлагаем num - 1 на 2^s * d
        d /= 2;
        s++;
    }

    for (int i = 0; i < k; i++) {  // Повторяем тест k раз (для повышения надежности)
        int a = rand() % (num - 4) + 2;  // Случайное основание
        int x = powMod(a, d, num);// Вычисляем a^d mod num
        int y;
        for (int j = 0; j < s; j++) {
            y = powMod(x, 2, num);  // Возведение в квадрат по модулю
            if (y == 1 && x != 1 && x != (num - 1)) {
                return false;  // число составное
            }
            x = y;
        }
        if (y != 1) {
            return false; //не 1 => составное
        }
    }
    return true;
}

// Функция для генерации простого числа с заданной битностью
tuple<int, vector<int>> calcN(vector<int> prime, int bit) {
    int maxIndex = prime.size() - 1;
    int maxPow = 1;
    // Находим максимальную степень числа, не превосходящую 2^(bit-1)
    for (; pow(2, maxPow) < pow(2, bit - 1); maxPow++);

    int m = 1;
    vector<int> qList;

    while (true) {
        int randIndex = randDist(0, maxIndex);  // Случайный индекс простого числа
        int alphaDegree = randDist(1, maxPow);  // Случайная степень

        if (pow(prime[randIndex], alphaDegree)) {
            if (m * pow(prime[randIndex], alphaDegree) < pow(2, bit - 1)) {
                m *= pow(prime[randIndex], alphaDegree);  // Умножаем m на простое число в степени alphaDegree
                qList.push_back(prime[randIndex]);  // Добавляем простое число в список qList
            }
        }

        if (m > pow(2, bit - 2)) {
            if (m >= pow(2, bit - 1)) {  // Если m слишком большое, начинаем заново
                m = 1;
                qList.clear();
            } else {
                break;
            }
        }
    }

    int n = 2 * m + 1;  // Генерируем число n
return make_tuple(n, qList);  // Возвращаем n и список простых чисел qList
}

// Тест Миллера для проверки простоты числа ( для метода Поклингтона)
bool Miller(int n, int t, vector<int> qList) {
    set<int> aSet;//уникальное основание
    int a;

    while (aSet.size() != t) {
        a = randDist(2, n - 1);
        aSet.insert(a);  // Добавляем основание в множество
    }

    // Этап 1. Проверка для каждого основания по теореме Ферма
    for (int aj : aSet) {
        if (powMod(aj, n - 1, n) != 1) { //a^n-1 не равно 1 по модулю n
            return false;  //число составное
        }
    }

    // Этап 2. Проверка для каждого простого делителя q из qList
    for (int q : qList) {
        bool isPrime = true;
        for (int aj : aSet) {
            int res = static_cast<int>(pow(aj, (n - 1) / q)) % n;  // Проверка для каждого основания
            if (res != 1) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            return false;  // Если n проходит проверку, оно составное
        }
    }

    return true;  // Если все проверки прошли успешно, n простое
}

int main() {
    int bit = 10, t = 5;
    vector<int> prime = sieveEratos(500);  // Строим таблицу малых простых чисел

    vector<int> qList; //список простых делителей q

    int n;//число - кандидат на простоту
    int k = 0; //отбракованные, новероятно простые

    vector<int> result;
    vector<string> testVer;
    vector<int> kCont;

    // 1) Генерация простых чисел
    while (result.size() < 10) {
        tie(n, qList) = calcN(prime, bit);  // 2) Генерация числа n и списка простых чисел

        if (Miller(n, t, qList)) {  // 3) тест Поклингтона
            result.push_back(n);

            // доп. тест Миллера-Рабина
            if (rabin(n, 3)) {
                testVer.push_back("+");
            } else {
                testVer.push_back("-");
            }
            kCont.push_back(k);
            k = 0;//счетчик k сбрасывается на 0 при нахождении простого числа
        } else {
            if (rabin(n, 3)) {
                k++;  // Если число не прошло тест Миллера-Рабина, увеличиваем счетчик k
            }
        }
    }

    // Выводим таблицу результатов
    cout << "\n╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║Num\t║";
    for (int i = 1; i < 11; i++)
        cout << "\t" << i << "\t║";
    cout << endl;
    cout << "╠═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣";
    cout << endl;
    cout << "║P\t║" << result;
    cout << "║Test\t║" << testVer;
    cout << "║k\t║";
    for (auto item : kCont)
        cout << "\t" << item << "\t║";
    cout << endl;
    cout << "╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝";
}
