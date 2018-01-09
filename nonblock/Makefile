CC = gcc
SRC_PATH = ../unp
INC = -I$(SRC_PATH)

STATIC_FILE = -L$(SRC_PATH) -lunp

vpath %.c $(SRC_PATH)

webobjects = home_page.o start_connect.o write_get_cmd.o web.o

targets = $(objects:%.o=%)

.PHONY : all
all : web
	@echo "\nCompleted!\n"
	-rm $(webobjects)

web : $(webobjects)
	@echo "\n"$< "=>" $@
	$(CC) -o $@ $(webobjects) $(STATIC_FILE)

$(webobjects) : %.o : %.c
	@echo "\n"$< "=>" $@
	$(CC) $(INC) -c $< -o $@

.PHONY : clean
clean :
		-rm $(objects:%.o=%) $(webobjects) web
