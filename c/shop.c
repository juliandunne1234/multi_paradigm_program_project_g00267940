#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Product {
	char* name;
	double price;
};

struct ProductStock {
	struct Product product;
	int quantity;
};

struct Shop {
	double cash;
	struct ProductStock stock[20];
	int index;
};

struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};

struct Shop createAndStockShop()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	read = getline(&line, &len, fp);
	float cash = atof(line);
	// printf("cash in shop is %.2f\n", cash);
	
	struct Shop shop = { cash };

    while ((read = getline(&line, &len, fp)) != -1) {
		char *n = strtok(line, ",");
		char *p = strtok(NULL, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		double price = atof(p);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		struct Product product = { name, price };
		struct ProductStock stockItem = { product, quantity };
		shop.stock[shop.index++] = stockItem;
		// printf("NAME OF PRODUCT %s PRICE %.2f QUANTITY %d\n", name, price, quantity);
    }
	return shop;
}

struct Customer customerOrderList(char * filename)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

	char * customerFile;
	char str1[50] = "../";
	customerFile = strcat(str1, filename);

	fp = fopen(customerFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	read = getline(&line, &len, fp);
	char * n = strtok(line, ",");
	char * b = strtok(NULL, ",");
	char * name = malloc(sizeof(char) * 50);
	strcpy(name, n);
	double budget = atof(b);

	struct Customer customerOrder = { name, budget };

    while ((read = getline(&line, &len, fp)) != -1) {
		char * n = strtok(line, ",");
		char * q = strtok(NULL, ",");
		char * name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		int quantity = atoi(q);
		struct Product product = { name };
		struct ProductStock customerItem = { product, quantity };
		customerOrder.shoppingList[customerOrder.index++] = customerItem;
    }
	
	return customerOrder;
}

void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
}

void printCustomer(struct Customer c, struct Shop s)
{
	printf("-------------\n");
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
	printf("-------------\n");
	for(int i = 0; i < c.index; i++)
	{
		for(int j=0; j < s.index; j++){
			if ((strcmp(c.shoppingList[i].product.name, s.stock[j].product.name) == 0)){
				printProduct(s.stock[j].product);
				printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
				double cost = c.shoppingList[i].quantity * s.stock[i].product.price; 
				printf("The cost to %s will be €%.2f\n", c.name, cost);
				printf("-------------\n");
			}
		}
	}
}

void printShop(struct Shop s)
{
	printf("Shop has %.2f in cash\n", s.cash);
	for (int i = 0; i < s.index; i++)
	{
		printProduct(s.stock[i].product);
		printf("The shop has %d of the above\n", s.stock[i].quantity);
	}
}

int main(void) 
{
	// Cash balance and stock
	struct Shop shop = createAndStockShop();

	int choice = -1;
	
	while (choice != 0)
	{
		// Enter number to select option from Menu
		fflush(stdin);
		printf("\n------------------------------");
		printf("\nSelect from the Menu:");
		printf("\n------------------------------");
		printf("\n1: Cash balance and curent stock in Shop");
		printf("\n2: Shop orders to be processed");
		printf("\n3: Shopping in the live environment");
		printf("\n0: To exit shop Menu");
		printf("\n------------------------------\n");
		scanf("%d", &choice);

		if(choice == 1){
			printShop(shop);

		} else if(choice == 2) {
			char * filename = (char*) malloc(50 * sizeof(char));
			// Enter the customer "name.csv" file 
			printf("\nEnter the customer order filename:\n");
			scanf("%s", filename);
			struct Customer customerOrder = customerOrderList(filename);
			printCustomer(customerOrder, shop);

		} else if(choice == 3) {
			printf("Shopping in the live environment");
		}
	}
    return 0;
}