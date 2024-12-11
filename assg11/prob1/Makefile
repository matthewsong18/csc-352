CC = gcc
CFLAGS = -Wall -g
OBJ = bacon.o file_parser.o graph.o bfs.o queue.o utils.o

bacon: $(OBJ)
	$(CC) $(CFLAGS) -o bacon $(OBJ)

bacon.o: bacon.c file_parser.h graph.h bfs.h utils.h
	$(CC) $(CFLAGS) -c bacon.c
file_parser.o: file_parser.c file_parser.h graph.h
	$(CC) $(CFLAGS) -c file_parser.c
graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c
bfs.o: bfs.c bfs.h queue.h graph.h
	$(CC) $(CFLAGS) -c bfs.c
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

test: test_file_parser
	./test_file_parser

test_file_parser: test_file_parser.c file_parser.o graph.o
	$(CC) $(CFLAGS) -o test_file_parser test_file_parser.c file_parser.o graph.o

clean:
	rm -f *.o bacon
	rm -f test_file_parser
	rm -f -r *.dSYM
