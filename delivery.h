#ifndef _DELIVERY_H_
#define _DELIVERY_H_

typedef struct Produto {
    int id;
    char nome[50];
    float preco;
} Produto;

typedef struct Cliente {
    int id;
    char nome[50];
    char endereco[100];
} Cliente;

typedef struct Pedido {
    int id;
    Cliente cliente;
    Produto produto;
    struct Pedido* proximo;
} Pedido;

typedef struct {
    Pedido* frente;
    Pedido* tras;
    int quantidade;
} Fila;


// Função para criar e inicializar a fila
Fila* criaFila();

// Verifica se a fila está vazia
int filaVazia(Fila* fila);

// Enfileira um pedido
void enfileira(Fila* fila, Pedido* pedido);

// Desenfileira um pedido
Pedido* desenfileira(Fila* fila);

// Função para criar um novo pedido
Pedido* criaPedido(int id, Cliente cliente, Produto produto);

// Salva os produtos em um arquivo
void salvaProdutos(Produto* produtos, int numProdutos, const char* nomeArquivo);

// Salva os clientes em um arquivo
void salvaClientes(Cliente* clientes, int numClientes, const char* nomeArquivo);

// Carrega os produtos de um arquivo
void carregaProdutos(Produto* produtos, int* numProdutos, const char* nomeArquivo);

// Carrega os clientes de um arquivo
void carregaClientes(Cliente* clientes, int* numClientes, const char* nomeArquivo);

// Exibe os pedidos na fila
void exibeFila(Fila* fila);


#endif