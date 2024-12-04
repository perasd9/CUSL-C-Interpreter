#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


int main(int argc, char *argv[]) {
	initVM();
	
	Chunk chunk;
	
	initChunk(&chunk);
	
	int constant = addConstant(&chunk, 1);
	
	writeChunk(&chunk, OP_CONSTANT, 10);
	writeChunk(&chunk, constant, 10);
	
	constant = addConstant(&chunk, 3);
	
	writeChunk(&chunk, OP_CONSTANT, 10);
	writeChunk(&chunk, constant, 10);
	
	writeChunk(&chunk, OP_ADD, 10);
	
	constant = addConstant(&chunk, 8);
	
	writeChunk(&chunk, OP_CONSTANT, 10);
	writeChunk(&chunk, constant, 10);
	
	writeChunk(&chunk, OP_DIVIDE, 10);
	
	writeChunk(&chunk, OP_NEGATE, 123);
	
	writeChunk(&chunk, OP_RETURN, 10);
	
	disassembleChunk(&chunk, "test_chunk");
	
	interpret(&chunk);
	
	freeVM();
	freeChunk(&chunk);
	
	return 0;
}


