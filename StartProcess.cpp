#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main() {
    // Вибираємо розмір буфера пам'яті
    const size_t bufferSize = sizeof(int);

    // Виділяємо пам'ять для буфера
    void* buffer = malloc(bufferSize);

    if (buffer == nullptr) {
        cerr << "Помилка виділення пам'яті\n";
        return 1;
    }

    // Записуємо початкове значення у буфер
    int* ptr = static_cast<int*>(buffer);
    *ptr = 0;

    // Виводимо адресу пам'яті та PID процесу
    cout << "Адреса пам'яті: " << ptr << std::endl;
    cout << "PID процесу: " << getpid() << std::endl;

    // Цикл збільшення значення кожні 5 секунд
    while (true) {
        this_thread::sleep_for(chrono::seconds(5));
        ++(*ptr);
        cout << *ptr << endl; 
    }

    // Звільняємо виділену пам'ять
    free(buffer);

    return 0;
}
