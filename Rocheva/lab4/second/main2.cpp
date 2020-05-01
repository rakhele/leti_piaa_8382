#include <iostream>
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;

// вычисление префикс-функции для подстроки s, начинающеся с индекса start
vector<int> prefix(string &s, int start){
    int n = s.size();
    vector<int> pi (n-start); // массив чисел pi

    cout << "Prefix-function for: ";
    int j = start;
    while (j < n) {
        cout << s[j];
        j++;
    }
    cout << endl;

    j = 0;
    cout << "first sumbol = " << s[start] << " with max suffix = 0" << endl;
    for (int i = (start+1); i < n; i++) {
        cout << "current symbol: " << s[i];
        while ((j > 0) && (s[i] != s[j])) {
            cout << " != " << s[j] << endl;
            cout << "change j = " << j << " to ";
            // теперь рассматриваем символ с индексом, равным максимальному суффиксу предыдущего символа (j-1)
            // т.е. в дальнейшем будем пытаться расширить этот суффикс
            j = pi[j-1];
            cout << j;
            cout << " and suffix has decreased";
        }

        if (s[i] == s[j]) { // увеличиваем суффикс
            cout << ". It matches a letter with index " << j << endl;
            j++;
            cout << "now j indicates a letter <" << s[j];
            cout << "> and suffix has increased";
        }
        cout << endl << "current max suffix for " << s[i] << " = " << j << endl;
        pi[i] = j;
    }

    return pi;
}

// алгоритм Кнута-Морриса-Пратта
int kmp(string &inputString, int wordSize) {
    int stringSize = inputString.size();
    int start = stringSize - wordSize;
    vector<int> wordPI = prefix(inputString, start); // значение префикс-функции для подстроки

    cout << "Value of prefix-function:" << endl;
    for (auto & p : wordPI)
        cout << p << " ";
    cout << endl << endl;

    int k = 0; // индекс в подстроке (настоящий индекс = k+start)

    for (int i = 0; i < start; i++) {

        while ((k > 0) && (inputString[k+start] != inputString[i])) {
            cout << "word[" << k << "] != string[" << i << "]" << endl;
            cout << "decrease k = " << k;
            // k отодвигается назад до значения максимального суффикса предыдущего до k символа
            k = wordPI[k-1];
            cout << ". Now k = " << k << endl;
        }

        if (inputString[k+start] == inputString[i]) {
            // увеличиваем k, чтобы при следующей итерации сравнить следующие символы
            cout << "word[" << k << "] = string[" << i << "] = " << inputString[k] << endl;
            k++;
        }

        if (k == wordSize){ // слово нашлось
            cout << "Start index: " << i - wordSize + 1 << endl;
            return  i - wordSize + 1;
        }
    }

    cout << "now found." << endl;
    return -1;
}


int main() {

    string inputString;

    cin >> inputString; // эта строка - циклический сдвиг
    inputString += inputString; // удваиваем строку
    int stringSize = inputString.size();
    char s;
    while(true){ // ввод строки, для которой inputString является циклическим сдвигом
        if (!(cin >> s) || s == '*') // '*' показывает, когда закончится ввод
            break;
        inputString += s;
    }

    // ищем вхождение word в удвоенную строку
    int result = kmp(inputString, inputString.size() - stringSize);
    cout << result << endl;

    return 0;
}