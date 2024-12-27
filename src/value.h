#ifndef cusl_value_h
#define cusl_value_h

#include "common.h"

typedef enum {
	VALUE_BOOL,
	VALUE_NIL,
	VALUE_NUMBER
} ValueType;

typedef struct {
	ValueType type;
	union {
		bool boolean;
		double number;
	} as;
} Value;

#define IS_BOOL(value) ((value.type) == VALUE_BOOL)
#define IS_NIL(value) ((value.type) == VALUE_NIL)
#define IS_NUMBER(value) ((value.type) == VALUE_NUMBER)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define BOOL_VAL(value) ((Value){VALUE_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VALUE_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VALUE_NUMBER, {.number = value}})

typedef struct {
	int capacity;
	int count;
	Value* values;
} ValueArray;

bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif
