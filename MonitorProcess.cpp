#include <iostream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Помилка вхідних даних";
        return 1;
    }

    pid_t target_pid = atoi(argv[1]);
    long addr = strtol(argv[2], nullptr, 16);

    // Підключаємось до процесу
    if (ptrace(PTRACE_ATTACH, target_pid, nullptr, nullptr) == -1) {
        cerr << "Помилка при підключенні до процесу\n";
        return 1;
    }

    // Чекаємо на завершення процесу
    waitpid(target_pid, nullptr, 0);

    // Отримуємо значення із зазначеної комірки пам'яті
    int original_value = ptrace(PTRACE_PEEKDATA, target_pid, addr, nullptr);
    cout << "Вихідне значення 'a': " << original_value << "\n";

    // Модифікуємо значення
    int new_value = original_value + 5;
    ptrace(PTRACE_POKEDATA, target_pid, addr, new_value);

    // Отримуємо нове значення
    int modified_value = ptrace(PTRACE_PEEKDATA, target_pid, addr, nullptr);
    cout << "Модифіковане значення 'a+5': " << modified_value << "\n";

    // Відключаємось від процесу
    ptrace(PTRACE_DETACH, target_pid, nullptr, nullptr);

    return 0;
}