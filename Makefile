bf: bf.c
	mkdir -p bin/
	gcc -std=c99 bf.c -o bin/bf
	gcc -std=c99 bfi.c -o bin/bfi
