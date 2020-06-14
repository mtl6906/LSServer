#ifndef SENDER_H
#define SENDER_H

#include "ls_def.h"
#include "Session.h"
#include "HttpCore.h"

namespace ls
{
	class Sender
	{
		HttpCore *core;
		EventManager *em;
		public:
			Sender(HttpCore *core);
			int Run(std::queue<Session *> &iq);
	};
}

#endif
