#ifndef POKETRUNFO_H
#define POKETRUNFO_H

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
    char lendario[50];
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
    Pokemon *pokemon;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

//Estrutura de uma pilha
typedef struct {
    Node* topo;
    int tamanho;
} Pilha;

TreeNode* insere_pokemon(TreeNode* raiz, Pokemon* p);
TreeNode* cria_pokedex(int* tamanho);
void armazena_pokemons(TreeNode* raiz, Pokemon* array, int* index);
void imprime_carta(Pokemon* carta);
Node* cria_no(Pokemon* carta);
Baralho* cria_baralho(void* pokedex, int tamanho);
Pokemon* buscarPorNome(TreeNode* root, const char* nome) ;
void imprime_baralho(Baralho* baralho);
Pokemon* buscarPorNome(TreeNode* root, const char* nome);
Pilha* embaralha_cartas(Baralho* baralho);
int compara_listas(Node* lista1, Node* lista2);
Pilha** divide_baralho(Baralho* baralho, int* num_jogadores, int tamanho, TreeNode* pokedex);


#endif