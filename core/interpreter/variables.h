//
// Copyright 2017 Timo Kloss
//
// This file is part of LowRes NX.
//
// LowRes NX is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LowRes NX is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LowRes NX.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef variables_h
#define variables_h

#include <stdio.h>
#include <stdbool.h>
#include "interpreter_config.h"
#include "value.h"

struct Core;
struct Interpreter;

struct SimpleVariable {
    int symbolIndex;
    uint8_t subLevel;
    uint8_t isReference:1;
    enum ValueType type;
    union Value v;
};

struct ArrayVariable {
    int symbolIndex;
    uint8_t subLevel;
    uint8_t isReference:1;
    enum ValueType type;
    int numDimensions;
    int dimensionSizes[MAX_ARRAY_DIMENSIONS];
    union Value *values;
};

struct SimpleVariable *var_getSimpleVariable(struct Interpreter *interpreter, int symbolIndex, int subLevel);
struct SimpleVariable *var_createSimpleVariable(struct Interpreter *interpreter, enum ErrorCode *errorCode, int symbolIndex, int subLevel, enum ValueType type, union Value *valueReference);
void var_freeSimpleVariables(struct Interpreter *interpreter, int minSubLevel);

struct ArrayVariable *var_getArrayVariable(struct Interpreter *interpreter, int symbolIndex, int subLevel);
union Value *var_getArrayValue(struct Interpreter *interpreter, struct ArrayVariable *variable, int *indices);
struct ArrayVariable *var_dimVariable(struct Interpreter *interpreter, enum ErrorCode *errorCode, int symbolIndex, int numDimensions, int *dimensionSizes);
struct ArrayVariable *var_createArrayVariable(struct Interpreter *interpreter, enum ErrorCode *errorCode, struct ArrayVariable *arrayReference);
void var_freeArrayVariables(struct Interpreter *interpreter, int minSubLevel);

#endif /* variables_h */
