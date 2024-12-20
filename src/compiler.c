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

typedef enum {
	PREC_NONE,
	PREC_ASSIGNMENT,
	PREC_OR,
	PREC_AND,
	PREC_EQUALITY,
	PREC_COMPARISON,
	PREC_TERM,
	PREC_FACTOR,
	PREC_UNARY,
	PREC_CALL,
	PREC_PRIMARY,
} Precedence;

typedef void (*ParseFn)();

typedef struct {
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
} ParseRule;

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

static void error(const char* message) {
	errorAt(&parser.previous, message);
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
		
		return 0;
	}
	
	return (uint8_t)constant;
}

static void emitConstant(Value value) {
	emitBytes(OP_CONSTANT, makeConstant(value));
}

static void number() {
	double value = strtod(parser.previous.start, NULL);
	
	emitConstant(value);
}

static void parsePrecedence(Precedence precedence) {
	
}

static void expression() {
	parsePrecedence(PREC_ASSIGNMENT);
	
	
}

static void grouping() {
	expression();
	
	consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary() {
	TokenType operatorType = parser.previous.type;
	
	parsePrecedence(PREC_UNARY);
	
	switch(operatorType) {
		case TOKEN_MINUS: emitByte(OP_NEGATE); break;
		
		default: return;
	}
}

static void binary() {
	TokenType operatorType = parser.previous.type;
	
	ParseRule* rule = getRule(operatorType);
	
	parsePrecedence((Precedence)(rule->precedence + 1));
	
	switch(operatorType) {
		case TOKEN_PLUS: emitByte(OP_ADD); break;
		case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
		case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
		case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
		
		default: return;
	}
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
