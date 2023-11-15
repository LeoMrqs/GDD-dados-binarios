#include "gerador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cpfValido(const char* cpf) {
    for (int i = 0; cpf[i] != '\0'; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') {
            return 0; // Retorna 0 se encontrar um caractere não numérico
        }
    }
    return 1; // Retorna 1 se todos os caracteres forem numéricos
}

// Insere um novo elemento na lista encadeada mantendo a ordem pelo CPF
void inserir(struct Node** inicio, struct Pessoa pessoa) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        // Em caso de falha de alocação de memória, o programa encerra
        fprintf(stderr, "[ERROR] Falha ao alocar memória para um novo node.\n");
        exit(1);
    }

    // Atribui os dados da pessoa ao novo node e define o próximo como NULL
    newNode->pessoa = pessoa;
    newNode->next = NULL;

    // Encontra a posição correta para inserção na lista ordenada
    if (*inicio == NULL || strcmp(pessoa.cpf, (*inicio)->pessoa.cpf) < 0) {
        // Insere no início se a lista estiver vazia ou se o CPF for menor que o do primeiro node
        newNode->next = *inicio;
        *inicio = newNode;
    } else {
        struct Node* atual = *inicio;
        // Percorre a lista para encontrar a posição de inserção
        while (atual->next != NULL && strcmp(atual->next->pessoa.cpf, pessoa.cpf) < 0) {
            atual = atual->next;
        }
        // Insere o novo node na posição encontrada
        newNode->next = atual->next;
        atual->next = newNode;
    }
}

// Salva os dados da lista em um arquivo binário
void salvarEmBinario(struct Node* inicio, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        // Em caso de falha ao abrir o arquivo, o programa encerra
        fprintf(stderr, "[ERROR] Não foi possível abrir o arquivo '%s' para gravação.\n", nomeArquivo);
        exit(1);
    }

    struct Node* atual = inicio;
    // Escreve os dados de cada node no arquivo
    while (atual != NULL) {
        fwrite(&atual->pessoa, sizeof(struct Pessoa), 1, arquivo);
        atual = atual->next;
    }

    // Fecha o arquivo após completar a gravação
    fclose(arquivo);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        // Verifica se o número de argumentos está correto
        fprintf(stderr, "[ERROR] Formato padrão: %s entrada.txt saida.txt \n", argv[0]);
        return 1;
    }

    const char* arquivoEntrada = argv[1];
    const char* arquivoSaida = argv[2];

    struct Node* inicio = NULL;
    FILE* arquivoTexto = fopen(arquivoEntrada, "r");
    if (arquivoTexto == NULL) {
        // Em caso de falha ao abrir o arquivo de entrada, o programa encerra
        fprintf(stderr, "[ERROR] Falha ao abrir o arquivo '%s' para leitura.\n", arquivoEntrada);
        return 1;
    }

    struct Pessoa dados;
    // Lê dados do arquivo de texto e insere na lista encadeada
    while (fscanf(arquivoTexto, "%s %49[^\n]", dados.cpf, dados.nome) == 2) { 
        if (!cpfValido(dados.cpf)) { // Verifica se o CPF são válidos
            fprintf(stderr, "[ERROR] CPF inválido encontrado: %s\n", dados.cpf);
            continue; // Pula para a próxima iteração se o CPF for inválido
        }
        inserir(&inicio, dados);
    }

    // Fecha o arquivo de texto após a leitura
    fclose(arquivoTexto);
    // Salva os dados lidos no arquivo binário
    salvarEmBinario(inicio, arquivoSaida);

    // Libera a memória alocada para a lista encadeada
    struct Node* atual = inicio;
    while (atual != NULL) {
        struct Node* proximo = atual->next;
        free(atual);
        atual = proximo;
    }

    // Informa ao usuário que o processo foi concluído com sucesso
    printf("[SUCESSO] Dados lidos de '%s' foram processados e salvos em '%s'.\n", arquivoEntrada, arquivoSaida);
    return 0;
}
