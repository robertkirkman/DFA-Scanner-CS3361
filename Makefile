scanner: scanner.c automaton.c scan.c
	gcc -std=gnu99 -pedantic -Wall -Wextra -Wshadow -Wpointer-arith \
		-Wstrict-prototypes -Wmissing-prototypes -o scanner scanner.c \
		automaton.c scan.c -lm
