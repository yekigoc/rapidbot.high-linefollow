#include "complabeling_dumb.h"

void ecv_complabeling_dumb(unsigned char * m_data, unsigned int m_size, ecv_cla * m_cla, ecv_params * m_par)
{
  printf ("new iteration\n");
  int i = 0;
  int j = 0;
  double pbyte;
  //  double pbyte=m_data[0];
  int offset = m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
  
  double avg = 0;
  int upctr = 0;
  int downctr = 0;
  
  //  pbyte = ((double)(m_data[offset+0]+m_data[offset+0+1]+m_data[offset+0+1]))/3;
  pbyte = 0;
  
  //  ecv_preparelines(&m_par->mbase, m_cla);
  
  double ctr = 0;

  
  for (i=0; i<m_cla->numlines;i++)
    {
      if (m_par->mbase.lines[i]->aggr!=NULL)
	{
	  printf ("deleting list\n");
	  ecv_aggr_deletelist(m_par->mbase.lines[i]->aggr);
	  printf("deleted list\n");
	}
      for (j=0; j<m_cla->width;j++)
	{
	  m_par->mbase.lines[i]->line[j]=0;
	}
    }
  m_par->mbase.topid=1;
  i=0;
  j=0;
  ecv_preparelines(&m_par->mbase, m_cla);
  for (; i<m_cla->numlines; i++)
    {
      upctr = 0;
      downctr = 0;
      
      for (j = 0; j<m_cla->width*3; j+=m_cla->discrete)
	{
	  if (m_cla->gray==1)
	    pbyte += ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
	}
      avg = pbyte/m_cla->width;
      pbyte=0;
      for (j=0; j<m_cla->width*3; j+=m_cla->discrete)
	{
	  if (m_cla->gray==1)
	    pbyte = ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;

	  if (pbyte>avg)
	    upctr++;
	  else
	    downctr++;
	}
      printf ("downctr=%i, upctr=%i\n",downctr, upctr);
      
      if (upctr>downctr)
	{
	  if (((double)downctr)/((double)upctr)>((double)m_cla->discardlevel)/100)
	    {
	      m_par->mbase.discarded[i]=ECV_DISCARDED;
	      printf ("downctr percents is %f, discarded line\n",((double)downctr)/((double)upctr));
	    }
	  else
	    {
	      for (j=0; j<m_cla->width*3; j+=m_cla->discrete)
		{
		  if (m_cla->gray==1)
		    pbyte = ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
		  if (pbyte>avg)
		    {
		      m_par->mbase.lines[i]->line[j/3]=0x0;
		    }
		  else
		    {
		      m_par->mbase.lines[i]->line[j/3]=ECV_MAX;
		    }
		}
	    }
	}
      else 
	{
	  if (((double)upctr)/((double)downctr)>((double)m_cla->discardlevel)/100)
	    {
	      m_par->mbase.discarded[i]=ECV_DISCARDED;
	      printf ("upctr percents is %f, discarded line\n",((double)upctr)/((double)downctr));
	    }
	  else
	    {
	      for (j=0; j<m_cla->width*3; j+=m_cla->discrete)
		{
		  if (m_cla->gray==1)
		    pbyte = ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
		  if (pbyte<avg)
		    {
		      m_par->mbase.lines[i]->line[j/3]=0x0;
		    }
		  else
		    {
		      m_par->mbase.lines[i]->line[j/3]=ECV_MAX;
		    }
		}
	    }
	}
      offset+=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
    }
  int k = 0;
  unsigned int lowestid=0;
  unsigned char clow=1;
  ecv_node * n;
  ecv_node * tmp;
  int li;
  int lj;
  ecv_tree * ttree;
  ecv_aggr * ma;
  ecv_aggr * mb;
	

  printf ("pass 2 \n");		  
  offset=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
  for (i=0; i<m_cla->numlines; i++)
    {
      if (m_par->mbase.discarded[i]!=ECV_DISCARDED)
	{
	  for (j=0;j<m_cla->width;j++)
	    {
	      if (m_par->mbase.lines[i]->line[j]==ECV_MAX)
		{
		  //n = m_par->mbase.lines[i]->nodeline[j];
		  if (j-1>=0)
		    {
		      if (m_par->mbase.lines[i]->line[j-1]!=0)
			{
			  if (m_par->mbase.lines[i]->aggr!=NULL)
			    {
			      ma = ecv_aggr_findbottom(m_par->mbase.lines[i]->aggr);
			      ma->width++;
			    }
			}
		      else
			{
			  if (m_par->mbase.lines[i]->aggr!=NULL)
			    {
			      ma = ecv_aggr_findbottom(m_par->mbase.lines[i]->aggr);
			      mb = ecv_aggr_createaggrfull(j,i,j+i);
			      ecv_aggr_merge(ma, mb);
			    }
			  else
			    {
			      m_par->mbase.lines[i]->aggr = ecv_aggr_createaggrfull(j,i,j+i);
			    }
			}
		    }
		  else
		    {
		      m_par->mbase.lines[i]->aggr = ecv_aggr_createaggrfull(j,i,j+i);
		    }
		}
	    }
	  offset+=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
	}
    }
  offset=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;

  printf ("pass 3\n");

  ecv_aggr * mc;
  for (i = 0;i<m_cla->numlines;i++)
    {
      if (m_par->mbase.discarded[i]!=ECV_DISCARDED)
	{
	  printf ("looking for tip\n");
	  ma = ecv_aggr_findhead(m_par->mbase.lines[i]->aggr);
	  printf ("founf %p\n",ma);
	  while (ma!=NULL)
	    {
	      printf ("checking width\n");
	      if (ma->width<m_cla->minwidth)
		{
		  printf ("too narrow\n");
		}
	      else
		{
		  printf ("ok\n");
		  for(j=0;j<ma->width;j++)
		    {
		      clow = clow + 1;
		      		      
		    }
		  
		}
	      printf ("ma = %p\n",ma);
	      ma = ma->child;
	      printf ("iterated\n");
	    }
	}
      offset+=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
    }
}
