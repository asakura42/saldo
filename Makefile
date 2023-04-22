all:
	cc saldo.c -std=c99 -Os -DNDEBUG -g0 -s -Wall -o saldo

man:
	groff -man ./saldo.1 -Tpdf > saldo.pdf

clean:
	rm saldo

.PHONY: clean
