#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// Função para inserir um Pokemon na árvore binária
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


// Funcao para buscar um Pokemon por nome
Pokemon* buscarPorNome(TreeNode* root, const char* nome) {
    if (root == NULL) return NULL;

    int cmp = strcasecmp(nome, root->pokemon->nome);
    if (cmp == 0) return root->pokemon;
    if (cmp < 0) return buscarPorNome(root->left, nome);
    return buscarPorNome(root->right, nome);
}

// Função para criar a Pokedex e inserir em uma árvore de busca binária
TreeNode* cria_pokedex(int* tamanho) {
    FILE* arquivo = fopen("pokemon.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    char linha[256];
    int count = 0;
    TreeNode *root = NULL;

    // Ignorar a primeira linha (cabeçalho)
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

        // Inserir na árvore binária
        root = inserir(root, pokemon);
        count++;
    }

    fclose(arquivo);
    *tamanho = count;
    return root;
}

// Função para armazenar os Pokemons da árvore binária em um array
void armazena_pokemons(TreeNode* raiz, Pokemon* array, int* index) {
    if (raiz == NULL) {
        return;
    }

    armazena_pokemons(raiz->left, array, index);
    array[*index] = *raiz->pokemon;
    (*index)++;
    armazena_pokemons(raiz->right, array, index);
}

// Função para imprimir a carta do Pokemon
void imprime_carta(Pokemon* carta) {
    printf("\n");
    printf("=========================\n");
    printf("| ID: %d\n", carta->id);
    printf("| Nome: %s\n", carta->nome);
    printf("| Tipo 1: %s\n", carta->tipo1);
    printf("| Tipo 2: %s\n", carta->tipo2);
    printf("| Total: %d\n", carta->total);
    printf("| HP: %d\n", carta->hp);
    printf("| Ataque: %d\n", carta->ataque);
    printf("| Defesa: %d\n", carta->defesa);
    printf("| Ataque Especial: %d\n", carta->ataque_especial);
    printf("| Defesa Especial: %d\n", carta->defesa_especial);
    printf("| Velocidade: %d\n", carta->velocidade);
    printf("| Geracao: %d\n", carta->geracao);
    printf("| Lendario: %s\n", carta->lendario); 
    printf("=========================\n");
    printf("\n");
}

// Função que cria um nó
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

// Função que cria um baralho e divide entre jogador_01 e máquina
Baralho* cria_baralho(TreeNode* pokedex, int tamanho, int num_cartas) {
    Baralho* baralho = (Baralho*) malloc(sizeof(Baralho));
    if (baralho == NULL) {
        printf("Erro ao alocar memória para o baralho\n");
        return NULL;
    }

    baralho->primeiro_jogador_01 = NULL;
    baralho->ultimo_jogador_01 = NULL;
    baralho->primeiro_maquina = NULL;
    baralho->ultimo_maquina = NULL;
    baralho->tamanho_jogador_01 = 0;
    baralho->tamanho_maquina = 0;

    // Criar um array temporário para armazenar os Pokemons
    Pokemon* array_pokemons = (Pokemon*) malloc(tamanho * sizeof(Pokemon));
    if (array_pokemons == NULL) {
        printf("Erro ao alocar memória para o array de Pokemons\n");
        return NULL;
    }

    // Armazenar os Pokemons da árvore binária no array
    int index = 0;
    armazena_pokemons(pokedex, array_pokemons, &index);

    // Array auxiliar para rastrear os índices já escolhidos
    int* escolhidos = (int*) calloc(tamanho, sizeof(int));
    if (escolhidos == NULL) {
        printf("Erro ao alocar memória para o array de índices escolhidos\n");
        free(array_pokemons);
        return NULL;
    }

    // Adicionar o número especificado de cartas ao baralho
    srand(time(NULL));
    for (int i = 0; i < num_cartas; i++) {
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

        // Distribuir as cartas entre o jogador_01 e a máquina em fila
        if (i % 2 == 0) {
            if (baralho->primeiro_jogador_01 == NULL) {
                baralho->primeiro_jogador_01 = no;
                baralho->ultimo_jogador_01 = no;
            } else {
                baralho->ultimo_jogador_01->proximo = no;
                no->anterior = baralho->ultimo_jogador_01;
                baralho->ultimo_jogador_01 = no;
            }
            baralho->tamanho_jogador_01++;
        } else {
            if (baralho->primeiro_maquina == NULL) {
                baralho->primeiro_maquina = no;
                baralho->ultimo_maquina = no;
            } else {
                baralho->ultimo_maquina->proximo = no;
                no->anterior = baralho->ultimo_maquina;
                baralho->ultimo_maquina = no;
            }
            baralho->tamanho_maquina++;
        }
    }

    free(array_pokemons);
    free(escolhidos);

    return baralho;
}

// Função para remover uma carta do início de um baralho
Pokemon* remove_carta_inicio(Node** primeiro, Node** ultimo, int* tamanho) {
    if (*primeiro == NULL) {
        return NULL;
    }

    Node* temp = *primeiro;
    Pokemon* carta = (Pokemon*) malloc(sizeof(Pokemon));
    if (carta == NULL) {
        printf("Erro ao alocar memória para a carta\n");
        return NULL;
    }

    *carta = temp->carta;
    *primeiro = temp->proximo;

    if (*primeiro != NULL) {
        (*primeiro)->anterior = NULL;
    } else {
        *ultimo = NULL;
    }

    free(temp);
    (*tamanho)--;

    return carta;
}

// Função para adicionar uma carta ao final de um baralho
void adiciona_carta_fim(Node** primeiro, Node** ultimo, Pokemon* carta, int* tamanho) {
    Node* no = cria_no(carta);
    if (no == NULL) {
        return;
    }

    if (*primeiro == NULL) {
        *primeiro = no;
        *ultimo = no;
    } else {
        (*ultimo)->proximo = no;
        no->anterior = *ultimo;
        *ultimo = no;
    }

    (*tamanho)++;
}

// Função para comparar atributos e decidir o vencedor
void compara_atributos(Baralho* baralho) {
    while (baralho->tamanho_jogador_01 > 0 && baralho->tamanho_maquina > 0) {
        // Remove a carta do início de cada baralho
        Pokemon* carta_jogador_01 = remove_carta_inicio(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, &baralho->tamanho_jogador_01);
        Pokemon* carta_maquina = remove_carta_inicio(&baralho->primeiro_maquina, &baralho->ultimo_maquina, &baralho->tamanho_maquina);

        if (carta_jogador_01 == NULL || carta_maquina == NULL) {
            break;
        }

        // Mostra as cartas na tela
        printf("=========================\n\n");
        printf("Carta do Jogador 01:\n");
        imprime_carta(carta_jogador_01);

        printf("Carta da Maquina:\n");
        imprime_carta(carta_maquina);

        // Seleção de atributo via menu numérico
        int escolha;
        printf("Escolha o atributo para comparar:\n");
        printf("1. HP\n");
        printf("2. Ataque\n");
        printf("3. Defesa\n");
        printf("4. Ataque Especial\n");
        printf("5. Defesa Especial\n");
        printf("6. Lendario\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);


        

        int valor_jogador_01 = 0, valor_maquina = 0;

        switch (escolha) {
            case 1:
                valor_jogador_01 = carta_jogador_01->hp;
                valor_maquina = carta_maquina->hp;
                break;
            case 2:
                valor_jogador_01 = carta_jogador_01->ataque;
                valor_maquina = carta_maquina->ataque;
                break;
            case 3:
                valor_jogador_01 = carta_jogador_01->defesa;
                valor_maquina = carta_maquina->defesa;
                break;
            case 4:
                valor_jogador_01 = carta_jogador_01->ataque_especial;
                valor_maquina = carta_maquina->ataque_especial;
                break;
            case 5:
                valor_jogador_01 = carta_jogador_01->defesa_especial;
                valor_maquina = carta_maquina->defesa_especial;
                break;
            case 6:
                valor_jogador_01 = strcmp(carta_jogador_01->lendario, "True") == 0 ? 1 : 0;
                valor_maquina = strcmp(carta_maquina->lendario, "True") == 0 ? 1 : 0;
                //se as duas cartas forem lendarias, será necessario escolher um novo atributo para comparar
                if (valor_jogador_01 == 1 && valor_maquina == 1){
                    printf("Ambas as cartas são lendarias, escolha um novo atributo para comparar:\n");
                    printf("1. HP\n");
                    printf("2. Ataque\n");
                    printf("3. Defesa\n");
                    printf("4. Ataque Especial\n");
                    printf("5. Defesa Especial\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &escolha);
                    switch (escolha) {
                        case 1:
                            valor_jogador_01 = carta_jogador_01->hp;
                            valor_maquina = carta_maquina->hp;
                            break;
                        case 2:
                            valor_jogador_01 = carta_jogador_01->ataque;
                            valor_maquina = carta_maquina->ataque;
                            break;
                        case 3:
                            valor_jogador_01 = carta_jogador_01->defesa;
                            valor_maquina = carta_maquina->defesa;
                            break;
                        case 4:
                            valor_jogador_01 = carta_jogador_01->ataque_especial;
                            valor_maquina = carta_maquina->ataque_especial;
                            break;
                        case 5:
                            valor_jogador_01 = carta_jogador_01->defesa_especial;
                            valor_maquina = carta_maquina->defesa_especial;
                            break;
                        default:
                            printf("Opcao invalida!\n");
                            free(carta_jogador_01);
                            free(carta_maquina);
                            continue;
                    }
                }
                
                
                break;
            default:
                printf("Opcao invalida!\n");
                free(carta_jogador_01);
                free(carta_maquina);
                continue;
        }
            printf("\nValor da carta do jogador_01 (%s): %d\n", carta_jogador_01->nome, valor_jogador_01);
            printf("Valor da carta da Maquina (%s): %d\n\n", carta_maquina->nome, valor_maquina);

        // Determina o vencedor
        if (valor_jogador_01 > valor_maquina) {
            printf("Jogador 01 venceu a rodada!\n\n");
            adiciona_carta_fim(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, carta_jogador_01, &baralho->tamanho_jogador_01);
            adiciona_carta_fim(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, carta_maquina, &baralho->tamanho_jogador_01);
        } else if (valor_maquina > valor_jogador_01) {
            printf("Maquina venceu a rodada!\n\n");
            adiciona_carta_fim(&baralho->primeiro_maquina, &baralho->ultimo_maquina, carta_maquina, &baralho->tamanho_maquina);
            adiciona_carta_fim(&baralho->primeiro_maquina, &baralho->ultimo_maquina, carta_jogador_01, &baralho->tamanho_maquina);
        } else {
            /*em caso de empate será retirada mais uma carta de cada jogador para novamente ser comparado oa atributos
            o vencedor após o empate ficará com as 4 cartas jogadas*/
            printf("Empate!\n\n");
            Pokemon* carta_jogador_01_2 = remove_carta_inicio(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, &baralho->tamanho_jogador_01);
            Pokemon* carta_maquina_2 = remove_carta_inicio(&baralho->primeiro_maquina, &baralho->ultimo_maquina, &baralho->tamanho_maquina);
            if (carta_jogador_01_2 == NULL || carta_maquina_2 == NULL) {
               return;
            }
            printf("=========================\n\n");
            printf("Carta do Jogador 01:\n");
            imprime_carta(carta_jogador_01_2);

            printf("Carta da Maquina:\n");
            imprime_carta(carta_maquina_2);

            printf("Escolha o atributo para comparar:\n");
            printf("1. HP\n");
            printf("2. Ataque\n");
            printf("3. Defesa\n");
            printf("4. Ataque Especial\n");
            printf("5. Defesa Especial\n");
            printf("6. Lendario\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &escolha);

            switch (escolha) {
            case 1:
                valor_jogador_01 = carta_jogador_01_2->hp;
                valor_maquina = carta_maquina_2->hp;
                break;
            case 2:
                valor_jogador_01 = carta_jogador_01_2->ataque;
                valor_maquina = carta_maquina_2->ataque;
                break;
            case 3:
                valor_jogador_01 = carta_jogador_01_2->defesa;
                valor_maquina = carta_maquina_2->defesa;
                break;
            case 4:
                valor_jogador_01 = carta_jogador_01_2->ataque_especial;
                valor_maquina = carta_maquina_2->ataque_especial;
                break;
            case 5:
                valor_jogador_01 = carta_jogador_01_2->defesa_especial;
                valor_maquina = carta_maquina_2->defesa_especial;
                break;
            case 6:
                valor_jogador_01 = strcmp(carta_jogador_01_2->lendario, "True") == 0 ? 1 : 0;
                valor_maquina = strcmp(carta_maquina_2->lendario, "True") == 0 ? 1 : 0;
                if (valor_jogador_01 == 1 && valor_maquina == 1){
                    printf("Ambas as cartas são lendarias, escolha um novo atributo para comparar:\n");
                    printf("1. HP\n");
                    printf("2. Ataque\n");
                    printf("3. Defesa\n");
                    printf("4. Ataque Especial\n");
                    printf("5. Defesa Especial\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &escolha);

                    switch (escolha){
                    case 1:
                        valor_jogador_01 = carta_jogador_01_2->hp;
                        valor_maquina = carta_maquina_2->hp;
                        break;
                    case 2:
                        valor_jogador_01 = carta_jogador_01_2->ataque;
                        valor_maquina = carta_maquina_2->ataque;
                        break;
                    case 3:
                        valor_jogador_01 = carta_jogador_01_2->defesa;
                        valor_maquina = carta_maquina_2->defesa;
                        break;
                    case 4:
                        valor_jogador_01 = carta_jogador_01_2->ataque_especial;
                        valor_maquina = carta_maquina_2->ataque_especial;
                        break;
                    case 5:
                        valor_jogador_01 = carta_jogador_01_2->defesa_especial;
                        valor_maquina = carta_maquina_2->defesa_especial;
                        break;
                    
                    default:
                        printf("Opcao invalida!\n");
                            free(carta_jogador_01);
                            free(carta_maquina);
                            continue;
                    }
                }
                
            
            default:
                printf("Opcao invalida!\n");
                            free(carta_jogador_01);
                            free(carta_maquina);
                            continue;
            }
            printf("\nValor da carta do jogador_01 (%s): %d\n", carta_jogador_01_2->nome, valor_jogador_01);
            printf("Valor da carta da Maquina (%s): %d\n\n", carta_maquina_2->nome, valor_maquina);
            // Determina o vencedor, o vencedor ficara com todas as cartas jogadas
            if (valor_jogador_01 > valor_maquina) {
                printf("Jogador 01 venceu a rodada!\n\n");
                //o jogador 01 ficara com as 4 cartas jogadas
                adiciona_carta_fim(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, carta_jogador_01, &baralho->tamanho_jogador_01);
                adiciona_carta_fim(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, carta_maquina, &baralho->tamanho_jogador_01);
                adiciona_carta_fim(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, carta_jogador_01_2, &baralho->tamanho_jogador_01);
                adiciona_carta_fim(&baralho->primeiro_jogador_01, &baralho->ultimo_jogador_01, carta_maquina_2, &baralho->tamanho_jogador_01);
            }else if(valor_jogador_01 < valor_maquina){
                printf("Maquina venceu a rodada!\n\n");
                //a maquina ficara com as cartas jogadas
                adiciona_carta_fim(&baralho->primeiro_maquina, &baralho->ultimo_maquina, carta_maquina, &baralho->tamanho_maquina);
                adiciona_carta_fim(&baralho->primeiro_maquina, &baralho->ultimo_maquina, carta_jogador_01, &baralho->tamanho_maquina);
                adiciona_carta_fim(&baralho->primeiro_maquina, &baralho->ultimo_maquina, carta_maquina_2, &baralho->tamanho_maquina);
                adiciona_carta_fim(&baralho->primeiro_maquina, &baralho->ultimo_maquina, carta_jogador_01_2, &baralho->tamanho_maquina);
            }
            


        }

        printf("Cartas restantes:\nJogador 01: %d\nMaquina: %d\n\n", baralho->tamanho_jogador_01, baralho->tamanho_maquina);
    }
    

    

    if (baralho->tamanho_jogador_01 == 0) {
        printf("Maquina venceu o jogo!\n\n");
    } else if (baralho->tamanho_maquina == 0) {
        printf("Jogador 01 venceu o jogo!\n\n");
    }
}


// Função para exibir o menu inicial
void menu_inicial(TreeNode* pokedex, int tamanho) {
    int opcao;
    Baralho* baralho = NULL;

    do {
        printf("========== MENU ==========\n");
        printf("1. Acessar Pokedex\n");
        printf("2. Iniciar Partida\n");
        printf("0. Sair\n");
        printf("===========================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
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
            case 2: {
                int num_cartas;
                printf("Digite o numero de cartas para a partida: ");
                scanf("%d", &num_cartas);

                if (num_cartas % 2 != 0) {
                    printf("O numero de cartas deve ser par!\n");
                    break;
                }

                baralho = cria_baralho(pokedex, tamanho, num_cartas);
                if (baralho != NULL) {
                    compara_atributos(baralho);
                    free(baralho);
                }
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);
}

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

