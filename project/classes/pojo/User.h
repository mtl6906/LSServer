#ifndef USER_H
#define USER_H

#include "LSJson.h"

class User : public ls::JsonParse
{
	int id;
	ls::Text name;
	int age;
	public:
		User();
		int &Age();
		int &Id();
		ls::Text &Name();
		void ParseTo(ls::Json &root) override;
		int ParseFrom(ls::Json &root) override;
};

#endif
