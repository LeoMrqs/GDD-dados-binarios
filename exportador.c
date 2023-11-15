#include "exportador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Insere um novo elemento na lista encadeada ordenando pelo CPF
void inserirPorCPF(struct Node** inicio, struct Pessoa pessoa) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "[ERROR] Falha na alocação de memória.\n");
        exit(1);
    }
    newNode->pessoa = pessoa;
    newNode->next = NULL;

    struct Node* atual = *inicio;
    struct Node* anterior = NULL;
    // Encontra a posição correta para inserção baseada no CPF
    while (atual != NULL && strcmp(pessoa.cpf, atual->pessoa.cpf) > 0) {
        anterior = atual;
        atual = atual->next;
    }

    // Insere o novo node na posição correta
    if (anterior == NULL) {
        newNode->next = *inicio;
        *inicio = newNode;
    } else {
        newNode->next = atual;
        anterior->next = newNode;
    }
}

// Insere um novo elemento na lista encadeada ordenando pelo Nome
void inserirPorNome(struct Node** inicio, struct Pessoa pessoa) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "[ERROR] Falha na alocação de memória.\n");
        exit(1);
    }
    newNode->pessoa = pessoa;
    newNode->next = NULL;

    struct Node* atual = *inicio;
    struct Node* anterior = NULL;
    // Encontra a posição correta para inserção baseada no Nome
    while (atual != NULL && strcmp(pessoa.nome, atual->pessoa.nome) > 0) {
        anterior = atual;
        atual = atual->next;
    }

    // Insere o novo node na posição correta
    if (anterior == NULL) {
        newNode->next = *inicio;
        *inicio = newNode;
    } else {
        newNode->next = atual;
        anterior->next = newNode;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "[ERROR] Formato padrão: %s <arquivo_binario> <arquivo_texto>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo binário para leitura
    FILE* arquivoBinario = fopen(argv[1], "rb");
    if (arquivoBinario == NULL) {
        fprintf(stderr, "[ERROR] Erro ao abrir o arquivo binário para leitura.\n");
        return 1;
    }

    struct Node* inicio = NULL;
    struct Pessoa dados;

    // O usuário escolhe como os dados serão ordenados na lista
    int opcaoInsercao;
    printf("Escolha o modo de salvar os dados:\n");
    printf("1. Inserir em Ordem padrão (CPF ordenado)\n");
    printf("2. Inserir em Ordem alfabética por Nome\n");
    scanf("%d", &opcaoInsercao);

    // Lê os dados do arquivo binário e os insere na lista encadeada
    while (fread(&dados, sizeof(struct Pessoa), 1, arquivoBinario) == 1) {
        if (opcaoInsercao == 1) {
            inserirPorCPF(&inicio, dados);
        } else if (opcaoInsercao == 2) {
            inserirPorNome(&inicio, dados);
        }
    }
    fclose(arquivoBinario); // Fecha o arquivo binário após a leitura

    // Abre o arquivo de texto para gravação
    FILE* arquivoTexto = fopen(argv[2], "w");
    if (arquivoTexto == NULL) {
        fprintf(stderr, "[ERROR] Erro ao abrir o arquivo de texto para gravação.\n");
        return 1;
    }

    // Escreve os dados da lista no arquivo de texto
    struct Node* atual = inicio;
    while (atual != NULL) {
        fprintf(arquivoTexto, "%s %s\n", atual->pessoa.cpf, atual->pessoa.nome);
        atual = atual->next;
    }
    fclose(arquivoTexto); // Fecha o arquivo de texto após a gravação

    // Libera a memória alocada para a lista encadeada
    while (inicio != NULL) {
        struct Node* proximo = inicio->next;
        free(inicio);
        inicio = proximo;
    }

    return 0;
}
