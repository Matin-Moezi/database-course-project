/* Matin-Moezi #9512058 - Final Database Project - Spring 2019 */

#include <stdio.h>
#include <stdlib.h>

#include "final_project.h"

int create_tbls(){
	FILE *file;
	int errnum;
	char tbl_name[30] = {0}, query[1000] = {0};
	file = fopen(tbls_query_path, "r");
	if(file == NULL)
		return -1;
	while(fscanf(file,"[%[^]]]\n%[^[]",tbl_name,query) != EOF){
		printf("\tCreating '%s' table...",tbl_name);
		if(mysql_query(mysql,query) != 0){
			mysql_close(mysql);
			return -2;
		}
		printf("done.\n");
	}
	fclose(file);
	return 0;
}

