#include "structs.h"

void ecv_tree_addroot(ecv_tree * m_tree);

ecv_tree * ecv_tree_createtree();

void ecv_tree_free(ecv_tree * m_tree);

void ecv_tree_freeall(ecv_tree * m_tree);

ecv_tree * ecv_tree_findhead(ecv_tree * m_tree);

ecv_tree * ecv_tree_findbottom(ecv_tree * m_tree);

void ecv_tree_addtree(ecv_tree * m_where, ecv_tree * m_what);
