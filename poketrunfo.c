#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char nome[50];
    char tipo1[20];
    char tipo2[20];
    int total;
    int hp;
    int ataque;
    int defesa;
    int ataque_especial;
    int defesa_especial;
    int velocidade;
    int geracao;
    int lendario;
} Pokemon;

// Estrutura de uma lista duplamente encadeada
typedef struct Node {
    Pokemon carta;
    struct Node* proximo;
    struct Node* anterior;
} Node;
//estrutura do baralho
typedef struct {
    Node* primeiro;
    Node* ultimo;
    int tamanho;
} Baralho;

// Estrutura de uma árvore binária
typedef struct TreeNode {
    Pokemon carta;
    struct TreeNode* esquerda;
    struct TreeNode* direita;
} TreeNode;

TreeNode* insere_pokemon(TreeNode* raiz, Pokemon* p) {
    if (raiz == NULL) {
        TreeNode* novo_no = (TreeNode*) malloc(sizeof(TreeNode));
        if (novo_no == NULL) {
            printf("Erro ao alocar memória para o nó da árvore\n");
            return NULL;
        }
        novo_no->carta = *p;
        novo_no->esquerda = NULL;
        novo_no->direita = NULL;
        return novo_no;
    }

    if (p->id < raiz->carta.id) {
        raiz->esquerda = insere_pokemon(raiz->esquerda, p);
    } else {
        raiz->direita = insere_pokemon(raiz->direita, p);
    }

    return raiz;
}

// Função para criar a pokedex
TreeNode* cria_pokedex(int* tamanho) {
    printf("Abrindo arquivo pokemon.csv\n");
    FILE* arquivo = fopen("pokemon.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return NULL;
    }

    // Contar o número de linhas no arquivo
    char linha[256];
    *tamanho = 0;
    while (fgets(linha, sizeof(linha), arquivo)) {
        (*tamanho)++;
    }

    // Voltar ao início do arquivo
    rewind(arquivo);

    // Inicializar a árvore binária
    TreeNode* raiz = NULL;

    // Ler os dados do arquivo e inserir na árvore
    while (fgets(linha, sizeof(linha), arquivo)) {
        Pokemon p;
        sscanf(linha, "%d,%49[^,],%19[^,],%19[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d",
               &p.id, p.nome, p.tipo1, p.tipo2,
               &p.total, &p.hp, &p.ataque, &p.defesa,
               &p.ataque_especial, &p.defesa_especial, &p.velocidade,
               &p.geracao, &p.lendario);
        raiz = insere_pokemon(raiz, &p);
    }

    fclose(arquivo);
    return raiz;
}

// Função para armazenar os Pokémons da árvore binária em um array
void armazena_pokemons(TreeNode* raiz, Pokemon* array, int* index) {
    if (raiz == NULL) {
        return;
    }
    armazena_pokemons(raiz->esquerda, array, index);
    array[*index] = raiz->carta;
    (*index)++;
    armazena_pokemons(raiz->direita, array, index);
}

// Função para imprimir a carta do Pokémon
void imprime_carta(Pokemon* carta) {
    printf("ID: %d\n", carta->id);
    printf("Nome: %s\n", carta->nome);
    printf("Tipo 1: %s\n", carta->tipo1);
    printf("Tipo 2: %s\n", carta->tipo2);
    printf("Total: %d\n", carta->total);
    printf("HP: %d\n", carta->hp);
    printf("Ataque: %d\n", carta->ataque);
    printf("Defesa: %d\n", carta->defesa);
    printf("Ataque Especial: %d\n", carta->ataque_especial);
    printf("Defesa Especial: %d\n", carta->defesa_especial);
    printf("Velocidade: %d\n", carta->velocidade);
    printf("Geracao: %d\n", carta->geracao);
    printf("Lendario: %d\n", carta->lendario);
}

//função que cria um nó
Node* cria_no(Pokemon* carta) {
    Node* no = (Node*) malloc(sizeof(Node));
    if (no == NULL) {
        printf("Erro ao alocar memória para o nó\n");
        return NULL;
    }

    no->carta = *carta;
    no->proximo = NULL;
    no->anterior = NULL;

    return no;
}

//função que cria um baralho de 64 cartas
Baralho* cria_baralho(TreeNode* pokedex, int tamanho) {
    Baralho* baralho = (Baralho*) malloc(sizeof(Baralho));
    if (baralho == NULL) {
        printf("Erro ao alocar memória para o baralho\n");
        return NULL;
    }

    baralho->primeiro = NULL;
    baralho->ultimo = NULL;
    baralho->tamanho = 0;

    // Criar um array temporário para armazenar os Pokémons
    Pokemon* array_pokemons = (Pokemon*) malloc(tamanho * sizeof(Pokemon));
    if (array_pokemons == NULL) {
        printf("Erro ao alocar memória para o array de Pokémons\n");
        return NULL;
    }

    // Armazenar os Pokémons da árvore binária no array
    int index = 0;
    armazena_pokemons(pokedex, array_pokemons, &index);

    // Adicionar 64 cartas aleatórias ao baralho
    srand(time(NULL));
    for (int i = 0; i < 64; i++) {
        int indice = rand() % tamanho;
        Node* no = cria_no(&array_pokemons[indice]);
        if (no == NULL) {
            return NULL;
        }

        if (baralho->primeiro == NULL) {
            baralho->primeiro = no;
            baralho->ultimo = no;
        } else {
            baralho->ultimo->proximo = no;
            no->anterior = baralho->ultimo;
            baralho->ultimo = no;
        }

        baralho->tamanho++;
    }

    free(array_pokemons);
    return baralho;
}

// Função para imprimir o baralho
void imprime_baralho(Baralho* baralho) {
    Node* no = baralho->primeiro;
    int i = 1;
    while (no != NULL) {
        printf("Carta numero %d:\n", i);
        imprime_carta(&no->carta);
        printf("\n");
        no = no->proximo;
        i++;
    }
}

//função para buscar um pokemon na pokedex pelo nome
Pokemon* busca_pokemon(Pokemon* pokedex, int tamanho, char* nome) {
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(pokedex[i].nome, nome) == 0) {
            return &pokedex[i];
        }
    }

    return NULL;
}

int main() {
    int tamanho;
    TreeNode* pokedex = cria_pokedex(&tamanho);
    if (pokedex == NULL) {
        return 1;
    }

    Baralho* baralho = cria_baralho(pokedex, tamanho);
    if (baralho == NULL) {
        return 1;
    }

    imprime_baralho(baralho);

    return 0;
}