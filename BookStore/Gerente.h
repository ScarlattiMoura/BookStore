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

class Gerente : public Funcionario {
private:
    string departamento;
public:
    Gerente(string n = "", string e = "", string t = "", int m = 0, double s = 0.0, string d = "")
        : Funcionario(n, e, t, m, s), departamento(d) {}

    string getTipo() const override { return "Gerente"; }

    void exibir() const override {
        Funcionario::exibir();
        cout << "Departamento: " << departamento << endl;
    }

    string toFileString() const override {
        return Funcionario::toFileString() + ";" + departamento;
    }

    void fromFileString(const string& data) override {
        size_t lastPos = data.find_last_of(';');
        size_t prevPos = data.find_last_of(';', lastPos - 1);
        Funcionario::fromFileString(data.substr(0, prevPos));
        departamento = data.substr(lastPos + 1);
    }
};
