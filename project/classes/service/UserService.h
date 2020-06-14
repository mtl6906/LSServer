#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "UserDao.h"

class UserService
{
	UserDao userDao;
	public:
		User FindUserById(int id);
		std::vector<User> FindUsers();
		int AddUser(User &user);
		int ModifyUser(User &user);
		int RemoveUserById(int id);
};

#endif
