#include "utilities.h"

void *clean(void *ptr)
{
    if (ptr != NULL)
    {
        free(ptr);
        ptr = NULL;
    }
    return ptr;
}

void **clean_matrix(void **tab, int rows)
{
    for (int i = 0; i < rows; i++)
        if (tab[i] != 0)
            free(tab[i]);

    tab = clean(tab);
    return tab;
}