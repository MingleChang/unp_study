CC = gcc
SRC_PATH = ../unp
INC = -I$(SRC_PATH)

STATIC_FILE = -L$(SRC_PATH) -lunp

vpath %.c $(SRC_PATH)

clientObjects = client.o
serv01Objects = serv01.o pr_cpu_time.o web_child.o sig_chld_waitpid.o
serv02Objects = serv02.o pr_cpu_time.o web_child.o sig_chld_waitpid.o child02.o

clientTarget = client
serv01Target = serv01
serv02Target = serv02

objects = $(clientObjects) $(serv01Objects) $(serv02Objects)
targets = $(clientTarget) $(serv01Target) $(serv02Target)

.PHONY : all
all : $(targets)
	@echo $(targets)

$(clientTarget) : $(clientObjects)
	$(CC) -o  $(clientTarget) $(clientObjects) $(STATIC_FILE)

$(serv01Target) : $(serv01Objects)
	$(CC) -o  $(serv01Target) $(serv01Objects) $(STATIC_FILE)

$(serv02Target) : $(serv02Objects)
	$(CC) -o  $(serv02Target) $(serv02Objects) $(STATIC_FILE)

$(objects) : %.o : %.c
	@echo "\n"$< "=>" $@
	$(CC) $(INC) -c $< -o $@

.PHONY : clean
clean :
		-rm $(targets) $(objects)
