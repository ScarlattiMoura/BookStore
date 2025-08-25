#include <iostream>
#include <windows.h>
#include "MinhaConta.h";

using namespace std;

int main() {
    int opcao = 0; // 👈 inicializar!

    while (opcao != 5) {
        SetConsoleOutputCP(CP_UTF8);

        cout << "\n<------- LIVRARIA ------->" << endl;
        cout << "1 - CATÁLOGO LIVROS" << endl;
        cout << "2 - MINHA CONTA" << endl;
        cout << "3 - MINHAS COMPRAS" << endl;
        cout << "4 - CARRINHO" << endl;
        cout << "5 - SAIR" << endl;
        cout << "Escolha: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                break;
            case 2:
                MinhaConta C;
                C.menu();
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                cout << "ENCERRANDO PROGRAMA..." << endl;
                break;
            default:
                cout << "OPÇÃO INVÁLIDA!!!" << endl;
        }
    }

    return 0;
}
