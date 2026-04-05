#ifndef AOSORA_H_
#define AOSORA_H_

extern "C" long aosora_load(char *, long);
extern "C" int aosora_unload(long);
extern "C" char *aosora_request(long, char *, long *);

#endif // AOSORA_H_
