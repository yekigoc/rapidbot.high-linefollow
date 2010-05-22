bin_PROGRAMS = linefollow
linefollow_SOURCES = main.c ../../../rapidbot-high-inputdemux/lib/rgi.pb-c.c ./ecv/edgedetect.c ./ecv/complabeling_dumb.c ./ecv/complabeling.c ./ecv/hashutils.c ./ecv/gstutils.c ./ecv/listutils.c ./ecv/treeutils.c
linefollow_LDFLAGS = -Wl,--export-dynamic -rdynamic -lprotobuf-c -pthread -lm -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lrt -lxml2 -lglib-2.0 -lgstapp -L/usr/lib/gstreamer-0.10 -Wl,--rpath -Wl,/usr/lib/gstreamer-0.10 
linefollow_CFLAGS = -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/google/protobuf-c -std=gnu99 -I../../../rapidbot-high-inputdemux/lib -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/libxml2 -pthread -I./ecv/
