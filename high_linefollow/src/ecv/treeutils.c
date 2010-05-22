#include "treeutils.h"

ecv_tree * ecv_tree_createtree()
{
  ecv_tree * t = malloc(sizeof(ecv_tree));
  t->parent = NULL;
  t->child = NULL;
  t->root = NULL;
  return t;
}

void ecv_tree_free(ecv_tree * m_tree)
{
  free(m_tree);
}

void ecv_tree_freeall(ecv_tree * m_tree)
{
  ecv_tree * next;
  ecv_tree * t = ecv_tree_findhead(m_tree);
  while (t != NULL)
    {
      next = t;
      t = t->child;
      free(next);
    }
}

ecv_tree * ecv_tree_findhead(ecv_tree * m_tree)
{
  ecv_tree * t = m_tree;
  while (t->parent != NULL)
    {
      t = t->parent;
    }
  return t;
}

ecv_tree * ecv_tree_findbottom(ecv_tree * m_tree)
{
  ecv_tree * t = m_tree;
  while (t->child != NULL)
    {
      t = t->child;
    }
  return t;
}

void ecv_tree_addtree(ecv_tree * m_where, ecv_tree * m_what)
{
  ecv_tree * t = ecv_tree_findbottom(m_where);
  t->child = m_what;
}
