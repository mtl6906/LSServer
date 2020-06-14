#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "UserService.h"
#include "LSJson.h"

class UserController
{
	UserService us;
	public:
	/* *
	 *	GET /user/all 
	 *
	 *	-> {"users" : [{"id":1, "name":"xm", "age":15}]}
	 * */

	int GetAllUsers(ls::Json &root);
	/* *
	 *	GET /user/id/1
	 *
	 *	-> {"user" : {"id":1, "name", "xm", "age":15}}
	 * */
	int  GetUserById(int id, ls::Json &root);
	/* *
	 *	POST /user 
	 *
	 *	{"user" : {"id":1, "name":"xm", "age":5}}
	 *
	 *	-> {"id":1}
	 * */
	int AddUser(ls::Json &user);
	/* *
	 *	DELETE /user/id/1
	 *
	 *	-> 0
	 * */
	int RemoveUserById(int id);
	/* *
	 *	PUT /user
	 *
	 *	{"user" : {"id":1, "name", "xm", "age":15}}
	 *
	 *	-> 0
	 * */
	int ModifyUser(ls::Json &user);
};

#endif
