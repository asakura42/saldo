all:
	cc saldo.c -std=c99 -Os -DNDEBUG -g0 -s -Wall -o saldo

man:
	groff -man ./saldo.1 -Tpdf > saldo.pdf

clean:
	rm saldo

install: all
	cp saldo /usr/local/bin

uninstall:
	rm /usr/local/bin/saldo
	
help:
	@echo "Available targets: all, man, clean, install, uninstall, help"

.PHONY: all man clean install uninstall help
