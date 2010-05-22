#include "structs.h"

void ecv_initlines(ecv_cla * m_cla, ecv_base * m_ecv);

void ecv_freepoints(ecv_line * m_line);

void ecv_preparepoints(ecv_line * m_line);

void ecv_freeaggregations(ecv_line * m_line);

void ecv_prepareaggregations(ecv_line * m_line);

void ecv_line_addaggregation(ecv_base * m_ecv, int m_line, int m_x);

void ecv_preparelines(ecv_base * m_ecv, ecv_cla * m_cla);

void ecv_finalizelines(ecv_base * m_ecv, ecv_cla * m_cla);

void ecv_line_addpoint(ecv_base * m_ecv, int m_line, int m_x);
