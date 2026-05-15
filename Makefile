CXX					= clang++
CXXFLAGS		= -I . -fPIC -I skeleton -Wall -O3 -std=c++17
LD					= clang++
LDFLAGS			= -shared
OBJS				= foo.o

SHIORI_PATH		= /opt/ninix-kagari/lib

SKELETON_OBJS	= skeleton/base/header.o
EXAMPLE_OBJS		= implementation/example/lib.o
CURL_OBJS		= implementation/saori_curl/lib.o
AOSORA_OBJS		= implementation/saori_aosora/lib.o
KAWARI_OBJS		= implementation/saori_kawari/lib.o
SATORI_OBJS		= implementation/saori_satori/lib.o
YAYA_OBJS		= implementation/saori_yaya/lib.o
TRANSLATOR_OBJS = implementation/saori_call_translator/lib.o
ALL					= all

PREFIX=/opt/ninix-kagari/lib/saori/

.SUFFIXES: .cc .o

.PHONY: all
$(ALL): libexample.so libsaori_curl.so libsaori_call_translator.so libsaori_aosora.so libsaori_kawari.so libsaori_satori.so libsaori_yaya.so

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

libsaori_call_translator.so: $(SKELETON_OBJS) $(TRANSLATOR_OBJS)
	$(LD) $(LDFLAGS) -o libsaori_call_translator.so $(TRANSLATOR_OBJS) $(SKELETON_OBJS)
ifeq ("$(wildcard call_translator.dll)", "")
	ln -s libsaori_call_translator.so call_translator.dll
endif

libsaori_aosora.so: $(AOSORA_OBJS)
	$(LD) -o $@ $(LDFLAGS) -L $(SHIORI_PATH)/aosora -laosora
ifeq ("$(wildcard aosora.dll)", "")
	ln -s libsaori_aosora.so aosora.dll
endif

libsaori_kawari.so: $(KAWARI_OBJS)
	$(LD) -o $@ $(LDFLAGS) -L $(SHIORI_PATH)/kawari8 -lshiori
ifeq ("$(wildcard kawari.dll)", "")
	ln -s libsaori_kawari.so kawari.dll
endif

libsaori_satori.so: $(SATORI_OBJS)
	$(LD) -o $@ $(LDFLAGS) -L $(SHIORI_PATH)/satori -lsatori
ifeq ("$(wildcard satori.dll)", "")
	ln -s libsaori_satori.so satori.dll
endif

libsaori_yaya.so: $(YAYA_OBJS)
	$(LD) -o $@ $(LDFLAGS) -L $(SHIORI_PATH)/yaya -laya5
ifeq ("$(wildcard yaya.dll)", "")
	ln -s libsaori_yaya.so yaya.dll
endif

.cc.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: install
install:
	mkdir -p $(PREFIX)
	cp -r lib*.so *.dll $(PREFIX)

.PHONY: clean
clean:
	rm -f *.o */*/*.o *.so *.dll
