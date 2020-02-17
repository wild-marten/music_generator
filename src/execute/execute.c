#include "index.h" 
int main(int argc, char **argv)
{   
    int status;
    
    if(argc==1){
        printf("Please run program with a file");
        return 0;
    }
    
    char *cmd="python abcplay.py ";
    char *play_abc_cmd=malloc(strlen(argv[1]) + strlen(cmd) + 1);
    strcat(play_abc_cmd, cmd);
    strcat(play_abc_cmd, argv[1]);

    status = system(play_abc_cmd);
    status = remove(argv[1]);

    return 0;
}