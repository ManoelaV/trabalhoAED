#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura do Pokemon
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

// Estrutura do no da arvore binaria
typedef struct TreeNode {
    Pokemon *pokemon;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Declaracao da funcao inserir
TreeNode* inserir(TreeNode* root, Pokemon *pokemon);

// Funcao para criar a pokedex e armazenar em uma arvore binaria
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

// Funcao para inserir um Pokemon na arvore binaria
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

// Funcao para imprimir a carta do Pokemon
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
    printf("Lendario: %s\n", carta->lendario);
}

// Funcao para buscar um Pokemon por nome
Pokemon* buscarPorNome(TreeNode* root, const char* nome) {
    if (root == NULL) return NULL;

    int cmp = strcasecmp(nome, root->pokemon->nome);
    if (cmp == 0) return root->pokemon;
    if (cmp < 0) return buscarPorNome(root->left, nome);
    return buscarPorNome(root->right, nome);
}

// Funcao principal
int main() {
    int tamanho;
    int resposta;
    TreeNode* pokedex = cria_pokedex(&tamanho);

    if (pokedex == NULL || tamanho <= 0) {
        printf("Erro ao criar a pokedex ou nenhum Pokemon encontrado\n");
        return 1;
    }

    do {
        char string[50];  // Declara a variável `string` para armazenar o nome do Pokémon
        
        printf("Digite o nome do Pokemon: ");
        scanf("%s", string);
        Pokemon* p = buscarPorNome(pokedex, string);
        if (p) {
            imprime_carta(p);
        } else {
            printf("Pokemon nao encontrado.\n");
        }

        // Pergunta ao usuário se deseja realizar uma nova busca
        printf("\nDeseja realizar uma nova busca? \n1. Sim\n2. Nao\n");
        scanf("%d", &resposta);

    } while (resposta == 1); // Continua enquanto a resposta for 1



    return 0;
}
