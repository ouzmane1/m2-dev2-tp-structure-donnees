CC = gcc
CFLAGS = -Wall -Wextra -O2
SRCS = dynamic_array.c linked_list.c hash_table.c main.c
OBJS = $(SRCS:.c=.o)
TARGET = tp_benchmark

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)