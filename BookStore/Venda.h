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

#include "ItemVenda.h"
#include "Funcionarios.h"

using namespace std;

class Venda {
private:
    int id;
    Data data;
    vector<ItemVenda> itens;
    double total;
    int funcionarioId;
public:
    Venda(int i = 0, Data d = Data(), int fid = 0)
        : id(i), data(d), total(0.0), funcionarioId(fid) {}

    void adicionarItem(int livroId, int quantidade, double preco) {
        itens.emplace_back(livroId, quantidade);
        total += preco * quantidade;
    }

    int getId() const { return id; }
    Data getData() const { return data; }
    double getTotal() const { return total; }
    int getFuncionarioId() const { return funcionarioId; }
    const vector<ItemVenda>& getItens() const { return itens; }
    void setFuncionarioId(int fid) { funcionarioId = fid; }

    void exibir(const vector<Livro>& livros, const vector<unique_ptr<Pessoa>>& pessoas) const {
        cout << "Venda #" << id << " - Data: " << data << "\nTotal: R$ " << fixed << setprecision(2) << total;

        // Encontrar funcionário
        string nomeFuncionario = "N/A";
        for (const auto& pessoa : pessoas) {
            if (pessoa->getTipo() == "Funcionario" || pessoa->getTipo() == "Gerente") {
                Funcionario* func = static_cast<Funcionario*>(pessoa.get());
                if (func->getMatricula() == funcionarioId) {
                    nomeFuncionario = func->getNome();
                    break;
                }
            }
        }
        cout << "\nFuncionário: " << nomeFuncionario;

        cout << "\nItens:" << endl;
        for (const auto& item : itens) {
            // Encontrar livro
            string tituloLivro = "N/A";
            for (const auto& livro : livros) {
                if (livro.getId() == item.getLivroId()) {
                    tituloLivro = livro.getTitulo();
                    break;
                }
            }
            cout << "  - " << tituloLivro << " x" << item.getQuantidade() << endl;
        }
    }

    string toFileString() const {
        string result = to_string(id) + ";" + data.toString() + ";" + to_string(total) + ";" +
                      to_string(funcionarioId) + ";";
        for (const auto& item : itens) {
            result += item.toFileString() + "|";
        }
        if (!itens.empty()) result.pop_back();
        return result;
    }

    void fromFileString(const string& dataStr) {
        vector<string> parts;
        stringstream ss(dataStr);
        string part;

        while (getline(ss, part, ';')) {
            parts.push_back(part);
        }

        if (parts.size() >= 4) {
            id = stoi(parts[0]);
            data.fromString(parts[1]);
            total = stod(parts[2]);
            funcionarioId = stoi(parts[3]);

            if (parts.size() > 4) {
                stringstream itemsSS(parts[4]);
                string itemStr;
                while (getline(itemsSS, itemStr, '|')) {
                    ItemVenda item;
                    item.fromFileString(itemStr);
                    itens.push_back(item);
                }
            }
        }
    }
};
