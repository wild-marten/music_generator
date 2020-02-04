#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{   
    //const char *filename=argv[1];
    
    FILE *fp = fopen(argv[1], "r");
    if(fp==NULL)
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

    //int status = system("python abcplay.py aaa.abc");

    return 0;
}