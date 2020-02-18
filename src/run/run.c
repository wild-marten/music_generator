#include <stdlib.h>
#include<stdio.h>

int main()
{
    int status = system("./build/sequence_generator");
    if (status == 0)
        status = system("python 'includes/abcplay.py' 'build/txt_src/song.abc' 1>/dev/null");
    if (status == 0)
        status = remove("build/txt_src/song.abc");

    return 0;
}