all: minHeap.o huffman.o BST.o
	gcc -o fileCompressor fileCompressor.c minHeap.o huffman.o BST.o
BST.o: BST.c
	gcc -g -c BST.c
minHeap.o: minHeap.c
	gcc -g -c minHeap.c
huffman.o: huffman.c
	gcc -g -c huffman.c
gdb: minHeap.o huffman.o BST.o
	gcc -g fileCompressor.c minHeap.o huffman.o BST.o -o f
clean:
	rm huffman.o; rm minHeap.o; rm fileCompressor; rm HuffmanCodebook; rm BST.o	
gdbClean:
	rm huffman.o; rm minHeap.o; rm BST.o; rm f
