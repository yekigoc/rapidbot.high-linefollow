#radon construction suite
#local
include ../config_main
include ./../$(CONFINC)
PURESOURCES=main.c ../sock.c sdlutils.c edgedetect.c complabeling_dumb.c complabeling.c hashutils.c gstutils.c listutils.c treeutils.c
PURECFLAGS+=`pkg-config gstreamer-0.10 --cflags` `sdl-config --cflags` -I/usr/include/gtk-1.2 -I/usr/include -g -O0
PURELDFLAGS+=-L/usr/lib `pkg-config gstreamer-0.10 --libs` `sdl-config --libs` -lgstapp -L/usr/lib/gstreamer-0.10 -Wl,--rpath -Wl,/usr/lib/gstreamer-0.10 
PURESHARED=
PUREOBJECTS=$(PURESOURCES:.c=.o)

PUREEXECUTABLE=../main

all: $(PUREEXECUTABLE) $(BIND)

$(PUREEXECUTABLE): $(PUREOBJECTS)
	$(ECHO) "----- Building pure executable -----"
	$(CC) $(PURESHARED) $(PURELDFLAGS) $(PUREOBJECTS) -o $@

$(PUREOBJECTS): %.o: %.c
	$(CC) $(PURECFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(DELETE) $(BIND) $(PUREEXECUTABLE) $(PUREOBJECTS)

