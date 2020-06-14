#ifndef EVENT_H
#define EVENT_H

#include "sys/epoll.h"
#include "vector"

namespace ls
{
	class EventManager
	{
		int epollfd;
		std::vector<epoll_event>events;
		public:
			int Create(int max_num);
			int Wait(int timeout);
			int Add(int fd, int type);
			int Remove(int fd);
			epoll_event &Event(int i);
	};
}

#endif
