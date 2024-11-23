CC = gcc
CFLAGS = -Wall -g

mymake: mymake.o graph.o parser.o safety.o
	$(CC) $(CFLAGS) -o mymake mymake.o graph.o parser.o safety.o 
	
mymake.o : mymake.c mymake.h
	$(CC) $(CFLAGS) -c mymake.c

graph.o : graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

parser.o : parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

safety.o : safety.c safety.h
	$(CC) $(CFLAGS) -c safety.c

# Build the test binary
test_safety: safety.c test_safety.c safety.h
	$(CC) $(CFLAGS) -o test_safety safety.c test_safety.c

test_graph: graph.c test_graph.c graph.h safety.c safety.h
	$(CC) $(CFLAGS) -o test_graph graph.c test_graph.c safety.c 

test_parser: parser.c parser.h test_parser.c safety.c safety.h graph.c graph.h
	$(CC) $(CFLAGS) -o test_parser parser.c test_parser.c safety.c graph.c

# Run tests
test: test_safety test_graph test_parser
	./test_safety
	./test_graph
	./test_parser

clean:
	rm -rf mymake test_safety test_graph test_parser test_graph_output.txt*.o *.dSYM
	rm -f tests/outputs/*
