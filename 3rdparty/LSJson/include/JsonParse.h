#ifndef JSONPARSE_H
#define JSONPARSE_H

namespace ls {
	class Json;
	class JsonParse
	{
		public:
			virtual void ParseTo(Json &json) = 0;
			virtual int ParseFrom(Json &json) = 0;
	};
}

#endif
