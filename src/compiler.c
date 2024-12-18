#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
	Token previous;
	Token current;
	bool hadError;
	bool panicMode;
} Parser;

Parser parser;
Chunk* chunk;

static void errorAt(Token* current, const char* message) {
	if(parser.panicMode) return;
	parser.panicMode = true;	
	
	fprintf(stderr, "[line %d] Error", current->line);
	
	if(current->type == TOKEN_EOF) {
		fprintf(stderr, " at end.");	
	} else if(current->type == TOKEN_ERROR) {
		
	} else {
		fprintf(stderr, " at '%.*s'", current->length, current->start);
	}
	
	fprintf(stderr, ": %s\n", message);
	
	parser.hadError = true;
}

static void errorAtCurrent(const char* message) {
	errorAt(&parser.current, message);
}

static void advance() {
	parser.previous = parser.current;
	
	for(;;) {
		parser.current = scanToken();
		if(parser.current.type != TOKEN_ERROR) break;
		
		errorAtCurrent(parser.current.start);
	}
}

static void consume(TokenType type, const char* message) {
	if(parser.current.type == type) {
		advance();
		
		return;
	}
	
	errorAtCurrent(message);
}

static Chunk* currentChunk() {
	return chunk;
}

static void emitByte(uint8_t byte) {
	writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
	emitByte(byte1);
	emitByte(byte2);
}

static void emitReturn() {
	emitByte(OP_RETURN);
}

static void endCompiler() {
	emitReturn();
}

static uint8_t makeConstant(Value value) {
	int constant = addConstant(currentChunk(), value);
	
	if(constant > UINT8_MAX) {
		error("Too many constants in one chunk.");
	}
}

static void emitConstant(Value value) {
	emitBytes(OP_CONSTANT, makeConstant(value));
}

static void number() {
	double value = strtod(parser.previous.start, NULL);
	
	emitConstant(value);
}

static void expression() {
	
}

bool compile(const char* source, Chunk* chnk) {
	initScanner(source);
	chunk = chnk;
	
	parser.hadError = false;
	parser.panicMode = false;
	
	advance();
	expression();
	consume(TOKEN_EOF, "Expect end of expression.");
	
	endCompiler();
	
	return !parser.hadError;
}
