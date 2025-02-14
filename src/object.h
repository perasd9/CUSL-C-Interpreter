#ifndef cusl_object_h
#define cusl_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"
#include "table.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)
#define IS_CLASS(value) isObjType(value, OBJ_CLASS)
#define IS_INSTANCE(value) isObjType(value, OBJ_INSTANCE)

#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
#define AS_NATIVE(value) (((ObjNativeFunction*)AS_OBJ(value))->function)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define AS_CLOSURE(value) ((ObjClosure*)AS_OBJ(value))
#define AS_CLASS(value) ((ObjClass*)AS_OBJ(value))
#define AS_INSTANCE(value) ((ObjInstance*)AS_OBJ(value))

typedef enum {
	OBJ_STRING,
	OBJ_FUNCTION,
	OBJ_NATIVE,
	OBJ_CLOSURE,
	OBJ_UPVALUE,
	OBJ_CLASS,
	OBJ_INSTANCE,
} ObjType;

struct Obj {
	ObjType type;
	Obj* next;
	
	bool isMarked;
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

typedef struct {
	Obj obj;
	ObjString* name;
	
	Table methods;
} ObjClass;

typedef struct {
	Obj obj;
	ObjClass* clas;
	Table fields;
} ObjInstance;

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
ObjClass* newClass(ObjString* name);
ObjInstance* newInstance(ObjClass* clas);

static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
