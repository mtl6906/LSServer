#ifndef LS_DEF_H
#define LS_DEF_H

#include "string"
#include "map"
#include "vector"
#include "queue"

#define LS_OK 0
#define LS_FAILED -1
#define LS_NOT_FOUND -2
#define LS_INCOMLETE -3

class Session;

typedef std::string Text;
typedef std::queue<void *> Args;

extern std::map<int, Text> errorCode;
extern std::map<Text, Text> messageMapper;



#endif
