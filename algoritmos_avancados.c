#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// ============================
// ESTRUTURA DA ÁRVORE BINÁRIA
// ============================
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// ============================
// FUNÇÃO PARA CRIAR UMA SALA
// ============================
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

// ============================================
// FUNÇÃO INTERATIVA DE EXPLORAÇÃO DA MANSÃO
// ============================================
void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: ** %s **\n", atual->nome);

        // Nó-folha → fim do caminho
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Você chegou ao fim deste caminho!\n");
            return;
        }

        printf("Para onde deseja ir?\n");
        if (atual->esq != NULL) printf("  (e) - Ir para a esquerda (%s)\n", atual->esq->nome);
        if (atual->dir != NULL) printf("  (d) - Ir para a direita (%s)\n", atual->dir->nome);
        printf("  (s) - Sair da exploração\n");
        printf("Escolha: ");

        scanf(" %c", &opcao);
        opcao = tolower(opcao);

        if (opcao == 'e' && atual->esq != NULL) {
            atual = atual->esq;
        } else if (opcao == 'd' && atual->dir != NULL) {
            atual = atual->dir;
        } else if (opcao == 's') {
            printf("Saindo da exploração...\n");
            return;
        } else {
            printf("Opção inválida!\n");
        }
    }
}

// ============================
// CRIAÇÃO DO MAPA DA MANSÃO
// ============================
Sala* montarMansao() {
    // Nó raiz
    Sala *hall = criarSala("Hall de Entrada");

    // Segundo nível
    hall->esq = criarSala("Sala de Estar");
    hall->dir = criarSala("Corredor Principal");

    // Terceiro nível
    hall->esq->esq = criarSala("Biblioteca Antiga");
    hall->esq->dir = criarSala("Sala de Música");

    hall->dir->esq = criarSala("Cozinha");
    hall->dir->dir = criarSala("Jardim Interno");

    return hall;  // Retorna o ponto inicial do jogo
}

// ============================
// PROGRAMA PRINCIPAL
// ============================
int main() {
    Sala *inicio = montarMansao();

    printf("=============================================\n");
    printf("       Bem-vindo ao Detective Quest!         \n");
    printf("        Exploração da Mansão – Novato        \n");
    printf("=============================================\n");

    explorarSalas(inicio);

    printf("\nObrigado por jogar!\n");

    return 0;
}