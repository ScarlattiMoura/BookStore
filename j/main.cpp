#include <ncurses.h>
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

// ==================== CONFIGURAÇÕES NCURSES ====================
#define WIDTH 80
#define HEIGHT 24

// Cores
void init_colors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);
}

void draw_border(WINDOW *win) {
    box(win, 0, 0);
    wrefresh(win);
}

void center_text(WINDOW *win, int y, const string &text) {
    int x = (getmaxx(win) - text.length()) / 2;
    mvwprintw(win, y, x, "%s", text.c_str());
}

void show_message(const string &msg, int color_pair = 4) {
    WINDOW *msg_win = newwin(3, msg.length() + 4, (HEIGHT - 3) / 2, (WIDTH - msg.length() - 4) / 2);
    wattron(msg_win, COLOR_PAIR(color_pair));
    draw_border(msg_win);
    center_text(msg_win, 1, msg);
    wattroff(msg_win, COLOR_PAIR(color_pair));
    wrefresh(msg_win);
    napms(1500);
    delwin(msg_win);
    touchwin(stdscr);
    refresh();
}

string get_input(WINDOW *win, const string &prompt, int y, int x) {
    echo();
    curs_set(1);

    wattron(win, COLOR_PAIR(7));
    mvwprintw(win, y, x, "%s", prompt.c_str());
    wattroff(win, COLOR_PAIR(7));

    char input[100];
    wgetnstr(win, input, 99);

    noecho();
    curs_set(0);

    return string(input);
}

// ==================== CLASSES BÁSICAS ====================
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
};

// ==================== CLASSES DE PESSOAS ====================
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
    virtual void exibir(WINDOW *win, int &y) const {
        mvwprintw(win, y++, 2, "Nome: %s", nome.c_str());
        mvwprintw(win, y++, 2, "Email: %s", email.c_str());
        mvwprintw(win, y++, 2, "Telefone: %s", telefone.c_str());
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

    void exibir(WINDOW *win, int &y) const override {
        Pessoa::exibir(win, y);
        mvwprintw(win, y++, 2, "Matrícula: %d", matricula);
        mvwprintw(win, y++, 2, "Salário: R$ %.2f", salario);
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

class Gerente : public Funcionario {
private:
    string departamento;
public:
    Gerente(string n = "", string e = "", string t = "", int m = 0, double s = 0.0, string d = "")
        : Funcionario(n, e, t, m, s), departamento(d) {}

    string getTipo() const override { return "Gerente"; }

    void exibir(WINDOW *win, int &y) const override {
        Funcionario::exibir(win, y);
        mvwprintw(win, y++, 2, "Departamento: %s", departamento.c_str());
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

// ==================== CLASSES DE NEGÓCIO ====================
class Livro {
private:
    int id;
    string titulo;
    string genero;
    double preco;
    int quantidade;
    Data dataPublicacao;
    string autor;
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

    void exibir(WINDOW *win, int &y) const {
        mvwprintw(win, y++, 2, "ID: %d", id);
        mvwprintw(win, y++, 2, "Título: %s", titulo.c_str());
        mvwprintw(win, y++, 2, "Autor: %s", autor.c_str());
        mvwprintw(win, y++, 2, "Gênero: %s", genero.c_str());
        mvwprintw(win, y++, 2, "Preço: R$ %.2f", preco);
        mvwprintw(win, y++, 2, "Quantidade: %d", quantidade);
        mvwprintw(win, y++, 2, "Data Publicação: %s", dataPublicacao.toString().c_str());
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

    void exibir(WINDOW *win, const vector<Livro>& livros, const vector<unique_ptr<Pessoa>>& pessoas) const {
        int y = 1;
        mvwprintw(win, y++, 2, "Venda #%d - Data: %s", id, data.toString().c_str());
        mvwprintw(win, y++, 2, "Total: R$ %.2f", total);

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
        mvwprintw(win, y++, 2, "Funcionário: %s", nomeFuncionario.c_str());

        mvwprintw(win, y++, 2, "Itens:");
        for (const auto& item : itens) {
            string tituloLivro = "N/A";
            for (const auto& livro : livros) {
                if (livro.getId() == item.getLivroId()) {
                    tituloLivro = livro.getTitulo();
                    break;
                }
            }
            mvwprintw(win, y++, 4, "- %s x%d", tituloLivro.c_str(), item.getQuantidade());
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

// ==================== CLASSE GERENCIADORA ====================
class GerenciadorLivraria {
private:
    vector<unique_ptr<Pessoa>> pessoas;
    vector<Livro> livros;
    vector<Venda> vendas;
    string arquivoDados;

    int proximoIdLivro;
    int proximoIdVenda;

    WINDOW *main_win;

    int PesquisarLivroPorTitulo(const string& titulo) const {
        vector<int> indices;
        for (size_t i = 0; i < livros.size(); i++) {
            if (livros[i].getTitulo().find(titulo) != string::npos) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) return -1;
        if (indices.size() == 1) return indices[0];

        WINDOW *result_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(result_win);
        center_text(result_win, 1, "MÚLTIPLOS RESULTADOS ENCONTRADOS");

        int y = 3;
        mvwprintw(result_win, y++, 2, "Foram encontrados %zd resultados:", indices.size());

        for (size_t i = 0; i < indices.size() && y < HEIGHT - 6; i++) {
            mvwprintw(result_win, y++, 2, "%zd. %s", i + 1, livros[indices[i]].getTitulo().c_str());
        }

        mvwprintw(result_win, HEIGHT - 6, 2, "Escolha o número do item desejado (0 para cancelar): ");
        wrefresh(result_win);

        echo();
        curs_set(1);
        int escolha;
        wscanw(result_win, "%d", &escolha);
        noecho();
        curs_set(0);

        delwin(result_win);
        touchwin(stdscr);
        refresh();

        if (escolha > 0 && escolha <= static_cast<int>(indices.size())) {
            return indices[escolha - 1];
        }

        return -1;
    }

    int PesquisarPessoaPorNome(const string& nome) const {
        vector<int> indices;
        for (size_t i = 0; i < pessoas.size(); i++) {
            if (pessoas[i]->getNome().find(nome) != string::npos) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) return -1;
        if (indices.size() == 1) return indices[0];

        WINDOW *result_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(result_win);
        center_text(result_win, 1, "MÚLTIPLOS RESULTADOS ENCONTRADOS");

        int y = 3;
        mvwprintw(result_win, y++, 2, "Foram encontrados %zd resultados:", indices.size());

        for (size_t i = 0; i < indices.size() && y < HEIGHT - 6; i++) {
            mvwprintw(result_win, y++, 2, "%zd. %s", i + 1, pessoas[indices[i]]->getNome().c_str());
        }

        mvwprintw(result_win, HEIGHT - 6, 2, "Escolha o número do item desejado (0 para cancelar): ");
        wrefresh(result_win);

        echo();
        curs_set(1);
        int escolha;
        wscanw(result_win, "%d", &escolha);
        noecho();
        curs_set(0);

        delwin(result_win);
        touchwin(stdscr);
        refresh();

        if (escolha > 0 && escolha <= static_cast<int>(indices.size())) {
            return indices[escolha - 1];
        }

        return -1;
    }

    vector<Funcionario*> getFuncionarios() const {
        vector<Funcionario*> funcionarios;
        for (const auto& pessoa : pessoas) {
            if (pessoa->getTipo() == "Funcionario" || pessoa->getTipo() == "Gerente") {
                funcionarios.push_back(static_cast<Funcionario*>(pessoa.get()));
            }
        }
        return funcionarios;
    }

    int show_menu(const vector<string> &options, const string &title) {
        int choice = 0;
        int highlight = 0;

        while (true) {
            WINDOW *menu_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
            draw_border(menu_win);
            center_text(menu_win, 1, title);

            for (int i = 0; i < (int)options.size(); i++) {
                if (i == highlight) {
                    wattron(menu_win, A_REVERSE);
                    mvwprintw(menu_win, i + 3, 4, "%s", options[i].c_str());
                    wattroff(menu_win, A_REVERSE);
                } else {
                    mvwprintw(menu_win, i + 3, 4, "%s", options[i].c_str());
                }
            }

            mvwprintw(menu_win, HEIGHT - 8, 4, "Use ↑↓ para navegar, Enter para selecionar");
            wrefresh(menu_win);

            int ch = getch();
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + options.size()) % options.size();
                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % options.size();
                    break;
                case 10:
                    choice = highlight;
                    delwin(menu_win);
                    return choice;
                case 27:
                    delwin(menu_win);
                    return -1;
            }

            delwin(menu_win);
        }
    }

public:
    GerenciadorLivraria(WINDOW *win, const string& arquivo = "dados_livraria.txt")
        : main_win(win), arquivoDados(arquivo), proximoIdLivro(1), proximoIdVenda(1) {
        carregarDados();
    }

    void carregarDados() {
        ifstream file(arquivoDados);
        if (!file.is_open()) {
            pessoas.push_back(make_unique<Funcionario>("João Silva", "joao@livraria.com", "4444-4444", 1001, 2500.00));
            pessoas.push_back(make_unique<Gerente>("Maria Santos", "maria@livraria.com", "5555-5555", 1002, 4500.00, "Vendas"));

            livros.push_back(Livro(1, "1984", "Ficção Distópica", 45.90, 15, Data(1949, 6, 8), "George Orwell"));
            livros.push_back(Livro(2, "O Senhor dos Anéis", "Fantasia", 89.90, 8, Data(1954, 7, 29), "J.R.R. Tolkien"));
            livros.push_back(Livro(3, "Dom Casmurro", "Romance", 29.90, 20, Data(1899, 1, 1), "Machado de Assis"));
            proximoIdLivro = 4;
            proximoIdVenda = 1;
            return;
        }

        string linha;
        while (getline(file, linha)) {
            if (linha.empty()) continue;

            string tipo = linha.substr(0, linha.find(';'));
            string dados = linha.substr(linha.find(';') + 1);

            if (tipo == "PESSOA") {
                string subtipo = dados.substr(0, dados.find(';'));
                string dadosPessoa = dados.substr(dados.find(';') + 1);

                if (subtipo == "Funcionario") {
                    auto func = make_unique<Funcionario>();
                    func->fromFileString(dadosPessoa);
                    pessoas.push_back(move(func));
                }
                else if (subtipo == "Gerente") {
                    auto gerente = make_unique<Gerente>();
                    gerente->fromFileString(dadosPessoa);
                    pessoas.push_back(move(gerente));
                }
            }
            else if (tipo == "LIVRO") {
                Livro livro;
                livro.fromFileString(dados);
                livros.push_back(livro);
                if (livro.getId() >= proximoIdLivro) {
                    proximoIdLivro = livro.getId() + 1;
                }
            }
            else if (tipo == "VENDA") {
                Venda venda;
                venda.fromFileString(dados);
                vendas.push_back(venda);
                if (venda.getId() >= proximoIdVenda) {
                    proximoIdVenda = venda.getId() + 1;
                }
            }
        }
        file.close();
    }

    void salvarDados() {
        ofstream file(arquivoDados);
        for (const auto& pessoa : pessoas) {
            file << "PESSOA;" << pessoa->getTipo() << ";" << pessoa->toFileString() << endl;
        }

        for (const auto& livro : livros) {
            file << "LIVRO;" << livro.toFileString() << endl;
        }

        for (const auto& venda : vendas) {
            file << "VENDA;" << venda.toFileString() << endl;
        }
        file.close();
    }

    void executar() {
        int opcao;

        while (true) {
            vector<string> main_menu = {
                "Gerenciar Livros",
                "Gerenciar Pessoas",
                "Realizar Venda",
                "Relatórios",
                "Sair"
            };

            opcao = show_menu(main_menu, "SISTEMA DE LIVRARIA - MENU PRINCIPAL");

            if (opcao == -1) break;

            switch(opcao) {
                case 0: menuLivros(); break;
                case 1: menuPessoas(); break;
                case 2: realizarVenda(); break;
                case 3: menuRelatorios(); break;
                case 4:
                    show_message("Saindo do sistema...");
                    salvarDados();
                    return;
            }
        }
    }

    void menuLivros() {
        while (true) {
            vector<string> menu_options = {
                "Adicionar Livro",
                "Listar Livros",
                "Buscar Livro",
                "Editar Livro",
                "Remover Livro",
                "Voltar"
            };

            int choice = show_menu(menu_options, "GERENCIAMENTO DE LIVROS");

            if (choice == -1 || choice == 5) break;

            switch(choice) {
                case 0: adicionarLivro(); break;
                case 1: listarLivros(); break;
                case 2: buscarLivro(); break;
                case 3: editarLivro(); break;
                case 4: removerLivro(); break;
            }
        }
    }

    void adicionarLivro() {
        WINDOW *input_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(input_win);
        center_text(input_win, 1, "ADICIONAR LIVRO");

        string titulo = get_input(input_win, "Título: ", 3, 4);
        string autor = get_input(input_win, "Autor: ", 4, 4);
        string genero = get_input(input_win, "Gênero: ", 5, 4);
        string preco_str = get_input(input_win, "Preço: R$ ", 6, 4);
        string qtd_str = get_input(input_win, "Quantidade: ", 7, 4);
        string data_str = get_input(input_win, "Data (DD/MM/AAAA): ", 8, 4);

        double preco = stod(preco_str);
        int quantidade = stoi(qtd_str);

        Data data;
        data.fromString(data_str);

        livros.push_back(Livro(proximoIdLivro++, titulo, genero, preco, quantidade, data, autor));

        delwin(input_win);
        show_message("Livro adicionado com sucesso!");
    }

    void listarLivros() {
        if(livros.empty()) {
            show_message("Nenhum livro cadastrado.", 6);
            return;
        }

        WINDOW *list_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(list_win);
        center_text(list_win, 1, "LISTA DE LIVROS");

        int y = 3;
        for(const auto& livro : livros) {
            if (y >= HEIGHT - 6) {
                mvwprintw(list_win, HEIGHT - 6, 2, "Pressione qualquer tecla para continuar...");
                wrefresh(list_win);
                getch();
                werase(list_win);
                draw_border(list_win);
                center_text(list_win, 1, "LISTA DE LIVROS");
                y = 3;
            }

            mvwprintw(list_win, y++, 4, "ID: %d | %s | %s | R$ %.2f | Estoque: %d",
                     livro.getId(), livro.getTitulo().c_str(), livro.getAutor().c_str(),
                     livro.getPreco(), livro.getQuantidade());
        }

        mvwprintw(list_win, HEIGHT - 6, 2, "Pressione qualquer tecla para voltar...");
        wrefresh(list_win);
        getch();
        delwin(list_win);
    }

    void buscarLivro() {
        WINDOW *search_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(search_win);
        center_text(search_win, 1, "BUSCAR LIVRO");

        string termo = get_input(search_win, "Digite o título: ", 3, 4);
        delwin(search_win);

        int indice = PesquisarLivroPorTitulo(termo);
        if (indice == -1) {
            show_message("Livro não encontrado!", 5);
        } else {
            WINDOW *result_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
            draw_border(result_win);
            center_text(result_win, 1, "LIVRO ENCONTRADO");

            int y = 3;
            livros[indice].exibir(result_win, y);

            mvwprintw(result_win, HEIGHT - 6, 2, "Pressione qualquer tecla para voltar...");
            wrefresh(result_win);
            getch();
            delwin(result_win);
        }
    }

    void editarLivro() {
        WINDOW *search_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(search_win);
        center_text(search_win, 1, "EDITAR LIVRO");

        string termo = get_input(search_win, "Digite o título: ", 3, 4);
        delwin(search_win);

        int indice = PesquisarLivroPorTitulo(termo);
        if (indice == -1) {
            show_message("Livro não encontrado!", 5);
            return;
        }

        Livro& livro = livros[indice];

        WINDOW *edit_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(edit_win);
        center_text(edit_win, 1, "EDITAR LIVRO");

        mvwprintw(edit_win, 3, 4, "Editando: %s", livro.getTitulo().c_str());

        string novoTitulo = get_input(edit_win, "Novo título: ", 5, 4);
        string novoAutor = get_input(edit_win, "Novo autor: ", 6, 4);
        string novoGenero = get_input(edit_win, "Novo gênero: ", 7, 4);
        string novoPreco = get_input(edit_win, "Novo preço: ", 8, 4);
        string novaQuantidade = get_input(edit_win, "Nova quantidade: ", 9, 4);

        if(!novoTitulo.empty()) livro.setTitulo(novoTitulo);
        if(!novoAutor.empty()) livro.setAutor(novoAutor);
        if(!novoGenero.empty()) livro.setGenero(novoGenero);
        if(!novoPreco.empty()) livro.setPreco(stod(novoPreco));
        if(!novaQuantidade.empty()) livro.setQuantidade(stoi(novaQuantidade));

        delwin(edit_win);
        show_message("Livro editado com sucesso!");
    }

    void removerLivro() {
        WINDOW *search_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(search_win);
        center_text(search_win, 1, "REMOVER LIVRO");

        string termo = get_input(search_win, "Digite o título: ", 3, 4);
        delwin(search_win);

        int indice = PesquisarLivroPorTitulo(termo);
        if (indice == -1) {
            show_message("Livro não encontrado!", 5);
            return;
        }

        WINDOW *confirm_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(confirm_win);
        center_text(confirm_win, 1, "CONFIRMAR REMOÇÃO");

        mvwprintw(confirm_win, 3, 4, "Tem certeza que deseja remover '%s'?", livros[indice].getTitulo().c_str());
        mvwprintw(confirm_win, 5, 4, "Pressione 'S' para confirmar ou qualquer outra tecla para cancelar");
        wrefresh(confirm_win);

        int ch = getch();
        delwin(confirm_win);

        if (ch == 's' || ch == 'S') {
            livros.erase(livros.begin() + indice);
            show_message("Livro removido com sucesso!");
        } else {
            show_message("Operação cancelada.", 6);
        }
    }

    void menuPessoas() {
        while (true) {
            vector<string> menu_options = {
                "Adicionar Pessoa",
                "Listar Pessoas",
                "Buscar Pessoa",
                "Editar Pessoa",
                "Remover Pessoa",
                "Voltar"
            };

            int choice = show_menu(menu_options, "GERENCIAMENTO DE PESSOAS");

            if (choice == -1 || choice == 5) break;

            switch(choice) {
                case 0: adicionarPessoa(); break;
                case 1: listarPessoas(); break;
                case 2: buscarPessoa(); break;
                case 3: editarPessoa(); break;
                case 4: removerPessoa(); break;
            }
        }
    }

    void adicionarPessoa() {
        vector<string> tipo_options = {"Funcionário", "Gerente"};
        int tipo_choice = show_menu(tipo_options, "SELECIONE O TIPO DE PESSOA");

        if (tipo_choice == -1) return;

        WINDOW *input_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(input_win);
        center_text(input_win, 1, "ADICIONAR PESSOA");

        string nome = get_input(input_win, "Nome: ", 3, 4);
        string email = get_input(input_win, "Email: ", 4, 4);
        string telefone = get_input(input_win, "Telefone: ", 5, 4);
        string matricula_str = get_input(input_win, "Matrícula: ", 6, 4);
        string salario_str = get_input(input_win, "Salário: R$ ", 7, 4);

        int matricula = stoi(matricula_str);
        double salario = stod(salario_str);

        if (tipo_choice == 0) {
            pessoas.push_back(make_unique<Funcionario>(nome, email, telefone, matricula, salario));
        } else {
            string departamento = get_input(input_win, "Departamento: ", 8, 4);
            pessoas.push_back(make_unique<Gerente>(nome, email, telefone, matricula, salario, departamento));
        }

        delwin(input_win);
        show_message("Pessoa adicionada com sucesso!");
    }

    void listarPessoas() {
        if(pessoas.empty()) {
            show_message("Nenhuma pessoa cadastrada.", 6);
            return;
        }

        WINDOW *list_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(list_win);
        center_text(list_win, 1, "LISTA DE PESSOAS");

        int y = 3;
        for(const auto& pessoa : pessoas) {
            if (y >= HEIGHT - 6) {
                mvwprintw(list_win, HEIGHT - 6, 2, "Pressione qualquer tecla para continuar...");
                wrefresh(list_win);
                getch();
                werase(list_win);
                draw_border(list_win);
                center_text(list_win, 1, "LISTA DE PESSOAS");
                y = 3;
            }

            mvwprintw(list_win, y++, 4, "%s: %s", pessoa->getTipo().c_str(), pessoa->getNome().c_str());
        }

        mvwprintw(list_win, HEIGHT - 6, 2, "Pressione qualquer tecla para voltar...");
        wrefresh(list_win);
        getch();
        delwin(list_win);
    }

    void buscarPessoa() {
        WINDOW *search_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(search_win);
        center_text(search_win, 1, "BUSCAR PESSOA");

        string termo = get_input(search_win, "Digite o nome: ", 3, 4);
        delwin(search_win);

        int indice = PesquisarPessoaPorNome(termo);
        if (indice == -1) {
            show_message("Pessoa não encontrada!", 5);
        } else {
            WINDOW *result_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
            draw_border(result_win);
            center_text(result_win, 1, "PESSOA ENCONTRADA");

            int y = 3;
            pessoas[indice]->exibir(result_win, y);

            mvwprintw(result_win, HEIGHT - 6, 2, "Pressione qualquer tecla para voltar...");
            wrefresh(result_win);
            getch();
            delwin(result_win);
        }
    }

    void editarPessoa() {
        WINDOW *search_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(search_win);
        center_text(search_win, 1, "EDITAR PESSOA");

        string termo = get_input(search_win, "Digite o nome: ", 3, 4);
        delwin(search_win);

        int indice = PesquisarPessoaPorNome(termo);
        if (indice == -1) {
            show_message("Pessoa não encontrada!", 5);
            return;
        }

        WINDOW *edit_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(edit_win);
        center_text(edit_win, 1, "EDITAR PESSOA");

        mvwprintw(edit_win, 3, 4, "Editando: %s", pessoas[indice]->getNome().c_str());

        string novoNome = get_input(edit_win, "Novo nome: ", 5, 4);
        string novoEmail = get_input(edit_win, "Novo email: ", 6, 4);
        string novoTelefone = get_input(edit_win, "Novo telefone: ", 7, 4);

        if(!novoNome.empty()) pessoas[indice]->setNome(novoNome);

        delwin(edit_win);
        show_message("Pessoa editada com sucesso!");
    }

    void removerPessoa() {
        WINDOW *search_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(search_win);
        center_text(search_win, 1, "REMOVER PESSOA");

        string termo = get_input(search_win, "Digite o nome: ", 3, 4);
        delwin(search_win);

        int indice = PesquisarPessoaPorNome(termo);
        if (indice == -1) {
            show_message("Pessoa não encontrada!", 5);
            return;
        }

        WINDOW *confirm_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(confirm_win);
        center_text(confirm_win, 1, "CONFIRMAR REMOÇÃO");

        mvwprintw(confirm_win, 3, 4, "Tem certeza que deseja remover '%s'?", pessoas[indice]->getNome().c_str());
        mvwprintw(confirm_win, 5, 4, "Pressione 'S' para confirmar ou qualquer outra tecla para cancelar");
        wrefresh(confirm_win);

        int ch = getch();
        delwin(confirm_win);

        if (ch == 's' || ch == 'S') {
            pessoas.erase(pessoas.begin() + indice);
            show_message("Pessoa removida com sucesso!");
        } else {
            show_message("Operação cancelada.", 6);
        }
    }

    void realizarVenda() {
        vector<Funcionario*> funcionarios = getFuncionarios();
        if (funcionarios.empty()) {
            show_message("Nenhum funcionário cadastrado!", 5);
            return;
        }

        vector<string> func_options;
        for (const auto& func : funcionarios) {
            func_options.push_back(func->getNome() + " (" + func->getTipo() + ")");
        }

        int func_choice = show_menu(func_options, "SELECIONE O FUNCIONÁRIO");
        if (func_choice == -1) return;

        Funcionario* funcionario = funcionarios[func_choice];
        Venda novaVenda(proximoIdVenda++, Data::dataAtual(), funcionario->getMatricula());

        while (true) {
            WINDOW *venda_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
            draw_border(venda_win);
            center_text(venda_win, 1, "REALIZAR VENDA");

            mvwprintw(venda_win, 3, 4, "Funcionário: %s", funcionario->getNome().c_str());
            mvwprintw(venda_win, 4, 4, "Total atual: R$ %.2f", novaVenda.getTotal());

            string titulo = get_input(venda_win, "Digite o título do livro (ou 'finalizar'): ", 6, 4);
            delwin(venda_win);

            if (titulo == "finalizar") break;

            int indice = PesquisarLivroPorTitulo(titulo);
            if (indice == -1) {
                show_message("Livro não encontrado!", 5);
                continue;
            }

            Livro& livro = livros[indice];

            WINDOW *qtd_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
            draw_border(qtd_win);
            center_text(qtd_win, 1, "SELECIONAR QUANTIDADE");

            mvwprintw(qtd_win, 3, 4, "Livro: %s", livro.getTitulo().c_str());
            mvwprintw(qtd_win, 4, 4, "Preço: R$ %.2f", livro.getPreco());
            mvwprintw(qtd_win, 5, 4, "Estoque: %d", livro.getQuantidade());

            string qtd_str = get_input(qtd_win, "Quantidade: ", 7, 4);
            delwin(qtd_win);

            int quantidade = stoi(qtd_str);

            if (quantidade <= 0) {
                show_message("Quantidade inválida!", 5);
                continue;
            }

            if (quantidade > livro.getQuantidade()) {
                show_message("Estoque insuficiente!", 5);
                continue;
            }

            novaVenda.adicionarItem(livro.getId(), quantidade, livro.getPreco());
            livro.setQuantidade(livro.getQuantidade() - quantidade);

            show_message("Item adicionado à venda!");
        }

        if (!novaVenda.getItens().empty()) {
            vendas.push_back(novaVenda);
            show_message("Venda realizada com sucesso! Total: R$ " + to_string(novaVenda.getTotal()));
        } else {
            show_message("Venda cancelada.", 6);
        }
    }

    void menuRelatorios() {
        while (true) {
            vector<string> menu_options = {
                "Relatório de Pessoas",
                "Relatório de Livros",
                "Relatório de Vendas",
                "Relatório de Estoque",
                "Voltar"
            };

            int choice = show_menu(menu_options, "RELATÓRIOS");

            if (choice == -1 || choice == 4) break;

            switch(choice) {
                case 0: relatorioPessoas(); break;
                case 1: relatorioLivros(); break;
                case 2: relatorioVendas(); break;
                case 3: relatorioEstoque(); break;
            }
        }
    }

    void relatorioPessoas() {
        WINDOW *report_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(report_win);
        center_text(report_win, 1, "RELATÓRIO DE PESSOAS");

        int y = 3;
        mvwprintw(report_win, y++, 4, "Total de pessoas: %zd", pessoas.size());

        int funcionarios = 0, gerentes = 0;
        for (const auto& pessoa : pessoas) {
            if (pessoa->getTipo() == "Funcionario") funcionarios++;
            else if (pessoa->getTipo() == "Gerente") gerentes++;
        }

        mvwprintw(report_win, y++, 4, "Funcionários: %d", funcionarios);
        mvwprintw(report_win, y++, 4, "Gerentes: %d", gerentes);

        mvwprintw(report_win, HEIGHT - 6, 4, "Pressione qualquer tecla para voltar...");
        wrefresh(report_win);
        getch();
        delwin(report_win);
    }

    void relatorioLivros() {
        WINDOW *report_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(report_win);
        center_text(report_win, 1, "RELATÓRIO DE LIVROS");

        int y = 3;
        mvwprintw(report_win, y++, 4, "Total de livros: %zd", livros.size());

        double valorTotal = 0.0;
        int quantidadeTotal = 0;
        for (const auto& livro : livros) {
            valorTotal += livro.getPreco() * livro.getQuantidade();
            quantidadeTotal += livro.getQuantidade();
        }

        mvwprintw(report_win, y++, 4, "Quantidade em estoque: %d", quantidadeTotal);
        mvwprintw(report_win, y++, 4, "Valor total do estoque: R$ %.2f", valorTotal);

        mvwprintw(report_win, HEIGHT - 6, 4, "Pressione qualquer tecla para voltar...");
        wrefresh(report_win);
        getch();
        delwin(report_win);
    }

    void relatorioVendas() {
        WINDOW *report_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(report_win);
        center_text(report_win, 1, "RELATÓRIO DE VENDAS");

        int y = 3;
        mvwprintw(report_win, y++, 4, "Total de vendas: %zd", vendas.size());

        double totalVendas = 0.0;
        for (const auto& venda : vendas) {
            totalVendas += venda.getTotal();
        }

        mvwprintw(report_win, y++, 4, "Valor total em vendas: R$ %.2f", totalVendas);

        if (!vendas.empty()) {
            mvwprintw(report_win, y++, 4, "Últimas 5 vendas:");
            int start = max(0, static_cast<int>(vendas.size()) - 5);
            for (size_t i = start; i < vendas.size(); i++) {
                if (y >= HEIGHT - 6) break;
                mvwprintw(report_win, y++, 6, "Venda #%d - R$ %.2f", vendas[i].getId(), vendas[i].getTotal());
            }
        }

        mvwprintw(report_win, HEIGHT - 6, 4, "Pressione qualquer tecla para voltar...");
        wrefresh(report_win);
        getch();
        delwin(report_win);
    }

    void relatorioEstoque() {
        WINDOW *report_win = newwin(HEIGHT - 4, WIDTH - 4, 2, 2);
        draw_border(report_win);
        center_text(report_win, 1, "RELATÓRIO DE ESTOQUE");

        int y = 3;
        if(livros.empty()) {
            mvwprintw(report_win, y++, 4, "Nenhum livro em estoque.");
        } else {
            double valorTotal = 0.0;
            int quantidadeTotal = 0;
            int livrosBaixoEstoque = 0;

            for(const auto& livro : livros) {
                valorTotal += livro.getPreco() * livro.getQuantidade();
                quantidadeTotal += livro.getQuantidade();
                if (livro.getQuantidade() < 5) livrosBaixoEstoque++;
            }

            mvwprintw(report_win, y++, 4, "Total de livros: %zd", livros.size());
            mvwprintw(report_win, y++, 4, "Quantidade total: %d", quantidadeTotal);
            mvwprintw(report_win, y++, 4, "Valor total: R$ %.2f", valorTotal);
            mvwprintw(report_win, y++, 4, "Livros com estoque baixo: %d", livrosBaixoEstoque);

            if (livrosBaixoEstoque > 0) {
                mvwprintw(report_win, y++, 4, "Livros com estoque crítico:");
                for(const auto& livro : livros) {
                    if(livro.getQuantidade() < 5) {
                        mvwprintw(report_win, y++, 6, "- %s (%d unidades)", livro.getTitulo().c_str(), livro.getQuantidade());
                        if (y >= HEIGHT - 6) break;
                    }
                }
            }
        }

        mvwprintw(report_win, HEIGHT - 6, 4, "Pressione qualquer tecla para voltar...");
        wrefresh(report_win);
        getch();
        delwin(report_win);
    }
};

// ==================== FUNÇÃO PRINCIPAL ====================

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors() == FALSE) {
        endwin();
        printf("Seu terminal não suporta cores\n");
        return 1;
    }

    init_colors();

    WINDOW *main_win = newwin(HEIGHT, WIDTH, 0, 0);
    wattron(main_win, COLOR_PAIR(1));
    draw_border(main_win);
    center_text(main_win, 1, "SISTEMA DE LIVRARIA");
    center_text(main_win, 2, "VISÃO DO FUNCIONÁRIO");
    wattroff(main_win, COLOR_PAIR(1));
    wrefresh(main_win);

    GerenciadorLivraria gerenciador(main_win);
    gerenciador.executar();

    delwin(main_win);
    endwin();

    return 0;
}
