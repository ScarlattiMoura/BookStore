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

class Pessoa {
protected:
    string nome;
    string email;
    string telefone;
public:
    Pessoa(string n = "", string e = "", string t = "")
        : nome(n), email(e), telefone(t) {}

    virtual ~Pessoa() {}

    string getNome() const { return nome; }
    void setNome(string n) { nome = n; }
    string getEmail() const { return email; }
    string getTelefone() const { return telefone; }

    virtual string getTipo() const { return "Pessoa"; }
    virtual void exibir() const {
        cout << "Nome: " << nome << "\nEmail: " << email << "\nTelefone: " << telefone << endl;
    }

    virtual string toFileString() const {
        return nome + ";" + email + ";" + telefone;
    }

    virtual void fromFileString(const string& data) {
        size_t pos1 = data.find(';');
        size_t pos2 = data.find(';', pos1 + 1);
        nome = data.substr(0, pos1);
        email = data.substr(pos1 + 1, pos2 - pos1 - 1);
        telefone = data.substr(pos2 + 1);
    }
};
