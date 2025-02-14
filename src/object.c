#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"
#include "table.h"

#define ALLOCATE_OBJ(type, objType) \
	(type*)allocateObject(sizeof(type), objType)


static ObjString* allocateString(char* chars, int length, uint32_t hash);

static uint32_t hashString(const char* key, int length) {
	uint32_t hash = 2166136261u;
	
	for(int i = 0; i < length; i++) {
		hash ^= (uint8_t)key[i];
		hash *= 16777619;
	}
	
	return hash;
}

ObjString* copyString(const char* chars, int length) {
	uint32_t hash = hashString(chars, length);
	
	ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
	
	if(interned != NULL) return interned;
	
	char* heapChars = ALLOCATE(char, length + 1);
	
	memcpy(heapChars, chars, length);
	
	heapChars[length] = '\0';
	
	return allocateString(heapChars, length, hash);
}
	
static Obj* allocateObject(size_t size, ObjType type) {
	Obj* object = (Obj*)reallocate(NULL, 0, size);
	
	object->type = type;
	object->isMarked = false;
	
	object->next = vm.objects;
	vm.objects = object;
	
	#ifdef DEBUG_LOG_GARBAGE_COLLECTOR
		printf("----- %p allocate %zu for %d \n", (void*)object, size, type);
	#endif
	
	return object;
}

static ObjString* allocateString(char* chars, int length, uint32_t hash) {
	ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
	
	string->length = length;
	string->chars = chars;
	string->hash = hash;
	
	push(OBJ_VAL(string));
	insert(&vm.strings, string, NIL_VAL);
	pop();
	
	return string;
}

static void printFunction(ObjFunction* function) {
	if(function->name == NULL) {
		printf("<script>");
		
		return;
	}
	printf("<fn %s>", function->name->chars);
}

void printObject(Value value) {
	switch(OBJ_TYPE(value)) {
		case OBJ_STRING: 
			printf("%s", AS_CSTRING(value));
			break;
		case OBJ_FUNCTION:
			printFunction(AS_FUNCTION(value));
			break;
		case OBJ_NATIVE:
			printf("<native fn>");
			break;
		case OBJ_CLOSURE:
			printFunction(AS_CLOSURE(value)->function);
			break;
		case OBJ_UPVALUE:
			printf("upvalue");
			break;
		case OBJ_CLASS:
			printf("%s", AS_CLASS(value)->name->chars);
			break;
		case OBJ_INSTANCE:
			printf("%s instance", AS_INSTANCE(value)->clas->name->chars);
			break;
	}
}

ObjString* takeString(char* chars, int length) {
	uint32_t hash = hashString(chars, length);
	
	ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
	
	if(interned != NULL) {
		FREE_ARRAY(char, chars, length + 1);
		
		return interned;
	}
	
	return allocateString(chars, length, hash);
}

ObjFunction* newFunction() {
	ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
	
	function->arity = 0;
	function->name = NULL;
	function->upvalueCount = 0;
	initChunk(&function->chunk);
	
	return function;
}

ObjNativeFunction* newNativeFn(NativeFn function) {
	ObjNativeFunction* native = ALLOCATE_OBJ(ObjNativeFunction, OBJ_NATIVE);
	
	native->function = function;
	
	return native;
}

ObjClosure* newClosure(ObjFunction* function) {
	ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
	
	for(int i = 0; i < function->upvalueCount; i++) {
		upvalues[i] = NULL;
	}
	
	ObjClosure* closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
	
	closure->function = function;
	closure->upvalues = upvalues;
	closure->upvalueCount = function->upvalueCount;
	
	return closure;
}

ObjUpvalue* newUpvalue(Value* slot) {
	ObjUpvalue* upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
	
	upvalue->location = slot;
	upvalue->next = NULL;
	upvalue->closed = NIL_VAL;
	
	return upvalue;
}

ObjClass* newClass(ObjString* name) {
	ObjClass* clas = ALLOCATE_OBJ(ObjClass, OBJ_CLASS);
	
	clas->name = name;
	initTable(&clas->methods);
	
	return clas;
}

ObjInstance* newInstance(ObjClass* clas) {
	ObjInstance* instance = ALLOCATE_OBJ(ObjInstance, OBJ_INSTANCE);
	
	instance->clas = clas;
	initTable(&instance->fields);
	
	return instance;
}
