CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = simulator
OBJS = main.o memory.o utils.o pagetable_1lvl.o pagetable_2lvl.o pagetable_inv.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
