
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERROR_FILE_MSG "Nao foi possivel abrir o arquivo!\n"
#define INVALID_OPTION "Opcao invalida! Tente novamente!"

enum boolean { FALSO = 0, VERDADEIRO = 1 };

// Prototipos
FILE *carregar(char quadro[9][9]);
FILE *carregue_continue_jogo(char quadro[9][9], char *nome_arquivo);
void carregue_novo_jogo(char quadro[9][9], char *nome_arquivo);
int determinar_quadrante(int x, int y);
FILE *crie_arquivo_binario(char quadro[9][9]);
int e_valido(const char quadro[9][9], int x, int y, int valor);
int e_valido_na_coluna(const char quadro[9][9], int y, int valor);
int e_valido_no_quadro3x3(const char quadro[9][9], int x, int y, int valor);
int e_valido_na_linha(const char quadro[9][9], int x, int valor);
int existe_campo_vazio(const char quadro[9][9]);
void imprime(const char quadro[9][9]);
void jogar();
void resolver(FILE *, char quadro[9][9]);
void resolver_um_passo(char quadro[9][9]);
void salvar_jogada_bin(FILE *fb, char quadro[9][9]);
int resolver_backtracking(char quadro[9][9], int x, int y);
int jogada_eh_boa(const char quadro[9][9], int x, int y, int valor);

/* Funcoes auxiliares */
int fim_x(int quadr);
int fim_y(int quadr);
int le_opcao();
void gen_random(char *s, const int len);
int ini_x(int quadr);
int ini_y(int quadr);
void menu();
void menu_arquivo();

/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * MAIN
 * /////////////////////////////////////////////////////////////////////////////
 */
int main()
{

    // inicia o jogo
    jogar();

    return 0;
}

/* -----------------------------------------------------------------------------
 * CARREGAR
 * Inicializa o SUDOKU a partir de um novo jogo ou estado de jogo anterior
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE *carregar(char quadro[9][9])
{
    char nome_arquivo_bin[11];
    char nome_arquivo[11];
    FILE* fb = NULL;

    int opcao;

    menu_arquivo();
    opcao = le_opcao();

    // TODO Função incompleta

    switch (opcao)
    {

    // carregar novo sudoku
    case 1:
        // inserindo nome do arquivo TEXTO
        printf("Insira o nome do arquivo que deseja carregar o novo jogo (.txt) ");
        scanf("%s", nome_arquivo);

        carregue_novo_jogo(quadro, nome_arquivo);

        // fechando

        break;

    // continuar jogo
    case 2:
        // inserindo nome do arquivo BIN
        printf("Insira o nome do arquivo que deseja carregar o jogo do arquivo "
               "(.dat) ");
        scanf("%s", nome_arquivo_bin);

        return carregue_continue_jogo(quadro, nome_arquivo_bin);
        // nao fecha pra continuar salvando sem fechar

        break;

    // retornar ao menu anterior
    case 9:

        break;

    default:
        puts(INVALID_OPTION);
        break;
    }
    return fb;
}

/* -----------------------------------------------------------------------------
 * CARREGAR CONTINUAR JOGO
 * Carrega um estado de jogo a partir de um arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE *carregue_continue_jogo(char quadro[9][9], char *nome_arquivo)
{

    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (arquivo == NULL)
    {
        printf(ERROR_FILE_MSG);
    }
    fseek(arquivo, sizeof(int), SEEK_SET);
    fread(quadro, sizeof(char), 81, arquivo);

    return arquivo;

    // TODO
}

/* -----------------------------------------------------------------------------
 * CARREGAR NOVO JOGO
 * Carrega um novo jogo do Sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void carregue_novo_jogo(char quadro[9][9], char *nome_arquivo)
{

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        printf(ERROR_FILE_MSG);
        return;
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (fscanf(arquivo, "%d", &quadro[i][j]) != 1)
            {
                printf(ERROR_FILE_MSG);

                return;
            }
        }
    }

    fclose(arquivo);
    printf("Novo jogo carregado\n  ");

    // TODO
}

/* -----------------------------------------------------------------------------
 * CRIAR ARQUIVO BINARIO
 * Criar arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE *crie_arquivo_binario(char quadro[9][9])
{
    char nome_arquivo_bin[11];
    int total_jogadas = 0;

    printf("Insira o nome do arquivo que deseja criar arquivo de salvamento "
           "(.dat) ");
    scanf("%s", nome_arquivo_bin);

    FILE *fp = fopen(nome_arquivo_bin, "wb+");
    if (fp == NULL)
    {
        printf(ERROR_FILE_MSG);
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    fwrite(&total_jogadas, sizeof(int), 1, fp);
    fseek(fp, sizeof(int), SEEK_SET);
    fwrite(quadro, sizeof(char), 81, fp);

    return fp;
}

// TODO

/* -----------------------------------------------------------------------------
 * DETERMINAR QUADRANTE
 * Dado as posicoes x e y, determina o quadrante do quadro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int determinar_quadrante(int x, int y)
{
    if (x < 3 && y < 3)
        return 1;
    else if (x < 3 && y < 6)
        return 2;
    else if (x < 3 && y < 9)
        return 3;
    else if (x < 6 && y < 3)
        return 4;
    else if (x < 6 && y < 6)
        return 5;
    else if (x < 6 && y < 9)
        return 6;
    else if (x < 9 && y < 3)
        return 7;
    else if (x < 9 && y < 6)
        return 8;
    else
        return 9;
}

/* -----------------------------------------------------------------------------
 * E VALIDO
 * Determina se um valor na posicao x e y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido(const char quadro[9][9], int x, int y, int valor)
{

    // verifica as tres condicoes
    if (!e_valido_na_coluna(quadro, y, valor))
        return FALSO;
    if (!e_valido_na_linha(quadro, x, valor))
        return FALSO;
    if (!e_valido_no_quadro3x3(quadro, x, y, valor))
        return FALSO;

    return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA COLUNA
 * Verifica se um valor na coluna y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_na_coluna(const char quadro[9][9], int y, int valor)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if (quadro[i][y] == valor)
        {
            return FALSO;
        }
    }
    return VERDADEIRO;
    // TODO
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA LINHA
 * Verifica se um valor na linha x e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_na_linha(const char quadro[9][9], int x, int valor)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if (quadro[x][i] == valor)
        {
            return FALSO;
        }
    }
    return VERDADEIRO;

    // TODO
}

/* -----------------------------------------------------------------------------
 * E VALIDO NO QUADRO 3X3
 * Verifica se um valor e valido no quadrante da posicao x, y
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_no_quadro3x3(const char quadro[9][9], int x, int y, int valor)
{
    int quadrante = determinar_quadrante(x, y);
    int i, j;
    for (i = ini_x(quadrante); i <= fim_x(quadrante); i++)
    {
        for (j = ini_y(quadrante); j <= fim_y(quadrante); j++)
        {
            if (quadro[i][j] == valor)
            {
                return FALSO;
            }
        }
    }
    return VERDADEIRO;
}

// TODO

/* -----------------------------------------------------------------------------
 * EXISTE CAMPO VAZIO
 * Verifica se existe um campo nao preenchido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int existe_campo_vazio(const char quadro[9][9])
{
    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (quadro[i][j] == 0)
                return VERDADEIRO;
        }
    }

    return FALSO;
}

/* -----------------------------------------------------------------------------
 * IMPRIMIR
 * Imprime o quadro inteiro do sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void imprime(const char quadro[9][9])
{
    int i, j;

    //	puts("~~~~~~~~ SUDOKU ~~~~~~~~");
    puts("    0 1 2   3 4 5   6 7 8");
    for (i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
            puts("  -------------------------");
        for (j = 0; j < 9; j++)
        {

            if (j == 0)
                printf("%d | ", i);
            else if (j % 3 == 0)
                printf("| ");

            if (quadro[i][j] == 0)
                printf("- ");
            else
                printf("%d ", quadro[i][j]);
        }
        puts("|");
    }
    puts("  -------------------------");
}

/* -----------------------------------------------------------------------------
 * JOGAR
 * Realiza toda a logica do jogo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void jogar()
{
    int opcao;
    char quadro[9][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    FILE *fb = NULL;

    opcao = 0;

    while (opcao != 9)
    {
        // imprime na tela o quadro atual
        imprime(quadro);

        // apresenta um menu com as opcoes
        menu();
        opcao = le_opcao();

        switch (opcao)
        {

        // carregar sudoku
        case 1:
            fb = carregar(quadro);
            if(fb ==NULL)
            {
                fb = crie_arquivo_binario(quadro);
            }

            break;

        // preencher quadro com um valor
        case 2:
        {
            int x, y, valor;

            printf("Entre com a posicao e o valor (linha, coluna, valor): ");
            scanf("%d %d %d", &x, &y, &valor);

            if (e_valido(quadro, x, y, valor) &&
                    jogada_eh_boa(quadro, x, y, valor == VERDADEIRO))
            {
                quadro[x][y] = valor;
            }
            else
            {
                printf("jogada errada ou atrapalha depois");
            }
        }

        break;

        // resolver 1 passo
        case 3:
            resolver_um_passo(quadro);
            salvar_jogada_bin(fb, quadro);
            puts("Um passo resolvido!");

            break;

        // resolver o sudoku
        case 4:
            resolver(fb, quadro);
            break;

        case 9:

            puts("Programa finalizado ..");

            fclose(fb);
            break;

        default:
            puts(INVALID_OPTION);
            break;
        }
    }
}

/* -----------------------------------------------------------------------------
 * RESOLVER
 * Resolve o sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolver(FILE *fb, char quadro[9][9])
{

    resolver_backtracking(quadro, 0, 0);
    salvar_jogada_bin(fb, quadro);
}

/* -----------------------------------------------------------------------------
 * RESOLVER UM PASSO
 * Preenche apenas um campo vazio
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolver_um_passo(char quadro[9][9])
{
    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (quadro[i][j] == 0)
            {
                for (int valor = 1; valor <= 9; valor++)
                {
                    if (e_valido(quadro, i, j, valor))
                    {
                        // replica
                        char quadro_teste[9][9];
                        for (int x = 0; x < 9; x++)
                        {
                            for (int y = 0; y < 9; y++)
                            {
                                quadro_teste[x][y] = quadro[x][y];
                            }
                        }

                        // preenche
                        quadro_teste[i][j] = valor;

                        // Verifica se o preenchimento ta certo
                        if (resolver_backtracking(quadro_teste, 0, 0))
                        {

                            quadro[i][j] = valor;
                            return VERDADEIRO;
                        }
                    }
                }

                return FALSO;
            }
        }
    }
    //
    return FALSO;

    // TODO
}

/* -----------------------------------------------------------------------------
 * SALVAR JOGADA BINARIO
 * Salva o estado atual do quadro no arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void salvar_jogada_bin(FILE *fb, char quadro[9][9])
{
    int num_jogadas;

    if (fb == NULL)
    {
        printf(ERROR_FILE_MSG);
        return;
    }
    fseek(fb, 0, SEEK_SET);
    fread(&num_jogadas, sizeof(int), 1, fb);
    num_jogadas++;
    printf("Em %d jogadas\n", num_jogadas);
    fseek(fb, 0, SEEK_SET);
    fwrite(&num_jogadas, sizeof(int), 1, fb);
    fseek(fb, sizeof(int), SEEK_SET);
    fwrite(quadro, sizeof(char), 81, fb);

    // TODO
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 							FUNCOES AUXILIARES
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -----------------------------------------------------------------------------
 * FIM X
 * Indice final da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_x(int quadr)
{
    switch (quadr)
    {
    case 1:
    case 2:
    case 3:
        return 2;

    case 4:
    case 5:
    case 6:
        return 5;

    default:
        return 8;
    }
}

/* -----------------------------------------------------------------------------
 * FIM Y
 * Indice final da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_y(int quadr)
{
    switch (quadr)
    {
    case 1:
    case 4:
    case 7:
        return 2;

    case 2:
    case 5:
    case 8:
        return 5;

    default:
        return 8;
    }
}

/* -----------------------------------------------------------------------------
 * GEN_RANDOM
 * Gera uma cadeira de caracteres randomica de tamanho len
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void gen_random(char *s, const int len)
{
    srand(time(NULL));
    static const char alphanum[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";
    int i;

    for (i = 0; i < len; ++i)
    {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

/* -----------------------------------------------------------------------------
 * INI X
 * Indice inicial da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_x(int quadr)
{
    switch (quadr)
    {
    case 1:
    case 2:
    case 3:
        return 0;

    case 4:
    case 5:
    case 6:
        return 3;

    default:
        return 6;
    }
}

/* -----------------------------------------------------------------------------
 * INI Y
 * Indice inicial da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_y(int quadr)
{
    switch (quadr)
    {
    case 1:
    case 4:
    case 7:
        return 0;

    case 2:
    case 5:
    case 8:
        return 3;

    default:
        return 6;
    }
}

/* -----------------------------------------------------------------------------
 * LE OPCAO
 * Imprime a mensagem a faz a leitura da opcao
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int le_opcao()
{
    int opcao;

    printf("Opcao: ");
    scanf("%d", &opcao);

    return opcao;
}

/* -----------------------------------------------------------------------------
 * MENU
 * Imprime o menu de opcoes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu()
{
    puts("\n~~~~~~~~ SUDOKU ~~~~~~~~");
    puts("[1] - Carregar jogo");
    puts("[2] - Jogar");
    puts("[3] - Resolver um passo");
    puts("[4] - Resolver");
    puts("[9] - Finalizar");
    puts("--------");
}

/* -----------------------------------------------------------------------------
 * MENU ARQUIVO
 * Imprime o menu de opcoes do arquivo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu_arquivo()
{
    puts("\n~~~~~ MENU ARQUIVO ~~~~~");
    puts("[1] - Novo jogo");
    puts("[2] - Continuar jogo");
    puts("[9] - Retornar ao menu anterior");
    puts("--------");
}

int resolver_backtracking(char quadro[9][9], int x, int y)
{

    if (x == 9)
    {
        x = 0;
        y++;
        if (y == 9)
        {
            return VERDADEIRO;
        }
    }

    if (quadro[x][y] != 0)
    {
        return resolver_backtracking(quadro, x + 1, y);
    }

    for (int valor = 1; valor <= 9; valor++)
    {
        if (e_valido(quadro, x, y, valor))
        {
            quadro[x][y] = valor;

            if (resolver_backtracking(quadro, x + 1, y))
            {
                return VERDADEIRO;
            }

            quadro[x][y] = 0;
        }
    }

    return FALSO;
}
int jogada_eh_boa(const char quadro[9][9], int x, int y, int valor)
{
    // cria um quadro teste
    char teste_quadro[9][9];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            teste_quadro[i][j] = quadro[i][j];
        }
    }

    if (!e_valido(teste_quadro, x, y, valor))
    {
        return VERDADEIRO;
    }

    teste_quadro[x][y] = valor;

    // tenta resolver
    if (resolver_backtracking(teste_quadro, x, y) == 0)
    {
        return FALSO; // A jogada não levou a progresso
    }

    return VERDADEIRO; // A jogada é boa
}
