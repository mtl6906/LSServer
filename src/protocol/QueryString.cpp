#include "QueryString.h"
#include "ls_error.h"

using std::map;

namespace ls
{
	QueryString::QueryString(std::map<Text, Text> &data) : data(data)
	{
	
	}
	int QueryString::ParseFrom(Text &text)
	{
		int st = 0, ed = 0;
		for(;;)
		{
			ed = text.find_first_of("=", st);
			auto &value = data[text.substr(st, ed - st)];
			st = ed + 1;
			ed = text.find_first_of("&", st);
			if(ed == Text::npos)
			{
				value = std::move(text.substr(st));
				break;
			}
			value = std::move(text.substr(st, ed - st));
			st = ed + 1;
		}
		Text ot;
		ParseTo(ot);
		if(ot != text)
		{
			data.clear();
			return LS_FAILED;
		}
		return LS_OK;
	}

	void QueryString::ParseTo(Text &text)
	{
		int len = 0;
		for(auto &it : data)
			len += it.first.size() + it.second.size() + 2;
		text.resize(len, '\0');
		for(auto &it : data)
		{
			text.append(it.first);
			text.append("=");
			text.append(it.second);
			text.append("&");
		}
		text.back() = '\0';
	}
}
