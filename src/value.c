#include <stdlib.h>

#include "memory.h"
#include "value.h"

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
	printf("%g", value);
}
