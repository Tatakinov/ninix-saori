#ifndef UNIX_SAORI_H_
#define UNIX_SAORI_H_

#ifndef LIBRARY_NAME
#error "define LIBRARY_NAME"
#endif

#define HELPER(a, b) a ## b
#define FUNC(a, b) HELPER(a, b)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

long FUNC(LIBRARY_NAME, _saori_load)(char *path, long len);
int FUNC(LIBRARY_NAME, _saori_unload)(long id);
char *FUNC(LIBRARY_NAME, _saori_request)(long id, char *request, long *len);

#ifdef __cplusplus
}
#endif // __cplusplus

#undef HELPER
#undef FUNC

#endif // UNIX_SAORI_H_
