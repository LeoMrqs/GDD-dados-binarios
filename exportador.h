#ifndef EXPORTADOR_H
#define EXPORTADOR_H

// Estrutura para armazenar os dados de cada pessoa
struct Pessoa {
    char cpf[12]; // CPF como chave
    char nome[50]; // Nome
};

// Nodo da lista encadeada
struct Node {
    struct Pessoa pessoa;
    struct Node* next;
};

// Funções
void inserirPorCPF(struct Node** inicio, struct Pessoa pessoa);
void inserirPorNome(struct Node** inicio, struct Pessoa pessoa);

#endif // EXPORTADOR_H
