#ifndef ARRAYITEM_H
#define ARRAYITEM_H

#include "JsonType.h"
#include "vector"

namespace ls{

	class ArrayItem : public JsonType
	{
		std::vector<Text> &data;
		protected:
			bool Is(Text &text) override;
			int Parse(Text &text) override;
		public:
			ArrayItem(std::vector<Text> &value);
			void ParseTo(Text &) override;
			std::vector<Text> &Data();
	};

}

#endif
