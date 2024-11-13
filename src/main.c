#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, char *argv[]) {
	Chunk chunk;
	
	initChunk(&chunk);
	
	int constant = addConstant(&chunk, 1.2);
	
	
	writeChunk(&chunk, OP_CONSTANT, 10);
	writeChunk(&chunk, constant, 10);
	
	writeChunk(&chunk, OP_RETURN, 10);
	
	disassembleChunk(&chunk, "test_chunk");
	
	freeChunk(&chunk);
	
	
	return 0;
}


