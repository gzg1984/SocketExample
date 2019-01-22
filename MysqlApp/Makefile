OS:=$(shell uname)
MYSQL_LIB+=$(shell /usr/local/mysql/bin/mysql_config --libs)
MYSQL_HEAD+=$(shell /usr/local/mysql/bin/mysql_config --include)

QueryApp:query.c
	@echo -n $(OS):
	gcc $^ -o $@ $(MYSQL_LIB) $(MYSQL_HEAD)

run:
	DYLD_LIBRARY_PATH=/usr/local/mysql/lib ./QueryApp

clean:
	rm QueryApp

