#include "delivery.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

Fila* criaFila() {

    Fila* fila = (Fila*)malloc(sizeof(Fila));

    if (fila != NULL) {
        fila->frente = NULL;
        fila->tras = NULL;
        fila->quantidade = 0;
    }

    return fila;
}


int filaVazia(Fila* fila) {

    return fila->quantidade == 0;

}


void enfileira(Fila* fila, Pedido* pedido) {

    if (filaVazia(fila)) {
        fila->frente = pedido;
    } else {
        fila->tras->proximo = pedido;
    }

    fila->tras = pedido;
    pedido->proximo = NULL;
    fila->quantidade++;
}


Pedido* desenfileira(Fila* fila) {

    if (filaVazia(fila)) {
        return NULL;
    }

    Pedido* removido = fila->frente;

    fila->frente = fila->frente->proximo;
    if (fila->frente == NULL) {
        fila->tras = NULL;
    }

    fila->quantidade--;
    return removido;
}


Pedido* criaPedido(int id, Cliente cliente, Produto produto) {

    Pedido* novoPedido = (Pedido*)malloc(sizeof(Pedido));

    if (novoPedido == NULL) { // Verifica se a alocação de memória foi bem-sucedida
        perror("Erro ao alocar memória para o novo pedido");
        exit(1);
    }

    novoPedido->id = id;
    novoPedido->cliente = cliente;
    novoPedido->produto = produto;
    novoPedido->proximo = NULL;
    return novoPedido;
}



void salvaProdutos(Produto* produtos, int numProdutos, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }
    fwrite(produtos, sizeof(Produto), numProdutos, arquivo);
    fclose(arquivo);
}

// Salva os clientes em um arquivo
void salvaClientes(Cliente* clientes, int numClientes, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }
    fwrite(clientes, sizeof(Cliente), numClientes, arquivo);
    fclose(arquivo);
}


void carregaProdutos(Produto* produtos, int* numProdutos, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }
    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    rewind(arquivo);
    *numProdutos = tamanhoArquivo / sizeof(Produto);
    fread(produtos, sizeof(Produto), *numProdutos, arquivo);
    fclose(arquivo);
}


void carregaClientes(Cliente* clientes, int* numClientes, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }
    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    rewind(arquivo);
    *numClientes = tamanhoArquivo / sizeof(Cliente);
    fread(clientes, sizeof(Cliente), *numClientes, arquivo);
    fclose(arquivo);
}


void exibeFila(Fila* fila) {

    Pedido* atual = fila->frente;

    while (atual != NULL) {
        printf("Pedido ID: %d, Cliente: %s, Produto: %s\n", atual->id, atual->cliente.nome, atual->produto.nome);
        atual = atual->proximo;
    }

    printf("\n");

    printf("Total de pedidos na fila: %d\n", fila->quantidade);

    printf("\n");

}