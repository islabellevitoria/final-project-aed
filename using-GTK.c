#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Para a função access

#define MAX_PRODUTOS 100
#define MAX_CLIENTES 100

typedef struct {
    int id;
    char nome[50];
    float preco;
} Produto;

typedef struct {
    int id;
    char nome[50];
    char endereco[100];
} Cliente;

typedef struct ItemPedido {
    Produto produto;
    int quantidade;
    struct ItemPedido* proximo;
} ItemPedido;

typedef struct Pedido {
    int id;
    Cliente cliente;
    ItemPedido* itens;
    float valor_total;
    struct Pedido* proximo;
} Pedido;

typedef struct {
    Pedido* frente;
    Pedido* tras;
    int quantidade;
} Fila;

// Funções para criar e manipular a fila
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

void exibeFila(Fila* fila, GtkTextBuffer *buffer) {
    Pedido* atual = fila->frente;
    gtk_text_buffer_set_text(buffer, "", -1);
    while (atual != NULL) {
        char info[400];
        snprintf(info, sizeof(info), "\nPedido ID: %d, Cliente: %s\nEndereço: %s\nValor Total: %.2f\n",
                 atual->id, atual->cliente.nome, atual->cliente.endereco, atual->valor_total);
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, info, -1);

        // Exibir os itens do pedido
        ItemPedido* item_atual = atual->itens;
        while (item_atual != NULL) {
            snprintf(info, sizeof(info), "  Produto: %s, Quantidade: %d\n",
                     item_atual->produto.nome, item_atual->quantidade);
            gtk_text_buffer_get_end_iter(buffer, &end);
            gtk_text_buffer_insert(buffer, &end, info, -1);
            item_atual = item_atual->proximo;
        }
        atual = atual->proximo;
    }
    char total[50];
    snprintf(total, sizeof(total), "Total de pedidos na fila: %d\n", fila->quantidade);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, total, -1);
}



// Funções para salvar e carregar produtos e clientes
void salvarProdutos(Produto* produtos, int numProdutos, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para salvar produtos");
        return;
    }
    for (int i = 0; i < numProdutos; i++) {
        fprintf(file, "%d,%s,%f\n", produtos[i].id, produtos[i].nome, produtos[i].preco);
    }
    fclose(file);
}

void salvarClientes(Cliente* clientes, int numClientes, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para salvar clientes");
        return;
    }
    for (int i = 0; i < numClientes; i++) {
        fprintf(file, "%d,%s,%s\n", clientes[i].id, clientes[i].nome, clientes[i].endereco);
    }
    fclose(file);
}

void carregarProdutos(Produto* produtos, int* numProdutos, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para carregar produtos");
        return;
    }
    *numProdutos = 0;
    while (fscanf(file, "%d,%49[^,],%f\n", &produtos[*numProdutos].id, produtos[*numProdutos].nome, &produtos[*numProdutos].preco) == 3) {
        printf("Produto carregado: ID=%d, Nome=%s, Preco=%.2f\n", produtos[*numProdutos].id, produtos[*numProdutos].nome, produtos[*numProdutos].preco);
        (*numProdutos)++;
    }
    fclose(file);
}


void carregarClientes(Cliente* clientes, int* numClientes, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para carregar clientes");
        return;
    }
    *numClientes = 0;
    while (fscanf(file, "%d,%49[^,],%99[^\n]\n", &clientes[*numClientes].id, clientes[*numClientes].nome, clientes[*numClientes].endereco) == 3) {
        (*numClientes)++;
    }
    fclose(file);
}

void criarArquivosIniciais() {
    Produto produtos[3] = {
        {1, "Pizza", 10.0},
        {2, "Hamburguer", 15.0},
        {3, "Sushi", 20.0}
    };
    Cliente clientes[3] = {
        {1, "João", "Rua A"},
        {2, "Maria", "Rua B"},
        {3, "Pedro", "Rua C"}
    };
    salvarProdutos(produtos, 3, "produtos.txt");
    salvarClientes(clientes, 3, "clientes.txt");
}

void criarArquivosSeNaoExistirem() {
    if (access("produtos.txt", F_OK) != 0) {
        criarArquivosIniciais();
    }
    if (access("clientes.txt", F_OK) != 0) {
        criarArquivosIniciais();
    }
}

// Funções auxiliares
ItemPedido* criaItemPedido(Produto produto, int quantidade) {
    ItemPedido* item = (ItemPedido*)malloc(sizeof(ItemPedido));
    if (item != NULL) {
        item->produto = produto;
        item->quantidade = quantidade;
        item->proximo = NULL;
    }
    return item;
}

void adicionaItemPedido(Pedido* pedido, Produto produto, int quantidade) {
    ItemPedido* novo_item = (ItemPedido*)malloc(sizeof(ItemPedido));
    if (novo_item == NULL) {
        perror("Erro ao alocar memória para o novo item do pedido");
        return;
    }

    novo_item->produto = produto;
    novo_item->quantidade = quantidade;
    novo_item->proximo = pedido->itens;
    pedido->itens = novo_item;

    // Atualizar o valor total do pedido
    pedido->valor_total += produto.preco * quantidade;
}




// Funções de callback para a interface
void on_add_pedido_clicked(GtkButton *button, gpointer user_data) {
    Fila* fila = (Fila*)user_data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *combo_cliente;
    GtkWidget *produto_area;
    GtkWidget *scroll_produtos;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *entry;
    char label_text[100];

    // Carregar produtos e clientes do arquivo
    Produto produtos[MAX_PRODUTOS];
    Cliente clientes[MAX_CLIENTES];
    int numProdutos = 0;
    int numClientes = 0;
    carregarProdutos(produtos, &numProdutos, "produtos.txt");
    carregarClientes(clientes, &numClientes, "clientes.txt");

    // Criar uma caixa de diálogo para adicionar um pedido
    dialog = gtk_dialog_new_with_buttons("Adicionar Pedido", NULL, GTK_DIALOG_MODAL,
                                         "OK", GTK_RESPONSE_OK,
                                         "Cancelar", GTK_RESPONSE_CANCEL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // ComboBox para selecionar o cliente
    combo_cliente = gtk_combo_box_text_new();
    for (int i = 0; i < numClientes; i++) {
        snprintf(label_text, sizeof(label_text), "%d - %s", clientes[i].id, clientes[i].nome);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_cliente), label_text);
    }
    GtkWidget *label_cliente = gtk_label_new("Cliente:");
    gtk_grid_attach(GTK_GRID(grid), label_cliente, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo_cliente, 1, 0, 1, 1);

    // Área para listar produtos e quantidades
    produto_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    scroll_produtos = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_produtos), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scroll_produtos, TRUE);
    gtk_widget_set_size_request(scroll_produtos, 300, 200);
    gtk_container_add(GTK_CONTAINER(scroll_produtos), produto_area);
    gtk_grid_attach(GTK_GRID(grid), scroll_produtos, 0, 1, 2, 1);

    for (int i = 0; i < numProdutos; i++) {
        box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        label = gtk_label_new("");
        entry = gtk_entry_new();
        snprintf(label_text, sizeof(label_text), "%d - %s (%.2f)", produtos[i].id, produtos[i].nome, produtos[i].preco);
        gtk_label_set_text(GTK_LABEL(label), label_text);
        gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(box), entry, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(produto_area), box, FALSE, FALSE, 0);
    }
    
    gtk_widget_show_all(dialog);

    // Esperar pela resposta do usuário
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        // Criar um novo pedido com os dados inseridos
        Pedido* pedido = (Pedido*)malloc(sizeof(Pedido));
        if (pedido == NULL) {
            perror("Erro ao alocar memória para o novo pedido");
            gtk_widget_destroy(dialog);
            return;
        }

        // Inicializar valor total do pedido
        pedido->valor_total = 0.0;

        // Definir o cliente do pedido
        int clienteIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_cliente));
        sscanf(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_cliente)), "%d - %49[^\n]", &pedido->cliente.id, pedido->cliente.nome);

        // Encontrar o cliente selecionado para obter o endereço
        for (int i = 0; i < numClientes; i++) {
            if (clientes[i].id == pedido->cliente.id) {
                strcpy(pedido->cliente.endereco, clientes[i].endereco);
                break;
            }
        }

        pedido->id = fila->quantidade + 1; // ID do pedido
        pedido->itens = NULL;
        pedido->proximo = NULL;

        // Adicionar itens ao pedido
        GList *children = gtk_container_get_children(GTK_CONTAINER(produto_area));
        for (GList *iter = children; iter != NULL; iter = iter->next) {
            GtkWidget *box = GTK_WIDGET(iter->data);
            GList *box_children = gtk_container_get_children(GTK_CONTAINER(box));
            
            GtkWidget *label = GTK_WIDGET(g_list_nth_data(box_children, 0));
            GtkWidget *entry = GTK_WIDGET(g_list_nth_data(box_children, 1));
            
            // Extrair a quantidade do item
            const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
            int quantidade = atoi(text);
            if (quantidade > 0) {
                // Extrair o ID e nome do produto do rótulo
                int produtoID;
                char produtoNome[50];
                float produtoPreco;
                sscanf(gtk_label_get_text(GTK_LABEL(label)), "%d - %49[^ (] ( %f )", &produtoID, produtoNome, &produtoPreco);
                
                // Adicionar o item ao pedido
                Produto produto = {produtoID, "", produtoPreco};
                strncpy(produto.nome, produtoNome, sizeof(produto.nome) - 1);
                
                adicionaItemPedido(pedido, produto, quantidade);
            }
            
            g_list_free(box_children);
        }

        enfileira(fila, pedido);
    }

    gtk_widget_destroy(dialog);
}





void on_remove_pedido_clicked(GtkButton *button, gpointer user_data) {
    Fila* fila = (Fila*)user_data;
    Pedido* removido = desenfileira(fila);
    if (removido != NULL) {
        // Liberar itens do pedido
        ItemPedido* item = removido->itens;
        while (item != NULL) {
            ItemPedido* temp = item;
            item = item->proximo;
            free(temp);
        }
        free(removido);
    }
}

void on_update_display_clicked(GtkButton *button, gpointer user_data) {
    GtkTextBuffer *buffer = (GtkTextBuffer*)g_object_get_data(G_OBJECT(button), "buffer");
    Fila* fila = (Fila*)user_data;
    exibeFila(fila, buffer);
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *btn_add_pedido;
    GtkWidget *btn_remover_pedido;
    GtkWidget *btn_atualizar_display;
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
    Fila* fila;

    // Criar a fila de pedidos
    fila = criaFila();

    // Criar arquivos se não existirem
    criarArquivosSeNaoExistirem();

    // Criar a janela principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Sistema de Pedidos");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Criar um container box
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Botão para adicionar pedido
    btn_add_pedido = gtk_button_new_with_label("Adicionar Pedido");
    g_signal_connect(btn_add_pedido, "clicked", G_CALLBACK(on_add_pedido_clicked), fila);
    gtk_box_pack_start(GTK_BOX(box), btn_add_pedido, FALSE, FALSE, 0);

    // Botão para remover pedido
    btn_remover_pedido = gtk_button_new_with_label("Remover Pedido");
    g_signal_connect(btn_remover_pedido, "clicked", G_CALLBACK(on_remove_pedido_clicked), fila);
    gtk_box_pack_start(GTK_BOX(box), btn_remover_pedido, FALSE, FALSE, 0);

    // Botão para atualizar display
    btn_atualizar_display = gtk_button_new_with_label("Atualizar Display");
    gtk_box_pack_start(GTK_BOX(box), btn_atualizar_display, FALSE, FALSE, 0);

    // Criar e configurar o GtkScrolledWindow
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    // Criar e configurar o GtkTextView
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // Conectar o buffer de texto ao botão de atualizar display
    g_object_set_data(G_OBJECT(btn_atualizar_display), "buffer", buffer);

    // Conectar o botão de atualizar display à função de atualização
    g_signal_connect(btn_atualizar_display, "clicked", G_CALLBACK(on_update_display_clicked), fila);

    gtk_widget_show_all(window);
}



int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.deliverysystem", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
