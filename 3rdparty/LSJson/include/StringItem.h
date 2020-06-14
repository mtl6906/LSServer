#ifndef STRINGITEM_H
#define STRINGITEM_H

#include "JsonType.h"

namespace ls {

	class StringItem : public JsonType
	{
		std::string &data;
		protected:
			bool Is(Text &text) override;
			int Parse(Text &text) override;
		public:
			StringItem(std::string &value);
			void ParseTo(Text &text) override;
	};
}

#endif
