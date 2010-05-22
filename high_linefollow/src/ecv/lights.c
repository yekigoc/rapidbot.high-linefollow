#include "lights.h"

void ecv_lights(unsigned char * m_data, unsigned int m_size, ecv_cla * m_cla, ecv_params * m_par)
{
  int i = 0;
  int j = 0;
  double pbyte=m_data[0];
  int offset=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
  
  GHashTableIter it;
  ecv_point * stptr;
  unsigned int * key;
  double avg = 0;
  
  pbyte = ((double)(m_data[offset+0]+m_data[offset+0+1]+m_data[offset+0+1]))/3;

  ecv_preparelines(&m_par->mbase, m_cla);

  double ctr = 0;

  for (; i<m_cla->numlines; i++)
    {
      avg = 0;
      ctr = 0;
      for (j=0; j<m_cla->width*3; j+=m_cla->discrete)
	{
	  if (fabs(pbyte-((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3)>fabs((double)(m_cla->threshold)/10))
	    {
	      ecv_line_addpoint(&m_par->mbase, i, j/3);
	      avg += j/3;
	      ctr += 1;
	    }
	  pbyte = ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
	}

      if (ctr != 0)
	{
	  m_par->mbase.lines[i]->respoint->x=(unsigned int)avg/ctr;
	}
      if (m_par->mbase.lines[i]->topid!=0)
	{
	  memset(m_par->nextframe->pixels+offset,0x0,m_cla->width*3);
	  memset(m_par->nextframe->pixels+offset+m_par->mbase.lines[i]->respoint->x*3,0x0,1);
	  memset(m_par->nextframe->pixels+offset+m_par->mbase.lines[i]->respoint->x*3+1,0x0,1);
	  memset(m_par->nextframe->pixels+offset+m_par->mbase.lines[i]->respoint->x*3+2,0xFF,1);
	  g_hash_table_iter_init(&it, m_par->mbase.lines[i]->points);

	  while (g_hash_table_iter_next(&it, (void**)&key, (void**)&stptr))
	    {
	      memset(m_par->nextframe->pixels+offset+(stptr->x)*3,0x0,1);
	      memset(m_par->nextframe->pixels+offset+(stptr->x)*3+1,0x0,1);
	      memset(m_par->nextframe->pixels+offset+(stptr->x)*3+2,0xFF,1);
	      memset(m_par->nextframe->pixels+offset-m_cla->width*3+(stptr->x)*3,0x0,1);
	      memset(m_par->nextframe->pixels+offset-m_cla->width*3+(stptr->x)*3+1,0x0,1);
	      memset(m_par->nextframe->pixels+offset-m_cla->width*3+(stptr->x)*3+2,0xFF,1);
	      memset(m_par->nextframe->pixels+offset+m_cla->width*3+(stptr->x)*3,0x0,1);
	      memset(m_par->nextframe->pixels+offset+m_cla->width*3+(stptr->x)*3+1,0x0,1);
	      memset(m_par->nextframe->pixels+offset+m_cla->width*3+(stptr->x)*3+2,0xFF,1);
	    }
	}
      offset+=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;    
    }
  m_par->ifacein.offset = m_par->mbase.lines[0]->respoint->x;
  printf ("offset = %i\n",m_par->ifacein.offset);
}
