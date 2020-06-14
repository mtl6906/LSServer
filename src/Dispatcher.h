#ifndef DISPATCHERSERVLET_H
#define DISPATCHERSERVLET_H

#include "ls_def.h"
#include "HttpServlet.h"
#include "ErrorServlet.h"
#include "FileServlet.h"
#include "Worker.h"
#include "Context.h"

namespace ls
{

	class Dispatcher
	{
		std::map<Text, HttpServlet *> servletMapper;
		std::map<Text, HttpServlet *> suffixFilter;
		FileServlet fileServlet;

		public:
			Dispatcher();
			~Dispatcher();
			int Run(std::queue<Session *> &iq, std::queue<Context> &oq);
	};
}

#endif
