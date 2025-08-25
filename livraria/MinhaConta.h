#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class MinhaConta {
private:
    string nome, cpf, endereco, email;

public:

    void Cadastro() {
         cin.ignore();

        cout << "DIGITE SEUS DADOS" << endl;

        cout << "NOME: ";
        getline(cin, nome);

        cout << "CPF: ";
        getline(cin, cpf);

        cout << "E-MAIL: ";
        getline(cin, email);

        cout << "ENDEREÇO: ";
        getline(cin, endereco);

        Salvar(); // salva automaticamente após cadastro
    }

    void Mostrar() {
        cout << "\n--- MEUS DADOS ---\n";
        cout << "Nome: " << nome << endl;
        cout << "CPF: " << cpf << endl;
        cout << "E-mail: " << email << endl;
        cout << "Endereço: " << endereco << endl;
    }

    void Salvar() {
        ofstream arquivo("conta.txt"); // abre para escrita
        if (arquivo.is_open()) {
            arquivo << nome << endl;
            arquivo << cpf << endl;
            arquivo << email << endl;
            arquivo << endereco << endl;
            arquivo.close();
            cout << "Dados salvos com sucesso!" << endl;
        } else {
            cout << "Erro ao salvar os dados!" << endl;
        }
    }

    void Carregar() {
        ifstream arquivo("conta.txt"); // abre para leitura
        if (arquivo.is_open()) {
            getline(arquivo, nome);
            getline(arquivo, cpf);
            getline(arquivo, email);
            getline(arquivo, endereco);
            arquivo.close();
        } else {
            cout << "Nenhum cadastro encontrado. Faça seu cadastro primeiro." << endl;
        }
    }

    void menu() {
        int opcao = 0;

        Carregar(); // carrega dados ao abrir o menu

        while (opcao != 3) {
            cout << "\n<------- MINHA CONTA ------->" << endl;
            cout << "1 - DADOS" << endl;
            cout << "2 - EDITAR DADOS" << endl;
            cout << "3 - VOLTAR" << endl;
            cout << "Escolha: ";
            cin >> opcao;
            cin.ignore(); // 👈 limpa buffer antes de getline no próximo passo

            switch (opcao) {
                case 1:
                    Mostrar();
                    break;

                case 2:
                    Cadastro(); // chama cadastro e já salva
                    break;

                case 3:
                    cout << "Voltando..." << endl;
                    break;

                default:
                    cout << "OPÇÃO INVÁLIDA!!!" << endl;
            }
        }
    }
};

