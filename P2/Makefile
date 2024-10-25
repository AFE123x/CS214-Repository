CC = gcc
CFLAGS =  -fsanitize=address,undefined,leak -std=c99 -O2 -Wall -g
spchk: spchk.o tries.o parser.o
	$(CC) -o $@ $^ $(CFLAGS)
#for parser testing
spchk.o: spchk.c
	$(CC) -c $^ $(CFLAGS)
parser.o: parser.c
	$(CC) -c parser.c $(CFLAGS)
# for the tries testing
triestest: triestest.o tries.o
	$(CC) -o $@ $^ $(CFLAGS)
triestest.o: triestest.c
	$(CC) -c $< $(CFLAGS)
tries.o: tries.c
	$(CC) -c $< $(CFLAGS)
clean: triestest spchk
	rm *.o $^