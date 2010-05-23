#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <debug_macros.h>
#include <rgi.pb-c.h>
#include <usbcommon.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>



#include "gstutils.h"
#include "hashutils.h"
#include "edgedetect.h"
#include "complabeling.h"
#include "complabeling_dumb.h"




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



struct carpad
{
  int leftstickx;
  int leftsticky;
  int rightstickx;
  int rightsticky;
};

#define MYID 10
#define c1 (double)104.0/32767.0
//#define c1 (double)60.0/32767.0
//#define c2 (double)125.0/32767.0
#define c2 (double)30.0/32767.0

int fd;
int rc;
struct carpad titc;
int pressed1;

unsigned int portno = 30000;
char hostname[] = "localhost";

int login(int * sd)
{
  Rgi__Tellid tellid = RGI__TELLID__INIT;
  tellid.id = MYID;
  unsigned short sz = rgi__tellid__get_packed_size(&tellid);
  char *buffer = (char*)malloc(sz*(sizeof(char)));
  rgi__tellid__pack(&tellid, buffer);
  unsigned short head = PER_TELLID;
  if(write(*sd, &head, 2)<0)
    return -1;
  if(write(*sd, &sz, 2)<0)
    return -1;
  if(write(*sd, buffer, sz)<0)
    return -1;
  free (buffer);
}

int sendpwm(int *sd)
{
  Rgi__Setpwmchan pwmchan = RGI__SETPWMCHAN__INIT;
  pwmchan.channel = 0;
  double k1 = ((double)titc.leftstickx);
  pwmchan.value = 396+k1;
  double pm = 396.0+k1;
  pmsg("channel 0 = %f\n", pm);
  unsigned short sz = rgi__setpwmchan__get_packed_size(&pwmchan);
  char * buffer = (char*)malloc(sz*(sizeof(char)));
  rgi__setpwmchan__pack(&pwmchan, buffer);
  unsigned short head = PER_PWM;
  if (write(*sd, &head, 2)<0)
    return -1;
  if (write(*sd, &sz, 2)<0)
    return -1;
  if (write(*sd, buffer, sz)<0)
    return -1;
  free (buffer);

  pwmchan.channel = 1;
  double k2 = ((((double)titc.rightsticky)));
  pwmchan.value = 374+k2*(c2);
  double sm = 374.0+k2*(c2);
  pmsg("channel 1 = %f\n", sm);
  sz = rgi__setpwmchan__get_packed_size(&pwmchan);
  buffer = (char*)malloc(sz*(sizeof(char)));
  rgi__setpwmchan__pack(&pwmchan, buffer);

  if (write(*sd, &head, 2)<0)
    return -1;
  if (write(*sd, &sz, 2)<0)
    return -1;
  if (write(*sd, buffer, sz)<0)
    return -1;
  free (buffer);
}

int lockusb(int * sd)
{
  Rgi__Setlock lock = RGI__SETLOCK__INIT;
  lock.lock = 1;
  unsigned short sz = rgi__setlock__get_packed_size(&lock);
  char *buffer = (char*)malloc(sz*(sizeof(char)));
  rgi__setlock__pack(&lock, buffer);
  unsigned short head = PER_SETLOCK;
  if(write(*sd, &head, 2)<0)
    {
      pmsg ("short write (head)\n");
      return -1;
    }
  if(write(*sd, &sz, 2)<0)
    {
      pmsg ("short write (sz)\n");
      return -1;
    }
  if(write(*sd, buffer, sz)<0)
    {
      pmsg ("short write (buffer)\n");
      return -1;
    }

  pmsg("reading reply\n");
  int ret = 0;
  read(*sd, &head, 2);
  head = ntohs(head);
  pmsg ("head = %i\n", head);
  pmsg("reading size\n");

  read(*sd, &sz, 2);
  sz = ntohs(sz);
  pmsg ("sz = %i\n", sz);
  pmsg ("head = %i\n", head);
  if (head == PER_TELLREADY)
    {
      pmsg ("PER_TELLREADY\n");
      ret = 0;
    }
  else
    {
      pmsg ("PER_TELLLOCKED\n");
      ret = 1;
    }
  pmsg ("reading rest data\n");
  char buf[16];
  read(*sd, buf, sz);
      
  return ret;
}

int unlockusb(int * sd)
{
  Rgi__Setunlock unlock = RGI__SETUNLOCK__INIT;
  unlock.lock = 1;
  unsigned short sz = rgi__setunlock__get_packed_size(&unlock);
  char *buffer = (char*)malloc(sz*(sizeof(char)));
  rgi__setunlock__pack(&unlock, buffer);
  unsigned short head = PER_SETUNLOCK;
  if(write(*sd, &head, 2)<0)
    {
      pmsg ("short write (head)\n");
      return -1;
    }
  if(write(*sd, &sz, 2)<0)
    {
      pmsg ("short write (sz)\n");
      return -1;
    }
  if(write(*sd, buffer, sz)<0)
    {
      pmsg ("short write (buffer)\n");
      return -1;
    }

  int ret = 0;
  pmsg("reading reply\n");
  read(*sd, &head, 2);
  head = ntohs(head);
  pmsg ("head = %i\n", head);
  pmsg("reading size\n");

  read(*sd, &sz, 2);
  sz = ntohs(sz);
  pmsg ("sz = %i\n", sz);
  pmsg ("head = %i\n", head);
  if (head == PER_TELLREADY)
    {
      pmsg ("PER_TELLREADY\n");
      ret = 0;
    }
  else
    {
      pmsg ("PER_TELLLOCKED\n");
      ret = 1;
    }
  pmsg ("reading rest data\n");
  char buf[16];
  read(*sd, buf, sz);

  return ret;
}

static void *thread_func(void *vptr_args)
{
  int xctr = 0;
  int yctr = 0;
  int a = 1;
  int m = 5;
  while (1)
    {
      if (yctr>32767)
	{
	  a=-a;
	}
      else if(yctr<-32767)
	{
	  a=-a;
	}
      yctr += m*a;
      
      usleep(10000);
    }
 
  return NULL;
}

int main (int argc, char **argv)
{
  ecv_params par;

  par.quit=0;

  ecv_cla mcla;

  ecv_clainit(argc, argv, &mcla);

  printf("******************************\n");
  printf("trying to create pipe:\n");
  printf("width = %i\n", mcla.width);
  printf("height = %i\n", mcla.height);
  printf("device = %s\n", mcla.device);
  printf("******************************\n");


  ecv_initlines(&mcla, &par.mbase);
  par.mbase.roottree = NULL;
  
  ecv_create_src_pipeline(&mcla, &par);







  titc.leftstickx=0;
  titc.leftsticky=0;
  titc.rightstickx=0;
  titc.rightsticky=0;
  pressed1 = 0;
  
  int sd = socket(AF_INET, SOCK_STREAM, 0);  /* init socket descriptor */
  struct sockaddr_in sin;
  struct hostent *host = gethostbyname(hostname);
  int len;

  /*** PLACE DATA IN sockaddr_in struct ***/
  memcpy(&sin.sin_addr.s_addr, host->h_addr, host->h_length);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(portno);

  /*** CONNECT SOCKET TO THE SERVICE DESCRIBED BY sockaddr_in struct ***/
  if (connect(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
      perror("connecting");
      exit(1);
    }

  pthread_t thread;
  
  /*  if (pthread_create(&thread, NULL, thread_func, NULL) != 0)
    {
      return EXIT_FAILURE;
      }*/

  sleep(1);   /* give server time to reply */
  unsigned short head;
  int locked = 0;
  int ret = 0;
  login(&sd);
  ret = lockusb(&sd);
  printf ("lockusb = %i\n", ret);
  locked = 1;


  if (ecv_play(par.s_app.pipe_pro)!=0)
    return -1;

  GstBuffer *buf;
  unsigned int ctr = 0;

  int state = 1;
  //1 - linefollow
  //2 - collision avoid

  while (!gst_app_sink_is_eos (GST_APP_SINK (par.s_app.sink)) && par.quit == 0) 
    {
      buf = gst_app_sink_pull_buffer (GST_APP_SINK (par.s_app.sink));
      //      printf ("retrieved buffer %p\n", buf);
      if (buf != NULL)
	{
	  printf ("retrieved buffer %i\n", ctr);
	  ctr = ctr+1;
	  //	  printf("size = %i\n", buf->size);

	  if (mcla.method == ECV_EDGEDETECT)
	    ecv_edgedetect(buf->data, buf->size, &mcla, &par);
	  else if (mcla.method == ECV_COMPLABELING)
	    ecv_complabeling(buf->data, buf->size, &mcla, &par);
	  else if (mcla.method == ECV_COMPLABELING_SIMP)
	    ecv_complabeling_dumb(buf->data, buf->size, &mcla, &par);

	  gst_buffer_unref (buf);
	}

      switch (state)
      {
      case 1:
	if (par.offset>170 && par.offset<200)
	  titc.leftstickx=50;
	else if (par.offset>200)
	  titc.leftstickx=100;
	else if (par.offset<130 && par.offset>100)
	  titc.leftstickx=-50;
	else if (par.offset<100) 
	  titc.leftstickx=-100;
	else if (par.offset == 0)
	  titc.leftstickx=0;
      }


      sendpwm(&sd);
      printf ("offset=%i\n", par.offset);
      usleep(50000);
    }
  ecv_stop(par.s_app.pipe_pro);

  ret = unlockusb(&sd);
  printf ("unlockusb = %i\n", ret);
  locked = 0;
  
  close(sd);
}
