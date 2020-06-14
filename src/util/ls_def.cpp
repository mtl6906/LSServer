#include "ls_def.h"

std::map<int, Text> errorCode = {
	{LS_OK, "200"},
	{LS_NOT_FOUND, "404"},
	{LS_FAILED, "400"},
	{LS_LENGTH_REQUIRED, "411"},
	{LS_NOT_IMPLEMENTED, "501"}
};

std::map<Text, Text> messageMapper = {
	{"200", "OK"},
	{"400", "Bad Request"},
	{"404", "Not Found"},
	{"411", "Length Required"},
	{"501", "Not Implemented"}
};
