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

// Stock the shop by reading from the stock.csv file
struct Shop createAndStockShop()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	// Read in the amount of "cash" the shop has from first line in stock.csv  
	read = getline(&line, &len, fp);
	float cash = atof(line);
	struct Shop shop = { cash };

	// Next read in the shop stock including 
	// "product name" "price" and "quantity" from stock.csv
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
    }
	return shop;
}

// Read the customer order from the '"filename".csv' file
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

	// Read in the customer "name" and "budget" from the .csv file
	read = getline(&line, &len, fp);
	char * n = strtok(line, ",");
	char * b = strtok(NULL, ",");
	char * name = malloc(sizeof(char) * 50);
	strcpy(name, n);
	double budget = atof(b);

	struct Customer customerOrder = { name, budget };

	// Read in the "product name" and "quantity 
	// the customer requires from the .csv file
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

// Method that prints product name and price
void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: €%.2f\n", p.name, p.price);
}

// Method that prints the current shop stock and cash balance
// Pointers are used to update stock and cash in shop
void printShop(struct Shop* s)
{
	printf("Shop has €%.2f in cash\n", s->cash);
	for (int i = 0; i < s->index; i++)
	{
		printProduct(s->stock[i].product);
		printf("The shop has %d of the above\n", s->stock[i].quantity);
	}
}

// Method that prints the customer order and budget
void printCustomer(struct Customer* c, struct Shop* s)
{

	printf("\n------------------------------\n");
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: €%.2f\n", c->name, c->budget);
	printf("\n------------------------------");
	printf("\nCUSTOMER ORDER:\n");

	for(int i = 0; i < c->index; i++)
	{
		printf("PRODUCT NAME: %s \nQUANTITY REQUIRED: %d\n", c->shoppingList[i].product.name, c->shoppingList[i].quantity);
		printf("\n------------------------------");
	}
}

// Method that prints the current shop stock
// Used in Live Environment
void printCurrentStock(struct Shop* s)
{
	printf("\nSHOP STOCK INCLUDES:\n");
	for (int i = 0; i < s->index; i++){
		printf("PRODUCT NAME: %s CURRENT STOCK: %d\n", s->stock[i].product.name, s->stock[i].quantity);	
	}
	printf("\n------------------------------");
}

// Process the customer order where possible
// Update the shop stock and cash balance after processing order
struct Shop processingOrder(struct Customer* c, struct Shop* s) {
	
	// variables to be used for processing order
	// double totalCostForCustomer = 0.00;
	double shopCash = 0.00;
	double startingBudget = c->budget;
	double finalCost = 0.00;
	double quantitySingleProductCost = 0.00;
	
	// Count variable will remain 0 if none of product names
	// on the shopping list are on the stock list
	int count = 0;

	// loop through customer index
	for(int i = 0; i < c->index; i++){

		// loop through shop index
		for(int j = 0; j < s->index; j++){
			
			// If product name is on shopping list and stock list
			if (strcmp(c->shoppingList[i].product.name, s->stock[j].product.name) == 0){
				// double quantitySingleProductCost = 0.00;
				int custRequires = c->shoppingList[i].quantity;
                int shopStock = s->stock[j].quantity;
				
				count += 1;

				if (s->stock[j].quantity >= c->shoppingList[i].quantity) {
					quantitySingleProductCost = s->stock[j].product.price * c->shoppingList[i].quantity;

						// If customer budget can afford total cost of available shopping items
						if (c->budget >= quantitySingleProductCost){
							s->cash += quantitySingleProductCost;
                        	c->budget -= quantitySingleProductCost;

							printf("\nPRODUCT NAME: %s\n", c->shoppingList[i].product.name);
							printf("QUANTITY PURCHASED: %d @ PRICE: €%.2f\n", c->shoppingList[i].quantity, s->stock[j].product.price * c->shoppingList[i].quantity);
							printf("\n------------------------------");

							// Updating stock quantities
							s->stock[j].quantity = s->stock[j].quantity - c->shoppingList[i].quantity;

						} else if ((c->budget < quantitySingleProductCost)){
							printf("\nBUDGET DOES NOT COVER: %s", c->shoppingList[i].product.name);
							printf("\n------------------------------");
						}
					}
					// If stock in shop is less than customer requires
					if (shopStock < custRequires){
						printf("\nPRODUCT NAME: %s\nSTOCK AVAILABLE FOR SALE: %d\n", c->shoppingList[i].product.name, s->stock[j].quantity);

                    	quantitySingleProductCost = s->stock[j].product.price * s->stock[j].quantity;

						if (c->budget >= quantitySingleProductCost){		
                        	s->cash += quantitySingleProductCost;
                        	c->budget -= quantitySingleProductCost;
                    	
						} else if((c->budget < quantitySingleProductCost)){
                        	printf("\nBUDGET DOES NOT COVER: %s", c->shoppingList[i].product.name);
                    	}
					}
				} 	
			}
		}
		// If count is 0 display error message 
		// none of the customer's order is in the stock.csv file
		if (count == 0) {
		printf("\nTHIS IS NOT AMAZON\n");
		printf("ONLY CHOOSE FROM STOCK.CSV FILE");
		printf("\n------------------------------");
		}

		if (c->budget <= 0.00) {
			printf("\nTRANSACTION CANCELLED\n");
		}

	// If customer budget covers total cost for processed order
	// print total cost to customer and shop cash balance
	if (c->budget <= startingBudget) {
		finalCost = startingBudget - c->budget;
		printf("\nTOTAL COST: €%.2f", finalCost);
		printf("\nSHOP CASH: €%.2f\n", s->cash);
		printf("********************************\n");
	} 

	return * s;
}

int main(void) 
{
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
			printShop(&shop);

		} else if(choice == 2) {
			char * filename = (char*) malloc(50 * sizeof(char));
			printf("\nEnter the customer order filename:\n");
			scanf("%s", filename);
			
			struct Customer customerOrder = customerOrderList(filename);
			printCustomer(&customerOrder, &shop);
			
			printf("\nProcessing Customer Order\n");
			printf("\n------------------------------");
			processingOrder(&customerOrder, &shop);

		} else if(choice == 3) {
			printf("Welcome shopper to the live environment");
			printf("\n------------------------------\n");
			printCurrentStock(&shop);

			// Write customer name and budget to the liveCustomerOrder.csv file
			FILE * fpw;
			fpw = fopen("../liveCustomerOrder.csv", "w");

			if (fpw == NULL)
				exit(EXIT_FAILURE);

			char * custName = (char*) malloc(10 * sizeof(char));
			printf("\nPlease Enter Your First Name: ");
			scanf("%s", &custName);
			char * custBudget = (char*) malloc(10 * sizeof(char));
			printf("\nPlease Enter Your Budget: €");
			scanf("%s", &custBudget);

			fprintf(fpw, "%s, %s\n", &custName, &custBudget);

			fclose(fpw);

			// Conitnue Shopping variable to exit live environment
			int continueShopping = 1;

			char * prodName = (char*) malloc(10 * sizeof(char));
			char * prodQuantity = (char*) malloc(10 * sizeof(char));

			while (continueShopping == 1) {
				printf("\nPlease Enter Product Name: ");
				scanf("%s", &prodName);

				printf("\nPlease Enter Requried Quantity: #");
				scanf("%s", &prodQuantity);

				// Append product name and quantity requried to the liveCustomerOrder.csv file
				FILE * fpa;
				fpa = fopen("../liveCustomerOrder.csv", "a");

				if (fpa == NULL)
					exit(EXIT_FAILURE);
				
				fprintf(fpa, "%s, %s\n", &prodName, &prodQuantity);

				fclose(fpa);

				printf("\nEnter 1 to continue shopping or 0 to stop: ");
				scanf("%d", &continueShopping);

				// If exiting the shop. Process the order &
				// update the shop stock and balance
				if (continueShopping == 0) {
					char * filename = "liveCustomerOrder.csv";
					struct Customer customerLiveOrder = customerOrderList(filename);
					printf("\nProcessing Customer Order\n");
					printf("\n------------------------------");
					processingOrder(&customerLiveOrder, &shop);
				}
			}
		}
	}
    return 0;
}