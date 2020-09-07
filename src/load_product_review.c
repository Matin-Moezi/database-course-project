#include <stdio.h>

#include "final_project.h"

int load_product_review() {
    if (mysql_query(mysql,
                    "LOAD DATA LOCAL INFILE './data/4-oska9ni8.csv' INTO TABLE product_review FIELDS TERMINATED BY '^' ENCLOSED BY '\"' LINES TERMINATED BY '\\n' IGNORE 1 ROWS;") !=
        0)
        return -1;
    return 0;
}