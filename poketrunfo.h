#ifndef POKETRUNFO_H
#define POKETRUNFO_H

// Definição das estruturas
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
    char lendario[10];
} Pokemon;

typedef struct TreeNode {
    Pokemon* pokemon;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct Node {
    Pokemon carta;
    struct Node* proximo;
    struct Node* anterior;
} Node;

typedef struct {
    Node* primeiro_jogador_01;
    Node* ultimo_jogador_01;
    Node* primeiro_maquina;
    Node* ultimo_maquina;
    int tamanho_jogador_01;
    int tamanho_maquina;
} Baralho;

// Declaração das funções
TreeNode* inserir(TreeNode* root, Pokemon *pokemon);
Pokemon* buscarPorNome(TreeNode* root, const char* nome);
TreeNode* cria_pokedex(int* tamanho);
void armazena_pokemons(TreeNode* raiz, Pokemon* array, int* index);
void imprime_carta(Pokemon* carta) ;
Node* cria_no(Pokemon* carta);
Baralho* cria_baralho(TreeNode* pokedex, int tamanho, int num_cartas) ;
Pokemon* remove_carta_inicio(Node** primeiro, Node** ultimo, int* tamanho);
void compara_atributos(Baralho* baralho);
void menu_inicial(TreeNode* pokedex, int tamanho);


#endif