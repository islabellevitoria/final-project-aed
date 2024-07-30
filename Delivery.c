#include <stdio.h> 
#include <stdlib.h>
#include "delivery.h"


int main() {

    Fila* fila = criaFila();

    Produto produtos[] = {{1, "Pizza", 30.0}, {2, "Hamburguer", 15.0}, {3, "Sushi", 60.0}, {4, "Feijoada", 45.0} };
    Cliente clientes[] = {{1, "João Victor", "Rua A, 123"}, {2, "Maria", "Rua B, 456"}, {3, "Islabelle", "Rua C, 789"},
    {4, "Joao Vitor", "Rua D, 000"}};

    salvaProdutos(produtos, 4, "produtos.dat");
    salvaClientes(clientes, 4, "clientes.dat");

    // Carrega produtos e clientes
    Produto produtosCarregados[10];
    Cliente clientesCarregados[10];
    int numProdutos, numClientes;
    carregaProdutos(produtosCarregados, &numProdutos, "produtos.dat");
    carregaClientes(clientesCarregados, &numClientes, "clientes.dat");

    // Cria e enfileira pedidos
    Pedido* pedido1 = criaPedido(1, clientesCarregados[0], produtosCarregados[0]);
    enfileira(fila, pedido1);

    Pedido* pedido2 = criaPedido(2, clientesCarregados[1], produtosCarregados[1]);
    enfileira(fila, pedido2);

    Pedido* pedido3 = criaPedido(3, clientesCarregados[2], produtosCarregados[2]);
    enfileira(fila, pedido3);

    Pedido* pedido4 = criaPedido(4, clientesCarregados[3], produtosCarregados[3]);
    enfileira(fila, pedido4);

    // Exibe os pedidos na fila
    exibeFila(fila);

    // Desenfileira um pedido
    Pedido* pedidoDesenfileirado = desenfileira(fila);
    if (pedidoDesenfileirado != NULL) {
        printf("Pedido pronto - ID: %d, Cliente: %s, Produto: %s\n", pedidoDesenfileirado->id, pedidoDesenfileirado->cliente.nome, pedidoDesenfileirado->produto.nome);
        free(pedidoDesenfileirado);
        printf("\n");
    }

    exibeFila(fila);

    // Libera a memória dos pedidos restantes
    while ((pedidoDesenfileirado = desenfileira(fila)) != NULL) {
        free(pedidoDesenfileirado);
    }

    // Libera a memória da fila
    free(fila);

    return 0;
}