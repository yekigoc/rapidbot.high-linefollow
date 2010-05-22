bin_PROGRAMS = linefollow
linefollow_SOURCES = main.c ../../../rapidbot-high-inputdemultiplexor/lib/rgi.pb-c.c
linefollow_LDFLAGS = -Wl,--export-dynamic -rdynamic -lprotobuf-c -pthread -lm
linefollow_CFLAGS = -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/google/protobuf-c -std=gnu99 -I../../../rapidbot-high-inputdemultiplexor/lib