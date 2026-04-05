#ifndef KAWARI_H_
#define KAWARI_H_

extern "C" unsigned int so_create(char *, long);
extern "C" int so_dispose(unsigned int);
extern "C" char *so_request(unsigned int, char *, long *);
extern "C" void so_free(unsigned int, const char *);

#endif // KAWARI_H_
