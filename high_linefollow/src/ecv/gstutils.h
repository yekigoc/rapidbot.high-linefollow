#include "structs.h"

GstElement *ecv_create_src_pipeline (ecv_cla * m_cla, ecv_params * m_par);

int ecv_play(GstElement *p);

void ecv_stop(GstElement *p);
