#ifndef HTTPCORE_H
#define HTTPCORE_H

#include "Session.h"
#include "sys/epoll.h"
#include "arpa/inet.h"
#include "EventManager.h"
#include "mutex"
#include "queue"

namespace ls
{
	class HttpCore
	{
		private:
			EventManager em;
			std::mutex core_mutex;
		public:
			std::map<int, Session *> sessionMapper;
			std::queue<Session *> pool;

		public:
			EventManager &GetEventManager();
			int Create(int max_size);
			int Assign(int fd, int type);
			int Release(int fd);
			Session *SessionOf(int fd);
	};
}

#endif
