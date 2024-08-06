#ifndef POKETRUNFO_H
#define POKETRUNFO_H

typedef struct node Node;
typedef struct tree_node TreeNode;
typedef struct pokemon Pokemon;
typedef struct baralho Baralho;


TreeNode* insere_pokemon(TreeNode* raiz, Pokemon* p);
TreeNode* cria_pokedex(int* tamanho);
void armazena_pokemons(TreeNode* raiz, Pokemon* array, int* index);
void imprime_carta(Pokemon* carta);
Node* cria_no(Pokemon* carta);
Baralho* cria_baralho(TreeNode* pokedex, int tamanho);
void imprime_baralho(Baralho* baralho);
Pokemon* busca_pokemon(Pokemon* pokedex, int tamanho, char* nome);


#endif