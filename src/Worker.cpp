#include "Worker.h"
#include "ls_error.h"
#include "ls_util.h"
#include "unistd.h"
#include "LockedCache.h"
#include "ErrorServlet.h"

using namespace std;

extern ls::ErrorServlet errorServlet;

namespace ls
{
	int Worker::Run()
	{
		ilc.PopAll(iq);
		while(!iq.empty())
		{
			auto &context = iq.front();
			iq.pop();
			int code = context.servlet->Service(context.session);
			if(code != LS_OK)
				errorServlet.Service(context.session);
			oq.push(context.session);
		}
		olc.PushAll(oq);
		return LS_OK;
	}
	LockedCache<Context> &Worker::GetILC()
	{
		return ilc;
	}
	LockedCache<Session *> &Worker::GetOLC()
	{
		return olc;
	}
}

