CC=gcc
MPICC=mpicc
PREFLAGS=-Wall
CFLAGS=-lmpiP -lmpi -lmpe -lm
SRC= multiplication.c localmpi.c

EXEC_GNU=g_multiplication.o
EXEC_MPI=mpi_multiplication.o

GNU_MAIN=g_main.c
MPI_MAIN=mpi_main.c

MPIP = -c -p -y -t 1.0 -k 2 -f ./logs
FI_PROVIDER = sockets


## ACTIONS

all: gnu mpi

gnu: $(SRC) $(GNU_MAIN)
	$(CC) $(PREFLAGS) $(SRC) $(GNU_MAIN) -o $(EXEC_GNU)

gnu_run: $(EXEC_GNU)
	./$(EXEC_GNU)


mpi: $(SRC) $(MPI_MAIN)
	$(MPICC) $(PREFLAGS) $(SRC) $(MPI_MAIN) -o $(EXEC_MPI) $(CFLAGS)

mpi_run: $(EXEC_MPI)
	mpirun -np $(NP) --host fedora:$(NP) \
	       -x FI_PROVIDER=$(FI_PROVIDER) ./$(EXEC_MPI)

mpi_debug:
	mpirun -np 2 alacritty -e gdb \
	       -x FI_PROVIDER=$(FI_PROVIDER) ./$(EXEC_MPI)

mpi_prof:
	mpirun -np $(NP) --host fedora:$(NP) \
		-x MPIP="$(MPIP)" \
		-x FI_PROVIDER=$(FI_PROVIDER) ./$(EXEC_MPI)


parse_logs:
	ruby parser.rb

gen_records:
	ruby generate_matrix.rb $(name) $(size)

clean:
	rm -f $(EXEC_GNU) $(EXEC_MPI)

.PHONY: all gnu mpi clean
