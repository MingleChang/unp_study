CC = gcc
SRC_PATH = ../unp
INC = -I$(SRC_PATH)

STATIC_FILE = -L$(SRC_PATH) -lunp

vpath %.c $(SRC_PATH)

objects = tcprecv01.o tcpsend01.o tcprecv02.o tcprecv03.o
targets = $(objects:%.o=%)

.PHONY : all
all : $(targets)
	@echo "\nCompleted!\n"
	-rm $(objects)

$(targets) : % : %.o
	@echo "\n"$< "=>" $@
	$(CC) -o $@ $< $(STATIC_FILE)

$(objects) : %.o : %.c
	@echo "\n"$< "=>" $@
	$(CC) $(INC) -c $< -o $@

.PHONY : clean
clean :
		-rm $(objects:%.o=%) $(objects)
