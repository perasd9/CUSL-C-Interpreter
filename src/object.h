#ifndef cusl_object_h
#define cusl_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)

#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
#define AS_NATIVE(value) (((ObjNativeFunction*)AS_OBJ(value))->function)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define AS_CLOSURE(value) ((ObjClosure*)AS_OBJ(value))

typedef enum {
	OBJ_STRING,
	OBJ_FUNCTION,
	OBJ_NATIVE,
	OBJ_CLOSURE,
	OBJ_UPVALUE,
} ObjType;

struct Obj {
	ObjType type;
	Obj* next;
};

typedef struct {
	Obj obj;
	int arity;
	Chunk chunk;
	ObjString* name;
	
	int upvalueCount;
} ObjFunction;

typedef struct ObjUpvalue {
	struct ObjUpvalue* next;
	
	Obj obj;
	Value* location;
	
	Value closed;
} ObjUpvalue;

typedef struct {
	Obj obj;
	ObjFunction* function;
	
	ObjUpvalue** upvalues;
	int upvalueCount;
} ObjClosure;

typedef Value (*NativeFn) (int argCount, Value* args);

typedef struct {
	Obj obj;
	NativeFn function;
} ObjNativeFunction;

struct ObjString{
	Obj obj;
	int length;
	char* chars;
	uint32_t hash;
};

ObjString* copyString(const char* chars, int length);
void printObject(Value value);
ObjString* takeString(char* chars, int length);
ObjFunction* newFunction();
ObjNativeFunction* newNativeFn(NativeFn function);
ObjClosure* newClosure(ObjFunction* function);
ObjUpvalue* newUpvalue(Value* slot);

static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
