#include "index.h" 
int main(int argc, char **argv)
{   
    int status;
    if(argc==1){
        printf("Please run program with a file");
        return 0;
    }
               

    char *play_abc_cmd="python abcplay.py ";

    status = system("python abcplay.py aaa.abc");
    status = remove(argv[1]);

    return 0;
}