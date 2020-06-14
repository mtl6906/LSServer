#ifndef USERSERVLET_H
#define USERSERVLET_H

#include "vector"
#include "HttpServlet.h"
#include "UserController.h"

namespace ls
{
	class UserServlet : public HttpServlet
	{
		UserController uc;		
		public:	
			UserServlet();
			void Init() override;
	};
}

#endif
