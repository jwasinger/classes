create table s
  ( sno    char(5)  not null,
    sname  char(20) not null,
    status smallint,
    city   char(15),
    primary key (sno)
  );
create table p
  ( pno    char(6)  not null,
    pname  char(20) not null,
    color  char(6),
    weight smallint,
    city   char(15),
    primary key (pno)
  );
create table sp
  ( sno    char(5)  not null,
    pno    char(6)  not null,
    qty    integer  not null,
    primary key (sno, pno)
  );
