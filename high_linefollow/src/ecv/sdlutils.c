#include "sdlutils.h"

int ecv_initsdl(ecv_cla * m_cla, ecv_params * m_par)
{
  Uint32 flags = SDL_DOUBLEBUF;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
      fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
      return -2;
    }

  atexit(SDL_Quit);
 
  m_par->screen = SDL_SetVideoMode(m_cla->width, m_cla->height, 24, flags);
  if (!m_par->screen) 
    {
      printf("Unable to set video mode: %s\n", SDL_GetError());
      return -1;
    }
  m_par->nextframe = SDL_CreateRGBSurface(m_par->screen->flags, m_par->screen->w, m_par->screen->h, 24,0,0,0,0);

  return 0;
}

void ecv_drawframe(SDL_Surface *m_srcimg, ecv_cla * m_cla, ecv_params * m_par)
{
  SDL_Rect src, dst;
  
  src.x = 0;  src.y = 0;  src.w = m_cla->width;  src.h = m_cla->height;
  dst.x = 0;  dst.y = 0;  dst.w = m_cla->width;  dst.h = m_cla->height;
  SDL_BlitSurface(m_srcimg, &src, m_par->screen, &dst);
}
