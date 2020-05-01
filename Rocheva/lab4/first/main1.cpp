#include <iostream>
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;

// вычисление префикс-функции для строки s
vector<int> prefix(string &s, int n){
    cout << "Prefix-function for: ";
    int j = 0;
    while (j < n) {
        cout << s[j];
        j++;
    }
    cout << endl;

    vector<int> pi (n); // массив чисел pi
    j = 0; //
    cout << "first sumbol = " << s[0] << " with max suffix = 0" << endl;
    for (int i = 1; i < n; i++) {
        cout << "current symbol: " << s[i];
        while ((j > 0) && (s[i] != s[j])) { // уменьшаем суффикс
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
vector<int> kmp(string &inputString, int wordSize) {
    vector<int> wordPI = prefix(inputString, wordSize); // значение префикс-функции для подстроки

    cout << "Value of prefix-function:" << endl;
    for (auto & p : wordPI)
        cout << p << " ";
    cout << endl << endl;

    vector<int> resultArray;

    int k = 0;
    int stringSize = inputString.size();

    for (int i = wordSize; i < stringSize; i++) {
        while ((k > 0) && (inputString[k] != inputString[i])) {
            cout << "word[" << k << "] != string[" << i-wordSize << "]" << endl;
            cout << "decrease k = " << k;
            // k отодвигается назад до значения максимального суффикса предыдущего до k символа
            k = wordPI[k-1];
            cout << ". Now k = " << k << endl;
        }

        if (inputString[k] == inputString[i]) {
            // увеличиваем k, чтобы при следующей итерации сравнить следующие символы
            cout << "word[" << k << "] = string[" << i-wordSize << "] = " << inputString[k] << endl;
            k++;
        }

        if (k == wordSize){ // слово нашлось
            cout << "Word was found. Start index: " << i - (2*wordSize) + 1 << endl;
            resultArray.push_back(i - (2*wordSize) + 1);
            cout << "Continue comporation from letter ";
            k = wordPI[k-1];
            cout << inputString[k] << endl;
        }
    }

    cout << "end of string" << endl;
    return resultArray;
}

int main() {

    string inputString;

    cin >> inputString; // подстрока
    size_t wordSize = inputString.size();
    char s;
    while(true){
        if (!(cin >> s) || s == '*')
            break;
        inputString += s;
    }

    vector<int> resultArray = kmp(inputString, wordSize);
    cout << endl << "RESULT:" << endl;
    if (resultArray.empty()){
        cout << -1;
    } else {
        for (auto & index : resultArray){
            if (index != resultArray.front())
                cout << ",";
            cout << index;
        }
    }
	cout << endl;

    return 0;
}