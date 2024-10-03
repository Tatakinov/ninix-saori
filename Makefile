CXX					= clang++
CXXFLAGS		= -I . -fPIC -I skeleton -Wall -Ofast -std=c++17
LD					= clang++
LDFLAGS			= -shared
OBJS				= foo.o
SKELETON_OBJS	= skeleton/base/header.o
EXAMPLE_OBJS		= implementation/example/lib.o
CURL_OBJS		= implementation/saori_curl/lib.o
ALL					= all

PREFIX=/opt/ninix-kagari/lib/saori/

.SUFFIXES: .cc .o

.PHONY: all
$(ALL): libexample.so libsaori_curl.so

libexample.so: $(SKELETON_OBJS) $(EXAMPLE_OBJS)
	$(LD) $(LDFLAGS) -o libexample.so $(EXAMPLE_OBJS) $(SKELETON_OBJS)
ifeq ("$(wildcard saori_curl.dll)", "")
	ln -s libexample.so example.dll
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
