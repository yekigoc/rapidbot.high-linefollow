#include "hashutils.h"

void ecv_initlines(ecv_cla * m_cla, ecv_base * m_ecv)
{
  m_ecv->topid = 1;
  m_ecv->lines = malloc(sizeof(ecv_line)*m_cla->numlines);
  int i = 0;
  if (m_cla->method == ECV_COMPLABELING || ECV_COMPLABELING_SIMP)
    {
      m_ecv->discarded = malloc(sizeof(int)*m_cla->numlines);
    }
  for (;i<m_cla->numlines;i++)
    {
      m_ecv->lines[i] = malloc(sizeof(ecv_line));
      if (m_cla->method == ECV_EDGEDETECT)
	{
	  m_ecv->lines[i]->points = g_hash_table_new(g_int_hash, g_int_equal); 
	  m_ecv->lines[i]->aggregations = g_hash_table_new(g_int_hash, g_int_equal); 
	  m_ecv->lines[i]->respoint = malloc(sizeof(ecv_point));
	}
      else if (m_cla->method == ECV_COMPLABELING || ECV_COMPLABELING_SIMP)
	{
	  m_ecv->lines[i]->line = malloc(sizeof(unsigned int)*m_cla->width);
	  m_ecv->lines[i]->nodeline = malloc(sizeof(ecv_node*)*m_cla->width);
	  memset(m_ecv->lines[i]->nodeline, 0, sizeof(ecv_node*)*m_cla->width);
	  m_ecv->lines[i]->aggr = NULL;
	}
    }
}

void ecv_freepoints(ecv_line * m_line)
{
  GHashTableIter it;
  ecv_point * stptr;
  unsigned int * key;
  g_hash_table_iter_init(&it, m_line->points);
  while (g_hash_table_iter_next(&it, (void**)&key, (void**)&stptr))
    {
      free(stptr);
    }
  g_hash_table_steal_all (m_line->points);
  g_hash_table_destroy (m_line->points);
}

void ecv_preparepoints(ecv_line * m_line)
{
  GHashTableIter it;
  ecv_point * stptr;
  unsigned int * key;
  g_hash_table_iter_init(&it, m_line->points);
  while (g_hash_table_iter_next(&it, (void**)&key, (void**)&stptr))
    {
      free(stptr);
    }
  m_line->topid=0;
  g_hash_table_steal_all (m_line->points);
}

void ecv_freeaggregations(ecv_line * m_line)
{
  GHashTableIter it;
  ecv_point * stptr;
  unsigned int * key;
  g_hash_table_iter_init(&it, m_line->aggregations);
  while (g_hash_table_iter_next(&it, (void**)&key, (void**)&stptr))
    {
      free(stptr);
    }
  
  g_hash_table_steal_all (m_line->aggregations);
  g_hash_table_destroy (m_line->aggregations);
}

void ecv_prepareaggregations(ecv_line * m_line)
{
  GHashTableIter it;
  ecv_point * stptr;
  unsigned int * key;
  g_hash_table_iter_init(&it, m_line->aggregations);
  while (g_hash_table_iter_next(&it, (void**)&key, (void**)&stptr))
    {
      free(stptr);
    }
  m_line->agtopid=0;
  g_hash_table_steal_all (m_line->aggregations);
}

void ecv_line_addaggregation(ecv_base * m_ecv, int m_line, int m_x)
{  
  ecv_point * p = malloc(sizeof(ecv_point));
  p->x = m_x;
  g_hash_table_insert(m_ecv->lines[m_line]->aggregations, &m_ecv->lines[m_line]->agtopid, p);
  m_ecv->lines[m_line]->agtopid++;
}

void ecv_preparelines(ecv_base * m_ecv, ecv_cla * m_cla)
{
  int i = 0;
  for (;i<m_cla->numlines;i++)
    {
      if (m_cla->method == ECV_EDGEDETECT)
	{
	  ecv_preparepoints(m_ecv->lines[i]);
	  ecv_prepareaggregations(m_ecv->lines[i]);
	}
      else if (m_cla->method == ECV_COMPLABELING  || ECV_COMPLABELING_SIMP)
	{
	  m_ecv->lines[i]->aggr = NULL;
	  m_ecv->discarded[i]=0;
	}
    }
}

void ecv_finalizelines(ecv_base * m_ecv, ecv_cla * m_cla)
{
  int i = 0;
  for (;i<m_cla->numlines;i++)
    {
      free(m_ecv->lines[i]->respoint);
      ecv_freepoints(m_ecv->lines[i]);
      ecv_freeaggregations(m_ecv->lines[i]);
      free(m_ecv->lines[i]);
    }
  free(m_ecv->lines);
}

void ecv_line_addpoint(ecv_base * m_ecv, int m_line, int m_x)
{  
  ecv_point * p = malloc(sizeof(ecv_point));
  p->x = m_x;
  g_hash_table_insert(m_ecv->lines[m_line]->points, &m_ecv->lines[m_line]->topid, p);
  m_ecv->lines[m_line]->topid++;
}
