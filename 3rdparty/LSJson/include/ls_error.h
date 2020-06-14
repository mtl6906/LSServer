#ifndef LS_ERROR_H
#define LS_ERROR_H

#define LS_OK 0
#define LS_FAILED -1
#define LS_NOT_FOUND -2
#define LS_INCOMPLETE -3

#define FUNCTION_TRACE() fprintf(stderr, "[ %s ]\n line %d in file \"%s\"\n", __func__, __LINE__, __FILE__);

#define err_with(ret, condition, msg, op) { \
						errno = 0;\
						auto rc = (ret); \
						if(rc condition) \
						{ \
							FUNCTION_TRACE(); \
							if(errno) \
							{ \
								perror(msg); \
								errno = 0; \
							} \
							else if(msg[0] == '\0') \
								fprintf(stderr, "%s\n", msg); \
							op; \
						} \
					}

#define err_sys(ret, msg) err_with(ret, < 0, msg, return LS_FAILED);

#define err_ret(ret, msg) err_with(ret, < 0, msg, return rc);

#define err_throw(ret) err_with(ret, < 0, "", return rc);

#endif
