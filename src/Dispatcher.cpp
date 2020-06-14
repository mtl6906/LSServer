#include "Dispatcher.h"
#include "ls_error.h"
#include "ls_util.h"
#include "unistd.h"
#include "ErrorServlet.h"
#include "FileServlet.h"
#include "Worker.h"
#include "UserServlet.h"

using std::queue;
using std::map;

extern ls::ErrorServlet errorServlet;

namespace ls
{
	Dispatcher::Dispatcher() : servletMapper({
		{"/user", new UserServlet()}
	})
	{
		suffixFilter[".html"] = &fileServlet;
	}
	Dispatcher::~Dispatcher()
	{
		for(auto it : servletMapper)
			if(it.second)
				delete it.second;
	}
	int Dispatcher::Run(queue<Session *> &iq, queue<Context> &oq)
	{
		FUNCTION_TRACE();
		while(!iq.empty())
		{
			Session *session = iq.front();
			iq.pop();
			auto &req = session -> Request();
			auto &res = session -> Response();	
			Text uri;
			err_with(uri = req.URI(), == "", "URI error", {
				res.Code() = LS_FAILED;
				oq.emplace(session, &errorServlet);
				return LS_FAILED;
			});
		//	在servletMapper中寻找
			auto it = servletMapper.begin();
			while(it != servletMapper.end())
			{
				ls_log_tag(DISPATCHER_LOG, "%s %s\n", uri.c_str(), it->first.c_str());
				if(uri.compare(0, it->first.size(), it->first) == 0)
					break;
				++it;
			}
			if(it != servletMapper.end())
			{
				ls_log_tag(DISPATCHER_LOG, "compare with servlet: %s", it->first.c_str());
				oq.emplace(session, it->second);
				continue;
			}
		//	在后缀过滤器中寻找
			int n = uri.find_last_of('.');
			if(n == Text::npos)
				it = suffixFilter.end();
			else
				it = suffixFilter.find(uri.substr(n));
			if(it != suffixFilter.end())
			{
				oq.emplace(session, it->second);
				continue;
			}
			ls_log_tag(DISPATCHER_LOG, "not found");
			res.Code() = LS_NOT_FOUND;
			oq.emplace(session, &errorServlet);
		}
		return LS_OK;
	}
}
