#include "ls_error.h"
#include "HttpCore.h"
#include "Config.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/socket.h"
#include "cstring"
#include "mutex"
#include "assert.h"
using std::lock_guard;
using std::mutex;

namespace ls
{
	int HttpCore::Create(int max_size)
	{
		FUNCTION_TRACE();
		for(int i=0;i<max_size * 2;++i)
		{
			auto item = new Session(cc.session_buffer_size);
			err_with(item, == NULL,  "no memory", exit(1));
			pool.push(item);
		}
		em.Create(max_size);
		return LS_OK;
	}
	int HttpCore::Assign(int fd, int type)
	{
		FUNCTION_TRACE();
		lock_guard<mutex> lock_core(core_mutex);
		Session *it = pool.front();
		err_with(it, == NULL, "no memory", exit(2));

		em.Add(fd, type);
		it -> Request().Clear();
		it -> Response().Clear();
		it -> RecvBuffer().Clear();
		it -> SendBuffer().Clear();
		it -> Response().Fd() = fd;
		
		sessionMapper.emplace(fd, it);
		pool.pop();
		return LS_OK;
	}
	int HttpCore::Release(int fd)
	{
		FUNCTION_TRACE();
		lock_guard<mutex> lock_core(core_mutex);
//		em.Remove(fd);
		auto it = sessionMapper.find(fd);
		if(it != sessionMapper.end())
		{
			pool.push(it->second);
			sessionMapper.erase(fd);
			close(fd);
		}
		return LS_OK;
	}
	Session* HttpCore::SessionOf(int fd)
	{
		FUNCTION_TRACE();
		lock_guard<mutex> lock_core(core_mutex);
		auto it = sessionMapper.find(fd);
		if(it == sessionMapper.end())
			return NULL;
		return it -> second;
	}
	EventManager &HttpCore::GetEventManager()
	{
		return em;
	}
}
