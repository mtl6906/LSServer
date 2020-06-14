#ifndef JSONAPPLICATION_H
#define JSONAPPLICATION_H

#include "map"
#include "JsonParse.h"
#include "Protocol.h"

namespace ls
{
	class JsonApplication : public Protocol
	{
		JsonParse *jp;
		public:
			JsonApplication(JsonParse *jp);
			int ParseFrom(Text &text) override;
			void ParseTo(Text &text) override;
	};
}

#endif
