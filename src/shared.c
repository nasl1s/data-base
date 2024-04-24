#include <stdio.h>
#include <string.h>

#include "database.h"

#ifndef ENTITY
#define ENTITY LEVELS
DEF_SELECT(ENTITY);
DEF_INSERT(ENTITY);
DEF_UPDATE(ENTITY);
DEF_DELETE(ENTITY);
#endif