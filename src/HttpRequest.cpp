#include "HttpRequest.h"
#include "ls_error.h"
#include "HttpServlet.h"

using namespace std;

namespace ls
{
	HttpRequest::HttpRequest() : qs(parameter)
	{
		
	}
	void HttpRequest::SetDefaultHeader()
	{
		FUNCTION_TRACE();
		SetAttribute("Accept", "buffer/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		SetAttribute("Accept-Language", "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
	//	SetAttribute("Accept-Encoding", "gzip,deflate,br");
		SetAttribute("Accept-Encoding", "deflate,br");
		SetAttribute("Connection", "close");
	}
	void HttpRequest::SetURL(const Text &url)
	{
		FUNCTION_TRACE();
		int split = url.find_first_of('/');
		if(split == std::string::npos)
		{
			uri = "/";
			dirname = "/";
			filename = "";
		}
		else
		{
			Text tmp = url.substr(split);
			int n = tmp.find_first_of('?');
			if(Text::npos != n)
				queryText = tmp.substr(n + 1);
			else
				queryText = "";
			uri = std::move(tmp.substr(0, n));
			split = uri.find_last_of('/');
		
			dirname = std::move(uri.substr(0, split));
			filename = std::move(uri.substr(split + 1, n - split - 1));
		}
		
		ls_log_tag(HTTPREQUEST_LOG, "uri: %s", uri.c_str());
		ls_log_tag(HTTPREQUEST_LOG, "queryText: %s", queryText.c_str());
		ls_log_tag(HTTPREQUEST_LOG, "dirname: %s", dirname.c_str());
		ls_log_tag(HTTPREQUEST_LOG, "filename: %s", filename.c_str());
	}
	Text &HttpRequest::Dirname()
	{
		return dirname;
	}

	Text &HttpRequest::Filename()
	{
		return filename;
	}
	Text &HttpRequest::URI()
	{
		return uri;
	}
	Text &HttpRequest::QueryText()
	{
		return queryText;
	}
	Text &HttpRequest::Version()
	{
		return version;
	}
	Text &HttpRequest::Method()
	{
		return method;
	}
	Text &HttpRequest::Body()
	{
		return body;
	}
	void HttpRequest::ParseTo(Text &text)
	{
		text.append(method);
		text.append(" ", 1);
		text.append(uri);
		text.append(" ", 1);
		text.append(version);
		text.append("\r\n", 2);

		for(auto &t : attribute)
		{
			text.append(t.first);
			text.append(": ", 2);
			text.append(t.second);
			text.append("\r\n", 2);
		}
		text.append("\r\n", 2);
		if(method.compare("POST") == 0 || method.compare("PUT") == 0)
			text.append(body);
	}
	
	int HttpRequest::ParseHeader(Text &text)
	{
		FUNCTION_TRACE();
		int st = 0, ed = text.find_first_of(" ");	
		method = std::move(text.substr(st, ed - st));
	
		st = ed + 1;
		ed = text.find_first_of(" ", st);
		Text url = std::move(text.substr(st, ed - st));
		SetURL(url);

		st = ed + 1;
		ed = text.find("\r\n", st);
		version = std::move(text.substr(st, ed - st));
			
		Text key, value;
		while(text.find("\r\n\r\n", st) != ed)
		{
			st = ed + 2;
			ed = text.find_first_of(":", st);
			key = std::move(text.substr(st, ed - st));

			st = ed + 2;
			ed = text.find_first_of("\r\n", st);
			value = std::move(text.substr(st, ed - st));
			attribute.emplace(std::move(key), std::move(value));
		}
	//	获取Content-Type对应的协议解析类的对象, GET或DELETE请求时该属性会被忽略
		Text contentType = GetAttribute("Content-Type");
		if(0 == contentType.compare(""))
			protocol = NULL;
		else if(0 == contentType.compare("application/json"))
			protocol = &js;
		else if(contentType.compare("application/x-www-form-urlencoded"))
			protocol = &qs;

		return LS_OK;
	}
	int HttpRequest::ParseFrom(Text &text)
	{
		FUNCTION_TRACE();
		return ParseHeader(text);
	}
	void HttpRequest::Clear()
	{
		method = "";
		body = "";
		uri = "";
		queryText = "";
		version = "";
		dirname = "";
		filename = "";
		attribute.clear();
		parameter.clear();
		protocol = NULL;
	}
	Text HttpRequest::GetAttribute(const Text &key)
	{
		auto it = attribute.find(key);
		if(it == attribute.end())
			return "";
		return it -> second;
	}
	void HttpRequest::SetAttribute(const Text &key, const Text &value)
	{
		attribute[key] = value;
	}
	Text HttpRequest::GetParameter(const Text &key)
	{
		if(parameter.empty())
		{
			if(Method() == "GET")
			{
				Text queryText = QueryText();
				qs.ParseFrom(queryText);
			}
			else if(Method() == "POST")
			{
				if(NULL == protocol)
					return "";
				protocol -> ParseFrom(Body());
			}
		}
		auto it = parameter.find(key);
		if(parameter.end() == it)
			return "";
		return it->second;
	}
	void HttpRequest::SetParameter(const Text &key, const Text &value)
	{
		parameter[key] = value;
	}
}
