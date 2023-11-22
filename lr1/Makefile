# Compiler and flags
CC = mpicc
PRE_CFLAGS = -Wall
CFLAGS = -lmpiP -lmpi -lmpe -lm
EXEC = employee.o
SRC = main.c employee_filtering.c localmpi.c
MPIP = -c -p -y -t 1.0 -k 2 -f ./logs
FI_PROVIDER = sockets

all: $(SRC)
	$(CC) $(PRE_CFLAGS) $(SRC) -o $(EXEC) $(CFLAGS)

run:
	mpirun -np $(NP) --host fedora:$(NP) \
	       -x FI_PROVIDER=$(FI_PROVIDER) ./$(EXEC)

qrun:
	mpirun -np $(NP) --host fedora:$(NP) \
	       -x FI_PROVIDER=$(FI_PROVIDER) ./$(EXEC) 2> /dev/null

debug:
	mpirun -np 2 alacritty -e gdb \
	       -x FI_PROVIDER=$(FI_PROVIDER) ./$(EXEC)

prof:
	mpirun -np $(NP) --host fedora:$(NP) \
		-x MPIP="$(MPIP)" \
		-x FI_PROVIDER=$(FI_PROVIDER) ./$(EXEC)

gen_records:
	ruby generator.rb > employees.csv

parse_logs:
	ruby parser.rb

clean:
	rm -f $(EXEC)

.PHONY: all run debug init clean

