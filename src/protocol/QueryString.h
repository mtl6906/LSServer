#ifndef QUERYSTRING_H
#define QUERYSTRING_H

#include "Protocol.h"
#include "map"

namespace ls
{
	class QueryString : public Protocol
	{
		std::map<Text, Text> &data;
		public:
			QueryString(std::map<Text, Text> &data);
			void ParseTo(Text &text) override;
			int ParseFrom(Text &text) override;	
	};
}

#endif
