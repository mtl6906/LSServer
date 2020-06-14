#ifndef LSJSON_H
#define LSJSON_H

#define GET_INT(json, key, value) json.Get(key, IntItem(value))
#define GET_STRING(json, key, value) json.Get(key, StringItem(value))
#define GET_BOOL(json, key, value) json.Get(key, BoolItem(value))
#define GET_ARRAY(json, key, value) json.Get(key, ArrayItem(value))
#define GET_JSON(json, key, value) json.Get(key, value)

#include "JsonType.h"
#include "JsonParse.h"
#include "IntItem.h"
#include "BoolItem.h"
#include "StringItem.h"
#include "ArrayItem.h"
#include "Json.h"

#endif

