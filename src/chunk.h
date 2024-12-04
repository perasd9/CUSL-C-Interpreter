#ifndef cusl_chunk_h
#define cusl_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
	OP_CONSTANT,
	OP_RETURN,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_NEGATE

} OpCode;

typedef struct {
	int capacity;
	int count;
	uint8_t *code;
	int* lines;
	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);

#endif

