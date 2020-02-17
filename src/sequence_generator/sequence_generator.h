#ifndef SQ
#define SQ

#include"index.h"
#include "../lib/pcg-c-basic-0.9/pcg_basic.h"

void *clean(void *ptr);
void **clean_matrix(void **tab, int rows);
int **initialize_transition_matrix();

#endif