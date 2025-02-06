all: sim.c
	gcc -std=gnu99 -Wall -Wextra -Werror sim.c -o sim -lm

run: sim.c
	gcc -std=gnu99 -Wall -Wextra -Werror sim.c -o sim -lm
	./sim -p 285
	./sim -p 385
	./sim -p 450
	./sim -p 450 -c 68
	./sim -p 450 -c 28
	./sim -p 450 -c 28 -b 30
	./sim -p 450 -c 28 -b 671