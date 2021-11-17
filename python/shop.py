from dataclasses import dataclass, field
from typing import List
import csv
		
@dataclass
class Product:
	name: str
	price: float = 0.0

@dataclass 
class ProductStock:
	product: Product
	quantity: int

@dataclass 
class Shop:
	cash: float = 0.0
	stock: List[ProductStock] = field(default_factory=list)

@dataclass
class Customer:
	name: str = " "
	budget: float = 0.0
	shopping_list: List[ProductStock] = field(default_factory=list)

# Stock the shop by reading from the stock.csv file
def create_and_stock_shop():
	s = Shop()
	with open('../stock.csv') as csv_file:
		csv_reader = csv.reader(csv_file, delimiter=',')
		first_row = next(csv_reader)
		s.cash = float(first_row[0])
		for row in csv_reader:
			p = Product(row[0], float(row[1]))
			ps = ProductStock(p, float(row[2]))
			s.stock.append(ps)
	return s

# Read the customer order from the '"file_path".csv' file
def read_customer(file_path):
	with open(file_path) as csv_file:
		csv_reader = csv.reader(csv_file, delimiter=',')
		first_row = next(csv_reader)
		c = Customer(first_row[0], float(first_row[1]))
		for row in csv_reader:
			name = row[0]
			quantity = float(row[1])
			p = Product(name)
			ps = ProductStock(p, quantity)
			c.shopping_list.append(ps)
		return c 

def liveCustomerOrder():
	
	# Write customer name and budget to the liveCustomerOrder.csv file
	with open("../liveCustomerOrder.csv", "w") as csv_file:
		
		csv_writer = csv.writer(csv_file, lineterminator='\n')

		print("Please Enter Your First Name: ")
		name = str(input(""))
		print("Please Enter Your Budget: €")
		budget = float(input())
		c = (name, budget)

		csv_writer.writerow(c)

		csv_file.close()

	# Append product name and quantity requried to the liveCustomerOrder.csv file
	while True:
		with open("../liveCustomerOrder.csv", "a", newline="") as csv_file:

			csv_writer = csv.writer(csv_file, delimiter=',')

			print("Please Enter Product Name: ")
			prod_name = str(input(""))
			print("Please Enter Requried Quantity: #")
			prod_price = int(input())
			c = (prod_name, prod_price)
			csv_writer.writerow(c)
			print("Enter 1 to continue shopping or 0 to stop: ")
			continue_shopping = int(input())
			if continue_shopping == 0:
				break
	
		csv_file.close()
	
# Function that prints product name and price
def print_product(p):
	print(f'\nPRODUCT NAME: {p.name} \nPRODUCT PRICE: €{p.price}')

# Function that prints the current shop stock and cash balance     
def print_shop(s):
	print(f'\nShop has €{(round(s.cash, 2))} in cash')
	for item in s.stock:
		print_product(item.product)
		print(f'The Shop has {item.quantity} of the above')

# Function that prints the customer order and budget
def print_customer(c):
	print("\n------------------------------")
	print(f'CUSTOMER NAME: {c.name} \nCUSTOMER BUDGET: €{c.budget}')
	print("------------------------------")
	print("\nCUSTOMER ORDER:")

	for item in c.shopping_list:
		print("------------------------------")
		print(f'PRODUCT NAME: {item.product.name}\nQUANTITY REQUIRED: {item.quantity}')
	print("------------------------------")    

# Function that prints the current shop stock
# Used in Live Environment
def print_current_stock(s):
	print("SHOP STOCK INCLUDES:")
	for stockItem in s.stock:
	   
	   print(f'PRODUCT NAME: {stockItem.product.name} CURRENT STOCK: {stockItem.quantity}')
	print("------------------------------")

# Process the customer order where possible 
# Update the shop stock and cash balance after processing order
def processingOrder(c, s):
	print("\n------------------------------")
	print("Processing Customer Order\n")

	# variables to be used for processing order
	startingBudget = c.budget
	quantitySingleProductCost = 0.00
	finalCost = 0.00
	
	# Count variable will remain 0 if none of product names
	# on the shopping list are on the stock list
	count = 0
	
	# loop through customer items in shopping list
	for item in c.shopping_list:
		
		# loop through stock items in shop
		for stockItem in s.stock:

			# If product name is on shopping list and stock list
			if item.product.name == stockItem.product.name:
				
				count += 1

				if stockItem.quantity >= item.quantity:
					quantitySingleProductCost = item.quantity * stockItem.product.price
					
					# If customer budget can afford cost of shopping item in required quantity
					if quantitySingleProductCost <= c.budget:
						c.budget -= quantitySingleProductCost
						s.cash += quantitySingleProductCost
						print("------------------------------\n")
						print(f'PRODUCT NAME: {item.product.name}')
						print(f'QUANTITY PURCHASED: {item.quantity} @ PRICE: {stockItem.product.price}')
						# Updating stock quantities
						stockItem.quantity -= item.quantity 

					elif quantitySingleProductCost > c.budget:
						print(f'BUDGET DOES NOT COVER: {item.product.name}')
				
				# If stock in shop is less than customer requires
				elif stockItem.quantity < item.quantity:
					quantitySingleProductCost = stockItem.quantity * stockItem.product.price

					if quantitySingleProductCost <= c.budget:
						c.budget -= quantitySingleProductCost
						s.cash += quantitySingleProductCost

						print(f'\nPRODUCT NAME: {item.product.name}')
						print(f'\nSTOCK AVAILABLE: {stockItem.quantity} @ PRICE: {stockItem.product.price}')
						print("------------------------------")
						stockItem.quantity -= stockItem.quantity
					
					elif quantitySingleProductCost > c.budget:
						print(f'\nBUDGET DOES NOT COVER: {item.product.name}')

	# If count is 0 display error message 
	# none of the customer's order is in the stock.csv file
	if count == 0:
		print("------------------------------")
		print("THIS IS NOT AMAZON")
		print("ONLY CHOOSE FROM STOCK.CSV FILE")
		print("------------------------------")
	
	# If customer budget has changed after processing order
	# print total cost to customer and shop cash balance 
	finalCost = startingBudget - c.budget 
	print("------------------------------")
	print(f'\nTOTAL COST: €{(round(finalCost, 2))}') 
	print(f'\nSHOP CASH: €{(round(s.cash, 2))}\n')
	print("********************************\n")

def main():
	
	s = create_and_stock_shop() 

	while True:
		# Enter number to select option from Menu
		print("\n------------------------------");
		print("\nSelect from the Menu:");
		print("\n------------------------------");
		print("\n1: Cash balance and curent stock in Shop");
		print("\n2: Shop orders to be processed");
		print("\n3: Shopping in the live environment");
		print("\n0: To exit shop Menu");
		print("\n------------------------------\n");
		choice = int(input())
		
		if choice == 1:
			print_shop(s)

		elif choice == 2:
			print("\nEnter the customer order filename:\n");
			filename = str(input(""))
			file_path = ("../" + filename)
			c = read_customer(file_path)
			print_customer(c); processingOrder(c, s)
			
		elif choice == 3:
			print("\nWelcome shopper to the live environment")
			print("------------------------------")
			print_current_stock(s)
			liveCustomerOrder()
			c = read_customer("../liveCustomerOrder.csv")
			processingOrder(c, s)
			
		else:
			break

if __name__ == "__main__":

	main()

