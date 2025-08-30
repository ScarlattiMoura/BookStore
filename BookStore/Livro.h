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

#include "Livro.h"
#include "Data.h"

using namespace std;

class Livro {
private:
    int id;
    string titulo;
    string genero;
    double preco;
    int quantidade;
    Data dataPublicacao;
    string autor; // Agora é uma string simples
public:
    Livro(int i = 0, string t = "", string g = "", double p = 0.0, int q = 0,
          Data dp = Data(), string a = "")
        : id(i), titulo(t), genero(g), preco(p), quantidade(q), dataPublicacao(dp), autor(a) {}

    int getId() const { return id; }
    string getTitulo() const { return titulo; }
    string getGenero() const { return genero; }
    double getPreco() const { return preco; }
    int getQuantidade() const { return quantidade; }
    void setQuantidade(int q) { quantidade = q; }
    void setPreco(double p) { preco = p; }
    void setTitulo(const string& t) { titulo = t; }
    void setGenero(const string& g) { genero = g; }
    string getAutor() const { return autor; }
    void setAutor(const string& a) { autor = a; }
    Data getDataPublicacao() const { return dataPublicacao; }
    void setDataPublicacao(const Data& dp) { dataPublicacao = dp; }

    void exibir() const {
        cout << "ID: " << id << "\nTítulo: " << titulo << "\nGênero: " << genero
             << "\nPreço: R$ " << fixed << setprecision(2) << preco
             << "\nQuantidade: " << quantidade << "\nData Publicação: " << dataPublicacao
             << "\nAutor: " << autor << endl;
    }

    string toFileString() const {
        return to_string(id) + ";" + titulo + ";" + genero + ";" + to_string(preco) + ";" +
               to_string(quantidade) + ";" + dataPublicacao.toString() + ";" + autor;
    }

    void fromFileString(const string& data) {
        vector<string> parts;
        stringstream ss(data);
        string part;

        while (getline(ss, part, ';')) {
            parts.push_back(part);
        }

        if (parts.size() >= 7) {
            id = stoi(parts[0]);
            titulo = parts[1];
            genero = parts[2];
            preco = stod(parts[3]);
            quantidade = stoi(parts[4]);
            dataPublicacao.fromString(parts[5]);
            autor = parts[6];
        }
    }
};
