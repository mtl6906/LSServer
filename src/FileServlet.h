#ifndef FILESERVLET_H
#define FILESERVLET_H

#include "HttpServlet.h"

namespace ls
{
	class FileServlet : public HttpServlet
	{
		public:
			FileServlet();
			void Init() override;
	};
}

#endif
