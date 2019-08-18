CC=gcc
CFLAGS=-L /usr/lib64/mysql -I /usr/include/mysql -l mysqlclient
main: *.c final_project.h
	$(CC) $^ -o $@ $(CFLAGS)
debug: *.c final_project.h
	$(CC) -g $^ -o $@ $(CFLAGS)
