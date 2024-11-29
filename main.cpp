#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

string words[50], punctuation[50];

bool is_letter_or_number(const char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}

int main() {
    cout << "Выберите способ ввода текста:" << endl;
    cout << "1. С клавиатуры" << endl;
    cout << "2. Из файла" << endl;
    int ans = -1;
    do {
        cout << "Ваш выбор: ";
        cin >> ans;
    } while (ans != 1 && ans != 2);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string text;
    if (ans == 1) {
        cout << "Введите текст" << endl;
        getline(cin, text);
    } else {
        cout << "Введите путь до файла с текстом: ";
        string path;
        getline(cin, path);
        fstream file(path, ios_base::in);
        if (!file.is_open()) {
            cout << "Не удалось открыть файл, завершаю выполнение.";
            return 1;
        }
        getline(file, text);
        file.close();
    }

    int start = 0, ind1 = 0, ind2 = 0;
    for (int i = 0; i < text.length() - 1; i++) {
        if (is_letter_or_number(text[i]) && !is_letter_or_number(text[i + 1])) {
            words[ind1++] = text.substr(start, i - start + 1);
            start = i + 1;
        } else if (!is_letter_or_number(text[i]) && is_letter_or_number(text[i + 1])) {
            punctuation[ind2++] = text.substr(start, i - start + 1);
            start = i + 1;
        }
    }
    punctuation[ind2++] = ".";

    for (int i = 0; i < ind1; i++) {
        for (int j = 1; j < words[i].length(); j++) {
            words[i][j] = tolower(words[i][j]);
        }
    }


    for (int i = 0; i < ind2; i++) {
        string buffer;
        buffer.push_back(punctuation[i][0]);
        for (int j = 1; j < punctuation[i].length(); j++) {
            if (buffer.back() == '.' &&
                j + 1 < punctuation[i].length() &&
                punctuation[i][j] == '.' &&
                punctuation[i][j + 1] == '.') {
                buffer.push_back('.');
                buffer.push_back('.');
                j++;
                continue;
            }

            if (buffer.back() != punctuation[i][j]) {
                buffer.push_back(punctuation[i][j]);
            }
        }
        punctuation[i] = buffer;
    }

    for (int i = 0; i < ind1; i++) {
        cout << words[i] << punctuation[i];
    }
    return 0;
}
