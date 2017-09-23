#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

void drawper(int iFirst, int iSecnd, char* spOut)
{
    static int iCrlmm = 0;
    int iPer = iFirst*100/iSecnd;

    if(iFirst==iSecnd)
        sprintf(spOut, "[ %d%]\n",  iPer);
    else if( iCrlmm == 0)
        sprintf(spOut, "[| %d%]\r",  iPer);
    else if( iCrlmm == 1)
        sprintf(spOut, "[/ %d%]\r",  iPer);
    else if( iCrlmm == 2)
        sprintf(spOut, "[- %d%]\r",  iPer);
    else if( iCrlmm == 3)
        sprintf(spOut, "[\\ %d%]\r", iPer);

    if(iCrlmm==3)
        iCrlmm=0;
    else
        iCrlmm++;
}
void proessview()
{
    char szStr[255] = "=====================================================================";
    int  iStrLen = strlen(szStr);
    char cTmp       = '>';
    char szOut[10]  = "";
    int i = 0;
    for( i= 0; i<=iStrLen; i++)
    {
        memset( szOut, 0, sizeof(szOut) );

        szStr[i] = cTmp;
        drawper(i, iStrLen, szOut);
        memcpy(szStr+iStrLen, szOut, strlen(szOut) );
        fseek(stdout, 0, SEEK_SET);
        fprintf(stdout, szStr);
        fflush(stdout);

        usleep(300000);
    }
}

void test()
{
    printf("test\n");

    for (int i = 0; i < 100; ++i) 
    {
        printf("\r--%d", i);
        sleep(1);
        fflush(stdout);
    }
}

void test2()
{
    printf("begin\n");
    int nProgress=0;
    bool bIns = true;
    while(1)
    {
        if (bIns)
        {
            printf("#");
            nProgress++;
        }
        if (!bIns)
        {
            printf("\b \b");
            nProgress--;
        }
        if (nProgress >= 10)
        {
            bIns = false;
        }
        else if (nProgress <=0 )
        {
            bIns = true;
        }
        sleep(1);
        fflush(stdout);
    }
}

void update_process(int percent,int barlen){
    int i;
    putchar('[');
    for(i=1;i<=barlen;++i)
        putchar(i*100<=percent*barlen?'>':' ');
        putchar(']');
        printf("%3d%%",percent);
        for(i=0;i!=barlen+6;++i)
            putchar('\b');
}

void test3(){
    int i;
    for(i=0;i<101;++i){
        update_process(i,50);
        fflush(stdout);
        sleep(1);
    }
}

int main()
{
    //proessview();
    test();
    //test2();
    //test3();
    return 1;
}
