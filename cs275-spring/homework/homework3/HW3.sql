#---------- 2 -------------------

CREATE TABLE Emp( eid    INTEGER,
				  ename  CHAR(20),
				  age 	 INTEGER,
				  salary REAL,
				  PRIMARY KEY (eid),
				)

CREATE TABLE Works( eid    	  INTEGER,
				  	did    	  INTEGER,
				  	pct_time  INTEGER,

				  	PRIMARY KEY (eid, did),
				  	
				  	FOREIGN KEY (eid) REFERENCES Emp
				  		ON DELETE CASCADE
				  		ON UPDATE CASCADE 
			  		
			  		FOREIGN KEY (did) REFERENCES Dept
			  			ON DELETE SET NULL
			  			ON UPDATE CASCADE

				  )

CREATE TABLE Dept( did		 INTEGER,
				   dname	 CHAR(20),
				   budget	 REAL,
				   managerid INTEGER,

				   PRIMARY KEY (managerid)
				 )

#------- 3 ------------------------

CREATE TABLE Dept( did		 INTEGER,
				   dname	 CHAR(20),
				   budget	 REAL,
				   managerid INTEGER NOT NULL,
				   PRIMARY KEY (managerid)
				 )

#--------- 4 -----------------------

INSERT 
INTO Emp (eid, ename, age, salary)
VALUES (101, 'John Doe', 32, 15000)

#--------- 5 -----------------------

UPDATE Emp E
SET E.salary = E.salary * 1.10
 
#------- 6 -------------------------

DELETE FROM Dept
WHERE dname = 'Toy'

#------------- Project SQL ----------------------

CREATE TABLE TransportHub (
							ticket_price 		REAL,
							street address 		CHAR(40), 
							name 				CHAR(20),
							PRIMARY KEY (name)
						  );

CREATE TABLE TourCenter (
							name		CHAR(20),
							price		REAL,
							open_time	INTEGER, 
							close_time 	INTEGER,
							street_addr CHAR(40),
							PRIMARY KEY (name)
						);

CREATE TABLE Trip (
						landmark_name		CHAR(20),
						tramsport_hub_name		CHAR(20),
						PRIMARY KEY (landmark_name, transport_hub_name)
						FOREIGN KEY (landmark_name) REFERENCES Landmark
							ON DELETE CASCADE
							ON UPDATE CASCADE,

						FOREIGN KEY (travel_hub_name) REFERENCES TransportHub
							ON DELETE CASCADE
							ON UPDATE CASCADE,
				  );
						
CREATE TABLE GeneralStore (
							name 			CHAR(20),
							avg_price 		REAL,
							open_time		INTEGER,
							close_time 		INTEGER,
							street_addr 	CHAR(40),
							supplier_name 	CHAR(20),

							PRIMARY KEY (name),

							FOREIGN KEY (supplier_name) REFERENCES TransportHub
								ON DELETE SET NULL
								ON UPDATE CASCADE

						  );

CREATE TABLE Restaurant (
							name 			CHAR(20),
							avg_price 		REAL,
							open_time		INTEGER,
							close_time 		INTEGER,
							street_addr 	CHAR(40),
							supplier_name	CHAR(20),

							PRIMARY KEY (name),

							FOREIGN KEY (supplier_name) REFERENCES GeneralStore
								ON DELETE SET NULL
								ON UPDATE CASCADE,

						);

CREATE TABLE Landmark (
					   		name		CHAR(20),
					   		street_addr CHAR(40),
					   		PRIMARY KEY (name)
					  );

CREATE TABLE GovtOffice (
							street_addr		CHAR(40),
							office_id		INTEGER,
							PRIMARY KEY (office_id)
						);

CREATE TABLE Hotel (
						name 		CHAR(20),
						price 		REAL,
						vacancy 	BOOLEAN,
						smoking 	BOOLEAN,
						street_addr CHAR(40),
						caterer		CHAR(20),

						PRIMARY KEY (name),

						FOREIGN KEY (caterer) REFERENCES Restaurant
							ON DELETE SET NULL
							ON UPDATE CASCADE

				   );

CREATE TABLE Permit (
						restaurant_name		CHAR(20),
						office_id			INTEGER,

						start_date			DATE,
						expiration_date		DATE, 
						liquor_license		BOOLEAN,

						FOREIGN KEY (restaurant_name) REFERENCES Restaurant
							ON DELETE CASCADE
							ON UPDATE CASCADE,

						FOREIGN KEY (office_id) REFERENCES GovtOffice
							ON DELETE NO ACTION
							ON UPDATE NO ACTION,

						PRIMARY KEY (office_id) REFERENCES GovtOffice 
					);
