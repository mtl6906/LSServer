#include "ErrorServlet.h"
#include "ls_def.h"
#include "sys/stat.h"

namespace ls
{
	ErrorServlet::ErrorServlet()
	{
		methodMap["GET"]["*"] = methodMap["POST"]["*"] = methodMap["DELETE"]["*"] = methodMap["PUT"]["*"] = [this](HttpRequest &req, HttpResponse &res) {
			return res.Code();
		};
	}
	void ErrorServlet::Init()
	{
		errorPage["400"] = "400.html";
		errorPage["500"] = "500.html";
		errorPage["404"] = "404.html";
	}
};
