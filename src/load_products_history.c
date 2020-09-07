#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "final_project.h"

typedef struct {
    int id;
    int variant_id;
    char selling_price[20];
    char rrp_price[20];
    int order_limit;
    int show_in_price_history;
    int active;
    char created_at[50];
    int product_id;
    int marketplace_seller_id;
} ProductHistory;

static void parse_csv(char *line, ProductHistory *product, const char *sep) {
    int i = 0;
    char *token;
    while ((token = strsep(&line, sep)) != NULL) {
        sscanf(token, "\"%[^\"]\"", token);
        switch (i++) {
            case 0:
                product->id = atoi(token);
                break;
            case 1:
                product->variant_id = atoi(token);
                break;
            case 2:
                strcpy(product->selling_price, token);
                break;
            case 3:
                strcpy(product->rrp_price, token);
                break;
            case 6:
                product->order_limit = atoi(token);
                break;
            case 10:
                product->show_in_price_history = atoi(token);
                break;
            case 11:
                product->active = atoi(token);
                break;
            case 12:
                strcpy(product->created_at, token);
                break;
            case 13:
                product->product_id = atoi(token);
                break;
            case 14:
                product->marketplace_seller_id = atoi(token);
                break;
            default:
                break;
        }
    }
}

int insert_product_history(ProductHistory *productHistory) {
    char insert_cmd[8000];
    sprintf(insert_cmd,
            "INSERT INTO product_history (product_history_id, product_variant_id, selling_price, rrp_price, order_limit, show_in_price_history, active, created_at, product_id, marketplace_seller_id) VALUE (%d, %d, %s, %s, %d, %d, %d, '%s', %d, %d);",
            productHistory->id, productHistory->variant_id, productHistory->selling_price, productHistory->rrp_price,
            productHistory->order_limit, productHistory->show_in_price_history,
            productHistory->active, productHistory->created_at, productHistory->product_id,
            productHistory->marketplace_seller_id);
    if (mysql_query(mysql, insert_cmd) != 0)
        return -1;
    return 0;
}

int write_file(const char *path, ProductHistory *productHistory) {
    FILE *file;
    file = fopen(path, "a");
    if (file == NULL)
        return -1;
    fprintf(file, "%d,%d,%s,%s,%d,%d,%d,\"%s\",%d,%d\n",
            productHistory->id, productHistory->variant_id, productHistory->selling_price, productHistory->rrp_price,
            productHistory->order_limit, productHistory->show_in_price_history,
            productHistory->active, productHistory->created_at, productHistory->product_id,
            productHistory->marketplace_seller_id);
    fclose(file);
    return 0;
}

int load_products_history() {
    FILE *fp;
    fp = fopen(products_history_dataset_path, "r");
    if (fp == NULL)
        return -1;
    char line[10000];
    ProductHistory productHistory = {0};
    while (fscanf(fp, "%[^\n]\n", line) != EOF) {

        parse_csv(line, &productHistory, ",");
//        // Insert product history
//        if (insert_product_history(&productHistory) == -1) {
//            printf("\tInsert product history #%d failed: %s\n", productHistory.id, mysql_error(mysql));
//            continue;
//        }

        // Write to file
        if (write_file("./data/product_history.csv", &productHistory) == -1) {
            printf("Cannot write to file.\n");
            return -1;
        }
    }
    if (mysql_query(mysql,
                    "LOAD DATA LOCAL INFILE './data/product_history.csv' INTO TABLE product_history FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' LINES TERMINATED BY '\\n';") !=
        0) {
        printf("\tInsert product history #%d failed: %s\n", productHistory.id, mysql_error(mysql));
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}