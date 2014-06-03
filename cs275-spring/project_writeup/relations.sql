CREATE TABLE Users(		username	CHAR(32),
						password	CHAR(32),
						user_type 	CHAR(32),
						wallet_amt	REAL,

						PRIMARY KEY (username),
					 );

CREATE TABLE Items(		name	CHAR(32),
						price	REAL,

						PRIMARY KEY (name),
				  );

CREATE TABLE Orders(
						order_id	INTEGER,
						customer    CHAR(32),

						PRIMARY KEY (order_id),

						FOREIGN KEY (customer) REFERENCES Users
							ON UPDATE CASCADE
							ON DELETE CASCADE 
				  );

CREATE TABLE Items_Orders(
						 	order_id	INTEGER,
						 	item_name	CHAR(32),

						 	PRIMARY KEY (order_id, item_name),

						 	FOREIGN KEY (order_id) REFERENCES Orders
						 		ON DELETE CASCADE
						 		ON UPDATE CASCADE,

						 	FOREIGN KEY (item_name) REFERENCES Items
						 		ON DELETE CASCADE
						 		ON UPDATE CASCADE
						 );		

CREATE TABLE Ingredients (	order_id		INTEGER,
							name 			CHAR(32),
							expire_date 	DATE,
							qty				INTEGER,
							price			REAL,

							PRIMARY KEY (order_id),
						 );

CREATE TABLE Suppliers (	name	Char(32),
							PRIMARY KEY (name),
					   );

CREATE TABLE Combos (	name	CHAR(32),
						price 	REAL,
						PRIMARY KEY (name),
				   );