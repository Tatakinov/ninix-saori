#ifndef SATORI_H_
#define SATORI_H_

extern "C" int satori_load(char *, long);
extern "C" int satori_unload(long);
extern "C" char *satori_request(int, char *, long *);

#endif // SATORI_H_
