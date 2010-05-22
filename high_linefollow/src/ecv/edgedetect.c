#include "edgedetect.h"

void ecv_edgedetect(char * m_data, unsigned int m_size, ecv_cla * m_cla, ecv_params * m_par)
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
      offset+=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;    
    }
  m_par->offset = m_par->mbase.lines[0]->respoint->x;
  printf ("offset = %i\n",m_par->offset);
}
