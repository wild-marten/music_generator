#include<stdlib.h>

int main()
{
    int status = system("./build/analizer");
    if(status == 0)
        status = system("./build/matrix_generator");

    return 0;
}