CFLAGS := -std=c11 $(CFLAGS)

main: main.c
	$(CC) $(CFLAGS) -o main main.c

clean:
	rm main

.PHONY: clean
