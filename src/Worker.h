#ifndef WORKER_H
#define WORKER_H

#include "ls_def.h"
#include "LockedCache.h"
#include "Context.h"

namespace ls
{
	class Worker
	{
		LockedCache<Context> ilc;
		LockedCache<Session *> olc;
		std::queue<Context> iq;
		std::queue<Session *> oq;
		public:
			int Run();
			LockedCache<Context> &GetILC();
			LockedCache<Session *> &GetOLC();
	};
}

#endif
