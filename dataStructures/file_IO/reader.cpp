#include <stdio.h>
#include <string.h>

#define FILE_NAME "/tmp/tstFile"

int main(int argc, char *argv[])
{
   char readbuf[16] = {0x00};

   FILE *fp = fopen(FILE_NAME, "r");

   if(fp)
   {
       //fseek(file, 0, SEEK_SET);
       int ret = fread(readbuf, 9, 1, fp);
       printf("read %d byte\n", ret);

       #if 1
       for(int i = 0; i < 16; ++i)
       {
           printf("%02X ", readbuf[i]);
       }
       printf("\n");
       #else
       printf("%s\n", readbuf);
       #endif

       fclose(fp);
   }
   else
   {
       printf("open %s failed\n", FILE_NAME);
   }


   return 0;
}

