import csv

# FilePath class used to issue prompt
# and return the "filename.csv" string
class FilePath:
		
	def __init__(self):
		print("\nEnter the customer order filename:\n");
	
	def file_name(self):
		return ("../" + str(input("")))

#  Product class to 
class Product:

	def __init__(self, name, price=0):
		self.name = name
		self.price = price
	
	def __repr__(self):
		return f'PRODUCT NAME: {self.name} \nPRODUCT PRICE: €{self.price}'

class ProductStock:
	
	def __init__(self, product, quantity):
		self.product = product
		self.quantity = quantity
	
	def name(self):
		return self.product.name;
	
	def unit_price(self):
		return self.product.price;
		
	def cost(self):
		return self.unit_price() * self.quantity
		
	def __repr__(self):
		return f"{self.product} \nThe shop has {self.quantity} of the above\n"

# Customer class for creating the "liveCustomerOrder.csv" file
class Live_customer_order:
	
	def __init__(self):
		
		# Write customer name and budget to the liveCustomerOrder.csv file
		with open("../liveCustomerOrder.csv", "w") as csv_file:
			
			csv_writer = csv.writer(csv_file, lineterminator='\n')

			print("Please Enter Your First Name: ")
			self.name = str(input(""))
			print("Please Enter Your Budget: €")
			self.budget = float(input())
			c = (self.name, self.budget)

			csv_writer.writerow(c)

			csv_file.close()
		
		# Append product name and quantity requried to the liveCustomerOrder.csv file
		while True:
			with open("../liveCustomerOrder.csv", "a", newline="") as csv_file:
			
				csv_writer = csv.writer(csv_file, delimiter=',')

				print("Please Enter Product Name: ")
				self.prod_name = str(input(""))
				print("Please Enter Requried Quantity: #")
				self.prod_price = int(input())
				c = (self.prod_name, self.prod_price)
				csv_writer.writerow(c)
				print("Enter 1 to continue shopping or 0 to stop: ")
				continue_shopping = int(input())
				if continue_shopping == 0:
					break

			csv_file.close()
		
	def live_file_name(self):
		return ("../liveCustomerOrder.csv")

class Customer:

	# Function that reads the customer order from the "customer.csv" file
	def __init__(self, path):
		self.shopping_list = []
		with open(path) as csv_file:
			csv_reader = csv.reader(csv_file, delimiter=',')
			first_row = next(csv_reader)
			self.name = first_row[0]
			self.budget = float(first_row[1])
			for row in csv_reader:
				name = row[0]
				quantity = float(row[1])
				p = Product(name)
				ps = ProductStock(p, quantity)
				self.shopping_list.append(ps) 
				
	def calculate_costs(self, price_list):
		for shop_item in price_list:
			for list_item in self.shopping_list:
				if (list_item.name() == shop_item.name()):
					list_item.product.price = shop_item.unit_price()
	
	def order_cost(self):
		cost = 0
		for list_item in self.shopping_list:
			cost += list_item.cost()
		return cost
	
	def __repr__(self):
		
		print("\n------------------------------")
		print(f"CUSTOMER NAME: {self.name}\nCUSTOMER BUDGET: €{self.budget}")
		print("------------------------------\n")
		print(f'CUSTOMER ORDER:')
		print("------------------------------")
		for cust_item in self.shopping_list:
			print(f'PRODUCT NAME: {cust_item.name()} \nQUANTITY REQUIRED: {cust_item.quantity}')
			print("------------------------------")

		return ""
 
class Shop:
	
	# Function that reads in stock from "stock.csv file" 
	def __init__(self, path):
		self.stock = []
		with open(path) as csv_file:
			csv_reader = csv.reader(csv_file, delimiter=',')
			first_row = next(csv_reader)
			self.cash = float(first_row[0])
			for row in csv_reader:
				p = Product(row[0], float(row[1]))
				ps = ProductStock(p, float(row[2]))
				self.stock.append(ps)

	def current_stock(self, c):
		print("\nWelcome shopper to the live environment")
		print("------------------------------")
		print("SHOP STOCK INCLUDES:\n")
		for shop_item in self.stock:
		   
			print(f'PRODUCT NAME: {shop_item.name()} CURRENT STOCK: {shop_item.quantity}')
			print("------------------------------")
		
		return ""

	# Function that processes the customer order and updates
	# shop cash balance, inventory and customer budget after executing order
	def process_order(self, c):
		print("\n------------------------------")
		print("Processing Customer Order\n")
		self.starting_cash = self.cash
		self.count = 0
		for shop_item in self.stock:
			for list_item in c.shopping_list:
				if (list_item.name() == shop_item.name()):
					if shop_item.quantity >= list_item.quantity:
						if list_item.cost() <= c.budget:
							c.budget -= list_item.cost()
							self.cash += list_item.cost()
							("------------------------------\n")
							print(f'\nPRODUCT NAME: {shop_item.name()}') 
							print(f'QUANTITY PURCHASED: {list_item.quantity} @ PRICE: €{shop_item.unit_price()}')
							print("------------------------------")
							# Updating stock quantities
							shop_item.quantity -= list_item.quantity
						
						elif list_item.cost() > c.budget:
							print(f'\nBUDGET DOES NOT COVER: {list_item.name()}')
							print("------------------------------")
					
					# If stock in shop is less than customer requires
					elif shop_item.quantity < list_item.quantity:
						list_item.quantity = shop_item.quantity

						if list_item.cost() <= c.budget:
							c.budget -= list_item.cost()
							self.cash += list_item.cost()

							print(f'\nPRODUCT NAME: {shop_item.name()}')
							print(f'STOCK AVAILABLE: {list_item.quantity} @ PRICE: {shop_item.unit_price()}')
							print("------------------------------")
							shop_item.quantity -= list_item.quantity
						
						elif list_item.cost() > c.budget:
							print(f'\nBUDGET DOES NOT COVER: {list_item.name()}')
							print("------------------------------")

					self.count += 1
		
		# If count is 0 display error message 
		# none of the customer's order is in the stock.csv file
		if self.count == 0:
			print("------------------------------")
			print("THIS IS NOT AMAZON")
			print("ONLY CHOOSE FROM STOCK.CSV FILE")
			print("------------------------------")

		# print total cost to customer and shop cash balance 
		print("------------------------------")
		print(f'TOTAL COST: €{(round(self.cash - self.starting_cash, 2))}') 
		print(f'SHOP CASH: €{(round(self.cash, 2))}')
		print("********************************\n")
		
		return ""

	# Function that returns a string represenation
	# of the shop cash balance and lists the shop stock in quantities
	def __repr__(self):
		str = ""
		str += f'\nShop has €{(round(self.cash, 2))} in cash\n\n'
		for item in self.stock:
			str += f"{item}\n"
		return str

def main():
	
	s = Shop("../stock.csv")

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
			print(s)

		elif choice == 2:
			f = FilePath()
			c = Customer(f.file_name())
			c.calculate_costs(s.stock)
			print(c)
			s.process_order(c)
			
		elif choice == 3:
			print(s.current_stock(s))
			l = Live_customer_order()
			c = Customer(l.live_file_name())
			c.calculate_costs(s.stock)
			print(c)
			s.process_order(c)

		else:
			break

if __name__ == "__main__":
	
	main()