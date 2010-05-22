#include "main.h"

void fb(void * data)
{
  sockbuf *b;
  b = (sockbuf*)data;
  ecv_params * par = b->apppar;
  if (data == NULL)
    return;
  if (b->size<=0)
    return;
  printf ("b->size %i\n", b->size);
  int a = 120;
  printf ("reading buffer\n");
  int p = *b->buf;
  printf ("buf = %s\n", &p);
  if (par->ifacein.state == ECV_HEADER)
    {
      switch (p)
	{
	case ECV_DATAREQUEST:
	  printf ("ECV_DATAREQUEST\n");
	  a = ECV_DATAREPLY;
	  write(par->ifacein.params->sockin, &a, sizeof(int));
	  write(par->ifacein.params->sockin, &(par->ifacein.offset), sizeof(int));
	  printf ("sent it\n");
	  break;
	default:
	  break;
	}
    }
}

enum
  {
    WIDTH_OPT=0,
    HEIGHT_OPT,
    THRESHOLD_OPT,
    VERBOSE_OPT,
    NUMLINES_OPT,
    DISCRETE_OPT,
    SERV_OPT,
    RADIUS_OPT,
    BGTHRESHOLD_OPT,
    GRAY_OPT,
    MINWIDTH_OPT,
    R_OPT,
    G_OPT,
    B_OPT,
    AVGCORR_OPT,
    THE_END
  };

const char *se_opts[] =
  {
    [WIDTH_OPT] = "width",
    [HEIGHT_OPT] = "height",
    [THRESHOLD_OPT] = "threshold",
    [VERBOSE_OPT] = "verbose",
    [NUMLINES_OPT] = "lines",
    [DISCRETE_OPT] = "discrete",
    [SERV_OPT] = "s",
    [RADIUS_OPT] = "radius",
    //
    [BGTHRESHOLD_OPT] = "bgthreshold",
    [GRAY_OPT] = "gray",
    [MINWIDTH_OPT] = "minwidth",
    [R_OPT] = "r",
    [G_OPT] = "g",
    [B_OPT] = "b",
    [AVGCORR_OPT] = "avgcorr",
    //
    [THE_END] = NULL
  };

enum
  {
    EDGEDETECT_OPT=0,
    COMPLABELING_OPT,
    COMPLABELING_SIMP_OPT,
    LIGHTS_OPT,
    THEM_END
  };

const char *me_opts[] =
  {
    [EDGEDETECT_OPT] = "edgedetect",
    [COMPLABELING_OPT] = "complabeling",
    [COMPLABELING_SIMP_OPT] = "cl_simp",
    [LIGHTS_OPT] = "lights",
    [THEM_END] = NULL
  };


void ecv_clainit(int argc, char **argv, ecv_cla * m_cla)
{
  char *subopts, *value;
  int opt; 

  m_cla->width = 320;
  m_cla->height = 240;
  m_cla->threshold = 0;
  m_cla->debugwin = 0;
  m_cla->numlines = 4;
  m_cla->discrete = 3;
  m_cla->serv = 0;
  m_cla->radius = 30;
  m_cla->method = ECV_EDGEDETECT;
  m_cla->gray = 1;
  m_cla->r = 0;
  m_cla->g = 0;
  m_cla->b = 0;
  m_cla->discardlevel=50;
  m_cla->minwidth=50;
  m_cla->avgcorrection=0;

  while ((opt = getopt (argc, argv, "hd:o:m:")) != -1)
    switch (opt)
      {
      case 'h':
	printf ("simevo -d </dev/devname> -m edgedetect -o [width=int,height=int,threshold=int,verbose,lines=int,discrete=3+3*int,s,radius=int]\n");
	printf ("\nedgedetect - edge recognition based method\n");
	printf ("s - start server for lua module\n");
	printf ("raduis - not implemented yet\n");
	printf ("\ntypical usage\nsimevo -d /dev/video1 -m edgedetect -o width=640,height=480,threshold=100,verbose,lines=10,discrete=3,s]\n");
	printf ("\nsimevo -d </dev/devname> -m complabeling -o [width=int,height=int,verbose,lines=int,discrete=3+3*int,s,bgthreshold=int,bgup,bgdown,discardlevel]\n");
	printf ("\ncomplabeling - component labeling based method\n");
	printf ("s - start server for lua module\n");
	exit(0);
	break;
      case 'd':
	m_cla->device = optarg;
	break;
      case 'm':
	subopts = optarg;
	while (*subopts != '\0')
	  switch (getsubopt (&subopts, me_opts, &value))
	    {
	    case EDGEDETECT_OPT:
	      m_cla->method = ECV_EDGEDETECT;
	      break;
	    case COMPLABELING_OPT:
	      m_cla->method = ECV_COMPLABELING;
	      break;
	    case COMPLABELING_SIMP_OPT:
	      m_cla->method = ECV_COMPLABELING_SIMP;
	      break;
	    case LIGHTS_OPT:
	      m_cla->method = ECV_LIGHTS;
	      break;
	    default:
	      printf ("Unknown method `%s'\n", value);
	      break;
	    }
	//	m_cla->method = optarg;
	break;
      case 'o':
	subopts = optarg;
	while (*subopts != '\0')
	  switch (getsubopt (&subopts, se_opts, &value))
	    {
	    case WIDTH_OPT:
	      m_cla->width = atoi (value);
	      break;
	    case HEIGHT_OPT:
	      m_cla->height = atoi (value);
	      break;
	    case THRESHOLD_OPT:
	      m_cla->threshold = atoi (value);
	      break;
	    case VERBOSE_OPT:
	      m_cla->debugwin = 1;
	      break; 
	    case NUMLINES_OPT:
	      m_cla->numlines = atoi (value);
	      break;
	    case DISCRETE_OPT:
	      m_cla->discrete = atoi (value);
	      break;
	    case SERV_OPT:
	      m_cla->serv = 1;
	      break;
	    case GRAY_OPT:
	      m_cla->gray = 1;
	      m_cla->r = 0;
	      m_cla->g = 0;
	      m_cla->b = 0;
	      break;
	    case R_OPT:
	      m_cla->gray = 0;
	      m_cla->r = 1;
	      m_cla->g = 0;
	      m_cla->b = 0;
	      break;
	    case G_OPT:
	      m_cla->gray = 0;
	      m_cla->r = 0;
	      m_cla->g = 1;
	      m_cla->b = 0;
	      break;
	    case B_OPT:
	      m_cla->gray = 0;
	      m_cla->r = 0;
	      m_cla->g = 0;
	      m_cla->b = 1;
	      break;
	    case AVGCORR_OPT:
	      m_cla->avgcorrection = atoi (value);
	      break;
	    default:
	      /* Unknown suboption. */
	      printf ("Unknown suboption `%s'\n", value);
	      break;
	    }
	break;
      default:
	abort ();
      }
}

int main (int argc, char **argv)
{
  ecv_params par;

  par.ifacein.state = ECV_HEADER;
  par.quit=0;

  ecv_cla mcla;

  ecv_clainit(argc, argv, &mcla);

  printf("******************************\n");
  printf("trying to create pipe:\n");
  printf("width = %i\n", mcla.width);
  printf("height = %i\n", mcla.height);
  printf("device = %s\n", mcla.device);
  printf("******************************\n");

  if (mcla.serv == 1)
    {
      par.ifacein.params = bk_serverinit(LUAIFACEPORTI, 0, 10);
      par.ifacein.params->callback = fb;
      par.ifacein.params->apppar=&par;
      par.ifacein.readsize = ECV_MAGICK;
      printf ("starting serv on port %i\n",LUAIFACEPORTI);
    }

  ecv_initlines(&mcla, &par.mbase);
  par.mbase.roottree = NULL;
  
  ecv_create_src_pipeline(&mcla, &par);

  ecv_initsdl(&mcla, &par);

  if (ecv_play(par.s_app.pipe_pro)!=0)
    return -1;

  GstBuffer *buf;
  unsigned int ctr = 0;
  while (!gst_app_sink_is_eos (GST_APP_SINK (par.s_app.sink)) && par.quit == 0) 
    {
      buf = gst_app_sink_pull_buffer (GST_APP_SINK (par.s_app.sink));
      //      printf ("retrieved buffer %p\n", buf);
      if (buf != NULL)
	{
	  printf ("retrieved buffer %i\n", ctr);
	  ctr = ctr+1;
	  //	  printf("size = %i\n", buf->size);
	  SDL_LockSurface(par.nextframe);
	  memcpy(par.nextframe->pixels, buf->data, buf->size);

	  if (mcla.method == ECV_EDGEDETECT)
	    ecv_edgedetect(buf->data, buf->size, &mcla, &par);
	  else if (mcla.method == ECV_COMPLABELING)
	    ecv_complabeling(buf->data, buf->size, &mcla, &par);
	  else if (mcla.method == ECV_COMPLABELING_SIMP)
	    ecv_complabeling_dumb(buf->data, buf->size, &mcla, &par);

	  SDL_UnlockSurface(par.nextframe);
	  ecv_drawframe(par.nextframe, &mcla, &par);
	  SDL_Flip(par.screen);

	  gst_buffer_unref (buf);
	}
     
      while( SDL_PollEvent( &par.event ) ) 
	{ 
	  if( par.event.type == SDL_QUIT ) 
	    {
	      par.quit = 1; 
	    } 
	}
      if (mcla.serv == 1)
	bk_serverstep(par.ifacein.params, par.ifacein.readsize);
    }
  ecv_stop(par.s_app.pipe_pro);
  SDL_FreeSurface(par.screen);
  SDL_FreeSurface(par.nextframe);
  if (mcla.serv == 1)
    bk_serverfinalize(par.ifacein.params);

  return 0;
}
