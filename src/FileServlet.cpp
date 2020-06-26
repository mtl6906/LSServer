#include "FileServlet.h"
#include "ls_def.h"
#include "ls_error.h"
#include "cstdio"
#include "sys/stat.h"
#include "fcntl.h"
#include "Config.h"
#include "string"

namespace ls
{
	FileServlet::FileServlet() : contentTypeMapper({
			{".html", "text/html; charset=utf-8"},
			{".css", "text/css"},
			{".js", "application/javascript"}
	})
	{
		Init();
	}
	void FileServlet::Init()
	{

		methodMap["GET"]["*"] = [this](HttpRequest &req, HttpResponse &res) {
			auto pathname = cc.project_path + "/www" + req.URI();
			struct stat statbuf;
			err_with(lstat(pathname.c_str(), &statbuf), < 0, "lstat error", return LS_FAILED);
			res.Filename() = pathname;
			int pos = req.Filename().find_last_of(".");
			auto it = contentTypeMapper.find(req.Filename().substr(pos));
			if(it != contentTypeMapper.end())
				res.SetAttribute("Content-Type", it->second);
			res.SetAttribute("Content-Length", std::to_string(statbuf.st_size+2));
			return LS_OK;	
		};
	}
};
