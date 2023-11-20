# Compiler and flags
CC = mpicc
PRE_CFLAGS = -Wall
CFLAGS = -lmpiP -lmpi -lmpe -lm
EXEC = employee.o
SRC = main.c employee_filtering.c localmpi.c
MPIP = -c -p -y -t 1.0 -k 2 -f ./logs

# Build the MPI program
all: $(SRC)
	$(CC) $(PRE_CFLAGS) $(SRC) -o $(EXEC) $(CFLAGS)

# Run the MPI program
# Usage: make run ARG=<number_of_processes>
run:
	mpirun -np $(NP) --host fedora:$(NP) ./$(EXEC)

qrun:
	mpirun -np $(NP) --host fedora:$(NP) ./$(EXEC) 2> /dev/null

# Debug the MPI program
debug:
	mpirun -np 2 alacritty -e gdb ./$(EXEC)

prof:
	mpirun -np $(NP) --host fedora:$(NP)  -x MPIP="$(MPIP)" ./$(EXEC)

# Initialize the MPI environment
init:
	export FI_PROVIDER=sockets
	export LD_PRELOAD=/usr/local/lib

# Clean up
clean:
	rm -f $(EXEC)

.PHONY: all run debug init clean

