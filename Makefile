CC = gcc
INC = -IUtils
SRC_PATH = Utils

# vpath %.h Utils
vpath %.c $(SRC_PATH)

objects = main.o sum.o utils.o

.PHONY : all
all : main
	@echo "\nCompleted!\n"

main : $(objects)
		$(CC) -o main $(objects)

$(objects) : %.o : %.c
	@echo "\n"$< "=>" $@ 
	$(CC) $(INC) -c $< -o $@

.PHONY : clean
clean : 
		-rm main $(objects)