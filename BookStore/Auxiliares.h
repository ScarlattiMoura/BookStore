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

using namespace std;


void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\nPressione Enter para continuar...";
    limparBuffer();
}

void cabecalho(const string& titulo) {
    system("cls || clear");
    cout << "===================================" << endl;
    cout << "      " << titulo << endl;
    cout << "===================================" << endl;
}
