# A Makefile to build the game server

PROJECT =  basic_server
HEADERS =  $(PROJECT).h
OBJ     =  basic_server.o game_logic.o debug.o processing.o

C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror

$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm	

%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<	

clean :
	rm -f $(PROJECT) $(OBJ)