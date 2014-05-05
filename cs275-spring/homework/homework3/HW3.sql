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
				 )

#------- 3 ------------------------

CREATE TABLE Dept( did		 INTEGER,
				   dname	 CHAR(20),
				   budget	 REAL,
				   managerid INTEGER NOT NULL,
				 )

#--------- 4 -----------------------

INSERT 
INTO Emp (eid, ename, age, salary)
VALUES (101, 'John Doe', 32, 15000)

#--------- 5 -----------------------

UPDATE Emp E
SET E.salary = E.salary * 1.10

DELETE FROM Dept
WHERE dname = 'Toy'
