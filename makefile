CC = gcc
CFLAGS = -Werror -Wall -g -std=gnu99
LDFLAGS = -lrt -lpthread
TARGET = 160640950_xxxxxxxxx_a04 
OBJFILES = 160640950_xxxxxxxxx_a04.o 
all: $(TARGET)
160640950_xxxxxxxxx_a04: 160640950_xxxxxxxxx_a04.c
	$(CC) $(CFLAGS) -o 160640950_xxxxxxxxx_a04 160640950_xxxxxxxxx_a04.c $(LDFLAGS)

run: 160640950_xxxxxxxxx_a04
	./160640950_xxxxxxxxx_a04
clean:
	rm -f $(OBJFILES) $(TARGET) *~ 