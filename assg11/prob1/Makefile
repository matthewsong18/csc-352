CC = gcc
CFLAGS = -Wall -g
OBJ = bacon.o file_parser.o graph.o bfs.o queue.o utils.o

# Main bacon program
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

utils.o: utils.c utils.h bfs.h graph.h queue.h
	$(CC) $(CFLAGS) -c utils.c

# Test all components
test: test_file_parser test_graph test_queue test_bfs test_utils 
	./test_file_parser
	./test_graph
	./test_queue
	./test_bfs
	./test_utils

# Individual test components
test_file_parser: test_file_parser.c file_parser.o
	$(CC) $(CFLAGS) -o test_file_parser test_file_parser.c file_parser.o

test_graph: test_graph.c file_parser.o graph.o
	$(CC) $(CFLAGS) -o test_graph test_graph.c file_parser.o graph.o

test_queue: test_queue.c queue.o graph.o
	$(CC) $(CFLAGS) -o test_queue test_queue.c queue.o graph.o

test_bfs: test_bfs.c file_parser.o graph.o bfs.o queue.o
	$(CC) $(CFLAGS) -o test_bfs test_bfs.c file_parser.o graph.o bfs.o queue.o

test_utils: test_utils.c graph.o bfs.o queue.o utils.o
	$(CC) $(CFLAGS) -o test_utils test_utils.c graph.o bfs.o queue.o utils.o

# Clean up object files and executables
clean:
	rm -f *.o bacon
	rm -f test_file_parser
	rm -f test_graph
	rm -f test_queue
	rm -f test_bfs
	rm -f test_utils
	rm -f -r *.dSYM

