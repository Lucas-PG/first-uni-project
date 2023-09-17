#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Números de produtos, vendedores e vendas, respectivamente.
#define PRODUCTS_NUM 10
#define SELLERS_NUM 5
#define SALES_NUM 20


//Cria protótipos de objetos para utilizar mais tarde
typedef struct
{
  char code[3];
  float price;
  char description[50];
} Product;

typedef struct
{
  char code[3];
  char name[10];
} Seller;

typedef struct
{
  char seller_code[3];
  char product_code[3];
  int units_sold;
} Sale;

typedef struct
{
  char product_code[3];
  char description[50];
  float price;
  float total_sold;
} Per_Product;

typedef struct
{
  char seller_code[3];
  char seller_description[50];
  float total_sold;
} Per_Seller;

// cria objeto para produtos, a partir dos dados do arquivo de texto
void create_products(Product given_products[PRODUCTS_NUM], char file_name[])
{
  FILE *products_file = fopen(file_name, "r"); //abre o arquivo de texto
  char buffer_products[255]; //cria string vazia
  int products_index = 0; //indica o indíce do objeto

  while (fgets(buffer_products, 255, products_file) != NULL) //enquanto não houver uma linha vazia
  {
    char *current_code = strtok(buffer_products, " "); //pega o código
    char *current_price_str = strtok(NULL, " "); //pega o preço
    char *current_description = strtok(NULL, "\n"); //pega a descrição

    strncpy(given_products[products_index].code, current_code, sizeof(given_products[products_index].code)); //adiciona o código ao objeto, no indice "product_index"
    strncpy(given_products[products_index].description, current_description, sizeof(given_products[products_index].description)); //adiciona descrição
    given_products[products_index].price = atof(current_price_str); //adiciona preço
    products_index++;
  };
}

// cria objeto para vendedores, a partir dos dados do arquivo de texto
void create_sellers(Seller given_sellers[SELLERS_NUM], char file_name[255])
{
  FILE *sellers_file = fopen(file_name, "r"); //identico ao código de create_products
  char buffer_sellers[255];                   //mas para os dados de vendedores
  int sellers_index = 0;

  while (fgets(buffer_sellers, 255, sellers_file) != NULL)
  {
    char *current_code = strtok(buffer_sellers, " ");
    char *current_name = strtok(NULL, " ");

    strncpy(given_sellers[sellers_index].code, current_code, sizeof(given_sellers[sellers_index].code));
    strncpy(given_sellers[sellers_index].name, current_name, sizeof(given_sellers[sellers_index].name));
    sellers_index++;
  };
}

// cria objeto para vendas
void create_sales(Sale given_sales[SALES_NUM], char file_name[255])
{
  FILE *sales_file = fopen(file_name, "r"); //idem
  char buffer_sales[255];
  int sales_index = 0;

  while (fgets(buffer_sales, 255, sales_file) != NULL)
  {
    char *current_seller_code = strtok(buffer_sales, " ");
    char *current_product_code = strtok(NULL, " ");
    char *current_units_sold = strtok(NULL, " ");

    strncpy(given_sales[sales_index].seller_code, current_seller_code, sizeof(given_sales[sales_index].seller_code));
    strncpy(given_sales[sales_index].product_code, current_product_code, sizeof(given_sales[sales_index].product_code));
    given_sales[sales_index].units_sold = atof(current_units_sold);
    sales_index++;
  };
}

//calcula o total vendido em todas as vendas, a partir de sales e products
float total_sold(Sale sales[SALES_NUM], Product products[PRODUCTS_NUM])
{
  float total = 0;
  for (int i = 0; i < SALES_NUM; i++) 
  {
    for (int j = 0; j < PRODUCTS_NUM; j++)
    {
      if (strcmp(sales[i].product_code, products[j].code) == 0) //acha o produto vendido na venda
      {
        total += (float)sales[i].units_sold * products[j].price; //adiciona o total da venda
      }
    }
  }
  return total;
}

//calcula o total vendido por produto
void sales_per_product(Sale sales[SALES_NUM], Product products[PRODUCTS_NUM], Per_Product per_product[PRODUCTS_NUM])
{
  //adiciona ao objeto per_product o código, descrição, preço e inicia o total como 0.
  for (int i = 0; i < PRODUCTS_NUM; i++) 
  {                                      
    strncpy(per_product[i].product_code, products[i].code, sizeof(per_product[i].product_code));
    strncpy(per_product[i].description, products[i].description, sizeof(per_product[i].description));
    per_product[i].price = products[i].price;
    per_product[i].total_sold = 0;
  }

  //acha o produto vendido na venda em questão e adiciona o valor ao total.
  for (int i = 0; i < SALES_NUM; i++)
  {
    for (int j = 0; j < PRODUCTS_NUM; j++)
    {
      if (strcmp(sales[i].product_code, per_product[j].product_code) == 0)
      {
        per_product[j].total_sold += (sales[i].units_sold * products[j].price);
      }
    }
  }
}

//identico ao sales_per_product, mas para vendedores
void sales_per_seller(Sale sales[SALES_NUM], Seller sellers[SELLERS_NUM], Per_Seller per_seller[SELLERS_NUM], Product products[PRODUCTS_NUM])
{

  //adiciona código, descrição e inicia o total como 0.
  for (int i = 0; i < SELLERS_NUM; i++) 
  {
    strncpy(per_seller[i].seller_code, sellers[i].code, sizeof(per_seller[i].seller_code));
    strncpy(per_seller[i].seller_description, sellers[i].name, sizeof(per_seller[i].seller_description));
    per_seller[i].total_sold = 0;
  }

  //acha o vendedor e o produto, calcula o total da compra e adiciona ao total vendido pelo vendedor. 
  for (int i = 0; i < SALES_NUM; i++)
  {
    for (int j = 0; j < SELLERS_NUM; j++)
    {
      if (strcmp(sales[i].seller_code, per_seller[j].seller_code) == 0)
      {
        for (int k = 0; k < PRODUCTS_NUM; k++)
        {
          if (strcmp(sales[i].product_code, products[k].code) == 0)
          {
            per_seller[j].total_sold += (sales[i].units_sold * products[k].price);
          }
        }
      }
    }
  }
}

//função para dar print nos dados desejados
void print_output_file(Sale sales[SALES_NUM], Product products[PRODUCTS_NUM], Seller sellers[SELLERS_NUM], char file_name[255], float total, Per_Product per_product[PRODUCTS_NUM], Per_Seller per_seller[SELLERS_NUM])
{
  FILE *output_file = fopen(file_name, "w");

  fprintf(output_file, "Log de Vendas:\n");
  for (int i = 0; i < PRODUCTS_NUM; i++)
  {
    fprintf(output_file, "[%d] %s %s %d\n", i, sales[i].seller_code, sales[i].product_code, sales[i].units_sold);
  }

  fprintf(output_file, "\n\nCatalogo de Produtos:\n");
  for (int i = 0; i < PRODUCTS_NUM; i++)
  {
    fprintf(output_file, "[%d] %s %.2f %s\n", i, products[i].code, products[i].price, products[i].description);
  }

  fprintf(output_file, "\n\nLista de Vendedores:\n");
  for (int i = 0; i < SELLERS_NUM; i++)
  {
    fprintf(output_file, "[%d] %s %s\n", i, sellers[i].code, sellers[i].name);
  }

  fprintf(output_file, "\n\nTotal geral vendido: R$%.2f", total);

  fprintf(output_file, "\n\nTotal de vendas de cada produto:\n");
  for(int i = 0; i < PRODUCTS_NUM; i++){
    fprintf(output_file, "Produto %s (%s): R$%.2f\n", per_product[i].product_code, per_product[i].description, per_product[i].total_sold);
  }

  fprintf(output_file, "\n\nTotal de vendas de cada vendedor:\n");
  for(int i = 0; i < SELLERS_NUM; i++){
    fprintf(output_file, "Vendedor %s (%s): R$%.2f\n", per_seller[i].seller_code, per_seller[i].seller_description, per_seller[i].total_sold);
  }
}

int main()
{
  //cria os objetos com os dados e chama as funções desejadas
  Product cars[PRODUCTS_NUM];
  create_products(cars, "produtos.txt");

  Seller sellers[SELLERS_NUM];
  create_sellers(sellers, "vendedores.txt");

  Sale sales[SALES_NUM];
  create_sales(sales, "vendas.txt");

  float total_sales = total_sold(sales, cars);

  Per_Product per_product[PRODUCTS_NUM];
  sales_per_product(sales, cars, per_product);

  Per_Seller per_seller[SELLERS_NUM];
  sales_per_seller(sales, sellers, per_seller, cars);

  print_output_file(sales, cars, sellers, "totais.txt", total_sales, per_product, per_seller);

  return 0;
}