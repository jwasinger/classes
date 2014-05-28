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

CREATE TABLE Order(		order_id 	INTEGER,
						cost	 	REAL,

						PRIMARY KEY (order_id),
				  );

CREATE TABLE Item(		name	CHAR(32),
						price	REAL,

						PRIMARY KEY (name),
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