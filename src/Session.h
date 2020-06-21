#ifndef SESSION_H
#define SESSION_H

#include "ls_def.h"
#include "Buffer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace ls 
{
	class Session
	{
		Buffer recvBuffer;
		Buffer sendBuffer;
		HttpRequest request;
		HttpResponse response;
	
		public:
			Session(int buffersize);
			int PushHeader();
			int PushBody();
			HttpRequest &Request();
			HttpResponse &Response();
			Buffer &RecvBuffer();
			Buffer &SendBuffer();
			int Send();
	};
}

#endif
