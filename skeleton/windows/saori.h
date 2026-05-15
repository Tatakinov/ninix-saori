#ifndef WINDOWS_SAORI_H_
#define WINDOWS_SAORI_H_

#ifndef LIBRARY_NAME
#error "define LIBRARY_NAME"
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

__declspec(dllexport) BOOL __cdecl load(HGLOBAL *h, long len);
__declspec(dllexport) BOOL __cdecl unload();
__declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL *h, long *len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // WINDOWS_SAORI_H_
