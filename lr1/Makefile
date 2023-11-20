# Compiler and flags
CC = mpicc
CFLAGS = -Wall -L/usr/local/lib -lmpiP -lmpi -lmpe -lm
EXEC = employee.o
SRC = main.c employee_filtering.c

# Build the MPI program
all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

# Run the MPI program
# Usage: make run ARG=<number_of_processes>
run:
	mpirun -np $(NP) ./$(EXEC)

qrun:
	mpirun -np $(NP) ./$(EXEC) 2> /dev/null

# Debug the MPI program
debug:
	mpirun -np 2 alacritty -e gdb ./$(EXEC)

# Initialize the MPI environment
init:
	export FI_PROVIDER=sockets
	export MPIP="-c -p -y -t 1.0 -k 2 -f ./logs"

# Clean up
clean:
	rm -f $(EXEC)

.PHONY: all run debug init clean

