#ifndef LS_ERROR_H
#define LS_ERROR_H

#include "ls_util.h"

#define RECVER_LOG "RECVER_LOG"
#define SENDER_LOG "SENDER_LOG"
#define DISPATCHER_LOG "DISPATCHER_LOG"
#define WORKER_LOG "WORKER_LOG"
#define SESSION_LOG "SESSION_LOG"
#define HTTPREQUEST_LOG "HTTPREQUEST_LOG"
#define HTTPRESPONSE_LOG "HTTPRESPONSE_LOG"
#define HTTPSERVLET_LOG "HTTPSERVLET_LOG"
#define MAIN_LOG "MAIN_LOG"
#define CORE_LOG "CORE_LOG"

#define LS_OK 0
#define LS_FAILED -1
#define LS_NOT_FOUND -2
#define LS_INCOMPLETE -3

#define FUNCTION_TRACE() ls_log_tag("FT", "[ %s ]\n line %d in file \"%s\"\n", __func__, __LINE__, __FILE__);
#define ls_log_tag(tag, fmt, ...) if(getenv(tag)) \
				{ \
					printf("[ "); \
					PrintTimeFormat(stdout); \
				  	printf("] [ %s ] ", tag); \
					printf(fmt, ##__VA_ARGS__); \
					printf("\n"); \
				}
#define err_with(ret, condition, msg, op) { \
						auto rc = (ret); \
						if(rc condition) \
						{ \
							FUNCTION_TRACE(); \
							if(errno) \
							{ \
								perror(msg); \
								errno = 0; \
							} \
							else if(msg[0] != '\0') \
								fprintf(stderr, "%s\n", msg); \
							op; \
						} \
					}

#define err_sys(ret, msg) err_with(ret, < 0, msg, return LS_FAILED);

#define err_ret(ret, msg) err_with(ret, < 0, msg, return rc);

#define err_throw(ret) err_with(ret, < 0, "", return rc);

#endif
