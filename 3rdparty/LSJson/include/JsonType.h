#ifndef JSONTYPE_H
#define JSONTYPE_H

#include "string"
#include "Protocol.h"

namespace ls {
	class Json;
	class ArrayItem;

	class JsonType : public Protocol
	{
		protected:
			virtual bool Is(Text &value) = 0;
			virtual int Parse(Text &value) = 0;
		public:
			int ParseFrom(Text &value) override;
			int AddTo(const char *key, Json &json);
			int AddTo(Text &text, Json &json);
			void AddTo(ArrayItem &item);
	};
}

#endif
