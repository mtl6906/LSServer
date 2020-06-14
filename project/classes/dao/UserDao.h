#ifndef USERDAO_H
#define USERDAO_H

#include "vector"
#include "User.h"

class UserDao
{
	std::map<int, User> userTable;
	public:
		User SelectUserById(int id);
		std::vector<User> SelectUsers();
		int DeleteUserById(int id);
		int InsertUser(User &user);
		int UpdateUser(User &user);
};

#endif

