#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappbuffer.h>
#include <gst/app/gstappsink.h>

//#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

//#include <SDL.h>

#include <glib.h>

#define ECV_EDGEDETECT 1
#define ECV_COMPLABELING 2
#define ECV_COMPLABELING_SIMP 3
#define ECV_LIGHTS 3

#define ECV_DISCARDED 1

#define ECV_MAX 0xFFFFFFFF

typedef struct ecv_App
{
  GstElement *pipe_pro;
  GstElement *pipe_dbg;
  GstElement *src;
  GstElement *id;
  GstElement *cf;
  GstElement *cf2;
  GstElement *sink;
  GstElement *debugsrc;
  GstElement *debugsink;
} ecv_App;

typedef struct ecv_cla
{
  char * device;
  int width;
  int height;
  int threshold;
  int debugwin;
  int numlines;
  int discrete;
  int serv;
  int radius;
  int gray;
  int method;
  int discardlevel;
  int minwidth;
  int r;
  int g;
  int b;
  int avgcorrection;
} ecv_cla;

typedef struct ecv_point
{
  unsigned int x;
} ecv_point;

typedef struct ecv_node
{
  void * parent;
  void * child;
  unsigned int i;
  unsigned int j;
  unsigned int id;
  unsigned int processed;
} ecv_node;

typedef struct ecv_aggr
{
  unsigned int width;
  unsigned int spx;
  unsigned int spy;
  void * parent;
  void * child;
} ecv_aggr;

typedef struct ecv_line
{
  ecv_point * respoint;
  unsigned int topid;
  unsigned int agtopid;
  GHashTable * points;
  GHashTable * aggregations;
  int id;
  unsigned int * line;
  ecv_node ** nodeline;
  ecv_aggr * aggr;
  double resoffset;
  int numpoints;
  double multiplier;
} ecv_line;

typedef struct ecv_tree
{
  void * parent;
  void * child;
  ecv_node * root;
  int id;
  unsigned int size;
} ecv_tree;

typedef struct ecv_base
{
  int * discarded;
  unsigned int topid;
  ecv_line ** lines;
  ecv_tree * roottree;
} ecv_base;

typedef struct ecv_params
{
  ecv_App s_app;
  int quit;
  ecv_base mbase;
  int offset;
  //  ecv_luaiface ifacein;
} ecv_params;

#endif
