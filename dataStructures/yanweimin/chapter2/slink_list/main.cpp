#include <string.h>

#include "slink_list.h"

void testDifference()
{
    int head = -1;
    sLinkList sl;

    memset(sl, 0, MAXSIZE);

    difference(sl, head);

    printList(sl);   
}

int main(int argc, char *argv[])
{
    testDifference();

    return 0;
}
