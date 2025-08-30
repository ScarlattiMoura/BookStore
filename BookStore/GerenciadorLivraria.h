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

#include "Pessoas.h"
#include "Livro.h"
#include "Data.h"
#include "Venda.h"
#include "Auxiliares.h"
#include "Gerente.h"

using namespace std;

class GerenciadorLivraria {
private:
    vector<unique_ptr<Pessoa>> pessoas;
    vector<Livro> livros;
    vector<Venda> vendas;
    string arquivoDados;

    int proximoIdLivro;
    int proximoIdVenda;

    // Método de pesquisa para livros
    int PesquisarLivroPorTitulo(const string& titulo) const {
        vector<int> indices;
        for (size_t i = 0; i < livros.size(); i++) {
            if (livros[i].getTitulo().find(titulo) != string::npos) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) return -1;
        if (indices.size() == 1) return indices[0];

        cout << "Foram encontrados " << indices.size() << " resultados:" << endl;
        for (size_t i = 0; i < indices.size(); i++) {
            cout << i + 1 << ". ";
            livros[indices[i]].exibir();
            cout << "-------------------" << endl;
        }

        cout << "Escolha o número do item desejado: ";
        int escolha;
        cin >> escolha;
        limparBuffer();

        if (escolha > 0 && escolha <= static_cast<int>(indices.size())) {
            return indices[escolha - 1];
        }

        return -1;
    }

    // Método de pesquisa para pessoas
    int PesquisarPessoaPorNome(const string& nome) const {
        vector<int> indices;
        for (size_t i = 0; i < pessoas.size(); i++) {
            if (pessoas[i]->getNome().find(nome) != string::npos) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) return -1;
        if (indices.size() == 1) return indices[0];

        cout << "Foram encontrados " << indices.size() << " resultados:" << endl;
        for (size_t i = 0; i < indices.size(); i++) {
            cout << i + 1 << ". ";
            pessoas[indices[i]]->exibir();
            cout << "-------------------" << endl;
        }

        cout << "Escolha o número do item desejado: ";
        int escolha;
        cin >> escolha;
        limparBuffer();

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

public:
    GerenciadorLivraria(const string& arquivo = "dados_livraria.txt")
        : arquivoDados(arquivo), proximoIdLivro(1), proximoIdVenda(1) {
        carregarDados();
    }

    void carregarDados() {
        ifstream file(arquivoDados);

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

        do {
            cabecalho("LIVRARIA BYTE");
            cout << "1. Gerenciar Livros" << endl;
            cout << "2. Gerenciar Pessoas" << endl;
            cout << "3. Realizar Venda" << endl;
            cout << "4. Relatórios" << endl;
            cout << "5. Sair" << endl;
            cout << "===================================" << endl;
            cout << "Escolha uma opção: ";

            cin >> opcao;
            limparBuffer();

            switch(opcao) {
                case 1: menuLivros(); break;
                case 2: menuPessoas(); break;
                case 3: realizarVenda(); break;
                case 4: menuRelatorios(); break;
                case 5: cout << "Saindo..." << endl; break;
                default: cout << "Opção inválida!" << endl;
            }
        } while(opcao != 5);

        salvarDados();
    }



        // ==================== MENU LIVROS ====================
    void menuLivros() {
        int opcao;
        do {
            cabecalho("GERENCIAMENTO DE LIVROS");
            cout << "1. Adicionar Livro" << endl;
            cout << "2. Listar Livros" << endl;
            cout << "3. Buscar Livro" << endl;
            cout << "4. Editar Livro" << endl;
            cout << "5. Remover Livro" << endl;
            cout << "6. Voltar" << endl;
            cout << "Escolha: ";

            cin >> opcao;
            limparBuffer();

            switch(opcao) {
                case 1: adicionarLivro(); break;
                case 2: listarLivros(); break;
                case 3: buscarLivro(); break;
                case 4: editarLivro(); break;
                case 5: removerLivro(); break;
            }
        } while(opcao != 6);
    }

    void adicionarLivro() {
        cabecalho("ADICIONAR LIVRO");

        string titulo, genero, autor;
        double preco;
        int quantidade, dia, mes, ano;

        cout << "Título: ";
        getline(cin, titulo);

        cout << "Autor: ";
        getline(cin, autor);

        cout << "Gênero: ";
        getline(cin, genero);

        cout << "Preço: R$ ";
        cin >> preco;

        cout << "Quantidade: ";
        cin >> quantidade;

        cout << "Data de publicação (DD MM AAAA): ";
        cin >> dia >> mes >> ano;
        limparBuffer();

        livros.push_back(Livro(proximoIdLivro++, titulo, genero, preco, quantidade, Data(dia, mes, ano), autor));

        cout << "\nLivro adicionado com sucesso! ID: " << (proximoIdLivro - 1) << endl;
        pausar();
    }

    void listarLivros() {
        cabecalho("LISTA DE LIVROS");

        if(livros.empty()) {
            cout << "Nenhum livro cadastrado." << endl;
            pausar();
            return;
        }

        cout << left << setw(5) << "ID"
             << setw(30) << "TÍTULO"
             << setw(20) << "AUTOR"
             << setw(20) << "GÊNERO"
             << setw(10) << "PREÇO"
             << setw(10) << "ESTOQUE"
             << setw(12) << "PUBLICAÇÃO" << endl;
        cout << string(107, '-') << endl;

        for(const auto& livro : livros) {
            cout << left << setw(5) << livro.getId()
                 << setw(30) << (livro.getTitulo().length() > 28 ? livro.getTitulo().substr(0, 27) + "." : livro.getTitulo())
                 << setw(20) << (livro.getAutor().length() > 18 ? livro.getAutor().substr(0, 17) + "." : livro.getAutor())
                 << setw(20) << (livro.getGenero().length() > 18 ? livro.getGenero().substr(0, 17) + "." : livro.getGenero())
                 << setw(10) << fixed << setprecision(2) << livro.getPreco()
                 << setw(10) << livro.getQuantidade()
                 << setw(12) << livro.getDataPublicacao().toString() << endl;
        }

        pausar();
    }

    void buscarLivro() {
        cabecalho("BUSCAR LIVRO");

        string termo;
        cout << "Digite o título ou parte do título: ";
        getline(cin, termo);

        int indice = PesquisarLivroPorTitulo(termo);
        if (indice == -1) {
            cout << "Livro não encontrado!" << endl;
        } else {
            cout << "\n=== LIVRO ENCONTRADO ===" << endl;
            livros[indice].exibir();
        }

        pausar();
    }

    void editarLivro() {
        cabecalho("EDITAR LIVRO");

        string termo;
        cout << "Digite o título do livro a editar: ";
        getline(cin, termo);

        int indice = PesquisarLivroPorTitulo(termo);
        if (indice == -1) {
            cout << "Livro não encontrado!" << endl;
            pausar();
            return;
        }

        Livro& livro = livros[indice];
        cout << "Editando: " << livro.getTitulo() << endl;

        string novoTitulo, novoGenero, novoAutor;
        string precoStr, qtdStr;

        cout << "Novo título [" << livro.getTitulo() << "]: ";
        getline(cin, novoTitulo);
        if(!novoTitulo.empty()) {
            livro.setTitulo(novoTitulo);
        }

        cout << "Novo autor [" << livro.getAutor() << "]: ";
        getline(cin, novoAutor);
        if(!novoAutor.empty()) {
            livro.setAutor(novoAutor);
        }

        cout << "Novo gênero [" << livro.getGenero() << "]: ";
        getline(cin, novoGenero);
        if(!novoGenero.empty()) {
            livro.setGenero(novoGenero);
        }

        cout << "Novo preço [R$ " << livro.getPreco() << "]: ";
        getline(cin, precoStr);
        if(!precoStr.empty()) {
            livro.setPreco(stod(precoStr));
        }

        cout << "Nova quantidade [" << livro.getQuantidade() << "]: ";
        getline(cin, qtdStr);
        if(!qtdStr.empty()) {
            livro.setQuantidade(stoi(qtdStr));
        }

        cout << "Livro editado com sucesso!" << endl;
        pausar();
    }

    void removerLivro() {
        cabecalho("REMOVER LIVRO");

        string termo;
        cout << "Digite o título do livro a remover: ";
        getline(cin, termo);

        int indice = PesquisarLivroPorTitulo(termo);
        if (indice == -1) {
            cout << "Livro não encontrado!" << endl;
            pausar();
            return;
        }

        cout << "Tem certeza que deseja remover '" << livros[indice].getTitulo() << "'? (s/n): ";
        char confirmacao;
        cin >> confirmacao;
        limparBuffer();

        if(confirmacao == 's' || confirmacao == 'S') {
            // Verificar se o livro está em alguma venda
            bool podeRemover = true;
            for (const auto& venda : vendas) {
                for (const auto& item : venda.getItens()) {
                    if (item.getLivroId() == livros[indice].getId()) {
                        podeRemover = false;
                        break;
                    }
                }
                if (!podeRemover) break;
            }

            if (podeRemover) {
                livros.erase(livros.begin() + indice);
                cout << "Livro removido com sucesso!" << endl;
            } else {
                cout << "Não é possível remover. Livro está associado a vendas." << endl;
            }
        } else {
            cout << "Operação cancelada." << endl;
        }

        pausar();
    }

    // ==================== MENU PESSOAS ====================
    void menuPessoas() {
        int opcao;
        do {
            cabecalho("GERENCIAMENTO DE PESSOAS");
            cout << "1. Adicionar Pessoa" << endl;
            cout << "2. Listar Pessoas" << endl;
            cout << "3. Buscar Pessoa" << endl;
            cout << "4. Editar Pessoa" << endl;
            cout << "5. Remover Pessoa" << endl;
            cout << "6. Voltar" << endl;
            cout << "Escolha: ";

            cin >> opcao;
            limparBuffer();

            switch(opcao) {
                case 1: adicionarPessoa(); break;
                case 2: listarPessoas(); break;
                case 3: buscarPessoa(); break;
                case 4: editarPessoa(); break;
                case 5: removerPessoa(); break;
            }
        } while(opcao != 6);
    }

    void adicionarPessoa() {
        cabecalho("ADICIONAR PESSOA");
        cout << "1. Funcionário" << endl;
        cout << "2. Gerente" << endl;
        cout << "Escolha o tipo: ";

        int tipo;
        cin >> tipo;
        limparBuffer();

        string nome, email, telefone;
        cout << "Nome: ";
        getline(cin, nome);
        cout << "Email: ";
        getline(cin, email);
        cout << "Telefone: ";
        getline(cin, telefone);

        if (tipo == 1) {
            int matricula;
            double salario;
            cout << "Matrícula: ";
            cin >> matricula;
            cout << "Salário: R$ ";
            cin >> salario;
            limparBuffer();
            pessoas.push_back(make_unique<Funcionario>(nome, email, telefone, matricula, salario));
            cout << "Funcionário adicionado com sucesso!" << endl;
        }
        else if (tipo == 2) {
            int matricula;
            double salario;
            string departamento;
            cout << "Matrícula: ";
            cin >> matricula;
            cout << "Salário: R$ ";
            cin >> salario;
            limparBuffer();
            cout << "Departamento: ";
            getline(cin, departamento);
            pessoas.push_back(make_unique<Gerente>(nome, email, telefone, matricula, salario, departamento));
            cout << "Gerente adicionado com sucesso!" << endl;
        }
        else {
            cout << "Tipo inválido!" << endl;
        }

        pausar();
    }

    void listarPessoas() {
        cabecalho("LISTA DE PESSOAS");

        if(pessoas.empty()) {
            cout << "Nenhuma pessoa cadastrada." << endl;
            pausar();
            return;
        }

        for(const auto& pessoa : pessoas) {
            cout << "Tipo: " << pessoa->getTipo() << endl;
            pessoa->exibir();
            cout << "-------------------" << endl;
        }

        pausar();
    }

    void buscarPessoa() {
        cabecalho("BUSCAR PESSOA");

        string termo;
        cout << "Digite o nome ou parte do nome: ";
        getline(cin, termo);

        int indice = PesquisarPessoaPorNome(termo);
        if (indice == -1) {
            cout << "Pessoa não encontrada!" << endl;
        } else {
            cout << "\n=== PESSOA ENCONTRADA ===" << endl;
            cout << "Tipo: " << pessoas[indice]->getTipo() << endl;
            pessoas[indice]->exibir();
        }

        pausar();
    }

    void editarPessoa() {
        cabecalho("EDITAR PESSOA");

        string termo;
        cout << "Digite o nome da pessoa a editar: ";
        getline(cin, termo);

        int indice = PesquisarPessoaPorNome(termo);
        if (indice == -1) {
            cout << "Pessoa não encontrada!" << endl;
            pausar();
            return;
        }

        cout << "Editando: " << pessoas[indice]->getNome() << " (" << pessoas[indice]->getTipo() << ")" << endl;

        string novoNome, novoEmail, novoTelefone;
        cout << "Novo nome [" << pessoas[indice]->getNome() << "]: ";
        getline(cin, novoNome);

        cout << "Novo email [" << pessoas[indice]->getEmail() << "]: ";
        getline(cin, novoEmail);

        cout << "Novo telefone [" << pessoas[indice]->getTelefone() << "]: ";
        getline(cin, novoTelefone);

        if (!novoNome.empty()) pessoas[indice]->setNome(novoNome);

        cout << "Pessoa editada com sucesso!" << endl;
        pausar();
    }

    void removerPessoa() {
        cabecalho("REMOVER PESSOA");

        string termo;
        cout << "Digite o nome da pessoa a remover: ";
        getline(cin, termo);

        int indice = PesquisarPessoaPorNome(termo);
        if (indice == -1) {
            cout << "Pessoa não encontrada!" << endl;
            pausar();
            return;
        }

        cout << "Tem certeza que deseja remover '" << pessoas[indice]->getNome() << "'? (s/n): ";
        char confirmacao;
        cin >> confirmacao;
        limparBuffer();

        if(confirmacao == 's' || confirmacao == 'S') {
            // Verificar se a pessoa está associada a alguma venda
            bool podeRemover = true;
            string motivo;

            if (pessoas[indice]->getTipo() == "Funcionario" || pessoas[indice]->getTipo() == "Gerente") {
                Funcionario* func = static_cast<Funcionario*>(pessoas[indice].get());
                for (const auto& venda : vendas) {
                    if (venda.getFuncionarioId() == func->getMatricula()) {
                        podeRemover = false;
                        motivo = "Pessoa está associada a vendas";
                        break;
                    }
                }
            }

            if (podeRemover) {
                pessoas.erase(pessoas.begin() + indice);
                cout << "Pessoa removida com sucesso!" << endl;
            } else {
                cout << "Não é possível remover. " << motivo << "." << endl;
            }
        } else {
            cout << "Operação cancelada." << endl;
        }

        pausar();
    }

    // ==================== VENDAS ====================
    void realizarVenda() {
        cabecalho("REALIZAR VENDA");

        // Selecionar funcionário
        cout << "Funcionários disponíveis:" << endl;
        vector<Funcionario*> funcionarios = getFuncionarios();
        for (size_t i = 0; i < funcionarios.size(); i++) {
            cout << i + 1 << ". " << funcionarios[i]->getNome() << " (" << funcionarios[i]->getTipo() << ")" << endl;
        }

        cout << "Escolha o número do funcionário: ";
        int funcEscolha;
        cin >> funcEscolha;
        limparBuffer();

        if (funcEscolha <= 0 || funcEscolha > static_cast<int>(funcionarios.size())) {
            cout << "Escolha inválida!" << endl;
            pausar();
            return;
        }

        Funcionario* funcionarioSelecionado = funcionarios[funcEscolha - 1];
        Venda novaVenda(proximoIdVenda++, Data::dataAtual(), funcionarioSelecionado->getMatricula());

        cout << "Venda sendo realizada por: " << funcionarioSelecionado->getNome() << endl;

        char continuar;
        do {
            string titulo;
            cout << "\nDigite o título do livro: ";
            getline(cin, titulo);

            int indice = PesquisarLivroPorTitulo(titulo);
            if (indice == -1) {
                cout << "Livro não encontrado!" << endl;
                continue;
            }

            Livro& livro = livros[indice];
            cout << "Livro: " << livro.getTitulo() << " - Preço: R$ " << fixed << setprecision(2)
                 << livro.getPreco() << " - Estoque: " << livro.getQuantidade() << endl;
            cout << "Quantidade: ";
            int quantidade;
            cin >> quantidade;
            limparBuffer();

            if(quantidade <= 0) {
                cout << "Quantidade inválida!" << endl;
                continue;
            }

            if(quantidade > livro.getQuantidade()) {
                cout << "Estoque insuficiente!" << endl;
                continue;
            }

            novaVenda.adicionarItem(livro.getId(), quantidade, livro.getPreco());
            livro.setQuantidade(livro.getQuantidade() - quantidade); // Atualizar estoque

            cout << "Item adicionado. Subtotal: R$ " << fixed << setprecision(2)
                 << livro.getPreco() * quantidade << endl;

            cout << "Adicionar outro item? (s/n): ";
            cin >> continuar;
            limparBuffer();

        } while(continuar == 's' || continuar == 'S');

        vendas.push_back(novaVenda);

        cout << "\nVenda realizada com sucesso!" << endl;
        cout << "Total da venda: R$ " << fixed << setprecision(2) << novaVenda.getTotal() << endl;
        cout << "ID da venda: " << novaVenda.getId() << endl;

        pausar();
    }

    // ==================== RELATÓRIOS ====================
    void menuRelatorios() {
        int opcao;
        do {
            cabecalho("RELATÓRIOS");

            cout << "3. Relatório de Vendas" << endl;
            cout << "4. Relatório de Estoque" << endl;
            cout << "5. Voltar" << endl;
            cout << "Escolha: ";

            cin >> opcao;
            limparBuffer();

            switch(opcao) {
                case 1: relatorioPessoas(); break;
                case 2: relatorioLivros(); break;
                case 3: relatorioVendas(); break;
                case 4: relatorioEstoque(); break;
            }
        } while(opcao != 5);
    }

    void relatorioPessoas() {
        cabecalho("RELATÓRIO DE PESSOAS");
        cout << "Total de pessoas cadastradas: " << pessoas.size() << endl;

        int funcionarios = 0, gerentes = 0;
        for (const auto& pessoa : pessoas) {
            if (pessoa->getTipo() == "Funcionario") funcionarios++;
            else if (pessoa->getTipo() == "Gerente") gerentes++;
        }

        cout << "Funcionários: " << funcionarios << endl;
        cout << "Gerentes: " << gerentes << endl;

        pausar();
    }

    void relatorioLivros() {
        cabecalho("RELATÓRIO DE LIVROS");
        cout << "Total de livros cadastrados: " << livros.size() << endl;

        double valorTotal = 0.0;
        int quantidadeTotal = 0;
        for (const auto& livro : livros) {
            valorTotal += livro.getPreco() * livro.getQuantidade();
            quantidadeTotal += livro.getQuantidade();
        }

        cout << "Quantidade total em estoque: " << quantidadeTotal << endl;
        cout << "Valor total do estoque: R$ " << fixed << setprecision(2) << valorTotal << endl;

        pausar();
    }

    void relatorioVendas() {
        cabecalho("RELATÓRIO DE Vendas");
        cout << "Total de vendas realizadas: " << vendas.size() << endl;

        double totalVendas = 0.0;
        for (const auto& venda : vendas) {
            totalVendas += venda.getTotal();
        }

        cout << "Valor total em vendas: R$ " << fixed << setprecision(2) << totalVendas << endl;

        if (!vendas.empty()) {
            cout << "\nÚltimas 5 vendas:" << endl;
            int start = max(0, static_cast<int>(vendas.size()) - 5);
            for (size_t i = start; i < vendas.size(); i++) {
                vendas[i].exibir(livros, pessoas);
                cout << "-------------------" << endl;
            }
        }

        pausar();
    }

    void relatorioEstoque() {
        cabecalho("RELATÓRIO DE ESTOQUE");

        if(livros.empty()) {
            cout << "Nenhum livro em estoque." << endl;
            pausar();
            return;
        }

        double valorTotal = 0.0;
        int quantidadeTotal = 0;
        int livrosBaixoEstoque = 0;

        cout << left << setw(5) << "ID"
             << setw(30) << "TÍTULO"
             << setw(10) << "ESTOQUE"
             << setw(15) << "VALOR UNIT."
             << setw(15) << "VALOR TOTAL" << endl;
        cout << string(75, '-') << endl;

        for(const auto& livro : livros) {
            double valorItem = livro.getPreco() * livro.getQuantidade();
            cout << left << setw(5) << livro.getId()
                 << setw(30) << (livro.getTitulo().length() > 28 ? livro.getTitulo().substr(0, 27) + "." : livro.getTitulo())
                 << setw(10) << livro.getQuantidade()
                 << setw(15) << fixed << setprecision(2) << livro.getPreco()
                 << setw(15) << valorItem << endl;

            valorTotal += valorItem;
            quantidadeTotal += livro.getQuantidade();

            if (livro.getQuantidade() < 5) {
                livrosBaixoEstoque++;
            }
        }

        cout << string(75, '-') << endl;
        cout << "TOTAL GERAL: " << quantidadeTotal << " livros - Valor total: R$ "
             << fixed << setprecision(2) << valorTotal << endl;

        pausar();
    }
};
