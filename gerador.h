#ifndef GERADOR_H
#define GERADOR_H

// Estrutura para armazenar os dados de cada pessoa
struct Pessoa {
    char cpf[12];   // CPF como chave de identificação
    char nome[50];  // Nome da pessoa
};

// Nodo da lista encadeada
struct Node {
    struct Pessoa pessoa;
    struct Node* next;
};

// Funções
void inserir(struct Node** inicio, struct Pessoa pessoa);
void salvarEmBinario(struct Node* inicio, const char* nomeArquivo);

#endif // GERADOR_H
