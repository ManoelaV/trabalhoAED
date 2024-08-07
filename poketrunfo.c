#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketrunfo.h"

// Função para inserir um Pokémon na árvore binária
TreeNode* inserir(TreeNode* root, Pokemon *pokemon) {
    if (root == NULL) {
        TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
        newNode->pokemon = pokemon;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    if (strcmp(pokemon->nome, root->pokemon->nome) < 0) {
        root->left = inserir(root->left, pokemon);
    } else {
        root->right = inserir(root->right, pokemon);
    }
    return root;
}


// Função para criar a pokedex e enserir em uma arvore de busca binaria
TreeNode* cria_pokedex(int* tamanho) {
    FILE* arquivo = fopen("pokemon.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    char linha[256];
    int count = 0;
    TreeNode *root = NULL;

    // Ignorar a primeira linha (cabecalho)
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;
        char* token = strtok(linha, ",");

        Pokemon *pokemon = (Pokemon *)malloc(sizeof(Pokemon));
        pokemon->id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(pokemon->nome, token);

        token = strtok(NULL, ",");
        strcpy(pokemon->tipo1, token);

        token = strtok(NULL, ",");
        if (token == NULL || strcmp(token, "ND") == 0) {
            strcpy(pokemon->tipo2, "");
        } else {
            strcpy(pokemon->tipo2, token);
        }

        token = strtok(NULL, ",");
        pokemon->total = atoi(token);

        token = strtok(NULL, ",");
        pokemon->hp = atoi(token);

        token = strtok(NULL, ",");
        pokemon->ataque = atoi(token);

        token = strtok(NULL, ",");
        pokemon->defesa = atoi(token);

        token = strtok(NULL, ",");
        pokemon->ataque_especial = atoi(token);

        token = strtok(NULL, ",");
        pokemon->defesa_especial = atoi(token);

        token = strtok(NULL, ",");
        pokemon->velocidade = atoi(token);

        token = strtok(NULL, ",");
        pokemon->geracao = atoi(token);

        token = strtok(NULL, ",");
        strcpy(pokemon->lendario, token);

        // Inserir na arvore binaria
        root = inserir(root, pokemon);
        count++;
    }

    fclose(arquivo);
    *tamanho = count;
    return root;
}
//imprime a pokedex

void imprime_pokedex(TreeNode* raiz) {
    if (raiz == NULL) {
        return;
    }

    imprime_pokedex(raiz->left);
    imprime_carta(raiz->pokemon);
    printf("\n");
    imprime_pokedex(raiz->right);
}

// Função para armazenar os Pokémons da árvore binária em um array
void armazena_pokemons(TreeNode* raiz, Pokemon* array, int* index) {
    if (raiz == NULL) {
        return;
    }

    armazena_pokemons(raiz->left, array, index);
    array[*index] = *raiz->pokemon;
    (*index)++;
    armazena_pokemons(raiz->right, array, index);
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
    printf("Lendario: %s\n", carta->lendario); // Certifique-se de que este campo é um inteiro
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
Baralho* cria_baralho(void* pokedex, int tamanho) {
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

    // Array auxiliar para rastrear os índices já escolhidos
    int* escolhidos = (int*) calloc(tamanho, sizeof(int));
    if (escolhidos == NULL) {
        printf("Erro ao alocar memória para o array de índices escolhidos\n");
        free(array_pokemons);
        return NULL;
    }

    // Adicionar 64 cartas aleatórias ao baralho
    srand(time(NULL));
    for (int i = 0; i < 64; i++) {
        int indice;
        do {
            indice = rand() % tamanho;
        } while (escolhidos[indice] == 1);

        escolhidos[indice] = 1;
        Node* no = cria_no(&array_pokemons[indice]);
        if (no == NULL) {
            free(array_pokemons);
            free(escolhidos);
            return NULL;
        }

        if (baralho->primeiro == NULL) {
            baralho->primeiro = no;
            baralho->ultimo = no;
        } else {
            baralho->ultimo->proximo = no;
            baralho->ultimo = no;
        }
        baralho->tamanho++;
    }

    free(array_pokemons);
    free(escolhidos);
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

//Função para embaralhar as cartas e colocar em uma pilha
Pilha* embaralha_cartas(Baralho* baralho) {
    //aloca memoria para a pilha de cartas(baralho)
    Pilha* pilha = (Pilha*) malloc(sizeof(Pilha));
    if (pilha == NULL) {
        printf("Erro ao alocar memória para a pilha\n");
        return NULL;
    }
    //inicializa a pilha topo aponta para null e o tamanho é 0
    pilha->topo = NULL;
    pilha->tamanho = 0;

    // Embaralhar as cartas do baralho
    Node* no = baralho->primeiro;//começa no primeiro nó
    while (no != NULL) { //continua até o nó ser nulo
        Node* proximo = no->proximo; //armazena o proximo nó
        no->proximo = pilha->topo; //o proximo nó aponta para o topo da pilha
        pilha->topo = no; //o topo da pilha aponta para o nó
        pilha->tamanho++; //incrementa o tamanho da pilha
        no = proximo; //o nó atual aponta para o proximo nó
    }

    return pilha;
}

/*nota para entendimento da minha ideia, a função transforma um
baralho de cartas em uma pilha, invertendo a ordem das cartas no processo, ela faz isso 
iterando sobre cada carta do baralho e empilhando-as uma a uma na nova pilha*/

// Função para comparar duas listas de cartas (para verificar se as cartas foram embaralhadas)
int compara_listas(Node* lista1, Node* lista2) {
    while (lista1 != NULL && lista2 != NULL) {
        if (strcmp(lista1->carta.nome, lista2->carta.nome) != 0) {
            return 0; // As listas são diferentes
        }
        lista1 = lista1->proximo;
        lista2 = lista2->proximo;
    }
    return (lista1 == NULL && lista2 == NULL); // As listas são iguais se ambas forem NULL
}
// Funcao para buscar um Pokemon por nome
Pokemon* buscarPorNome(TreeNode* root, const char* nome) {
    if (root == NULL) return NULL;

    int cmp = strcasecmp(nome, root->pokemon->nome);
    if (cmp == 0) return root->pokemon;
    if (cmp < 0) return buscarPorNome(root->left, nome);
    return buscarPorNome(root->right, nome);
}

int main() {

    int tamanho;
    int resposta;
    TreeNode* pokedex = cria_pokedex(&tamanho);

    if (pokedex == NULL || tamanho <= 0) {
        printf("Erro ao criar a pokedex ou nenhum Pokemon encontrado\n");
        return 1;
    }

    do {
        printf("Escolha uma opcao:\n");
        printf("1 - Imprimir Pokedex\n");
        printf("2 - Buscar Pokemon por nome\n");
        printf("3 - Criar e imprimir um baralho\n");
        printf("4 - Embaralhar cartas e imprimir pilha\n");
        printf("5 - Sair\n");
        scanf("%d", &resposta);

        switch (resposta) {
            case 1:
                imprime_pokedex(pokedex);
                break;
            case 2:
                printf("Digite o nome do Pokemon a ser buscado: ");
                char nome[50];
                scanf(" %[^\n]", nome);
                Pokemon* pokemon = buscarPorNome(pokedex, nome);
                if (pokemon != NULL) {
                    printf("Pokemon encontrado:\n");
                    imprime_carta(pokemon);
                } else {
                    printf("Pokemon nao encontrado\n");
                }
                break;
            case 3:{
                Baralho* baralho = cria_baralho(pokedex, tamanho);
                if (baralho != NULL) {
                    imprime_baralho(baralho);
                }
            }
                break;
            case 4:{
                Baralho* baralho2 = cria_baralho(pokedex, tamanho);
                if (baralho2 != NULL) {
                    Pilha* pilha = embaralha_cartas(baralho2);
                    if (pilha != NULL) {
                        Node* no = pilha->topo;
                        int i = 1;
                        while (no != NULL) {
                            printf("Carta numero %d:\n", i);
                            imprime_carta(&no->carta);
                            printf("\n");
                            no = no->proximo;
                            i++;
                        }
                    }
                }
            }
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while (resposta != 5);

    return 0;


}


