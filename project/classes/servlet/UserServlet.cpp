#include "UserServlet.h"
#include "ls_def.h"
#include "ls_error.h"

namespace ls
{
	UserServlet::UserServlet()
	{
		Init();
	}
	void UserServlet::Init()
	{
		auto &getFunctionMap = methodMap["GET"];
		auto &postFunctionMap = methodMap["POST"];
		auto &deleteFunctionMap = methodMap["DELETE"];
		auto &putFunctionMap = methodMap["PUT"];

	//	GET /user/id/1 -> {"id":1, "name" : "xm", "age" : 15}
		getFunctionMap["/user/id"] = [this](HttpRequest &req, HttpResponse &res) -> int {
			int id;
			err_with(IntItem(id).ParseFrom(req.Filename()), < 0, "ParseFrom error", return LS_FAILED);
			Json root;
			int ret = uc.GetUserById(id, root);
			err_with(ret, != LS_OK, "GetUserById error", return ret);
			root.ParseTo(res.Body());
			return LS_OK;
		};
	//	GET /user/all -> {"users": [{"id": 1,"name": "xm", "age": 15}]}
		getFunctionMap["/user/all"] = [this](HttpRequest &req, HttpResponse &res) -> int {
			Json root;
			int ret = uc.GetAllUsers(root);
			err_with(ret, != LS_OK, "GetAllUsers error", return ret);
			printf("ret: %d\n", ret);
			root.ParseTo(res.Body());
			return LS_OK;
		};
	//	POST /user -> {"user": {"id": 1, "name": "xm", "age": 15}}
		postFunctionMap["/user"] = [this](HttpRequest &req, HttpResponse &res) {
			FUNCTION_TRACE();
			Json root;
			err_with(root.ParseFrom(req.Body()), != LS_OK, "ParseFrom error", return LS_FAILED);
			int ret = uc.AddUser(root);
			return ret;
		};
	//	PUT /user -> {"user": {"id": 1, "name": "xm", "age": "15"}}
		putFunctionMap["/user"] = [this](HttpRequest &req, HttpResponse &res) {
			FUNCTION_TRACE();
			Json root;
			err_with(root.ParseFrom(req.Body()), != LS_OK, "ParseFrom error", return LS_FAILED);
			int ret = uc.ModifyUser(root);
			return ret;
		};
	//	DELETE /user/id/1
		deleteFunctionMap["/user/id"] = [this](HttpRequest &req, HttpResponse &res) {
			int id;
			err_with(IntItem(id).ParseFrom(req.Filename()), != LS_OK, "no filename", return LS_FAILED);
			int ret = uc.RemoveUserById(id);
			return ret;
		};
	}
}
