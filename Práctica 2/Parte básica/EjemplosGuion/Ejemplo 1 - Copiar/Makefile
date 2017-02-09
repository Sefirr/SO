CC = gcc
CFLAGS = -Wall -g

OBJETOS = copiar.o
PROG = copiar

all : $(PROG) 

$(PROG) : $(OBJETOS)
	gcc -g -o $(PROG) $(OBJETOS)
	
%.o : %.c 
	gcc -c $(CFLAGS) $<

clean :
	-rm $(OBJETOS) $(PROG) 
