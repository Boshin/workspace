#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *buf = new char;

    delete((void*)buf);
}
