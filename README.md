# database-course-project
Database Course Final Project

This project is written in C programming language and used ‘libmysqlclient’ library as the Mysql
connector. ‘Libmysqlclient’ library contains many methods to connect to Mysql server and
execute queries. For more information about Mysql C connector visit:
https://dev.mysql.com/doc/refman/8.0/en/c-api-function-overview.html
#### Requirements:
- Should be run under the Linux operating system.
- GCC compiler version is 8.3.1 and C99
- Using the Mysql database server version 8.0.15.
#### Dependencies:
The project depends only on Mysql C connector and imports only one header file `mysql.h`.
For download Mysql C connector visit: https://dev.mysql.com/downloads/connector/c/
#### Initialization:
Two main files `app_config.ini` and `mysql_config.ini` are necessary to run the project.
`App_config.ini` contains project constants such as datasets and query file path.
`Mysql_config.ini` contains Mysql connection information.
Mysql connection setting such as ‘username’, ‘password’, ‘server IP address’, ‘port’
should be written in `mysql_config.ini`, for instance:  
`[client]`  
`user=sa`  
`host=localhost  //127.0.0.1`  
`password=0000000`  
`port=3306`  
- There are five datasets downloaded from Digikala Datasets in `./data/` directory and necessary
to running program correctly.
- Create tables queries are stored in `create_tbls_query.txt` **don’t touch it**.
- Since several datasets contain a huge amount of data (e.g. million rows) and loading through
usual ways (i.e INSERT query for every row) is inefficient, inserting those datasets from the file
that query is `LOAD DATA INFILE`. This approach improves performance significantly. In order
to execute this query, should be set some variables in the Mysql server by the user. So run
below command in the Mysql shell:  
`SET GLOBAL VARIABLE local_infile = ‘ON’;`
#### Run:
Go to the project directory and run below commands in Linux shell:  
`$ make`  
`$ ./main`  
