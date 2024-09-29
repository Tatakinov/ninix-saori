CXX					= clang++
CXXFLAGS		= -I . -fPIC -I skeleton -Wall -Ofast -std=c++17
LD					= clang++
LDFLAGS			= -shared
OBJS				= foo.o
SKELETON_OBJS	= skeleton/base/header.o
TEST_OBJS		= implementation/test/lib.o
CURL_OBJS		= implementation/saori_curl/lib.o
ALL					= all

PREFIX=/opt/ninix-kagari/lib/saori/

.SUFFIXES: .cc .o

.PHONY: all
$(ALL): libtest.so libsaori_curl.so

libtest.so: $(SKELETON_OBJS) $(TEST_OBJS)
	$(LD) $(LDFLAGS) -o libtest.so $(TEST_OBJS) $(SKELETON_OBJS)
ifeq ("$(wildcard saori_curl.dll)", "")
	ln -s libtest.so test.dll
endif

libsaori_curl.so: $(SKELETON_OBJS) $(CURL_OBJS)
	$(LD) $(LDFLAGS) -o libsaori_curl.so $(CURL_OBJS) $(SKELETON_OBJS) -lcurl
ifeq ("$(wildcard saori_curl.dll)", "")
	ln -s libsaori_curl.so saori_curl.dll
endif

.cc.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: install
install:
	cp lib*.so *.dll $(PREFIX)

.PHONY: clean
clean:
	rm -f *.o */*/*.o *.so *.dll
