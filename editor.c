#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Insere um novo elemento na lista encadeada
void inserir(struct Node** inicio, struct Pessoa pessoa) {
    // Alocação de memória para um novo nodo
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "[ERROR] Falha ao alocar memória.\n");
        exit(1);
    }

    // Atribuição dos dados da pessoa ao novo nodo e definição do próximo como NULL
    newNode->pessoa = pessoa;
    newNode->next = NULL;

    // Inserção na posição correta, baseada no CPF
    struct Node* atual = *inicio;
    struct Node* anterior = NULL;
    while (atual != NULL && strcmp(pessoa.cpf, atual->pessoa.cpf) > 0) {
        anterior = atual;
        atual = atual->next;
    }

    // Atualiza os ponteiros para incluir o novo nodo na lista
    if (anterior == NULL) {
        newNode->next = *inicio;
        *inicio = newNode;
    } else {
        newNode->next = atual;
        anterior->next = newNode;
    }
}

// Busca um elemento na lista pelo CPF
struct Pessoa* buscar(struct Node* inicio, char* cpf) {
    struct Node* atual = inicio;
    // Percorre a lista procurando pelo CPF
    while (atual != NULL) {
        if (strcmp(atual->pessoa.cpf, cpf) == 0) {
            return &atual->pessoa; // Retorna o elemento encontrado
        }
        atual = atual->next;
    }
    return NULL; // Retorna NULL se não encontrar
}

// Remove um elemento da lista pelo CPF
void remover(struct Node** inicio, char* cpf) {
    struct Node* atual = *inicio;
    struct Node* anterior = NULL;

    // Percorre a lista procurando pelo CPF
    while (atual != NULL && strcmp(atual->pessoa.cpf, cpf) != 0) {
        anterior = atual;
        atual = atual->next;
    }

    // Se não encontrou o elemento, exibe erro
    if (atual == NULL) {
        fprintf(stderr, "[ERROR] Pessoa com CPF %s não encontrado.\n", cpf);
        return;
    }

    // Remove o elemento da lista e libera a memória
    if (anterior == NULL) {
        *inicio = atual->next;
    } else {
        anterior->next = atual->next;
    }
    free(atual);
}

// Altera o nome de uma pessoa
void alterarNome(struct Node* inicio, char* cpf, char* novoNome) {
    struct Pessoa* pessoa = buscar(inicio, cpf);
    if (pessoa != NULL) {
        // Se a pessoa for encontrada, atualiza o nome
        strncpy(pessoa->nome, novoNome, 49);
        pessoa->nome[49] = '\0';
        printf("[SUCESSO] Nome alterado com sucesso.\n");
    } else {
        fprintf(stderr, "[ERROR] Pessoa com CPF %s não encontrada.\n", cpf);
    }
}

// Salva os dados da lista em um arquivo binário
void salvarEmArquivo(struct Node* inicio, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "[ERROR] Falha ao abrir o arquivo para gravação.\n");
        exit(1);
    }

    struct Node* atual = inicio;
    // Escreve os dados de cada pessoa no arquivo
    while (atual != NULL) {
        fwrite(&atual->pessoa, sizeof(struct Pessoa), 1, arquivo);
        atual = atual->next;
    }

    fclose(arquivo); // Fecha o arquivo após a gravação
}

int main(int argc, char* argv[]) {
    // Verifica os argumentos da linha de comando
    if (argc != 2) {
        fprintf(stderr, "[ERROR] Formato padrão: %s dados.bin>\n", argv[0]);
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

    // Lê os dados do arquivo binário e os insere na lista
    while (fread(&dados, sizeof(struct Pessoa), 1, arquivoBinario) == 1) {
        inserir(&inicio, dados);
    }

    fclose(arquivoBinario); // Fecha o arquivo após a leitura

    // Interface de usuário para gerenciar a lista de pessoas
    int opcao;
    char cpf[12], novoNome[50];
    while (1) {
        printf("Escolha uma opção:\n");
        printf("1. Inserir nova Pessoa\n");
        printf("2. Buscar uma Pessoa\n");
        printf("3. Excluir uma Pessoa\n");
        printf("4. Alterar nome de uma Pessoa\n");
        printf("5. Encerrar programa\n");

        if (scanf("%d", &opcao) != 1) {
            fprintf(stderr, "[ERROR] Entrada inválida (Números entre as opções). Tente novamente.\n");
            while (getchar() != '\n');  // Limpar buffer de entrada
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Inserir nova Pessoa:\n");
                struct Pessoa novaPessoa;
                printf("Digite o CPF: ");
                scanf("%s", novaPessoa.cpf);
                printf("Digite o Nome: ");
                scanf("%s", novaPessoa.nome);
                inserir(&inicio, novaPessoa);
                printf("[SUCESSO] Pessoa inserida com sucesso.\n");
                break;
            case 2:
                printf("Buscar Pessoa:\n");
                printf("CPF da Pessoa a ser buscada: ");
                scanf("%s", cpf);
                struct Pessoa* encontrada = buscar(inicio, cpf);
                if (encontrada != NULL) {
                    printf("[SUCESSO] Pessoa encontrada: CPF=%s, Nome=%s\n", encontrada->cpf, encontrada->nome);
                } else {
                    printf("[ERROR] Pessoa com CPF %s não encontrada.\n", cpf);
                }
                break;
            case 3:
                printf("Excluir Pessoa:\n");
                printf("CPF da Pessoa a ser excluída: ");
                scanf("%s", cpf);
                remover(&inicio, cpf);
                printf("[SUCESSO] Pessoa excluída com sucesso.\n");
                break;
            case 4:
                printf("Alterar nome:\n");
                printf("CPF da Pessoa: ");
                scanf("%s", cpf);
                printf("Novo nome: ");
                scanf("%s", novoNome);
                alterarNome(inicio, cpf, novoNome);
                break;
            case 5:
                printf("[SUCESSO] Dados atualizados e salvos com êxito.\n");
                salvarEmArquivo(inicio, argv[1]);

                // Liberando a memória alocada para a lista
                struct Node* atual = inicio;
                while (atual != NULL) {
                    struct Node* proximo = atual->next;
                    free(atual);
                    atual = proximo;
                }
                return 0;
            default:
                printf("[ERROR] Opção inválida. Tente novamente.\n");
                break;
        }
    }
}
