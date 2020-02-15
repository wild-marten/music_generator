#include "generator.h"

void *clean(void *ptr)
{
    if (ptr != NULL)
    {
        free(ptr);
        ptr = NULL;
    }
    return ptr;
}

void clean_chord_tab(char **tab)
{
    for(int i=0; i<NUM_OF_CHORDS; i++)
        if(tab[i]!=0)
            free(tab[i]);

    if(tab!=0)
        free(tab);
}

int *chord_table(FILE *fp)
{

    char **tab = calloc(NUM_OF_CHORDS,1);
    for(int i=0; i<NUM_OF_CHORDS; i++)
        tab[i]=calloc(CHORD_LENGHT,1);
    
    char *str = calloc(100, 1);
    int j=0, flag=0;

    while(fscanf(fp, "%s", str)==1)
    {   
        flag = 0;
        if(strcmp(str,"$")!=0)
        {
            for(int i=0; i<j; i++)
                if(strcmp(str, tab[i])==0)
                {
                    flag=1;
                    break;
                }
            
            if(flag==0)
            {
                strcat(tab[j], str);
                j++;
            }
        }

        str=clean(str);
        str = calloc(100, 1);
    }
    clean(str);


}

int main()
{ 
    FILE *fp=fopen("res.txt", "r");

    fclose(fp);
    return  0;
}