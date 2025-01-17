#ifndef cusl_compiler_h
#define cusl_compiler_h

#include "vm.h"
#include "object.h"

ObjFunction* compile(const char* source);

#endif
