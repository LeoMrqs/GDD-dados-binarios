#ifndef EDITOR_H
#define EDITOR_H

// Estrutura para armazenar dados de pessoa
struct Pessoa {
    char cpf[12];  // CPF como chave
    char nome[50]; // Nome
};

// Nodo da lista encadeada
struct Node {
    struct Pessoa pessoa;
    struct Node* next;
};

// Funções
void inserir(struct Node** inicio, struct Pessoa pessoa);
struct Pessoa* buscar(struct Node* inicio, char* cpf);
void remover(struct Node** inicio, char* cpf);
void alterarNome(struct Node* inicio, char* cpf, char* novoNome);
void salvarEmArquivo(struct Node* inicio, const char* nomeArquivo);

#endif // EDITOR_H
