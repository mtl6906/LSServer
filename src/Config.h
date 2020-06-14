#ifndef CONFIG_H
#define CONFIG_H

#include "LSJson.h"
#include "map"
#define CONFIG_PATH "config/config.json"

#define LS_INT_PARAM(key, addr) {key, {PrintInt, SetIntParameter, addr}}
#define LS_BOOL_PARAM(key, addr) {key, {PrintBool, SetBoolParameter, addr}}
#define LS_DOUBLE_PARAM(key, addr) {key, {PrintDouble, SetDoubleParameter, addr}}
#define LS_STRING_PARAM(key, addr) {key, {PrintString, SetStringParameter, addr}}

namespace ls
{
//	void SetDoubleParameter(const char *key, void *value);	
	void SetIntParameter(Json &root, const char *key, void *addr, void *value);
	void SetStringParameter(Json &root, const char *key, void *addr, void *value);
	void SetBoolParameter(Json &root, const char *key, void *addr, void *value);

	void PrintInt(void *addr);
	void PrintString(void *addr);
	void PrintBool(void *addr);

	struct Parameter
	{
		void (*Print)(void *);
		void (*SetParameter)(Json &, const char *, void *, void *);
		void *addr;
	};
	struct ConfigContext
	{
		int port;
		int thread_number;
		int worker_waiting_time;
		int sender_waiting_time;
		int max_connection_number;
		int session_buffer_size;
		Text project_path;
	};
	class Config
	{
		ls::Json root;
		const std::map<Text, Parameter> param_table;
		public:
			Config();
			int SetParameter(const char *key, void *value);
	};

}

extern ls::ConfigContext cc;

#endif
