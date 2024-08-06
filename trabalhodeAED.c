#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Função para criar a pokedex
Pokemon* cria_pokedex(int* tamanho) {
    printf("Abrindo arquivo pokemon.csv\n");
    FILE* arquivo = fopen("pokemon.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    char linha[256];
    int capacidade = 10;
    int count = 0;
    Pokemon* pokedex = (Pokemon*) malloc(capacidade * sizeof(Pokemon));
    if (pokedex == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }

    // Ignorar a primeira linha (cabeçalho)
    fgets(linha, sizeof(linha), arquivo);
    printf("Cabecalho ignorado\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        printf("Lendo linha: %s\n", linha);
        if (count >= capacidade) {
            capacidade *= 2;
            pokedex = (Pokemon*) realloc(pokedex, capacidade * sizeof(Pokemon));
            if (pokedex == NULL) {
                printf("Erro ao realocar memoria\n");
                exit(1);
            }
        }

        // Remover nova linha
        linha[strcspn(linha, "\n")] = 0;

        // Dividir a linha em campos
        char* token = strtok(linha, ",");
        pokedex[count].id = atoi(token); // Ler o ID

        token = strtok(NULL, ",");
        strcpy(pokedex[count].nome, token);

        token = strtok(NULL, ",");
        strcpy(pokedex[count].tipo1, token);

        token = strtok(NULL, ",");
        if (token == NULL || strcmp(token, "ND") == 0) {
            strcpy(pokedex[count].tipo2, "");
        } else {
            strcpy(pokedex[count].tipo2, token);
        }

        token = strtok(NULL, ",");
        pokedex[count].total = atoi(token);

        token = strtok(NULL, ",");
        pokedex[count].hp = atoi(token);

        token = strtok(NULL, ",");
        pokedex[count].ataque = atoi(token);

        token = strtok(NULL, ",");
        pokedex[count].defesa = atoi(token);

        token = strtok(NULL, ",");
        pokedex[count].ataque_especial = atoi(token);

        token = strtok(NULL, ",");
        pokedex[count].defesa_especial = atoi(token);

        token = strtok(NULL, ",");
        pokedex[count].velocidade = atoi(token);

        token = strtok(NULL, ",");
        pokedex[count].geracao = atoi(token);

        token = strtok(NULL, ",");
        strcpy(pokedex[count].lendario, token);

        printf("Pokemon %d adicionado: %s\n", count + 1, pokedex[count].nome);
        count++;
    }

    fclose(arquivo);
    *tamanho = count;
    printf("Arquivo fechado, pokedex criada com %d pokemons\n", count);
    return pokedex;
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
    printf("Lendario: %s\n", carta->lendario);
}

int main() {
    int tamanho;
    printf("Chamando cria_pokedex\n");
    Pokemon* pokedex = cria_pokedex(&tamanho);

    // Adicionar mensagem de depuração
    printf("Pokedex criada com %d pokemons\n", tamanho);

    // Verificar se a pokedex foi criada corretamente
    if (pokedex == NULL) {
        printf("Erro ao criar a pokedex\n");
        return 1;
    }

    // Verificar se o tamanho é maior que 0
    if (tamanho <= 0) {
        printf("Nenhum pokemon encontrado\n");
        return 1;
    }

    // Imprime as cartas
    for (int i = 0; i < tamanho; i++) {
        printf("Imprimindo carta do Pokemon %d\n", i + 1);
        imprime_carta(&pokedex[i]);
    }

    free(pokedex);

    return 0;
}
