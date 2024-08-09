#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poketrunfo.h"

int main(){
    int tamanho;
    TreeNode* pokedex = cria_pokedex(&tamanho);

    if (pokedex == NULL || tamanho <= 0) {
        printf("Erro ao criar a pokedex ou nenhum Pokemon encontrado\n");
        return 1;
    }

    menu_inicial(pokedex, tamanho);

    return 0;
}