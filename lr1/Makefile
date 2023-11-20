# Compiler and flags
CC = mpicc
CFLAGS = -Wall -L/usr/local/lib -lmpiP -lmpi -lmpe -lm
EXEC = employee.o
SRC = main.c employee_filtering.c localmpi.c
MPIP = -c -p -y -t 1.0 -k 2 -f ./logs

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

prof:
	mpirun -np $(NP) -x MPIP="$(MPIP)" ./$(EXEC)

# Initialize the MPI environment
init:
	export FI_PROVIDER=sockets
	export LD_PRELOAD=/usr/local/lib/libmpiP.so

# Clean up
clean:
	rm -f $(EXEC)

.PHONY: all run debug init clean

