#ifndef cusl_table_h
#define cusl_table_h

#include "common.h"
#include "value.h"

typedef struct {
  ObjString* key;
  Value value;
} Entry;

typedef struct {
	int count;
	int capacity;
	Entry* entries;
} Table;

void initTable(Table* table);
void freeTable(Table* table);
bool insert(Table* table, ObjString* key, Value value);
bool get(Table* table, ObjString* key, Value* value);
bool delete(Table* table, ObjString* key);
void tableAddAll(Table* from, Table* to);

#endif
