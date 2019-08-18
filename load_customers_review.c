#include <stdio.h>

#include "final_project.h"

int load_customers_review() {
    if (mysql_query(mysql,
                    "LOAD DATA LOCAL INFILE './data/2-p9vcb5bb.csv' INTO TABLE review FIELDS TERMINATED BY '^' ENCLOSED BY '\"' LINES TERMINATED BY '\\n' IGNORE 1 ROWS;") !=
        0)
        return -1;
    return 0;
}