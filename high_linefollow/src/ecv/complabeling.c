#include "complabeling.h"

void ecv_complabeling(unsigned char * m_data, unsigned int m_size, ecv_cla * m_cla, ecv_params * m_par)
{
  int i = 0;
  int j = 0;
  double pbyte;
  //  double pbyte=m_data[0];
  int offset = m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
  
  double avg = 0;
  int upctr = 0;
  int downctr = 0;
  
  pbyte = 0;
  
  //  ecv_preparelines(&m_par->mbase, m_cla);
  
  double ctr = 0;
  
  for (i=0; i<m_cla->numlines;i++)
    {
      m_par->mbase.lines[i]->resoffset=0.0;
      m_par->mbase.lines[i]->numpoints=0;
      m_par->mbase.lines[i]->multiplier=0.0;
      for (j=0; j<m_cla->width;j++)
	{
	  m_par->mbase.lines[i]->line[j]=0;
	  if (m_par->mbase.lines[i]->nodeline[j]!=NULL)
	    free(m_par->mbase.lines[i]->nodeline[j]);
	  m_par->mbase.lines[i]->nodeline[j]=NULL;
	}
    }
  m_par->mbase.topid=1;
  i=0;
  j=0;
  ecv_preparelines(&m_par->mbase, m_cla);
  if (m_par->mbase.roottree!=NULL)
    ecv_tree_freeall(m_par->mbase.roottree);
  m_par->mbase.roottree = NULL;
  for (; i<m_cla->numlines; i++)
    {
      upctr = 0;
      downctr = 0;
      
      for (j = 0; j<m_cla->width*3; j+=m_cla->discrete)
	{
	  if (m_cla->gray==1)
	    pbyte += ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
	  if (m_cla->r==1)
	    pbyte += ((double)(m_data[offset+j]));
	  if (m_cla->g==1)
	    pbyte += ((double)(m_data[offset+j+1]));
	  if (m_cla->b==1)
	    pbyte += ((double)(m_data[offset+j+2]));
	}
      avg = pbyte/m_cla->width;
      pbyte=0;
      for (j=0; j<m_cla->width*3; j+=m_cla->discrete)
	{
	  if (m_cla->gray==1)
	    pbyte = ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
	  if (m_cla->r==1)
	    pbyte = ((double)(m_data[offset+j]));
	  if (m_cla->g==1)
	    pbyte = ((double)(m_data[offset+j+1]));
	  if (m_cla->b==1)
	    pbyte = ((double)(m_data[offset+j+2]));

	  if (pbyte>avg)
	    upctr++;
	  else
	    downctr++;
	}
      //printf ("downctr=%i, upctr=%i\n",downctr, upctr);
      
      if (upctr>downctr)
	{
	  if (((double)downctr)/((double)upctr)>((double)m_cla->discardlevel)/100)
	    {
	      m_par->mbase.discarded[i]=ECV_DISCARDED;
	      //printf ("downctr percent is %f, discarded line\n",((double)downctr)/((double)upctr));
	    }
	  else
	    {
	      for (j=0; j<m_cla->width*3; j+=m_cla->discrete)
		{
		  if (m_cla->gray==1)
		    pbyte = ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
		  if (m_cla->r==1)
		    pbyte = ((double)(m_data[offset+j]));
		  if (m_cla->g==1)
		    pbyte = ((double)(m_data[offset+j+1]));
		  if (m_cla->b==1)
		    pbyte = ((double)(m_data[offset+j+2]));

		  if (pbyte>avg-m_cla->avgcorrection)
		    {
		      m_par->mbase.lines[i]->line[j/3]=0x0;
		      m_par->mbase.lines[i]->nodeline[j/3] = NULL;
		    }
		  else
		    {
		      m_par->mbase.lines[i]->line[j/3]=ECV_MAX;
		      m_par->mbase.lines[i]->nodeline[j/3] = ecv_list_createnode(i,j/3);
		    }
		}
	    }
	}
      else 
	{
	  if (((double)upctr)/((double)downctr)>((double)m_cla->discardlevel)/100)
	    {
	      m_par->mbase.discarded[i]=ECV_DISCARDED;
	      //printf ("upctr percent is %f, discarded line\n",((double)upctr)/((double)downctr));
	    }
	  else
	    {
	      for (j=0; j<m_cla->width*3; j+=m_cla->discrete)
		{
		  if (m_cla->gray==1)
		    pbyte = ((double)(m_data[offset+j]+m_data[offset+j+1]+m_data[offset+j+2]))/3;
		  if (m_cla->r==1)
		    pbyte = ((double)(m_data[offset+j]));
		  if (m_cla->g==1)
		    pbyte = ((double)(m_data[offset+j+1]));
		  if (m_cla->b==1)
		    pbyte = ((double)(m_data[offset+j+2]));

		  if (pbyte<avg+m_cla->avgcorrection)
		    {
		      m_par->mbase.lines[i]->line[j/3]=0x0;
		      m_par->mbase.lines[i]->nodeline[j/3] = NULL;
		    }
		  else
		    {
		      m_par->mbase.lines[i]->line[j/3]=ECV_MAX;
		      m_par->mbase.lines[i]->nodeline[j/3] = ecv_list_createnode(i,j/3);
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
			  
  offset=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
  for (i=0; i<m_cla->numlines; i++)
    {
      if (m_par->mbase.discarded[i]!=ECV_DISCARDED)
	{
	  for (j=0;j<m_cla->width;j++)
	    {
	      if (m_par->mbase.lines[i]->line[j]==ECV_MAX)
		{
		  n = m_par->mbase.lines[i]->nodeline[j];
		  lowestid=ECV_MAX;
		  k=i;
		  while ((k-1>=0))
		    {
		      k=k-1;
		      if (m_par->mbase.discarded[k]!=ECV_DISCARDED)
			break;
		    }
		  if (k!=i)
		    {
		      if (m_par->mbase.lines[k]->line[j]!=0)
			{
			  if (lowestid>m_par->mbase.lines[k]->line[j])
			    {
			      lowestid = m_par->mbase.lines[k]->line[j];
			      li = k;
			      lj = j;
			    }
			}
		      if (j-1>=0)
			{
			  if ( (m_par->mbase.lines[k]->line[j-1]!=0))
			    { 
			      if ((lowestid>m_par->mbase.lines[k]->line[j-1]))
				{
				  lowestid = m_par->mbase.lines[k]->line[j-1];
				  li = k;
				  lj = j-1;
				}
			    }
			}
		      if (j+1<m_cla->width)
			{
			  if ((m_par->mbase.lines[k]->line[j+1]!=0))
			    {
			      if (lowestid>m_par->mbase.lines[k]->line[j+1])
				{
				  lowestid = m_par->mbase.lines[k]->line[j+1];
				  li = k;
				  lj = j+1;
				}
			    }
			}
		    }
		  if (j-1>=0)
		    {
		      if ((m_par->mbase.lines[i]->line[j-1]!=0))
			{
			  if (lowestid>m_par->mbase.lines[i]->line[j-1])
			    {
			      lowestid = m_par->mbase.lines[i]->line[j-1];
			      li = i;
			      lj = j-1;
			    }
			}
		    }

		  if (lowestid == ECV_MAX)
		    {
		      m_par->mbase.lines[i]->line[j]=m_par->mbase.topid;
		      n->id = m_par->mbase.topid;
		      if (m_par->mbase.roottree == NULL)
			{
			  m_par->mbase.roottree = ecv_tree_createtree();
			  m_par->mbase.roottree->root = n;
			  m_par->mbase.roottree->id = n->id;
			  m_par->mbase.roottree->size = 0;
			}
		      else
			{
			  ttree = ecv_tree_createtree();
			  ttree->size = 0;
			  ttree->root = n;
			  ttree->id = n->id;
			  ecv_tree_addtree(m_par->mbase.roottree, ttree);
			}
		      m_par->mbase.topid++;
		      clow++;
		    }
		  else
		    {
		      m_par->mbase.lines[i]->line[j] = lowestid;
		      clow = (unsigned char)lowestid;
		      tmp = m_par->mbase.lines[li]->nodeline[lj];
		      if (tmp == NULL)
			{
			  //printf ("got null ptr\n");
			}
		      else
			{
			  if (ecv_list_findbyij(m_par->mbase.lines[li]->nodeline[lj], i, j)==NULL)
			    {
			      ecv_list_merge(m_par->mbase.lines[li]->nodeline[lj], m_par->mbase.lines[i]->nodeline[j]);
			    }
			}
		    }
		  k=i;
		  while ((k-1>=0))
		    {
		      k=k-1;
		      if (m_par->mbase.discarded[k]!=ECV_DISCARDED)
			break;
		    }
		  if (k!=i)
		    { 
		      if (m_par->mbase.lines[k]->line[j]!=0)
			{
			  if (ecv_list_findbyij(n, k, j)==NULL)
			    {
			      ecv_list_merge(n, m_par->mbase.lines[k]->nodeline[j]);
			    }
			}
		      if (j-1>=0)
			{
			  if ((m_par->mbase.lines[k]->line[j-1]!=0))
			    { 
			      if (ecv_list_findbyij(n, k, j-1)==NULL)
				{
				  ecv_list_merge(n, m_par->mbase.lines[k]->nodeline[j-1]);
				}
			    }
			}
		      if (j+1<m_cla->width)
			{
			  if ((m_par->mbase.lines[k]->line[j+1]!=0))
			    {
			      if (ecv_list_findbyij(n, k, j+1)==NULL)
				{
				  ecv_list_merge(n, m_par->mbase.lines[k]->nodeline[j+1]);
				}
			    }
			}
		    }
		  if (j-1>=0)
		    {
		      if ((m_par->mbase.lines[i]->line[j-1]!=0))
			{
			  if (ecv_list_findbyij(n, i, j-1)==NULL)
			    {
			      ecv_list_merge(n, m_par->mbase.lines[i]->nodeline[j-1]);
			    }
			}
		    }
		  if (j+1<m_cla->width)
		    {
		      if ((m_par->mbase.lines[i]->line[j+1]!=0))
			{
			  if (ecv_list_findbyij(n, i, j+1)==NULL)
			    {
			      ecv_list_merge(n, m_par->mbase.lines[i]->nodeline[j+1]);
			    }
			}		      
		    }
		  k=i;
		  while ((k+1<m_cla->numlines))
		    {
		      k=k+1;
		      if (m_par->mbase.discarded[k]!=ECV_DISCARDED)
			break;
		    }
		  if (k!=i)
		    {
		      if ((m_par->mbase.lines[k]->line[j]!=0))
			{
			  if (ecv_list_findbyij(n, k, j)==NULL)
			    {
			      ecv_list_merge(n, m_par->mbase.lines[k]->nodeline[j]);			  
			    }
			}
		      if (j-1>=0)
			{
			  if ( (m_par->mbase.lines[k]->line[j-1]!=0))
			    { 
			      if (ecv_list_findbyij(n, k, j-1)==NULL)
				{
				  ecv_list_merge(n, m_par->mbase.lines[k]->nodeline[j-1]);
				}
			    }
			}
		      if (j+1<m_cla->width)
			{
			  if ( (m_par->mbase.lines[k]->line[j+1]!=0))
			    { 
			      if (ecv_list_findbyij(n, k, j+1)==NULL)
				{
				  ecv_list_merge(n, m_par->mbase.lines[k]->nodeline[j+1]);
				}
			    }
			}
		    }
		}
	    }
	  offset+=m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
	}
    }
  ecv_node * stptr;
  ttree = m_par->mbase.roottree;
  unsigned int ts=0;
  unsigned int biggest=0;
  unsigned int biggestid=0;
  while (ttree!=NULL)
    {
      lowestid = ECV_MAX;
      //if (ttree->id==ttree->root->id)
      //{
	  tmp = ttree->root;
	  ts = 0;
	  while (tmp!=NULL)
	    {
	      if (tmp->id<lowestid)
		lowestid = tmp->id;
	      tmp = tmp->child;
	      ts++;
	    }
	  //printf ("id = %i ts = %i\n",lowestid, ts);
	  ttree->size = ts;
	  ttree->id = lowestid;
	  tmp = ttree->root;
	  while (tmp!=NULL)
	    {
	      tmp->id = lowestid;
	      tmp = tmp->child;
	    }

	  if (ttree->size>biggest)
	    {
	      biggest = ttree->size;
	      biggestid = ttree->id;
	    }
	  
	  //}
      ttree=ttree->child;
    }
  //  //printf ("biggestid = %i biggest = %i\n",biggestid, biggest);
  ttree = m_par->mbase.roottree;

  while (ttree!=NULL)
    {
      //if (ttree->id==ttree->root->id)
      //	{
	  //	  //printf ("ttree->id = %i ttree->size = %i\n",ttree->id, ttree->size);
	  if (ttree->id == biggestid)
	    {
	      //	      //printf ("biggest will be GREEN\n");
	      tmp = ttree->root;
	      while (tmp!=NULL)
		{
		  offset = m_cla->height/(m_cla->numlines+1)*3*m_cla->width;
		  for (lowestid = 0; lowestid<tmp->i; lowestid++)
		    offset += m_cla->height/(m_cla->numlines+1)*3*m_cla->width;

		  m_par->mbase.lines[tmp->i]->resoffset+=(double)tmp->j;
		  //printf ("tmp->i = %i; tmp->j = %i\n",tmp->i, tmp->j);
		  m_par->mbase.lines[tmp->i]->numpoints++;

		  tmp = tmp->child;
		}
	      //}
	}
      ttree=ttree->child;
    }
  double linectr=0.0;
  double lineoffset=0.0;
  for (i=0; i<m_cla->numlines; i++)
    {
      //printf ("i = %i\n",i);
      if (m_par->mbase.discarded[i]!=ECV_DISCARDED)
	{
	  //printf ("m_par->mbase.lines[tmp->i]->resoffset %i\n", m_par->mbase.lines[i]->resoffset);
	  linectr=linectr+1;
	  m_par->mbase.lines[i]->resoffset=m_par->mbase.lines[i]->resoffset/((double)(m_par->mbase.lines[i]->numpoints));
	  lineoffset+=m_par->mbase.lines[i]->resoffset;
	}
    }
  if (linectr>0.0)
    m_par->offset = (int)(lineoffset/linectr);
  else 
    m_par->offset = (int)0;
  if (m_par->offset > m_cla->width || m_par->offset < 0)
    m_par->offset = 0;
  //printf ("offset = %i\n",m_par->ifacein.offset);
}
