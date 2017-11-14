```
TARGET=fun file
OBJS=fun.o file.o

CC=gcc
GFLAGS=-Wall

$(TARGET):$(OBJS)

	$(CC) -o $@ $^

clean:
	rm -rf $(TARGET) $(OBJS)
```
