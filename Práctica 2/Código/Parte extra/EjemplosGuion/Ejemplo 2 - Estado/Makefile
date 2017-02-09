CC = gcc
CFLAGS = -Wall -g

OBJETOS = estado.o
PROG = estado

all : $(PROG) 

$(PROG) : $(OBJETOS)
	gcc -g -o $(PROG) $(OBJETOS)
	
%.o : %.c 
	gcc -c $(CFLAGS) $<

clean :
	-rm $(OBJETOS) $(PROG) 	
