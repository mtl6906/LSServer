#ifndef ERRORSERVLET_H
#define ERRORSERVLET_H

#include "HttpServlet.h"

namespace ls
{
	class ErrorServlet : public HttpServlet
	{
		std::map<Text, Text> errorPage;
		public:
			ErrorServlet();
			void Init() override;
	};
}

#endif
