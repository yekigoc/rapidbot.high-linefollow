#include "structs.h"

ecv_node * ecv_list_createnodefull(int m_i, int m_j, int m_id);

ecv_node * ecv_list_createnode(int m_i, int m_j);

void ecv_list_deletelist(ecv_node * m_node);

void ecv_list_add(ecv_node * m_node, ecv_node * m_newnode);

ecv_node * ecv_list_findhead(ecv_node * m_node);

ecv_node * ecv_list_findbottom(ecv_node * m_node);

void ecv_list_merge(ecv_node * m_parent, ecv_node * m_child);

ecv_node * ecv_list_findbyij(ecv_node * m_node, int m_i, int m_j);

//--
ecv_aggr * ecv_aggr_createaggrfull(int m_spx, int m_spy, int m_id);

void ecv_aggr_deletelist(ecv_aggr * m_aggr);

ecv_aggr * ecv_aggr_findhead(ecv_aggr * m_aggr);

ecv_aggr * ecv_aggr_findbottom(ecv_aggr * m_aggr);

void ecv_aggr_merge(ecv_aggr * m_parent, ecv_aggr * m_child);

void ecv_aggr_add(ecv_aggr * m_aggr, ecv_aggr * m_newaggr);
