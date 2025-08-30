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

class Funcionario : public Pessoa {
protected:
    int matricula;
    double salario;
public:
    Funcionario(string n = "", string e = "", string t = "", int m = 0, double s = 0.0)
        : Pessoa(n, e, t), matricula(m), salario(s) {}

    string getTipo() const override { return "Funcionario"; }
    int getMatricula() const { return matricula; }
    double getSalario() const { return salario; }

    void exibir() const override {
        Pessoa::exibir();
        cout << "Matrícula: " << matricula << "\nSalário: R$ " << fixed << setprecision(2) << salario << endl;
    }

    string toFileString() const override {
        return Pessoa::toFileString() + ";" + to_string(matricula) + ";" + to_string(salario);
    }

    void fromFileString(const string& data) override {
        size_t pos1 = data.find(';');
        size_t pos2 = data.find(';', pos1 + 1);
        size_t pos3 = data.find(';', pos2 + 1);
        size_t pos4 = data.find(';', pos3 + 1);
        Pessoa::fromFileString(data.substr(0, pos3));
        matricula = stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
        salario = stod(data.substr(pos4 + 1));
    }
};
