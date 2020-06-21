#include "HttpResponse.h"
#include "ls_error.h"
#include "time.h"

using namespace std;

namespace ls
{
/* *
 * 	1. 仅支持Content-Length
 * */
	int &HttpResponse::Fd()
	{
		return fd;
	}
	int &HttpResponse::Code()
	{
		return code;
	}
	Text &HttpResponse::Body()
	{
		return body;
	}
	int HttpResponse::SetDefaultHeader(HttpRequest &req)
	{
		version = req.Version();
		attribute["Content-Encoding"] = "identity";
		attribute["Content-Type"] = "text/html;charset=utf-8";
		attribute["Server"] = "LSS/1.0";

		char timeFormat[128];
		time_t cur_sec = time(NULL);
		tm cur_tm = {};
		localtime_r(&cur_sec, &cur_tm);
		strftime(timeFormat, sizeof(timeFormat), "%a, %d %b %Y %T GMT", &cur_tm);
		attribute["Date"] = timeFormat;
		attribute["Connection"] = "close";
		return LS_OK;
	}
	int HttpResponse::ParseFrom(Text &text)
	{
		int st = 0, ed = text.find_first_of(" ");
		code = atoi(text.substr(st, ed - st).c_str());

		st = ed + 1;
		ed = text.find("\r\n", st);
		message = std::move(text.substr(st, ed - st));

		pair<Text, Text> item;
		while(text.find("\r\n\r\n", st) != ed)
		{
		//	'\r' and '\n'
			st = ed + 2;
			ed = text.find_first_of(":", st);
			item.first = std::move(text.substr(st, ed - st));
		//	':' and ' '
			st = ed + 2;
			ed = text.find("\r\n", st);
			item.second = std::move(text.substr(st, ed - st));

			attribute.emplace(item);
		}

		auto it = attribute.find("Content-Length");
		err_with(it,  == attribute.end(),  "no content length\n", return LS_FAILED);

		st = ed + 4;
		ed = st + atoi(it->second.c_str());
		body = std::move(text.substr(st, ed - st));
		return 0;
	}

	void HttpResponse::ParseTo(Text &text)
	{
		if(body.size() > 0 && attribute.find("Content-Length") == attribute.end())
		{
			char content_length[16];
			snprintf(content_length, sizeof(content_length), "%lu", body.size() + 2);
			attribute["Content-Length"] = content_length;
		}

		text.append(version);
		text.append(" ", 1);
		text.append(errorCode[code]);
		text.append(" ", 1);
		text.append(messageMapper[errorCode[code]]);
		text.append("\r\n", 2);

		for(auto &it : attribute)
		{
			text.append(it.first);
			text.append(": ", 2);
			text.append(it.second);
			text.append("\r\n", 2);
		}
		text.append("\r\n", 2);
		if(body.size() > 0)
			text.append(body);
		text.append("\r\n", 2);
		ls_log_tag(HTTPRESPONSE_LOG, "%s\n", text.c_str());
	}
	void HttpResponse::Clear()
	{
		fd = 0;
		code = LS_OK;
		version = "";				
		body = "";
		message = "";
		attribute.clear();
	}
	Text &HttpResponse::Filename()
	{
		return filename;
	}
	void HttpResponse::SetAttribute(const string &key, const string &value)
	{
		attribute[key] = value;
	}
	Text HttpResponse::GetAttribute(const string &key)
	{
		auto it = attribute.find(key);
		err_with(it, == attribute.end(), "GetAttribute error", return "");
		return it->second;
	}
}
