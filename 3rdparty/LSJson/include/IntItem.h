#ifndef INTITEM_H
#define INTITEM_H

#include "JsonType.h"

namespace ls {
	class IntItem : public JsonType
	{
		int &data;
		protected:
			bool Is(Text &text) override;
			int Parse(Text &text) override;
		public:	
			IntItem(int &value);
			void ParseTo(Text &text) override;
	};	
}

#endif
