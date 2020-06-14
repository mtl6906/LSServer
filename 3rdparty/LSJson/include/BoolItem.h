#ifndef BOOLITEM_H
#define BOOLITEM_H

#include "JsonType.h"

namespace ls {

	class BoolItem : public JsonType
	{
		bool &data;
		protected:
			bool Is(Text &value) override;
			int Parse(Text &value) override;
		public:
			BoolItem(bool &value);
			void ParseTo(Text &value) override;
	};
}

#endif
