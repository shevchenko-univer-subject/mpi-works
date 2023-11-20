# Compiler and flags
CC = mpicc
PRE_CFLAGS = -Wall
CFLAGS = -lmpiP -lmpi -lmpe -lm
EXEC = employee.o
SRC = main.c employee_filtering.c localmpi.c
MPIP = -c -p -y -t 1.0 -k 2 -f ./logs

all: $(SRC)
	$(CC) $(PRE_CFLAGS) $(SRC) -o $(EXEC) $(CFLAGS)

run:
	mpirun -np $(NP) --host fedora:$(NP) ./$(EXEC)

qrun:
	mpirun -np $(NP) --host fedora:$(NP) ./$(EXEC) 2> /dev/null

debug:
	mpirun -np 2 alacritty -e gdb ./$(EXEC)

prof:
	mpirun -np $(NP) --host fedora:$(NP)  -x MPIP="$(MPIP)" ./$(EXEC)

init:
	export FI_PROVIDER=sockets
	export LD_PRELOAD=/usr/local/lib

gen_records:
	ruby generator.rb > employees.csv

clean:
	rm -f $(EXEC)

.PHONY: all run debug init clean

