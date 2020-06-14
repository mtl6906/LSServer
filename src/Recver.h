#ifndef RECVER_H
#define RECVER_H

#include "ls_def.h"
#include "arpa/inet.h"
#include "sys/epoll.h"
#include "Session.h"
#include "EventManager.h"
#include "HttpCore.h"

namespace ls
{
	class Recver
	{
		int listenfd;
		HttpCore *core;
		EventManager *em;
		sockaddr_in addr;
		public: 
			Recver(HttpCore *core);
			int Init(int port);
			int Run(std::queue<Session *> &oq);
	};
}

#endif
