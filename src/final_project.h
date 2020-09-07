/*      Matin-Moezi #9512058 - Final Database  Project          */
/*      Main Header File - Function Definition                  */
/*      Used Mysql C Connector - mysql.h header file            */

#include <mysql/mysql.h>

#define error(conn) mysql_error(conn)

#define app_config_path "./app_config.ini"
#define mysql_config_path "./mysql_config.ini"
#define dataset_dir_path "./data"

MYSQL *mysql;

// Define application paths
char tbls_query_path[50];
char products_dataset_path[50];
char products_history_dataset_path[50];
char customers_review_dataset_path[50];
char orders_dataset_path[50];
char reviews_quality_dataset_path[50];

// Define products title farsi
#define book_title_fa "کتاب چاپی"
#define puzzle_title_fa "پازل"
#define keyboard_title_fa "کیبورد (صفحه کلید)"
#define mice_title_fa "ماوس (موشواره)"
#define screen_title_fa "محافظ صفحه"
#define cover_title_fa "کیف و کاور گوشی"

int init();                                  /* application setup            		*/
int connect_mysql();                         /* connect to mysql server		        */
int create_db();                             /* create database        		        */
int create_tbls();                           /* create tables          		        */
int load_products();                         /* load products dataset 		        */
int load_products_history();                 /* load products history dataset       */
int load_customers_review();                 /* load customers review dataset       */
int load_orders();                           /* load orders dataset                 */
int load_product_review();                   /* load product review dataset         */
