#include <stdio.h>
#include <stdlib.h>

/* Constantes */
#define TAMANHO_TABULEIRO 10 /* Dimensão do tabuleiro (linhas e colunas) */
#define TAMANHO_NAVIO 3      /* Número de células que cada navio ocupa   */
#define TAMANHO_HAB 5        /* Dimensão das matrizes de habilidade (5x5)*/

#define AGUA 0   /* Valor que representa água no tabuleiro      */
#define NAVIO 3  /* Valor que representa parte de um navio      */
#define EFEITO 5 /* Valor que representa área de efeito de hab. */

void inicializar_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
int validar_horizontal(int linha, int coluna);
int validar_vertical(int linha, int coluna);
int verificar_sobreposicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                           int linha, int coluna, int horizontal);
void posicionar_navio_horizontal(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                 int linha, int coluna, int navio[TAMANHO_NAVIO]);
void posicionar_navio_vertical(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                               int linha, int coluna, int navio[TAMANHO_NAVIO]);
void exibir_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
int validar_diagonal_desc(int linha, int coluna);
int validar_diagonal_asc(int linha, int coluna);
int verificar_sobreposicao_diagonal_desc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                         int linha, int coluna);
int verificar_sobreposicao_diagonal_asc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                        int linha, int coluna);
void posicionar_navio_diagonal_desc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                    int linha, int coluna, int navio[TAMANHO_NAVIO]);
void posicionar_navio_diagonal_asc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                   int linha, int coluna, int navio[TAMANHO_NAVIO]);
void construir_hab_cone(int hab[TAMANHO_HAB][TAMANHO_HAB]);
void construir_hab_cruz(int hab[TAMANHO_HAB][TAMANHO_HAB]);
void construir_hab_octaedro(int hab[TAMANHO_HAB][TAMANHO_HAB]);
void aplicar_habilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                        int hab[TAMANHO_HAB][TAMANHO_HAB],
                        int origem_linha, int origem_coluna);

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
    int navio_vertical[TAMANHO_NAVIO] = {NAVIO, NAVIO, NAVIO};
    int navio_diag_desc[TAMANHO_NAVIO] = {NAVIO, NAVIO, NAVIO};
    int navio_diag_asc[TAMANHO_NAVIO] = {NAVIO, NAVIO, NAVIO};

    /*
     * Matrizes de habilidade 5x5 — nível mestre.
     * Cada matriz será preenchida dinamicamente pela
     * função construtora correspondente.
     */
    int hab_cone[TAMANHO_HAB][TAMANHO_HAB];
    int hab_cruz[TAMANHO_HAB][TAMANHO_HAB];
    int hab_octaedro[TAMANHO_HAB][TAMANHO_HAB];

    /*
     * Coordenadas iniciais dos navios (linha, coluna).
     *
     * Navio horizontal     : linha 2, colunas 1-2-3
     * Navio vertical       : linha 5, coluna  7  (linhas 5-6-7)
     * Navio diagonal desc  : linha 4, coluna  0  (4,0) > (5,1) > (6,2)
     * Navio diagonal asc   : linha 9, coluna  6  (9,6) > (8,7) > (7,8)
     */
    int linha_h = 2, coluna_h = 1;
    int linha_v = 5, coluna_v = 7;
    int linha_dd = 4, coluna_dd = 0;
    int linha_da = 9, coluna_da = 6;

    /*
     * Pontos de origem das habilidades no tabuleiro.
     * O ponto de origem é o CENTRO da matriz 5x5, portanto
     * o efeito se expande 2 células em cada direção a partir dele.
     *
     * Cone     : centrado em (1, 7)
     * Cruz     : centrado em (7, 3)
     * Octaedro : centrado em (4, 6)
     */
    int orig_cone_l = 1, orig_cone_c = 7;
    int orig_cruz_l = 7, orig_cruz_c = 3;
    int orig_octaedro_l = 4, orig_octaedro_c = 6;

    inicializar_tabuleiro(tabuleiro);

    if (!validar_horizontal(linha_h, coluna_h))
    {
        printf("ERRO: Navio horizontal fora dos limites do tabuleiro!\n");
        return 1;
    }
    if (verificar_sobreposicao(tabuleiro, linha_h, coluna_h, 1))
    {
        printf("ERRO: Navio horizontal se sobrepoem a outro navio!\n");
        return 1;
    }
    posicionar_navio_horizontal(tabuleiro, linha_h, coluna_h, navio_horizontal);
    printf("Navio horizontal posicionado: linha %d, colunas %d a %d\n",
           linha_h, coluna_h, coluna_h + TAMANHO_NAVIO - 1);

    if (!validar_vertical(linha_v, coluna_v))
    {
        printf("ERRO: Navio vertical fora dos limites do tabuleiro!\n");
        return 1;
    }
    if (verificar_sobreposicao(tabuleiro, linha_v, coluna_v, 0))
    {
        printf("ERRO: Navio vertical se sobrepoem a outro navio!\n");
        return 1;
    }
    posicionar_navio_vertical(tabuleiro, linha_v, coluna_v, navio_vertical);
    printf("Navio vertical posicionado: coluna %d, linhas %d a %d\n",
           coluna_v, linha_v, linha_v + TAMANHO_NAVIO - 1);

    if (!validar_diagonal_desc(linha_dd, coluna_dd))
    {
        printf("ERRO: Navio diagonal desc fora dos limites do tabuleiro!\n");
        return 1;
    }
    if (verificar_sobreposicao_diagonal_desc(tabuleiro, linha_dd, coluna_dd))
    {
        printf("ERRO: Navio diagonal desc se sobrepoem a outro navio!\n");
        return 1;
    }
    posicionar_navio_diagonal_desc(tabuleiro, linha_dd, coluna_dd, navio_diag_desc);
    printf("Navio diagonal (\\) posicionado: (%d,%d) -> (%d,%d)\n",
           linha_dd, coluna_dd,
           linha_dd + TAMANHO_NAVIO - 1, coluna_dd + TAMANHO_NAVIO - 1);

    if (!validar_diagonal_asc(linha_da, coluna_da))
    {
        printf("ERRO: Navio diagonal asc fora dos limites do tabuleiro!\n");
        return 1;
    }
    if (verificar_sobreposicao_diagonal_asc(tabuleiro, linha_da, coluna_da))
    {
        printf("ERRO: Navio diagonal asc se sobrepoem a outro navio!\n");
        return 1;
    }
    posicionar_navio_diagonal_asc(tabuleiro, linha_da, coluna_da, navio_diag_asc);
    printf("Navio diagonal (/) posicionado: (%d,%d) -> (%d,%d)\n\n",
           linha_da, coluna_da,
           linha_da - (TAMANHO_NAVIO - 1), coluna_da + TAMANHO_NAVIO - 1);

    construir_hab_cone(hab_cone);
    construir_hab_cruz(hab_cruz);
    construir_hab_octaedro(hab_octaedro);

    aplicar_habilidade(tabuleiro, hab_cone, orig_cone_l, orig_cone_c);
    printf("Habilidade CONE aplicada com origem em (%d,%d)\n",
           orig_cone_l, orig_cone_c);

    aplicar_habilidade(tabuleiro, hab_cruz, orig_cruz_l, orig_cruz_c);
    printf("Habilidade CRUZ aplicada com origem em (%d,%d)\n",
           orig_cruz_l, orig_cruz_c);

    aplicar_habilidade(tabuleiro, hab_octaedro, orig_octaedro_l, orig_octaedro_c);
    printf("Habilidade OCTAEDRO aplicada com origem em (%d,%d)\n\n",
           orig_octaedro_l, orig_octaedro_c);

    exibir_tabuleiro(tabuleiro);

    return 0;
}

void inicializar_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO])
{
    int i, j;
    for (i = 0; i < TAMANHO_TABULEIRO; i++)
        for (j = 0; j < TAMANHO_TABULEIRO; j++)
            tabuleiro[i][j] = AGUA;
}

int validar_horizontal(int linha, int coluna)
{
    if (linha < 0 || linha >= TAMANHO_TABULEIRO)
        return 0;
    if (coluna < 0 || coluna + TAMANHO_NAVIO - 1 >= TAMANHO_TABULEIRO)
        return 0;
    return 1;
}

int validar_vertical(int linha, int coluna)
{
    if (coluna < 0 || coluna >= TAMANHO_TABULEIRO)
        return 0;
    if (linha < 0 || linha + TAMANHO_NAVIO - 1 >= TAMANHO_TABULEIRO)
        return 0;
    return 1;
}

int verificar_sobreposicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                           int linha, int coluna, int horizontal)
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++)
    {
        if (horizontal)
        {
            if (tabuleiro[linha][coluna + k] == NAVIO)
                return 1;
        }
        else
        {
            if (tabuleiro[linha + k][coluna] == NAVIO)
                return 1;
        }
    }
    return 0;
}

void posicionar_navio_horizontal(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                 int linha, int coluna, int navio[TAMANHO_NAVIO])
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++)
        tabuleiro[linha][coluna + k] = navio[k];
}

void posicionar_navio_vertical(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                               int linha, int coluna, int navio[TAMANHO_NAVIO])
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++)
        tabuleiro[linha + k][coluna] = navio[k];
}

void exibir_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO])
{
    int i, j;

    printf("=== TABULEIRO BATALHA NAVAL ===\n");
    printf("    0 = agua  |  3 = navio  |  5 = efeito\n\n");

    /* Cabeçalho com os índices das colunas */
    printf("   ");
    for (j = 0; j < TAMANHO_TABULEIRO; j++)
        printf("%2d", j);
    printf("\n");

    /* Separador */
    printf("   ");
    for (j = 0; j < TAMANHO_TABULEIRO; j++)
        printf("--");
    printf("\n");

    /* Linhas da matriz com índice à esquerda */
    for (i = 0; i < TAMANHO_TABULEIRO; i++)
    {
        printf("%2d|", i);
        for (j = 0; j < TAMANHO_TABULEIRO; j++)
            printf("%2d", tabuleiro[i][j]);
        printf("\n");
    }

    printf("\n==================================================\n");
}

int validar_diagonal_desc(int linha, int coluna)
{
    if (linha < 0 || linha + TAMANHO_NAVIO - 1 >= TAMANHO_TABULEIRO)
        return 0;
    if (coluna < 0 || coluna + TAMANHO_NAVIO - 1 >= TAMANHO_TABULEIRO)
        return 0;
    return 1;
}

int validar_diagonal_asc(int linha, int coluna)
{
    if (linha - (TAMANHO_NAVIO - 1) < 0 || linha >= TAMANHO_TABULEIRO)
        return 0;
    if (coluna < 0 || coluna + TAMANHO_NAVIO - 1 >= TAMANHO_TABULEIRO)
        return 0;
    return 1;
}

int verificar_sobreposicao_diagonal_desc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                         int linha, int coluna)
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++)
        if (tabuleiro[linha + k][coluna + k] == NAVIO)
            return 1;
    return 0;
}

int verificar_sobreposicao_diagonal_asc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                        int linha, int coluna)
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++)
        if (tabuleiro[linha - k][coluna + k] == NAVIO)
            return 1;
    return 0;
}

void posicionar_navio_diagonal_desc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                    int linha, int coluna, int navio[TAMANHO_NAVIO])
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++)
        tabuleiro[linha + k][coluna + k] = navio[k];
}

void posicionar_navio_diagonal_asc(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                   int linha, int coluna, int navio[TAMANHO_NAVIO])
{
    int k;
    for (k = 0; k < TAMANHO_NAVIO; k++)
        tabuleiro[linha - k][coluna + k] = navio[k];
}

void construir_hab_cone(int hab[TAMANHO_HAB][TAMANHO_HAB])
{
    int i, j;
    int centro = TAMANHO_HAB / 2;

    for (i = 0; i < TAMANHO_HAB; i++)
    {
        for (j = 0; j < TAMANHO_HAB; j++)
        {
            if (i < 3 && abs(j - centro) <= i)
                hab[i][j] = 1;
            else
                hab[i][j] = 0;
        }
    }
}

void construir_hab_cruz(int hab[TAMANHO_HAB][TAMANHO_HAB])
{
    int i, j;
    int centro = TAMANHO_HAB / 2;

    for (i = 0; i < TAMANHO_HAB; i++)
    {
        for (j = 0; j < TAMANHO_HAB; j++)
        {
            if (i == centro || j == centro)
                hab[i][j] = 1;
            else
                hab[i][j] = 0;
        }
    }
}

void construir_hab_octaedro(int hab[TAMANHO_HAB][TAMANHO_HAB])
{
    int i, j;
    int centro = TAMANHO_HAB / 2;
    int raio = TAMANHO_HAB / 2;

    for (i = 0; i < TAMANHO_HAB; i++)
    {
        for (j = 0; j < TAMANHO_HAB; j++)
        {
            if (abs(i - centro) + abs(j - centro) <= raio)
                hab[i][j] = 1;
            else
                hab[i][j] = 0;
        }
    }
}

void aplicar_habilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                        int hab[TAMANHO_HAB][TAMANHO_HAB],
                        int origem_linha, int origem_coluna)
{
    int i, j;
    int raio = TAMANHO_HAB / 2;
    int linha_tab, coluna_tab;

    for (i = 0; i < TAMANHO_HAB; i++)
    {
        for (j = 0; j < TAMANHO_HAB; j++)
        {
            if (hab[i][j] == 1)
            {
                linha_tab = origem_linha + (i - raio);
                coluna_tab = origem_coluna + (j - raio);

                if (linha_tab >= 0 && linha_tab < TAMANHO_TABULEIRO &&
                    coluna_tab >= 0 && coluna_tab < TAMANHO_TABULEIRO)
                {

                    if (tabuleiro[linha_tab][coluna_tab] != NAVIO)
                        tabuleiro[linha_tab][coluna_tab] = EFEITO;
                }
            }
        }
    }