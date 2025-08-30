#include <iostream>
#include <vector>
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

class ItemVenda {
private:
    int livroId;
    int quantidade;
public:
    ItemVenda(int lid = 0, int q = 0) : livroId(lid), quantidade(q) {}

    int getLivroId() const { return livroId; }
    int getQuantidade() const { return quantidade; }

    string toFileString() const {
        return to_string(livroId) + ";" + to_string(quantidade);
    }

    void fromFileString(const string& data) {
        size_t pos = data.find(';');
        livroId = stoi(data.substr(0, pos));
        quantidade = stoi(data.substr(pos + 1));
    }
};
