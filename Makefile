all: edges.c nodes.c algo.c main.c edges.h nodes.h algo.h
	gcc -g -I. edges.c nodes.c algo.c main.c -o graph

clean:
	rm -f graph
