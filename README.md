# 📚 BookStore — Sistema de Gerenciamento de Livraria

Sistema de gerenciamento de livraria desenvolvido em **C++**, com interface via terminal. O projeto permite administrar livros, funcionários, gerentes, vendas e relatórios, aplicando conceitos de programação orientada a objetos.

## 📌 Sobre o projeto

O BookStore foi desenvolvido como projeto acadêmico durante meus estudos em Engenharia da Computação, com o objetivo de praticar C++ e implementar funcionalidades comuns em um sistema de gerenciamento.

A aplicação organiza as operações de uma livraria em menus interativos e armazena os dados em um arquivo de texto, permitindo recuperá-los quando o programa é executado novamente.

## ✨ Funcionalidades

### 📖 Gerenciamento de livros
- Cadastrar livros com título, autor, gênero, preço, quantidade em estoque e data de publicação.
- Listar os livros cadastrados.
- Buscar livros pelo título.
- Editar informações dos livros.
- Remover livros do cadastro.

### 👥 Gerenciamento de pessoas
- Cadastrar funcionários e gerentes.
- Consultar e listar pessoas cadastradas.
- Buscar pessoas pelo nome.
- Editar e remover cadastros.
- Armazenar informações como nome, e-mail, telefone, matrícula e salário.
- Registrar o departamento dos gerentes.

### 🛒 Gerenciamento de vendas
- Registrar vendas associadas a um funcionário.
- Adicionar livros e quantidades aos itens de uma venda.
- Calcular o valor total da venda.
- Atualizar o estoque conforme as vendas realizadas.
- Consultar informações das vendas registradas.

### 📊 Relatórios
- Consultar a quantidade de pessoas cadastradas.
- Visualizar informações sobre livros e estoque.
- Consultar o total de vendas e os valores acumulados.
- Identificar livros com estoque baixo.

### 💾 Persistência de dados
- Salvar livros, pessoas e vendas em arquivo de texto.
- Carregar os dados armazenados ao iniciar o programa.
- Manter os registros entre execuções da aplicação.

## 🛠️ Tecnologias e conceitos

- **C++** — linguagem utilizada no desenvolvimento.
- **Programação Orientada a Objetos (POO)** — organização do sistema em classes.
- **Herança e polimorfismo** — modelagem de pessoas, funcionários e gerentes.
- **STL (Standard Template Library)** — utilização de estruturas como `vector` e `unique_ptr`.
- **Manipulação de arquivos** — leitura e gravação de dados com arquivos de texto.
- **Git e GitHub** — versionamento e hospedagem do código-fonte.

## 📂 Estrutura do projeto

```text
BookStore/
├── BookStore/
│   ├── main.cpp
│   ├── GerenciadorLivraria.h
│   ├── Livro.h
│   ├── Pessoas.h
│   ├── Funcionarios.h
│   ├── Gerente.h
│   ├── Venda.h
│   ├── ItemVenda.h
│   ├── Data.h
│   ├── Auxiliares.h
│   ├── dados_livraria.txt
│   └── BookStore.cbp
└── README.md
```

## 🚀 Como executar

### Pré-requisitos

- Um compilador C++ compatível com C++11 ou superior.
- Code::Blocks, caso queira abrir o projeto pelo arquivo `.cbp`.

### Opção 1 — Code::Blocks

1. Clone ou baixe este repositório.
2. Abra o arquivo `BookStore/BookStore.cbp` no Code::Blocks.
3. Compile o projeto.
4. Execute a aplicação.

### Opção 2 — Terminal

Clone o repositório:

```bash
git clone https://github.com/ScarlattiMoura/BookStore.git
```

Entre na pasta do código-fonte:

```bash
cd BookStore/BookStore
```

Compile o programa com o GCC:

```bash
g++ -std=c++11 main.cpp -o BookStore
```

Execute no Linux:

```bash
./BookStore
```

No Windows, pelo terminal:

```powershell
.\BookStore.exe
```

> **Observação:** o programa utiliza o arquivo `dados_livraria.txt` para armazenar informações. Execute a aplicação em um diretório no qual o arquivo possa ser acessado e gravado. Se necessário, ajuste o caminho do arquivo utilizado pelo programa.

## 🎓 Objetivos de aprendizagem

Este projeto permitiu praticar conceitos importantes do desenvolvimento de software, incluindo:

- Modelagem de entidades por meio de classes.
- Organização de funcionalidades em diferentes módulos.
- Manipulação e atualização de coleções de dados.
- Implementação de operações de cadastro, consulta, edição e exclusão.
- Relacionamento entre livros, funcionários e vendas.
- Leitura e escrita de arquivos para persistência de informações.

## 👩‍💻 Autora

**Scarlatti Moura**

[GitHub](https://github.com/ScarlattiMoura)

---

*Projeto acadêmico desenvolvido para praticar programação em C++ e conceitos de Engenharia da Computação.*
