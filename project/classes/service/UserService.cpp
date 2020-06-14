#include "UserService.h"

using std::vector;

User UserService::FindUserById(int id)
{
	return userDao.SelectUserById(id);
}

vector<User> UserService::FindUsers()
{
	return userDao.SelectUsers();
}

int UserService::AddUser(User &user)
{
	return userDao.InsertUser(user);
}

int UserService::ModifyUser(User &user)
{
	return userDao.UpdateUser(user);
}

int UserService::RemoveUserById(int id)
{
	return userDao.DeleteUserById(id);
}
