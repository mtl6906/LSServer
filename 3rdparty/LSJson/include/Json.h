#ifndef JSON_H
#define JSON_H

#include "JsonType.h"
#include "map"
#include "string"

namespace ls{

	class Json : public JsonType
	{
		std::map<std::string, std::string> data;	
		protected:
			bool Is(Text &text) override;
			int Parse(Text &text) override;
			void ClearWhiteBlank(Text &text);
		public:	
			void ParseTo(Text &text);
			std::map<Text, Text> &Data();
			int Get(const Text &text, JsonType &value);
			int Get(const Text &text, JsonType &&value);
	};
}

#endif
