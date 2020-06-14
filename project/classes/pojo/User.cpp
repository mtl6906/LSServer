#include "User.h"
#include "ls_error.h"

using ls::Json;
using ls::StringItem;
using ls::IntItem;

User::User() : id(-1)
{

}

int &User::Id()
{
	return id;
}

int &User::Age()
{
	return age;
}

ls::Text &User::Name()
{
	return name;
}

void User::ParseTo(Json &root)
{
	FUNCTION_TRACE();
	IntItem(id).AddTo("id", root);
	FUNCTION_TRACE();
	IntItem(age).AddTo("age", root);
	FUNCTION_TRACE();
	StringItem(name).AddTo("name", root);
	FUNCTION_TRACE();
}

int User::ParseFrom(Json &root)
{
	return (GET_INT(root, "id", id) ||
		GET_INT(root, "age", age) ||
		GET_STRING(root, "name", name)) ? LS_OK : LS_NOT_FOUND;
}
