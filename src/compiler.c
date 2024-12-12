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

bool compile(const char* source, Chunk* chunk) {
	initScanner(source);
	
	parser.hadError = false;
	parser.panicMode = false;
	
	advance();
	expression();
	consume(TOKEN_EOF, "Expect end of expression.");
	
	return !parser.hadError;
}
