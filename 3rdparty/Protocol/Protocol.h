#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "string"

namespace ls
{
	typedef std::string Text;
	class Protocol
	{
		public:
			virtual void ParseTo(Text &text) = 0;
			virtual int ParseFrom(Text &text) = 0;
	};
}

#endif
