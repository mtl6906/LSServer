#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#define HTTP_METHOD_GET 0
#define HTTP_METHOD_POST 1

#define HTTP_1_1 0

#include "ls_def.h"
#include "Protocol.h"
#include "QueryString.h"
#include "LSJson.h"

namespace ls
{
	class HttpServlet;
	class HttpRequest : public Protocol
	{
		Text method;
		Text uri;
		Text dirname;
		Text filename;
		Text queryText;
		Text version;
		Text body;
		std::map<Text, Text> attribute;
		std::map<Text, Text> parameter;
		Protocol *protocol;
		QueryString qs;
		Json js;

		public:
			HttpRequest();

			void ParseTo(Text &text) override;
			int ParseFrom(Text &text) override;
			
			Text &Method();
			Text &Version();
			Text &URI();
			Text &QueryText();
			Text &Body();
			Text &Filename();
			Text &Dirname();

			void SetURL(const Text &text);
			void SetDefaultHeader();				
			int ParseHeader(Text &text);
			int ParseBody(Text &text);
			void InitFrom(HttpServlet *servlet);
		
			Text GetParameter(const Text &key);
			void SetParameter(const Text &key, const Text &value);
			Text GetAttribute(const Text &key);
			void SetAttribute(const Text &key, const Text &value);

			bool Empty();
			void Clear();
	};
}

#endif
