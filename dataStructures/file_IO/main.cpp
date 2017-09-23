#include <stdio.h>
#include <string.h>

#define FILE_NAME "/tmp/tstFile"

int main(int argc, char *argv[])
{
   char buf[] = {0x09, 0x08, 0x07, 0x06, 0x00, 0x04, 0x03, 0x02, 0x01};
   //char buf[] = "123456789\n";
   //char readbuf[16] = {0x00};

   FILE *file = fopen(FILE_NAME, "wb+");

   if(file)
   {
       int ret = fwrite(buf, sizeof(buf), 1, file);
       printf("write %d byte\n", ret);

       //fseek(file, 0, SEEK_SET);

       //ret = fread(readbuf, 16, 1, file);
       //printf("read %d byte\n", ret);


       #if 1
       //for(int i = 0; i < 16; ++i)
       //{
       //    printf("%02X ", readbuf[i]);
       //}
       //printf("\n");
       #else
       printf("%s\n", readbuf);
       #endif

       fclose(file);
   }
   else
   {
       printf("open %s failed\n", FILE_NAME);
   }


   return 0;
}
