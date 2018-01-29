.PHONY: clean

CC = gcc
FLAGS = --std=c99 -Wall
RM = rm

EXE = simple.elf
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

$(EXE) : $(OBJS)
	$(CC) $(FLAGS) -o $@ $^
%.o: %.c
	$(CC) $(FLAGS) -o $@ -c $^
clean:
	$(RM) -fr $(EXE) $(OBJS)

