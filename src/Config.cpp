#include "Config.h"
#include "ls_def.h"
#include "ls_error.h"
#include "cstring"

ls::ConfigContext cc;
namespace ls
{
	Config::Config() : param_table({
		LS_INT_PARAM("port", &cc.port),
		LS_INT_PARAM("thread_number", &cc.thread_number),
		LS_INT_PARAM("worker_waiting_time", &cc.worker_waiting_time),
		LS_INT_PARAM("sender_waiting_time", &cc.sender_waiting_time),
		LS_INT_PARAM("max_connection_number", &cc.max_connection_number),
		LS_INT_PARAM("session_buffer_size", &cc.session_buffer_size),
		LS_STRING_PARAM("project_path", &cc.project_path)
		})
	{
		Text text(4096, '\0');
		FILE *fp = fopen(CONFIG_PATH, "r");
		fread((char *)text.c_str(), 1, text.size(), fp);
		fclose(fp);		

		root.ParseFrom(text);


		GET_INT(root, "port", cc.port);
		GET_INT(root, "thread_number", cc.thread_number);
		GET_INT(root, "worker_waiting_time", cc.worker_waiting_time);
		GET_INT(root, "sender_waiting_time", cc.sender_waiting_time);
		GET_INT(root, "max_connection_number", cc.max_connection_number);
		GET_INT(root, "session_buffer_size", cc.session_buffer_size);
		GET_STRING(root, "project_path", cc.project_path);

		for(auto &it : param_table)
		{
			printf("%s: ", it.first.c_str());
			it.second.Print(it.second.addr);
			printf("\n");
		}

	}


	void PrintInt(void *addr)
	{
		printf("%d", *((int *)(addr)));
	}

	void PrintBool(void *addr)
	{
		printf("%s", *((bool *)(addr)) ? "true" : "false");
	}

	void PrintString(void *addr)
	{
		printf("%s", *((const char **)(addr)));
	}

	void SetIntParameter(Json &root, const char *key, void *addr, void *value)
	{
		IntItem(*((int *)(addr)) = *((int *)(value))).ParseTo(root.Data()[key]);
	}

	void SetBoolParameter(Json &root, const char *key, void *addr, void *value)
	{
		BoolItem(*((bool *)(addr)) = *((bool *)(value))).ParseTo(root.Data()[key]);
	}

	void SetStringParameter(Json &root, const char *key, void *addr, void *value)
	{		
		strcpy(*(char **)(addr), *((const char **)(value)));
		Text text = *((char **)(addr));
		StringItem(text).ParseTo(root.Data()[key]);
	}

	int Config::SetParameter(const char *key, void *value)
	{
		auto it = param_table.find(key);
		err_with(it, == param_table.end(), "no such parameter", return LS_FAILED);
		it -> second.SetParameter(root, key, it->second.addr, value);
		return LS_OK;
	}
}
