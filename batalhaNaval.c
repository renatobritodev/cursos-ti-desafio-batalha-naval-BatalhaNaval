#include <stdio.h>

/* Constantes */
#define TAMANHO_TABULEIRO 10   /* Dimensão do tabuleiro (linhas e colunas) */
#define TAMANHO_NAVIO     3    /* Número de células que cada navio ocupa   */

#define AGUA   0   /* Valor que representa água no tabuleiro */
#define NAVIO  3   /* Valor que representa parte de um navio */

/* Protótipos */
void inicializar_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
int  validar_horizontal(int linha, int coluna);
int  validar_vertical(int linha, int coluna);
int  verificar_sobreposicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                             int linha, int coluna, int horizontal);
void posicionar_navio_horizontal(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                  int linha, int coluna, int navio[TAMANHO_NAVIO]);
void posicionar_navio_vertical(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                int linha, int coluna, int navio[TAMANHO_NAVIO]);
void exibir_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);

/* ══════════════════════════════════════════════════════════ */
int main(void)
{
    /* Declaração do tabuleiro */
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

    /*
     * Vetores que representam os navios.
     * Cada elemento vale NAVIO (3), indicando que aquela
     * posição do navio está "intacta".
     */
    int navio_horizontal[TAMANHO_NAVIO] = {NAVIO, NAVIO, NAVIO};
    int navio_vertical[TAMANHO_NAVIO]   = {NAVIO, NAVIO, NAVIO};

    /*
     * Coordenadas iniciais dos navios (linha, coluna).
     * O navio cresce para a direita (horizontal) ou
     * para baixo (vertical) a partir da posição inicial.
     *
     * Navio horizontal: linha 2, colunas 1-2-3
     * Navio vertical  : linha 5, colunas 7-8-9 (linhas 5-6-7)
     */
    int linha_h  = 2, coluna_h = 1;   /* posição inicial do navio horizontal */
    int linha_v  = 5, coluna_v = 7;   /* posição inicial do navio vertical   */

    /* Passo 1: Inicializar o tabuleiro com água */
    inicializar_tabuleiro(tabuleiro);

    /* Passo 2: Validar e posicionar navio horizontal  */
    if (!validar_horizontal(linha_h, coluna_h)) {
        printf("ERRO: Navio horizontal fora dos limites do tabuleiro!\n");
        return 1;
    }
    if (verificar_sobreposicao(tabuleiro, linha_h, coluna_h, 1)) {
        printf("ERRO: Navio horizontal se sobrepoem a outro navio!\n");
        return 1;
    }
    posicionar_navio_horizontal(tabuleiro, linha_h, coluna_h, navio_horizontal);
    printf("Navio horizontal posicionado: linha %d, colunas %d a %d\n",
           linha_h, coluna_h, coluna_h + TAMANHO_NAVIO - 1);

    /* Passo 3: Validar e posicionar navio vertical  */
    if (!validar_vertical(linha_v, coluna_v)) {
        printf("ERRO: Navio vertical fora dos limites do tabuleiro!\n");
        return 1;
    }
    if (verificar_sobreposicao(tabuleiro, linha_v, coluna_v, 0)) {
        printf("ERRO: Navio vertical se sobrepoem a outro navio!\n");
        return 1;
    }
    posicionar_navio_vertical(tabuleiro, linha_v, coluna_v, navio_vertical);
    printf("Navio vertical posicionado: coluna %d, linhas %d a %d\n\n",
           coluna_v, linha_v, linha_v + TAMANHO_NAVIO - 1);

    /* Passo 4: Exibir o tabuleiro */
    exibir_tabuleiro(tabuleiro);

    return 0;
}

/* ══════════════════════════════════════════════════════════ */

/*
 * inicializar_tabuleiro
 * Preenche todas as células da matriz com ÁGUA (0).
 */
void inicializar_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO])
{
    int i, j;
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

/*
 * validar_horizontal
 * Retorna 1 se o navio horizontal cabe no tabuleiro,
 * ou 0 caso contrário.
 */
int validar_horizontal(int linha, int coluna)
{
    /* A linha deve estar dentro do tabuleiro */
    if (linha < 0 || linha >= TAMANHO_TABULEIRO)
        return 0;
    /* A coluna inicial e a coluna final devem estar dentro do tabuleiro */
    if (coluna < 0 || coluna + TAMANHO_NAVIO - 1 >= TAMANHO_TABULEIRO)
        return 0;
    return 1;
}

/*
 * validar_vertical
 * Retorna 1 se o navio vertical cabe no tabuleiro,
 * ou 0 caso contrário.
 */
int validar_vertical(int linha, int coluna)
{
    /* A coluna deve estar dentro do tabuleiro */
    if (coluna < 0 || coluna >= TAMANHO_TABULEIRO)
        return 0;
    /* A linha inicial e a linha final devem estar dentro do tabuleiro */
    if (linha < 0 || linha + TAMANHO_NAVIO - 1 >= TAMANHO_TABULEIRO)
        return 0;
    return 1;
}

/*
 * verificar_sobreposicao
 * Percorre as células que o navio ocuparia e verifica se
 * alguma delas já contém um navio (valor NAVIO).
 * Parâmetro 'horizontal': 1 = horizontal, 0 = vertical.
 * Retorna 1 se houver sobreposição, 0 se estiver livre.
 */
int verificar_sobreposicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                            int linha, int coluna, int horizontal)
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++) {
        if (horizontal) {
            /* Avança na coluna */
            if (tabuleiro[linha][coluna + k] == NAVIO)
                return 1;
        } else {
            /* Avança na linha */
            if (tabuleiro[linha + k][coluna] == NAVIO)
                return 1;
        }
    }
    return 0;   /* Nenhuma sobreposição encontrada */
}

/*
 * posicionar_navio_horizontal
 * Copia o valor de cada posição do vetor 'navio' para
 * a linha indicada, avançando nas colunas.
 */
void posicionar_navio_horizontal(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                  int linha, int coluna, int navio[TAMANHO_NAVIO])
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++) {
        tabuleiro[linha][coluna + k] = navio[k];
    }
}

/*
 * posicionar_navio_vertical
 * Copia o valor de cada posição do vetor 'navio' para
 * a coluna indicada, avançando nas linhas.
 */
void posicionar_navio_vertical(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                int linha, int coluna, int navio[TAMANHO_NAVIO])
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++) {
        tabuleiro[linha + k][coluna] = navio[k];
    }
}

/*
 * exibir_tabuleiro
 * Imprime a matriz no console com cabeçalho de colunas
 * e índice de linhas para facilitar a leitura.
 * 0 = água  |  3 = navio
 */
void exibir_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO])
{
    int i, j;

    printf("=== TABULEIRO BATALHA NAVAL (0=agua | 3=navio) ===\n\n");

    /* Cabeçalho com os índices das colunas */
    printf("   ");
    for (j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%2d", j);
    }
    printf("\n");

    /* Separador */
    printf("   ");
    for (j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("--");
    }
    printf("\n");

    /* Linhas da matriz com índice à esquerda */
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d|", i);   /* índice da linha */
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf("%2d", tabuleiro[i][j]);
        }
        printf("\n");
    }

    printf("\n==================================================\n");
}