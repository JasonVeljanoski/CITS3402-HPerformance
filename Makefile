# A Makefile to build the sparse matrix scripts
PROJECT =  sparse_matrices
HEADERS =  $(PROJECT).h
OBJ     =  sparse_matrices.o file_reader.o debug.o general.o sparse_format.o matrix_operations.o scalar_multiply.o trace.o addition.o transpose.o

C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror

$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm	

%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<	

clean :
	rm -f $(PROJECT) $(OBJ)