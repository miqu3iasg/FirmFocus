#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define CAPACIDADE_MAXIMA 100

struct Produto {
    char nome[50];
    int quantidade;
    float preco;
};

struct Produto estoque[CAPACIDADE_MAXIMA]; 
int totalProdutos = 0; 

struct Empregado {
    char nome[50];
    float salario;
    int idade;
    char email[50];
};

struct Empregado empregados[100]; 
int totalEmpregados = 0; 

long int tamanhoProduto = sizeof(struct Produto);
long int tamanhoEmpregado = sizeof(struct Empregado);
COORD cord = { 0, 0 };
FILE *fpProduto, *fpEmpregado, *ftProduto, *ftEmpregado;

void gotoxy(int x, int y) {
    cord.X = x;
    cord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
}

void adicionarProduto() {
  system("cls");
  fseek(fpProduto, 0, SEEK_END);
  char outro = 's';

  while (outro == 's' || outro == 'S') {
    struct Produto novoProduto;
    printf("\nDigite o Nome do Produto: ");
    scanf("%49s", novoProduto.nome);

    printf("Digite a Quantidade em Estoque: ");
    scanf("%d", &novoProduto.quantidade);

    if(novoProduto.quantidade > CAPACIDADE_MAXIMA) {
      printf("\nQuantidade maxima foi atingida!\n");
      exit(0);
    }

    printf("Digite o Preço Unitário: ");
    scanf("%f", &novoProduto.preco);

    fwrite(&novoProduto, tamanhoProduto, 1, fpProduto);

    printf("\nDeseja adicionar outro produto (S/N): ");
    fflush(stdin);
    scanf(" %c", &outro);
  }
}

void exibirEstoque() {
    system("cls");

    rewind(fpProduto);

    printf("\n==================================="
           "=========================================================================="
           "===");
    printf("\nNOME\t\tQUANTIDADE\t\tPREÇO\t\t"
           "VALOR TOTAL\t\tID");
    printf("\n==================================="
           "=========================================================================="
           "===");
  
    struct Produto produto;
    float totalValorEstoque = 0.0; 

    while (fread(&produto, tamanhoProduto, 1, fpProduto) == 1) {
        float valorTotalItem = produto.quantidade * produto.preco;
        totalValorEstoque += valorTotalItem;
        printf("\n%s\t\t%d\t\t%.2f\t\t%.2f\t\t%d",
               produto.nome, 
               produto.quantidade, 
               produto.preco, 
               valorTotalItem, 
               totalProdutos++);
    }

    printf("\n\nValor Total do Estoque: %.2f\n\n", totalValorEstoque);

    printf("\n\n\t");
    system("pause");
}


void atualizarQuantidade() {
    system("cls");
    char nomeProduto[50];
    char outro = 's';

    while (outro == 's' || outro == 'S') {
        printf("\nDigite o nome do produto para atualizar a quantidade: ");
        scanf("%49s", nomeProduto);

        rewind(fpProduto);

        while (fread(&estoque[totalProdutos], tamanhoProduto, 1, fpProduto) == 1) {
            if (strcmp(estoque[totalProdutos].nome, nomeProduto) == 0) {
                printf("Digite a nova quantidade em estoque: ");
                scanf("%d", &estoque[totalProdutos].quantidade);

                fseek(fpProduto, -tamanhoProduto, SEEK_CUR);
                fwrite(&estoque[totalProdutos], tamanhoProduto, 1, fpProduto);
                break;
            }
        }

        printf("\nDeseja atualizar outra quantidade (S/N): ");
        fflush(stdin);
        scanf(" %c", &outro);
    }
}

void adicionarRegistroEmpregado() {
    system("cls");
    fseek(fpEmpregado, 0, SEEK_END);
    char outro = 's';

    while (outro == 's' || outro == 'S') {
        struct Empregado novoEmpregado;
        printf("\nDigite o Nome do Empregado: ");
        scanf("%49s", novoEmpregado.nome);

        printf("Digite a Idade do Empregado: ");
        scanf("%d", &novoEmpregado.idade);

        printf("Digite o Salário do Empregado: ");
        scanf("%f", &novoEmpregado.salario);

        printf("\nDigite o email do empregado: \n");
        scanf("%d", &novoEmpregado.email);

        fwrite(&novoEmpregado, tamanhoEmpregado, 1, fpEmpregado);

        printf("\nDeseja adicionar outro registro de empregado (S/N): ");
        fflush(stdin);
        scanf(" %c", &outro);
    }
}

void deletarRegistroEmpregado() {
    system("cls");
    char nomeEmpregado[50];
    char outro = 's';

    while (outro == 's' || outro == 'S') {
        printf("\nDigite o nome do empregado a ser excluido: ");
        scanf("%49s", nomeEmpregado);

        ftEmpregado = fopen("tempEmpregado.txt", "wb");
        rewind(fpEmpregado);

        struct Empregado empregado;
        while (fread(&empregado, tamanhoEmpregado, 1, fpEmpregado) == 1) {
            if (strcmp(empregado.nome, nomeEmpregado) != 0)
                fwrite(&empregado, tamanhoEmpregado, 1, ftEmpregado);
        }

        fclose(fpEmpregado);
        fclose(ftEmpregado);
        remove("dataEmpregado.txt");
        rename("tempEmpregado.txt", "dataEmpregado.txt");
        fpEmpregado = fopen("dataEmpregado.txt", "rb+");

        printf("\nDeseja excluir outro registro de empregado (S/N): ");
        fflush(stdin);
        scanf(" %c", &outro);
    }
}

float calcularTotalGastosSalarios() {
    rewind(fpEmpregado);
    float totalGastos = 0.0;

    struct Empregado empregado;
    while (fread(&empregado, tamanhoEmpregado, 1, fpEmpregado) == 1) {
        totalGastos += empregado.salario;
    }

    return totalGastos;
}

void exibirRegistrosEmpregado() {
    system("cls");

    rewind(fpEmpregado);

    printf("\n==================================="
           "=============================="
           "===");
    printf("\nNOME\t\tIDADE\t\tSALÁRIO\t\t"
           "ID");
    printf("\n==================================="
           "=============================="
           "===");
  
    struct Empregado empregado;
    while (fread(&empregado, tamanhoEmpregado, 1, fpEmpregado) == 1)
        printf("\n%s\t\t%d\t\t%.2f\t\t%d",
               empregado.nome, 
               empregado.idade, 
               empregado.salario, 
               empregado.email
              );

    printf("\n\n");
    
    float totalGastosSalarios = calcularTotalGastosSalarios();
    printf("\nTotal de Gastos com Salários: %.2f\n\n", totalGastosSalarios);

    printf("\n\n\t");
    system("pause");
}


void modificarRegistroEmpregado() {
    system("cls");
    char nomeEmpregado[50];
    char outro = 's';

    while (outro == 's' || outro == 'S') {
        printf("\nDigite o nome do empregado a ser modificado: ");
        scanf("%49s", nomeEmpregado);

        rewind(fpEmpregado);

        while (fread(&empregados[totalEmpregados], tamanhoEmpregado, 1, fpEmpregado) == 1) {
            if (strcmp(empregados[totalEmpregados].nome, nomeEmpregado) == 0) {
                printf("Digite o novo nome do empregado: ");
                scanf("%49s", empregados[totalEmpregados].nome);
                printf("Digite a nova idade do empregado: ");
                scanf("%d", &empregados[totalEmpregados].idade);
                printf("Digite o novo salário do empregado: ");
                scanf("%f", &empregados[totalEmpregados].salario);
                printf("Digite o novo email do empregado: ");
                scanf("%d", &empregados[totalEmpregados].email);

                fseek(fpEmpregado, -tamanhoEmpregado, SEEK_CUR);
                fwrite(&empregados[totalEmpregados], tamanhoEmpregado, 1, fpEmpregado);
                break;
            }
        }

        printf("\nDeseja modificar outro registro de empregado (S/N): ");
        fflush(stdin);
        scanf(" %c", &outro);
    }
}

float calcularLucroMensal() {
    float gastosEmpregados = 0.0;
    float gastosProdutos = 0.0;
    float ganhosVendas = 0.0;

    return ganhosVendas - (gastosEmpregados + gastosProdutos);
}

void calcularLucroAnual() {
    float lucroAnual = 0.0;
    int mes;

    for (mes = 1; mes <= 12; mes++) {
        float lucroMensal = calcularLucroMensal();
        lucroAnual += lucroMensal;

        printf("\nMês %d - Lucro Mensal: %.2f", mes, lucroMensal);
    }

    printf("\n\nLucro Anual Estimado: %.2f\n\n", lucroAnual);
}

int main() {
    int escolha;

    fpProduto = fopen("dataProduto.txt", "rb+");

    if (fpProduto == NULL) {
        fpProduto = fopen("dataProduto.txt", "wb+");
        if (fpProduto == NULL) {
            printf("\nNão foi possível abrir o arquivo de produtos.");
            exit(1);
        }
    }

    fpEmpregado = fopen("dataEmpregado.txt", "rb+");

    if (fpEmpregado == NULL) {
        fpEmpregado = fopen("dataEmpregado.txt", "wb+");
        if (fpEmpregado == NULL) {
            printf("\nNão foi possível abrir o arquivo de empregados...");
            exit(1);
        }
    }

    printf("\n\n\n\n\t\t\t\t====================="
           "==============================\n");
    printf("\t\t\t\t[|:::>:::>:::>::>  "
           "SISTEMA DE CONTROLE  <::<:::<:::"
           "<:::|]\t");
    printf("\n\t\t\t\t====================="
           "==============================\n");
    
    system("pause");

    while (1) {
        system("cls");
        gotoxy(30, 10);
        printf("\n1. ADICIONAR REGISTRO DE EMPREGADO\n");
        gotoxy(30, 12);
        printf("\n2. EXCLUIR REGISTRO DE EMPREGADO\n");
        gotoxy(30, 14);
        printf("\n3. EXIBIR REGISTROS DE EMPREGADO\n");
        gotoxy(30, 16);
        printf("\n4. MODIFICAR REGISTRO DE EMPREGADO\n");
        gotoxy(30, 18);
        printf("\n5. ADICIONAR PRODUTO\n");
        gotoxy(30, 20);
        printf("\n6. EXIBIR ESTOQUE\n");
        gotoxy(30, 22);
        printf("\n7. ATUALIZAR QUANTIDADE DE PRODUTO\n");
        gotoxy(30, 24);
        printf("\n8. CALCULAR LUCRO ANUAL\n"); 
        gotoxy(30, 26);
        printf("\n9. SAIR\n"); 
        gotoxy(30, 28);
        printf("\nDIGITE SUA ESCOLHA...\n");
        fflush(stdin);
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                adicionarRegistroEmpregado();
                break;

            case 2:
                deletarRegistroEmpregado();
                break;

            case 3:
                exibirRegistrosEmpregado();
                break;

            case 4:
                modificarRegistroEmpregado();
                break;

            case 5:
                adicionarProduto();
                break;

            case 6:
                exibirEstoque();
                break;

            case 7:
                atualizarQuantidade();
                break;

            case 8:
                calcularLucroAnual(); 
                break;

            case 9:
                fclose(fpEmpregado);
                fclose(fpProduto);
                exit(0);
                break;

            default:
                printf("\nESCOLHA INVÁLIDA.\n");
        }
    }

    return 0;
}

