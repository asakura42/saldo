all:
	gcc -o saldo saldo.c

clean:
	rm -f saldo

.PHONY: clean
