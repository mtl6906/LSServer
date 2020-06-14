#include "Sender.h"
#include "sys/socket.h"
#include "ls_error.h"
#include "ls_util.h"
#include "HttpResponse.h"
#include "unistd.h"
#include "HttpCore.h"
#include "string.h"

using namespace std;

namespace ls
{
	Sender::Sender(HttpCore *core) : core(core)
	{
	}
	int Sender::Run(std::queue<Session *> &iq)
	{
		while(!iq.empty())
		{
			long long start_time, end_time;
			start_time = GetCurrentTime();
			auto session = iq.front();
			iq.pop();

			HttpResponse &res = session -> Response();
			Buffer &sb = session -> SendBuffer();
			Text text;
			res.ParseTo(text);
			sb.Push(text);
			
			int n;
			while((n = send(res.Fd(), sb.Begin(), sb.Size(), 0)) > 0)
			{
				sb.MoveOffset(n);
				ls_log_tag(SENDER_LOG, "send %d bytes data", n);
			}
			FUNCTION_TRACE();
			core -> Release(res.Fd());
			err_with(sb.Size(), != 0, "send failed", continue);
		}
		return LS_OK;
	}
}
