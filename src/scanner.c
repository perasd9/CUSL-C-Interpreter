#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
	const char* start;
	const char* current;
	int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

Token makeToken(TokenType type) {
	Token token;
	
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	
	token.type = type;
	token.line = scanner.line;
	
	return token;
}

Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	
	token.start = message;
	token.length = (int)strlen(message);
	
	token.line = scanner.line;
	
	return token;
}


//----------------------going through source code
static char advance() {
	scanner.current++;
	
	return scanner.current[-1];
}

static bool match(char expected) {
	if(isAtEnd()) return false;
	
	
	return *scanner.current++ == expected;
}

static char peek() {
	return *scanner.current;
}

static void skipWhitespace() {
	for(;;) {
		char c = peek();
		
		switch(c) {
			case ' ':
			case '\r':
			case '\t':
				advance();
				break;
			case '\n':
				scanner.line++;
				advance();
				break;
			case '/':
				if(peekNext() == '/') {
					while(peek() != '\n' && !isAtEnd()) advance();
				} else {
					return;
				}
				break;
			default: return;
		}
	}
}

static char peekNext() {
	if(isAtEnd()) return '\0';
	
	return scanner.current[1];
}


//-------------------------creating types
static Token string() {
	while(peek() != '"' && !isAtEnd()) {
		if(peek() == '\n') scanner.line++;
		advance();
	}
	
	if(isAtEnd()) return errorToken("Unterminated string.");
	
	//for closing quote
	advance();
	
	return makeToken(TOKEN_STRING);
}

static Token number() {
	while(isDigit(peek()) && !isAtEnd()) {
		advance();
	}
	
	if(peek() == '.' && isDigit(peekNext())) {
		advance();
		
		while(isDigit(peek())) advance();
	}
	
	return makeToken(TOKEN_NUMBER);
}

static TokenType identifierType() {
	return TOKEN_IDENTIFIER;
}
static Token identifier() {
	while(isAlpha(peek()) || isDigit(peek())) advance();
	
	return makeToken(identifierType());
}


//-----------------------helper functions
static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || 
		   (c >= 'A' && c <= 'Z') || 
		    c == '-';
}

bool isAtEnd() {
	return *scanner.current == '\0';
}


//------------------------
Token scanToken() {
	skipWhitespace();
	
	scanner.start = scanner.current;
	
	if(isAtEnd()) return makeToken(TOKEN_EOF);
	
	char c = advance();
	
	if(isAlpha(c)) return identifier();
	if(isDigit(c)) return number();
	
	switch(c) {
		case '(': return makeToken(TOKEN_LEFT_PAREN);
		case ')': return makeToken(TOKEN_RIGHT_PAREN);
		case '{': return makeToken(TOKEN_LEFT_BRACE);
		case '}': return makeToken(TOKEN_RIGHT_BRACE);
		case ';': return makeToken(TOKEN_SEMICOLON);
		case ',': return makeToken(TOKEN_COMMA);
		case '.': return makeToken(TOKEN_DOT);
		case '-': return makeToken(TOKEN_MINUS);
		case '+': return makeToken(TOKEN_PLUS);
		case '/': return makeToken(TOKEN_SLASH);
		case '*': return makeToken(TOKEN_STAR);
		case '!': {
			if(match('=')) return makeToken(TOKEN_BANG_EQUAL);
			else return makeToken(TOKEN_BANG);
		}
		case '=': {
			if(match('=')) return makeToken(TOKEN_EQUAL_EQUAL);
			else return makeToken(TOKEN_EQUAL);
		}
		case '<': {
			if(match('=')) return makeToken(TOKEN_LESS_EQUAL);
			else return makeToken(TOKEN_LESS);
		}
		case '>': {
			if(match('=')) return makeToken(TOKEN_GREATER_EQUAL);
			else return makeToken(TOKEN_GREATER);
		}
		case '"': return string();
	}
	
	return errorToken("Unexpected character");
}


