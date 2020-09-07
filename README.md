# MySQL Interface for Loading Massive Datasets in C
### Introuduction

This project proposes an interface for inserting a massive dataset in the CSV format to the MySQL database.
The datasets used in this project are come from the Digikala open datasets (digikala.com/opendata).
The proposed interface inserts the CSV datasets through bulk insertion in the following steps:
- Creating a MySQL database and corresponding tables using predefined static schemas
- Reading the CSV datasets and parsing them into the appropriate fields
- Inserting the rows into the relevant tables through the MySQL feature, Bulk Insertion
This project is written in C programming language and uses ‘libmysqlclient’ library as the MySQL
connector. For more information about MySQL C connector visit:
dev.mysql.com/doc/refman/8.0/en/c-api-function-overview.html
### Requirements:
- Should be run under the Linux operating system.
- GCC compiler version 8.3.1 and C99
- MySQL database server version 8.0.15.
### Dependencies:
The project depends only on Mysql C connector and imports only one header file `mysql.h`.
For download Mysql C connector visit: dev.mysql.com/downloads/connector/c/
### Initialization:
Two main files `app_config.ini` and `mysql_config.ini` are necessary to run the project.
`App_config.ini` contains project constants such as datasets and queries file path.
`Mysql_config.ini` contains the information of the MySQL connection.
MySQL connection settings such as ‘username’, ‘password’, ‘server IP address’, ‘port’
should be written in `mysql_config.ini`, for instance:  
`[client]`  
`user=sa`  
`host=localhost  //127.0.0.1`  
`password=0000000`  
`port=3306`  
- It is necessary to downloading above mentioned datasets and put them into the `data` subdirectory in the root directory
- Create tables queries are stored in `create_tbls_query.txt` **don’t touch it**.
- Since several datasets contain a huge amount of data (e.g. million rows) and loading through
usual ways (i.e INSERT query for every row) is inefficient, inserting those datasets from the file
that query is `LOAD DATA INFILE`. This approach improves performance significantly. In order
to execute this query, should be set some variables in the MySQL server by the user. So run
below command in the MySQL shell:  
`SET GLOBAL VARIABLE local_infile = ‘ON’;`
### Run:
In the `src` directory run the following commands:

`$ make`  
`$ ./main`  
