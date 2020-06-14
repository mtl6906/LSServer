#include "JsonApplication.h"
#include "LSJson.h"

using std::map;

namespace ls
{
	JsonApplication::JsonApplication(JsonParse *jp) : jp(jp)
	{

	}
	int JsonApplication::ParseFrom(Text &text)
	{
		Json root;
		root.ParseFrom(text);
		jp -> ParseFrom(root);
	}
	void JsonApplication::ParseTo(Text &text)
	{
		Json root;
		jp -> ParseTo(root);
		root.ParseTo(text);
	}
}
