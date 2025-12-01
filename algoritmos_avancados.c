#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// =====================================================
// ESTRUTURA DA ÁRVORE BINÁRIA QUE REPRESENTA A MANSÃO
// =====================================================
typedef struct Sala {
    char nome[50];
    char pista[50];   // pista encontrada nessa sala (se houver)
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// =====================================================
// ESTRUTURA DA ÁRVORE DE BUSCA (BST) PARA AS PISTAS
// =====================================================
typedef struct NodoPista {
    char pista[50];
    struct NodoPista *esq;
    struct NodoPista *dir;
} NodoPista;

// =====================================================
// FUNÇÕES DA BST DE PISTAS
// =====================================================

// Cria um novo nodo de pista
NodoPista* criarNodoPista(const char *texto) {
    NodoPista *n = (NodoPista*) malloc(sizeof(NodoPista));
    strcpy(n->pista, texto);
    n->esq = n->dir = NULL;
    return n;
}

// Inserção ordenada (strings)
NodoPista* inserirPista(NodoPista *raiz, const char *texto) {
    if (raiz == NULL) {
        return criarNodoPista(texto);
    }

    if (strcmp(texto, raiz->pista) < 0) {
        raiz->esq = inserirPista(raiz->esq, texto);
    } else if (strcmp(texto, raiz->pista) > 0) {
        raiz->dir = inserirPista(raiz->dir, texto);
    }

    return raiz;
}

// Busca por uma pista específica
int buscarPista(NodoPista *raiz, const char *texto) {
    if (raiz == NULL)
        return 0;

    int cmp = strcmp(texto, raiz->pista);

    if (cmp == 0)
        return 1;
    else if (cmp < 0)
        return buscarPista(raiz->esq, texto);
    else
        return buscarPista(raiz->dir, texto);
}

// Exibe as pistas em ordem alfabética
void listarPistasEmOrdem(NodoPista *raiz) {
    if (raiz == NULL) return;
    listarPistasEmOrdem(raiz->esq);
    printf(" - %s\n", raiz->pista);
    listarPistasEmOrdem(raiz->dir);
}

// =====================================================
// FUNÇÕES DA ÁRVORE DE SALAS
// =====================================================
Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    if (pista != NULL)
        strcpy(s->pista, pista);
    else
        s->pista[0] = '\0'; // sem pista
    s->esq = s->dir = NULL;
    return s;
}

// Monte o mapa da mansão
Sala* montarMansao() {
    Sala *hall = criarSala("Hall de Entrada", "");

    hall->esq = criarSala("Sala de Estar", "Fragmento de tecido azul");
    hall->dir = criarSala("Corredor Principal", "");

    hall->esq->esq = criarSala("Biblioteca Antiga", "Página rasgada de diário");
    hall->esq->dir = criarSala("Sala de Música", "");

    hall->dir->esq = criarSala("Cozinha", "Pegada suja");
    hall->dir->dir = criarSala("Jardim Interno", "");

    return hall;
}

// =====================================================
// INTERAÇÃO DO JOGADOR
// =====================================================
void explorarSalas(Sala *atual, NodoPista **raizPistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: ** %s **\n", atual->nome);

        // Se houver pista na sala, inserir na BST
        if (strlen(atual->pista) > 0) {
            if (!buscarPista(*raizPistas, atual->pista)) {
                *raizPistas = inserirPista(*raizPistas, atual->pista);
                printf(">> PISTA ENCONTRADA: %s\n", atual->pista);
            }
        }

        // Nó-folha: fim do caminho
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Você chegou ao fim deste caminho!\n");
            return;
        }

        printf("\nPara onde deseja ir?\n");
        if (atual->esq != NULL) printf("  (e) - Esquerda (%s)\n", atual->esq->nome);
        if (atual->dir != NULL) printf("  (d) - Direita (%s)\n", atual->dir->nome);
        printf("  (p) - Listar pistas encontradas\n");
        printf("  (s) - Sair da exploração\n");
        printf("Escolha: ");

        scanf(" %c", &opcao);
        opcao = tolower(opcao);

        if (opcao == 'e' && atual->esq != NULL)
            atual = atual->esq;
        else if (opcao == 'd' && atual->dir != NULL)
            atual = atual->dir;
        else if (opcao == 'p') {
            printf("\n===== PISTAS ENCONTRADAS =====\n");
            if (*raizPistas == NULL)
                printf("(nenhuma pista ainda)\n");
            else
                listarPistasEmOrdem(*raizPistas);
        }
        else if (opcao == 's') {
            printf("\nSaindo da exploração...\n");
            return;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

// =====================================================
// MAIN
// =====================================================
int main() {
    Sala *inicio = montarMansao();
    NodoPista *raizPistas = NULL;

    printf("===============================================\n");
    printf("        Detective Quest – Nível Aventureiro     \n");
    printf("     Agora você coleta pistas em uma BST!       \n");
    printf("===============================================\n");

    explorarSalas(inicio, &raizPistas);

    printf("\n===== PISTAS FINAIS COLETADAS =====\n");
    if (raizPistas == NULL)
        printf("(nenhuma pista encontrada)\n");
    else
        listarPistasEmOrdem(raizPistas);

    printf("\nObrigado por jogar!\n");

    return 0;
}