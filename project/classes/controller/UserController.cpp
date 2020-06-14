#include "UserController.h"
#include "ls_def.h"
#include "ls_error.h"

using std::vector;
using ls::Json;
using ls::ArrayItem;

int UserController::GetAllUsers(Json &root)
{
	FUNCTION_TRACE();
//	获取pojo对象数组
	auto users = us.FindUsers();
	if(users.size() == 0)
		return LS_NOT_FOUND;
//	转化为Text数组
	vector<Text> userTextList(users.size());
	int i = 0;
	FUNCTION_TRACE();
	for(auto &user : users)
	{
		Json root;
		FUNCTION_TRACE();
		user.ParseTo(root);
		FUNCTION_TRACE();
		root.ParseTo(userTextList[i]);
		FUNCTION_TRACE();
		++i;
	}
//	构建json对象
	FUNCTION_TRACE();
	ArrayItem(userTextList).AddTo("users", root);
	FUNCTION_TRACE();
	return LS_OK;
}

int UserController::GetUserById(int id, Json &root)
{
//	获取pojo对象
	User user = us.FindUserById(id);
	if(user.Id() == -1)
		return LS_NOT_FOUND;
//	转化为Text
	Json userJson;
       	user.ParseTo(userJson);
//	构建json对象
	userJson.AddTo("user", root);
	return LS_OK;
}

int UserController::AddUser(Json &root)
{
	Json user;
       	GET_JSON(root, "user", user);
//	转化为pojo对象
	User uo;
	uo.ParseFrom(user);
//	存储对象
	return us.AddUser(uo);
}

int UserController::RemoveUserById(int id)
{
	return us.RemoveUserById(id);	
}

int UserController::ModifyUser(Json &root)
{
	Json user;
	GET_JSON(root, "user", user);
//	转化为pojo对象
	User uo;
	uo.ParseFrom(user);
//	存储对象
	return us.ModifyUser(uo);
}
