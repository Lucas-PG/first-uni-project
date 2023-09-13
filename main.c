#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PRODUCTS_NUM 10
#define SELLERS_NUM 5
#define SALES_NUM 20

typedef struct
{
  char code[3];
  long long int price;
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
  long long int price;
  long long int total_sold;
} Per_Product;

typedef struct
{
  char seller_code[3];
  long long int total_sold;
} Per_Seller;

// cria objeto para produtos
void create_products(Product given_products[PRODUCTS_NUM], char file_name[])
{
  FILE *products_file = fopen(file_name, "r");
  char buffer_products[255];
  int products_index = 0;

  while (fgets(buffer_products, 255, products_file) != NULL)
  {
    char *current_code = strtok(buffer_products, " ");
    char *current_price_str = strtok(NULL, " ");
    char *current_description = strtok(NULL, "\n");

    strncpy(given_products[products_index].code, current_code, sizeof(given_products[products_index].code));
    strncpy(given_products[products_index].description, current_description, sizeof(given_products[products_index].description));
    given_products[products_index].price = atoi(current_price_str);
    products_index++;
  };
}

// cria objeto para vendedores
void create_sellers(Seller given_sellers[SELLERS_NUM], char file_name[255])
{
  FILE *sellers_file = fopen(file_name, "r");
  char buffer_sellers[255];
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
  FILE *sales_file = fopen(file_name, "r");
  char buffer_sales[255];
  int sales_index = 0;

  while (fgets(buffer_sales, 255, sales_file) != NULL)
  {
    char *current_seller_code = strtok(buffer_sales, " ");
    char *current_product_code = strtok(NULL, " ");
    char *current_units_sold = strtok(NULL, " ");

    strncpy(given_sales[sales_index].seller_code, current_seller_code, sizeof(given_sales[sales_index].seller_code));
    strncpy(given_sales[sales_index].product_code, current_product_code, sizeof(given_sales[sales_index].product_code));
    given_sales[sales_index].units_sold = atoi(current_units_sold);
    sales_index++;
  };
}

long long int total_sold(Sale sales[SALES_NUM], Product products[PRODUCTS_NUM])
{
  long long int total = 0;
  for (int i = 0; i < SALES_NUM; i++)
  {
    for (int j = 0; j < PRODUCTS_NUM; j++)
    {
      if (strcmp(sales[i].product_code, products[j].code) == 0)
      {
        total += (long long int)sales[i].units_sold * products[j].price;
      }
    }
  }
  return total;
}

void sales_per_product(Sale sales[SALES_NUM], Product products[PRODUCTS_NUM], Per_Product per_product[PRODUCTS_NUM])
{

  for (int i = 0; i < PRODUCTS_NUM; i++)
  {
    strncpy(per_product[i].product_code, products[i].code, sizeof(per_product[i].product_code));
    per_product[i].price = products[i].price;
    per_product[i].total_sold = 0;
  }

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

void sales_per_seller(Sale sales[SALES_NUM], Seller sellers[SELLERS_NUM], Per_Seller per_seller[SELLERS_NUM], Product products[PRODUCTS_NUM])
{

  for (int i = 0; i < SELLERS_NUM; i++)
  {
    strncpy(per_seller[i].seller_code, sellers[i].code, sizeof(per_seller[i].seller_code));
    per_seller[i].total_sold = 0;
  }

  for (int i = 0; i < SALES_NUM; i++)
  {
    for (int j = 0; j < SELLERS_NUM; j++)
    {
      if (strcmp(sales[i].seller_code, per_seller[j].seller_code) == 0)
      {
        // preciso dar um jeito de pegar o código em questão
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

  for(int i = 0; i < SELLERS_NUM; i++){
    printf("%s - %lld\n\n", per_seller[i].seller_code, per_seller[i].total_sold);
  }
}

void print_output_file(Sale sales[SALES_NUM], Product products[PRODUCTS_NUM], Seller sellers[SELLERS_NUM], char file_name[255], long long int total)
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
    fprintf(output_file, "[%d] %s %lld %s\n", i, products[i].code, products[i].price, products[i].description);
  }

  fprintf(output_file, "\n\nLista de Vendedores:\n");
  for (int i = 0; i < SELLERS_NUM; i++)
  {
    fprintf(output_file, "[%d] %s %s", i, sellers[i].code, sellers[i].name);
  }

  fprintf(output_file, "\n\nTotal geral vendido: R$%lld.00", total);
}

int main()
{

  Product cars[PRODUCTS_NUM];
  create_products(cars, "produtos.txt");

  Seller sellers[SELLERS_NUM];
  create_sellers(sellers, "vendedores.txt");

  Sale sales[SALES_NUM];
  create_sales(sales, "vendas.txt");

  long long int total_sales = total_sold(sales, cars);

  Per_Product per_product[PRODUCTS_NUM];
  sales_per_product(sales, cars, per_product);

  Per_Seller per_seller[SELLERS_NUM];
  sales_per_seller(sales, sellers, per_seller, cars);

  print_output_file(sales, cars, sellers, "totais.txt", total_sales);

  return 0;
}