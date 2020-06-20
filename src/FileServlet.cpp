#include "FileServlet.h"
#include "ls_def.h"
#include "ls_error.h"
#include "cstdio"
#include "sys/stat.h"
#include "fcntl.h"
#include "Config.h"

namespace ls
{
	FileServlet::FileServlet()
	{
		Init();
	}
	void FileServlet::Init()
	{
		methodMap["GET"]["*"] = [this](HttpRequest &req, HttpResponse &res) {
			auto pathname = cc.project_path + "/www" + req.URI();
			FILE *fp = NULL;
			err_with(fp = fopen(pathname.c_str(), "r"), == NULL, "fopen error", return LS_NOT_FOUND);
			struct stat statbuf;
			fstat(fileno(fp), &statbuf);
			res.Body().resize(statbuf.st_size);
			err_sys(fread((char *)res.Body().c_str(), 1, res.Body().size(), fp), "fread error"); 
			err_sys(fclose(fp), "fclose error");
			return LS_OK;	
		};
	}
};
