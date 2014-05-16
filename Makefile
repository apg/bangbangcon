CFLAGS = -Wall -g

all: naive

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

naive: vm.o naive.o run.o
	$(CC) $(CFLAGS) -o $@ $^

onepass: vm.o onepass.o run.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o onepass naive

.PHONY:
	clean
