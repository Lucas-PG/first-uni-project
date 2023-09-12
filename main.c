#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define PRODUCTS_NUM 10
#define SELLERS_NUM 5
#define SALES_NUM 20

typedef struct {
    char code[3];
    long long int price;
    char description[50];
  } Product;

typedef struct
  {
   char code[3]; 
   char name[10];
  } Seller;

  typedef struct{
    char seller_code[3];
    char product_code[3];
    int units_sold;
  } Sale;


//cria objeto para produtos
void create_products(Product given_products[PRODUCTS_NUM], char file_name[]) {
  FILE* products_file = fopen(file_name, "r");
  char buffer_products[255];
  int products_index = 0;

  while(fgets(buffer_products, 255, products_file) != NULL){
    char *current_code = strtok(buffer_products, " ");
    char *current_price_str = strtok(NULL, " ");
    char *current_description = strtok(NULL, "\n");

    strncpy(given_products[products_index].code, current_code, sizeof(given_products[products_index].code));
    strncpy(given_products[products_index].description, current_description, sizeof(given_products[products_index].description));
    given_products[products_index].price = atoi(current_price_str);
    products_index++;
    
  };
}

//cria objeto para vendedores
void create_sellers(Seller given_sellers[SELLERS_NUM], char file_name[255]){
  FILE* sellers_file = fopen(file_name, "r");
  char buffer_sellers[255];
  int sellers_index = 0;

  while(fgets(buffer_sellers, 255, sellers_file) != NULL){
    char *current_code = strtok(buffer_sellers, " ");
    char *current_name = strtok(NULL, " ");

    strncpy(given_sellers[sellers_index].code, current_code, sizeof(given_sellers[sellers_index].code));
    strncpy(given_sellers[sellers_index].name, current_name, sizeof(given_sellers[sellers_index].name));
    sellers_index++;
    
  };
}

//cria objeto para vendas
void create_sales(Sale given_sales[SALES_NUM], char file_name[255])
{
  FILE* sales_file = fopen(file_name, "r");
  char buffer_sales[255];
  int sales_index = 0;

  while(fgets(buffer_sales, 255, sales_file) != NULL){
    char *current_seller_code = strtok(buffer_sales, " ");
    char *current_product_code = strtok(NULL, " ");
    char *current_units_sold = strtok(NULL, " ");

    strncpy(given_sales[sales_index].seller_code, current_seller_code, sizeof(given_sales[sales_index].seller_code));
    strncpy(given_sales[sales_index].product_code, current_product_code, sizeof(given_sales[sales_index].product_code));
    given_sales[sales_index].units_sold = atoi(current_units_sold);
    sales_index++;
    
  };
}

Product find_product(char code[3], Product products[PRODUCTS_NUM]) {
  Product found_product;

  for (int i = 0; i < PRODUCTS_NUM; i++) {
    if (products[i].code == code) {
      printf("%s", products[i].description);
      found_product = products[i];
    }
  }

  return found_product;
}

int main(){

  Product cars[PRODUCTS_NUM];
  create_products(cars, "produtos.txt");

  Seller sellers[SELLERS_NUM];
  create_sellers(sellers, "vendedores.txt");

  Sale sales[SALES_NUM];
  create_sales(sales, "vendas.txt");

  Product found_product;
  found_product = find_product("04", cars);

  

  return 0;
}