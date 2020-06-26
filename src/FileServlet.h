#ifndef FILESERVLET_H
#define FILESERVLET_H

#include "HttpServlet.h"
#include "map"

namespace ls
{
	class FileServlet : public HttpServlet
	{
		std::map<Text, Text> contentTypeMapper;
		public:
			FileServlet();
			void Init() override;
	};
}

#endif
