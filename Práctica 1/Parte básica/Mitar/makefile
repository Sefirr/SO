TARGET = mitar
CC = gcc
CFLAGS = -g -Wall 
OBJS = mitar.o rut_mitar.o
SOURCES = $(addsuffix .c, $(basename $(OBJS)))
HEADERS = mitar.h

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o: 
	$(CC) $(CFLAGS) -c  $< -o $@

$(OBJS): $(HEADERS)

clean: 
	-rm -f *.o $(TARGET) 
