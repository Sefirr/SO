OBJDIR := obj
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix $(OBJDIR)/,$(notdir $(C_FILES:.c=.o)))

CFLAGS := -g -Wall $(shell pkg-config fuse --cflags)
#LDFLAGS := -lreadline $(shell pkg-config fuse --libs)
LDFLAGS := $(shell pkg-config fuse --libs)

TARGET = sf-fuse

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^
	
obj/%.o: src/%.c
	mkdir -p $(OBJDIR)
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f ./obj/*.o
	rm -f $(TARGET)
