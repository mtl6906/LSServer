#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/epoll.h"
#include "fcntl.h"
#include "cstring"

#include "ls_def.h"
#include "ls_error.h"
#include "ls_util.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Dispatcher.h"
#include "Recver.h"
#include "HttpCore.h"

using std::queue;

extern long long r1;
extern long long r2;

ls::ErrorServlet errorServlet;

namespace ls
{
	Recver::Recver(HttpCore *core) : core(core)
	{
	
	}
	int Recver::Init(int port)
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		err_sys(listenfd = socket(AF_INET, SOCK_STREAM, 0), "socket error");
		err_sys(bind(listenfd, (sockaddr *)&addr, sizeof(addr)), "bind error");
		err_sys(listen(listenfd, 128), "listen error");
		core->GetEventManager().Add(listenfd, EPOLLIN | EPOLLET);
	}
	int Recver::Run(std::queue<Session *> &oq)
	{
		auto &em = core -> GetEventManager();
		int ret = em.Wait(-1);
		ls_log_tag("RECVER_LOG", "epoll_wait: %d events happened...\n", ret);
		
		long long start_time, end_time;
		start_time = GetCurrentTime();

		for(int i=0;i<ret;++i)
		{
			int fd = em.Event(i).data.fd;
			ls_log_tag("RECVER_LOG", "event %d on fd %d:\n", i, fd);
			if(fd == listenfd)
			{
				int connfd;
				sockaddr_in caddr = {};
				socklen_t clen = sizeof(caddr);
				while((connfd = accept(listenfd, (sockaddr *)&caddr, &clen)) > 0)
				{
					ls_log_tag(RECVER_LOG, "get connection on fd %d\n", connfd);
					err_with(core->Assign(connfd, EPOLLIN | EPOLLET | EPOLLERR), == LS_FAILED, "pool empty", exit(5));
				}
				if(errno != EAGAIN && errno != EWOULDBLOCK)
				{
					perror("accept error");
					exit(1);
				}
			}
			else if(EPOLLIN & em.Event(i).events)
			{
				int ret = 0;
				Session *session = core -> SessionOf(fd);
				if(session == NULL)
				{
					core -> Release(fd);
					continue;
				}
				auto &rb = session -> RecvBuffer();
				while((ret = recv(fd, rb.End(), rb.RestSize(), 0)) > 0)
					rb.MoveBuffersize(ret);
				if(errno != EAGAIN && errno != EWOULDBLOCK)		
					err_with(ret, < 0, "recv error", {
						core -> Release(fd);
						continue;
					});

				err_with(ret = session->PushHeader(), == LS_INCOMPLETE, "", continue);
				err_with(ret, < 0, "PushHeader error", {
					core -> Release(fd);
					continue;
				});
				err_with(ret = session->PushBody(), == LS_INCOMPLETE, "", continue);
				err_with(ret, < 0, "PushBody error", {
					core -> Release(fd);
					continue;
				});
				ls_log_tag(RECVER_LOG, "parse ok...");
				oq.push(session);
			}
		}
		end_time = GetCurrentTime();
		r1 += end_time - start_time;
		return ret > 0 ? LS_OK : ret;
	}
}
