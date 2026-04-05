#ifndef YAYA_H_
#define YAYA_H_

extern "C" long multi_load(char *, long);
extern "C" int multi_unload(long);
extern "C" char *multi_request(long, char *, long *);

#endif // YAYA_H_
