#include "EventManager.h"
#include "ls_def.h"
#include "ls_error.h"
#include "cstring"
#include "fcntl.h"

namespace ls
{
	int AddFlag(int fd, int flag)
	{
		int old_flag;
		err_sys(old_flag = fcntl(fd, F_GETFL), "get flag error");
		err_sys(fcntl(fd, F_SETFL, old_flag | flag), "set flag] error");
		return LS_OK;
	}
//	PUBLIC METHOD
	int EventManager::Create(int max_num)
	{
		FUNCTION_TRACE();
		events.resize(max_num);
		err_sys(epollfd = epoll_create(5), "epoll_create error");
	}
	int EventManager::Wait(int timeout)
	{
		FUNCTION_TRACE();
		int ret;
	//	memset(events.data(), 0, sizeof(events[0]) * events.size());
		err_sys(ret = epoll_wait(epollfd, events.data(), events.size(), timeout), "epoll_wait error");
		return ret;
	}
	int EventManager::Add(int fd, int type)
	{
		FUNCTION_TRACE();
		epoll_event event = {};
		event.data.fd = fd;
		event.events |= type;
		err_sys(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event), "epoll_ctl add error");
		AddFlag(fd, O_NONBLOCK);
		return LS_OK;
	}
	int EventManager::Remove(int fd)
	{
		FUNCTION_TRACE();
		err_sys(epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL), "epoll_ctl del error");
		return LS_OK;
	}
	epoll_event &EventManager::Event(int i)
	{
		return events[i];
	}
}
