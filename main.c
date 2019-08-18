/* Matin-Moezi #9512058 - Final Database Project - Spring 2019 */

#include <stdio.h>

#include "final_project.h"

int main() {
    printf("Welcome to my final database project!\n");

    // Initialization
    if (init() != 0) {
        printf("'app_config.ini or 'mysql_config.ini' file(s) or dataset directory doesn't exist!\n");
        return -1;
    }
    printf("Connecting to Mysql server...");

    // Connecting to Mysql
    if (connect_mysql()) {
        printf("Connecting failed: %s\n", error(mysql));
        return -1;
    }
    printf("done.\n");

    // Creating database
    printf("Creating database...");
    if (create_db()) {
        printf("Creating database failed: %s\n", error(mysql));
        return -1;
    }
    printf("done.\n");

    // Creating tables
    printf("Creating tables...\n");
    switch (create_tbls()) {
        case -2:
            printf("failed: %s\n", error(mysql));
            return -1;
        case -1:
            printf("\tCannot read 'create_tbls_query.txt'.\n");
            return -1;
    }

    // Load products dataset
    printf("Loading products dataset...\n");
    if (load_products()) {
        printf("failed.\n");
    }

    // Load products history dataset
    printf("Loading products history dataset...\n");
    if (load_products_history()) {
        printf("failed.\n");
    }

    // Load customers review dataset
    printf("Loading customers review dataset...");
    fflush(stdout);
    if (load_customers_review())
        printf("failed: %s\n", mysql_error(mysql));
    else printf("done.\n");

    // Load orders dataset
    printf("Loading orders dataset...");
    fflush(stdout);
    if (load_orders())
        printf("failed: %s\n", mysql_error(mysql));
    else printf("done.\n");

    // Load product review dataset
    printf("Loading product review dataset...");
    fflush(stdout);
    if (load_product_review())
        printf("failed: %s\n", mysql_error(mysql));
    else printf("done.\n");

    mysql_close(mysql);
    return 0;
}
