#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 10

// =====================================================
// ESTRUTURA DA ÁRVORE BINÁRIA (MANSÃO)
// =====================================================
typedef struct Sala {
    char nome[50];
    char pista[50];  
    char suspeito[50];  
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// =====================================================
// ESTRUTURA DA BST DE PISTAS
// =====================================================
typedef struct NodoPista {
    char pista[50];
    struct NodoPista *esq;
    struct NodoPista *dir;
} NodoPista;

// =====================================================
// ESTRUTURA DA TABELA HASH
// =====================================================
typedef struct NodoHash {
    char suspeito[50];
    char pista[50];
    struct NodoHash *prox;
} NodoHash;

NodoHash* tabela[TAM_HASH];

// =====================================================
// FUNÇÃO HASH
// =====================================================
int hash(const char *str) {
    return (toupper(str[0]) - 'A') % TAM_HASH;
}

// =====================================================
// INSERIR NA HASH
// =====================================================
void inserirNaHash(const char *pista, const char *suspeito) {
    int h = hash(suspeito);

    NodoHash *novo = (NodoHash*) malloc(sizeof(NodoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[h];
    tabela[h] = novo;
}

// =====================================================
// LISTAR TUDO DA HASH
// =====================================================
void listarHash() {
    printf("\n===== TABELA HASH: SUSPEITOS E PISTAS =====\n");
    for (int i = 0; i < TAM_HASH; i++) {
        NodoHash *aux = tabela[i];
        if (aux != NULL)
            printf("\n[Bucket %d]\n", i);

        while (aux != NULL) {
            printf("Suspeito: %-15s | Pista: %s\n",
                   aux->suspeito, aux->pista);
            aux = aux->prox;
        }
    }
}

// =====================================================
// DESCOBRIR O SUSPEITO MAIS CITADO
// =====================================================
char* suspeitoMaisCitado() {
    static char mais[50];
    int maior = 0;

    // dicionário simples
    char nomes[20][50];
    int cont[20];
    int qtd = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        NodoHash *aux = tabela[i];
        while (aux != NULL) {
            int encontrado = 0;
            for (int j = 0; j < qtd; j++) {
                if (strcmp(nomes[j], aux->suspeito) == 0) {
                    cont[j]++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(nomes[qtd], aux->suspeito);
                cont[qtd] = 1;
                qtd++;
            }
            aux = aux->prox;
        }
    }

    for (int i = 0; i < qtd; i++) {
        if (cont[i] > maior) {
            maior = cont[i];
            strcpy(mais, nomes[i]);
        }
    }

    return (qtd == 0 ? "Nenhum suspeito" : mais);
}

// =====================================================
// FUNÇÕES DA BST (mesmas do Nível Aventureiro)
// =====================================================
NodoPista* criarNodoPista(const char *texto) {
    NodoPista *n = (NodoPista*) malloc(sizeof(NodoPista));
    strcpy(n->pista, texto);
    n->esq = n->dir = NULL;
    return n;
}

NodoPista* inserirPista(NodoPista *raiz, const char *texto) {
    if (raiz == NULL)
        return criarNodoPista(texto);

    if (strcmp(texto, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, texto);
    else if (strcmp(texto, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, texto);

    return raiz;
}

void listarPistas(NodoPista *raiz) {
    if (raiz == NULL) return;
    listarPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    listarPistas(raiz->dir);
}

// =====================================================
// CRIAÇÃO DA MANSÃO (AGORA COM SUSPEITOS)
// =====================================================
Sala* criarSala(const char *nome, const char *pista, const char *suspeito) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista);
    strcpy(s->suspeito, suspeito);
    s->esq = s->dir = NULL;
    return s;
}

Sala* montarMansao() {
    Sala *hall = criarSala("Hall de Entrada", "", "");

    hall->esq  = criarSala("Sala de Estar", "Tecido azul", "Arthur");
    hall->dir  = criarSala("Corredor", "", "");

    hall->esq->esq = criarSala("Biblioteca", "Página rasgada", "Helena");
    hall->esq->dir = criarSala("Sala de Música", "", "");

    hall->dir->esq = criarSala("Cozinha", "Pegada de barro", "Arthur");
    hall->dir->dir = criarSala("Jardim", "Luvas sujas", "Marcos");

    return hall;
}

// =====================================================
// EXPLORAÇÃO DO JOGO
// =====================================================
void explorar(Sala *atual, NodoPista **raiz) {
    char op;

    while (atual) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf(">> PISTA ENCONTRADA: %s\n", atual->pista);

            *raiz = inserirPista(*raiz, atual->pista);
            inserirNaHash(atual->pista, atual->suspeito);
        }

        if (!atual->esq && !atual->dir) {
            printf("Fim do caminho.\n");
            return;
        }

        printf("\nOpções:\n");
        if (atual->esq) printf("(e) Ir para %s\n", atual->esq->nome);
        if (atual->dir) printf("(d) Ir para %s\n", atual->dir->nome);
        printf("(p) Ver pistas\n");
        printf("(s) Sair\n");
        printf("Escolha: ");

        scanf(" %c", &op);

        if (op == 'e' && atual->esq)
            atual = atual->esq;
        else if (op == 'd' && atual->dir)
            atual = atual->dir;
        else if (op == 'p') {
            printf("\n===== PISTAS COLETADAS =====\n");
            listarPistas(*raiz);
        }
        else if (op == 's')
            return;
        else
            printf("Opção inválida!\n");
    }
}

// =====================================================
// MAIN
// =====================================================
int main() {
    for (int i = 0; i < TAM_HASH; i++)
        tabela[i] = NULL;

    Sala *inicio = montarMansao();
    NodoPista *pistas = NULL;

    printf("=============================================\n");
    printf("         Detective Quest – Nível Mestre       \n");
    printf("   Pistas, Suspeitos e Tabela Hash Final!     \n");
    printf("=============================================\n");

    explorar(inicio, &pistas);

    printf("\n===== TODAS AS PISTAS COLETADAS =====\n");
    listarPistas(pistas);

    listarHash();

    printf("\n>>> SUSPEITO MAIS PROVÁVEL: ** %s **\n",
           suspeitoMaisCitado());

    printf("\nObrigado por jogar!\n");

    return 0;
}