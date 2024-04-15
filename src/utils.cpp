#include <cstdio>
#include <cstdlib>

void perror_exit(const char * msg)
{
    perror(msg);
    exit(1);
}