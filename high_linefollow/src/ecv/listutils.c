#include "listutils.h"

ecv_node * ecv_list_createnodefull(int m_i, int m_j, int m_id)
{
  ecv_node * n = malloc(sizeof(ecv_node));
  n->i = m_i;
  n->j = m_j;
  n->id = m_id;
  n->parent = NULL;
  n->child = NULL;
  n->processed=0;
  return n;
}

ecv_node * ecv_list_createnode(int m_i, int m_j)
{
  ecv_node * n = malloc(sizeof(ecv_node));
  n->i = m_i;
  n->j = m_j;
  n->parent = NULL;
  n->child = NULL;
  n->id = ECV_MAX;
  n->processed=0;
  return n;
}

void ecv_list_deletelist(ecv_node * m_node)
{
  ecv_node * t;
  ecv_node * r = ecv_list_findbottom(m_node);
  while (r->parent != NULL)
    {
      t = r->parent;
      free(r);
      r = t;
    }
  free (t);
}

void ecv_list_add(ecv_node * m_node, ecv_node * m_newnode)
{
  m_node->child = m_newnode;
  m_newnode->parent = m_node;
}

void ecv_list_merge(ecv_node * m_parent, ecv_node * m_child)
{
  ecv_node * ch = ecv_list_findhead(m_child);
  ecv_node * p = ecv_list_findbottom(m_parent);
  ecv_list_add(p, ch);
}

ecv_node * ecv_list_findhead(ecv_node * m_node)
{
  ecv_node * temp = m_node;
  while (temp->parent!=NULL)
    temp = temp->parent;
  return temp;
}

ecv_node * ecv_list_findbottom(ecv_node * m_node)
{
  ecv_node * temp = m_node;
  while (temp->child!=NULL)
    temp = temp->child;
  return temp;
}

ecv_node * ecv_list_findbyij(ecv_node * m_node, int m_i, int m_j)
{
  ecv_node * temp = ecv_list_findhead(m_node);
  while (temp!=NULL)
    {
      if (temp->i == m_i && temp->j == m_j)
	{
	  return temp;
	}
      temp = temp->child;
    }
  return NULL;
}

//--
ecv_aggr * ecv_aggr_createaggrfull(int m_spx, int m_spy, int m_id)
{
  ecv_aggr * n = malloc(sizeof(ecv_aggr));
  n->spx = m_spx;
  n->spy = m_spy;
  n->width = 1;
  n->parent = NULL;
  n->child = NULL;
  return n;
}

void ecv_aggr_deletelist(ecv_aggr * m_aggr)
{
  ecv_aggr * t;
  ecv_aggr * r = ecv_aggr_findbottom(m_aggr);
  while (r != NULL)
    {
      t = r->parent;
      free(r);
      r = t;
    }
}

ecv_aggr * ecv_aggr_findhead(ecv_aggr * m_aggr)
{
  ecv_aggr * temp = m_aggr;
  while (temp->parent!=NULL)
    temp = temp->parent;
  return temp;
}

ecv_aggr * ecv_aggr_findbottom(ecv_aggr * m_aggr)
{
  ecv_aggr * temp = m_aggr;
  while (temp->child!=NULL)
    temp = temp->child;
  return temp;
}

void ecv_aggr_merge(ecv_aggr * m_parent, ecv_aggr * m_child)
{
  ecv_aggr * ch = ecv_aggr_findhead(m_child);
  ecv_aggr * p = ecv_aggr_findbottom(m_parent);
  ecv_aggr_add(p, ch);
}

void ecv_aggr_add(ecv_aggr * m_aggr, ecv_aggr * m_newaggr)
{
  m_aggr->child = m_newaggr;
  m_newaggr->parent = m_aggr;
}
