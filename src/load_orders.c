#include <stdio.h>

#include "final_project.h"

int load_orders() {
    if (mysql_query(mysql,
                    "LOAD DATA LOCAL INFILE './data/3-p5s3708k.csv' INTO TABLE `order` FIELDS TERMINATED BY ',' LINES TERMINATED BY '\\n' IGNORE 1 ROWS;") !=
        0)
        return -1;
    return 0;
}