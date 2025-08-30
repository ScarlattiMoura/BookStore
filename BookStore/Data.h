#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <memory>
#include <ctime>

#pragma once

using namespace std;

class Data {
private:
    int dia, mes, ano;
public:
    Data(int d = 1, int m = 1, int a = 2000) : dia(d), mes(m), ano(a) {}

    string toString() const {
        return to_string(dia) + "/" + to_string(mes) + "/" + to_string(ano);
    }

    void fromString(const string& dataStr) {
        size_t pos1 = dataStr.find('/');
        size_t pos2 = dataStr.find('/', pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos) {
            dia = stoi(dataStr.substr(0, pos1));
            mes = stoi(dataStr.substr(pos1 + 1, pos2 - pos1 - 1));
            ano = stoi(dataStr.substr(pos2 + 1));
        }
    }

    static Data dataAtual() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return Data(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    }

    friend ostream& operator<<(ostream& os, const Data& data) {
        os << data.toString();
        return os;
    }
};
