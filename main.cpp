#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>

using namespace std;

string words[50], lower_words[50], punctuation[50];

bool is_letter_or_number(const char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}

int linear_search_substring(const string &text, const string &pattern) {
    const size_t n = text.length();
    const size_t m = pattern.length();

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (tolower(text[i + j]) != tolower(pattern[j]))
                break;
        }

        if (j == m)
            return i;
    }

    return -1;
}

int search_substrings_count(string text, const string &pattern) {
    int count = 0, index;
    do {
        index = linear_search_substring(text, pattern);
        if (index != -1) {
            count++;
            text = text.substr(index + 1, text.length() - index - 1);
        }
    } while (index != -1);
    return count;
}

vector<int> compute_lps(string pattern) {
    int n = pattern.length();
    vector<int> lps(n, 0);
    int len = 0;
    int i = 1;

    while (i < n) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}

vector<int> kmp(string text, string pattern) {
    vector<int> lps = compute_lps(pattern);
    vector<int> occurrences;
    int n = text.length();
    int m = pattern.length();
    int i = 0;
    int j = 0;

    while (i < n) {
        if (tolower(text[i]) == tolower(pattern[j])) {
            i++;
            j++;
        }

        if (j == m) {
            occurrences.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return occurrences;
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
        lower_words[i] = words[i];
        lower_words[i][0] = tolower(lower_words[i][0]);
    }

    cout << endl;
    sort(lower_words, lower_words + ind1);
    cout << "Задание 3. Вариант 2. Слова последовательности в алфавитном порядке." << endl;
    for (int i = 0; i < ind1; i++) {
        cout << lower_words[i] << " ";
    }
    cout << endl;
    cout << "Задание 4. Вариант 2. Вывести на экран количество символов в каждом слове исходной последовательности." <<
            endl;
    for (int i = 0; i < ind1; i++) {
        cout << words[i] << " - " << words[i].length() << " || ";
    }
    cout << endl;
    cout << "Задание 5. Поиск подстроки в строке методом линейного поиска и КМП." << endl;
    string long_line;
    for (int i = 0; i < ind1; i++) {
        long_line += words[i] + punctuation[i];
    }
    string pattern;
    cout << "Введите подстроку для поиска: ";
    getline(cin, pattern);
    int result = search_substrings_count(long_line, pattern);
    auto kmp_result = kmp(long_line, pattern);
    cout << "Найдено " << result << " подстрок." << endl;
    cout << "Найдено " << kmp_result.size() << " подстрок." << endl;
    return 0;
}
