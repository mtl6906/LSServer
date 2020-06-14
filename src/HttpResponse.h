#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "map"
#include "Protocol.h"
#include "HttpRequest.h"


namespace ls 
{
	typedef std::string Text;
	class HttpResponse : public Protocol
	{
		int fd;
		Text version;
		int code;
		Text body;
		Text message;
		std::map<Text, Text> attribute;
		public:
			int &Fd();
			int &Code();
			Text &Body();
			int SetAttribute(const Text &key, const Text &value);
			int SetDefaultHeader(HttpRequest &req);

			int ParseFrom(Text &text) override;			
			void ParseTo(Text &text) override;

			void Clear();
	};
}
#endif
