#ifndef CONTEXT_H
#define CONTEXT_H

#include "HttpServlet.h"
#include "Session.h"

namespace ls
{
	struct Context
	{
		Session *session;
		HttpServlet *servlet;
		Context(Session *session, HttpServlet *servlet) : session(session), servlet(servlet)
		{
		
		}
	};
}

#endif
