#ifndef HTTPSERVLET_H
#define HTTPSERVLET_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Sender.h"
#include "Session.h"
#include "LSJson.h"
#include "functional"
#include "ls_def.h"
#include "ls_error.h"

namespace ls
{
	class HttpServlet
	{
		protected:
			std::map<Text, std::map<Text, std::function<int(HttpRequest &, HttpResponse &)>>> methodMap;
		public:
			HttpServlet()
			{
			}
			int Service(Session *session)
			{
				HttpRequest &req = session -> Request();
				HttpResponse &res = session -> Response();
				res.SetDefaultHeader(req);
				Text method = req.Method();
				auto methodIt = methodMap.find(method);
				err_with(methodIt, == methodMap.end(), "Not Implement", return LS_NOT_IMPLEMENTED);
				auto &functionMap = methodIt->second;
			//	判断dirname对应的方法(有参)是否存在
				auto functionIt = functionMap.find(req.Dirname());
				if(functionIt != functionMap.end())
				{
					ls_log_tag(HTTPSERVLET_LOG, "dirname compare ok");
					return res.Code() = functionIt->second(req, res);
				}
			//	判断uri对应的方法(无参)是否存在
				functionIt = functionMap.find(req.URI());
				if(functionIt != functionMap.end())
				{
					ls_log_tag(HTTPSERVLET_LOG, "uri compare ok");
					return res.Code() = functionIt->second(req, res);
				}
			//	判断是否为通配符, 通配符则调用对应的方法
				functionIt = functionMap.find("*");
				if(functionIt != functionMap.end())
				{
					ls_log_tag(HTTPSERVLET_LOG, "* compare ok");
					return res.Code() = functionIt->second(req, res);
				}
				return res.Code() = LS_NOT_FOUND;
			}
			virtual void Init() {}
			virtual void Destroy() {}
	};
}

#endif
