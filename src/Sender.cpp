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
			Text header;
			res.ParseTo(header);
		//	先发送header, 在根据filename是否为空决定是否进行读文件操作
			sb.Push(header);
			session->Send();

			if(res.Filename() != "")
			{
				FILE *fp = fopen(res.Filename().c_str(), "r");
				int n, len = stoi(res.GetAttribute("Content-Length"));
				while((n = fread(sb.End(), 1, sb.RestSize(), fp)) > 0)
				{
					sb.MoveBuffersize(n);
					session->Send();
					sb.Clear();
				}
				sb.Push("\r\n");
				session->Send();
				fclose(fp);
			}
			else
			{
				sb.Push(res.Body());
				session->Send();
			}
			core -> Release(res.Fd());
			err_with(sb.Size(), != 0, "send failed", continue);
		}
		return LS_OK;
	}
}
