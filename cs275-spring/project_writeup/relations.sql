CREATE TABLE Customer(	username	CHAR(32),
						password	CHAR(32),
						wallet_amt	REAL,

						PRIMARY KEY (username),
					 );

CREATE TABLE Manager(	username	CHAR(32),
						password	CHAR(32),
						wallet_amt	REAL,
						
						PRIMARY KEY (username),
					);

CREATE TABLE Order(		customer    CHAR(32)
						order_id 	INTEGER,
						cost	 	REAL,

						PRIMARY KEY (order_id),

						FOREIGN KEY (customer) REFERENCES Customer 
							ON DELETE CASCADE
							ON UPDATE CASCADE
				  );

CREATE TABLE Item(		name	CHAR(32),
						price	REAL,

						PRIMARY KEY (name),
				);

CREATE TABLE Order_Item(	item    CHAR(32),
							order   INTEGER,

							FOREIGN KEY item REFERENCES Item,
								ON UPDATE CASCADE
								ON DELET RESTRICT

							FOREIGN KEY order REFERENCES Order,
								ON UPDATE CASCADE
								ON DELET RESTRICT
					   );			

CREATE TABLE Ingredient (	order_id		INTEGER,
							name 			CHAR(32),
							expire_date 	DATE,
							qty				INTEGER,
							price			REAL,

							PRIMARY KEY (order_id),
						);

CREATE TABLE Supplier (		name	Char(32),
							PRIMARY KEY (name),
					  );

CREATE TABLE Combo (	name	CHAR(32),
						price 	REAL,
						PRIMARY KEY (name),
				   );