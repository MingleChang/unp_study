CC = gcc
SRC_PATH = ../unp
INC = -I$(SRC_PATH)

STATIC_FILE = -L$(SRC_PATH) -lunp

vpath %.c $(SRC_PATH)

objects = main.o recv_v4.o recv_v6.o traceloop.o sig_alrm.o icmpcode_v4.o icmpcode_v6.o
targets = $(objects:%.o=%)

.PHONY : all
all : main
	@echo "\nCompleted!\n"

main : $(objects)
	@echo "\n"$< "=>" $@
	$(CC) -o $@ $(objects) $(STATIC_FILE)

# $(targets) : % : %.o
# 	@echo "\n"$< "=>" $@
# 	$(CC) -o $@ $< $(STATIC_FILE)

$(objects) : %.o : %.c
	@echo "\n"$< "=>" $@
	$(CC) $(INC) -c $< -o $@

.PHONY : clean
clean :
		-rm $(objects:%.o=%) $(objects)
