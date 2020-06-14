#include "UserDao.h"
#include "vector"
#include "ls_def.h"

using std::vector;
using std::map;

User UserDao::SelectUserById(int id)
{
	auto it = userTable.find(id);
	if(it == userTable.end())
		return User();
	return it -> second;
}

std::vector<User> UserDao::SelectUsers()
{
	vector<User> userList;
	for(auto &it : userTable)
		userList.emplace_back(it.second);
	return userList;
}

int UserDao::DeleteUserById(int id)
{
	auto it=userTable.find(id);
	if(it == userTable.end())
		return LS_NOT_FOUND;
	userTable.erase(it);
	return LS_OK;
}

int UserDao::InsertUser(User &user)
{
	if(userTable.find(user.Id()) != userTable.end())
		return LS_FAILED;
	userTable.emplace(user.Id(), user);
	return LS_OK;
}

int UserDao::UpdateUser(User &user)
{
	auto it = userTable.find(user.Id());
	if(it == userTable.end())
		return LS_NOT_FOUND;
	it -> second = user;
	return LS_OK;
}


