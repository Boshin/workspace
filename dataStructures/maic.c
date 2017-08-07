#include <stdio.h>
#include <unistd.h>

int a[] = { 6, 3, 4, 2, 1, 5, 8, 7, 9 };
int size = sizeof(a) / sizeof(a[0]);
int maxIndex = 0;

void printArr()
{
    for(int i = 0; i < size; ++i) {
        printf("%d, ", a[i]);
    }

    printf("\n");
    sleep(1);
}

void quickSort(int li, int ri)
{
    int basicV = a[li];
    int basicLi = li;
    int basicRi = ri;

    printf("baiscrLi: %d, basicRi: %d, basicV: %d\n", basicLi, basicRi, basicV);
    while (1) {
        if (a[ri] > basicV) {
            --ri;
            continue;
        }

        if (li == ri) {
            a[basicLi] = a[li];
            a[li] = basicV;

            printf("heihh\n");
            printArr();
            if (basicLi < (li - 1))
                quickSort(basicLi, li - 1);
            if ((basicRi - ri) > 1)
                quickSort(ri + 1, basicRi);
            return;
        }
        else {
            if (a[li] > basicV) {
                int tmp = a[li];
                a[li] = a[ri];
                a[ri] = tmp;
      //          printf("li: %d, ri: %d", li, ri);
      //          printArr();
            }
            else {
                ++li;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    maxIndex = size - 1;
    printArr();
    quickSort(0, maxIndex);
    printArr();

    return 0;
}
