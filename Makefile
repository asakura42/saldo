all:
	cc -o saldo saldo.c

man:
	groff -man ./saldo.1 -Tpdf > saldo.pdf

clean:
	rm saldo

.PHONY: clean
