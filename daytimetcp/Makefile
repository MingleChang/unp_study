CC = gcc
SRC_PATH = ../unp
INC = -I$(SRC_PATH)

STATIC_FILE = -L$(SRC_PATH) -lunp

vpath %.c $(SRC_PATH)

objects = client.o clientv6.o server.o byteorder.o
targets = $(objects:%.o=%)

.PHONY : all
all : $(targets)
	@echo "\nCompleted!\n"

$(targets) : % : %.o
	@echo "\n"$< "=>" $@ 
	$(CC) -o $@ $< $(STATIC_FILE)

$(objects) : %.o : %.c
	@echo "\n"$< "=>" $@ 
	$(CC) $(INC) -c $< -o $@

.PHONY : clean
clean : 
		-rm $(objects:%.o=%) $(objects)