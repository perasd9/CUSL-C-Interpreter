#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "value.h"
#include "object.h"

void initValueArray(ValueArray* array) {
	array->capacity = 0;
	array->count = 0;
	array->values = NULL;
}

void writeValueArray(ValueArray* array, Value value) {
	if(array->count + 1 > array->capacity) {
		int oldCapacity = array->capacity;
		
		array->capacity = GROW_CAPACITY(array->capacity);
		
		array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
	}
	
	array->values[array->count++] = value;
}

void freeValueArray(ValueArray* array) {
	FREE_ARRAY(Value, array->values, array->count);
	
	initValueArray(array);
}

void printValue(Value value) {
	switch(value.type) {
		case VALUE_BOOL:
			printf(AS_BOOL(value) ? "true" : "false");
			break;
		case VALUE_NIL: printf("nil"); break;
		case VALUE_NUMBER: printf("%g", AS_NUMBER(value)); break;
		case VALUE_OBJ: printObject(value); break;
	}
}

bool valuesEqual(Value a, Value b) {
	if(a.type != b.type) return false;
	
	switch(a.type) {
		case VALUE_BOOL: return AS_BOOL(a) == AS_BOOL(b);
		case VALUE_NIL: return true;
		case VALUE_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
		case VALUE_OBJ: return AS_OBJ(a) == AS_OBJ(b);
		default: return false;
	}
}
