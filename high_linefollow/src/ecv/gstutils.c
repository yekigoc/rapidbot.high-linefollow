#include "gstutils.h"

GstElement *ecv_create_src_pipeline (ecv_cla * m_cla, ecv_params * m_par)
{
  ecv_App *app = &m_par->s_app;
  
  gst_init (0, 0);

  app->pipe_pro = gst_pipeline_new (NULL);
  g_assert (app->pipe_pro);

  char device[256];
  
  if (m_cla->method != ECV_LIGHTS)
    {
      sprintf (device,"v4l2src device=%s",m_cla->device);
      printf ("v4l2src device=%s\n",m_cla->device);
    }
  else
    {
      sprintf (device,"filesrc location=%s",m_cla->device);
      printf ("filesrc location=%s",m_cla->device);
    }

  app->src = gst_parse_launch(device, NULL);
  g_assert (app->src);
  gst_bin_add (GST_BIN (app->pipe_pro), app->src);

  /*  if (m_cla->method != ECV_LIGHTS)
    {
      sprintf(device, "capsfilter caps=\"video/x-raw-yuv,width=%i,height=%i\"",m_cla->width,m_cla->height);
      printf("capsfilter caps=\"video/x-raw-yuv,format:fourcc=YV12,width=%i,height=%i\"\n",m_cla->width,m_cla->height);
      app->cf = gst_parse_launch(device, NULL);
      g_assert (app->cf);
      gst_bin_add (GST_BIN (app->pipe_pro), app->cf);
      }*/


  app->id = gst_element_factory_make ("ffmpegcolorspace", NULL);
  g_assert (app->id);
  gst_bin_add (GST_BIN (app->pipe_pro), app->id);

  sprintf(device, "capsfilter caps=\"video/x-raw-rgb,width=%i,height=%i,bpp=24,red_mask=255, green_mask=65280, blue_mask=16711680, endianness=4321\"",m_cla->width,m_cla->height);
  printf("capsfilter caps=\"video/x-raw-rgb,width=%i,height=%i,bpp=(int)24,red_mask=(int)255, green_mask=(int)65280, blue_mask=(int)16711680, endianness=(int)4321\"\n",m_cla->width,m_cla->height);
  app->cf2 = gst_parse_launch(device, NULL);
  g_assert (app->cf2);
  gst_bin_add (GST_BIN (app->pipe_pro), app->cf2);

  app->sink = gst_element_factory_make ("appsink", NULL);
  g_assert (app->sink);

  gst_app_sink_set_max_buffers(app->sink, 1);
  gst_app_sink_set_drop(app->sink, TRUE);
  gst_bin_add (GST_BIN (app->pipe_pro), app->sink);


  gst_element_link (app->src, app->id);
  gst_element_link (app->id, app->cf2);
  gst_element_link (app->cf2, app->sink);
  return app->pipe_pro;
}

int ecv_play(GstElement *p)
{
  if (gst_element_set_state(GST_ELEMENT(p),GST_STATE_PLAYING)==GST_STATE_CHANGE_FAILURE)
    {
      printf ("Gstreamer failed to create pipeline\n");
      return -1;
    }
  return 0;
}

void ecv_stop(GstElement *p) 
{
  gst_element_set_state(GST_ELEMENT(p),GST_STATE_NULL);
}
