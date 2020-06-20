#ifndef LS_DEF_H
#define LS_DEF_H

#include "string"
#include "map"
#include "vector"
#include "queue"
#include "list"

#define LS_OK 0
#define LS_FAILED -1
#define LS_NOT_FOUND -2
#define LS_INCOMPLETE -3
#define LS_LENGTH_REQUIRED -4
#define LS_NOT_IMPLEMENTED -5

typedef std::string Text;
typedef std::list<void *> Args;

extern std::map<int, Text> errorCode;
extern std::map<Text, Text> messageMapper;


#endif
