/* Maitn-Moezi #9512058 - Final Database Project - Spring 2019 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "final_project.h"

int init() {
    if ((access(mysql_config_path, R_OK) || access(app_config_path, R_OK) || access(dataset_dir_path, R_OK)) != 0)
        return -1;
    FILE *fp;
    fp = fopen(app_config_path, "r");
    if (fp == NULL)
        return -1;
    char path_name[50], path[50];
    while (fscanf(fp, "[%[^]]]\n%[^\n]\n", path_name, path) != EOF) {
        if (strcmp(path_name, "tbls_query_path") == 0)
            strcpy(tbls_query_path, path);
        if (strcmp(path_name, "products_dataset_path") == 0)
            strcpy(products_dataset_path, path);
        if (strcmp(path_name, "products_history_dataset_path") == 0)
            strcpy(products_history_dataset_path, path);
        if (strcmp(path_name, "customers_review_dataset_path") == 0)
            strcpy(customers_review_dataset_path, path);
        if (strcmp(path_name, "orders_dataset_path") == 0)
            strcpy(orders_dataset_path, path);
        if (strcmp(path_name, "reviews_quality_dataset_path") == 0)
            strcpy(reviews_quality_dataset_path, path);
    }
    fclose(fp);
    return 0;
}

int connect_mysql() {
    mysql = mysql_init(mysql);
    if (!mysql) {
        return -1;
    }
    mysql_options(mysql, MYSQL_READ_DEFAULT_FILE, mysql_config_path);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "client");
    mysql_options(mysql, MYSQL_OPT_LOCAL_INFILE, 0);
    if (!mysql_real_connect(mysql, NULL, NULL, NULL, NULL, 0, NULL, 0)) {
        mysql_close(mysql);
        return -1;
    }
    return 0;
}

int create_db() {

    // Create FinalProject database
    if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS FinalProject") != 0) {
        mysql_close(mysql);
        return -1;
    }

    // Use FinalProject database
    if (mysql_select_db(mysql, "FinalProject") != 0) {
        mysql_close(mysql);
        return -1;
    }

    return 0;
}
