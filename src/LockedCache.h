#ifndef LOCKEDCACHE_H
#define LOCKEDCACHE_H

#include "ls_error.h"
#include "ls_def.h"
#include "mutex"
#include "queue"


namespace ls {
	template<typename T>
	class LockedCache
	{
		std::mutex cache_mutex;
		std::queue<T> data;
		public:
			int Push(std::queue<T> &o, int n)
			{		
				FUNCTION_TRACE();
				std::lock_guard<std::mutex> queue_lock(cache_mutex);
				if(n > o.size())
					n = o.size();
				for(int i=0;i<n;++i)
				{
					data.push(o.front());
					o.pop();
				}			
			}
			int Pop(std::queue<T> &o, int n)
			{
				FUNCTION_TRACE();
				std::lock_guard<std::mutex> queue_lock(cache_mutex);
				if(n > data.size())
					n = data.size();
				for(int i=0;i<n;++i)
				{
					o.push(data.front());
					data.pop();
				}
			}
			int PopAll(std::queue<T> &o)
			{
				std::lock_guard<std::mutex> queue_lock(cache_mutex);
				if(o.empty())
					o = std::move(data);
			}
			int PushAll(std::queue<T> &o)
			{
				std::lock_guard<std::mutex> queue_lock(cache_mutex);
				if(data.empty())
					data = std::move(o);
			}
			int Size()
			{
				return data.size();
			}
	};
}
#endif
