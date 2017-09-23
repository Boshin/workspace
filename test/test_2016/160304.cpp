#include <map>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define HD_STR  "-HD"

#define NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH 22

typedef unsigned char BYTE;

BYTE m_settingSavingData[23];

typedef struct _cmdData
{
    int  dataSize;
    BYTE data[515];

}uComData;

void test1()
{
    typedef map<int, int> mapType;
    mapType tmp;

    if (tmp.empty())
    {
        printf("map is empty");
    }

    mapType::iterator itBegin = tmp.begin();
    mapType::iterator itEnd = tmp.end();

    if (itBegin == itEnd)
    {
        printf("\n begin == end\n");
    }
    else
    {
        printf("begin != end\n");
    }
}


void test2()
{
    string str1 = "-PIXX";
    // string str1 = "PIXX-B-HD1";
    printf("index: %u\n", str1.find_last_of('-'));

    string subStr = str1.substr(0, str1.find_last_of('-'));
    // printf("sbustr: %s\n", subStr.c_str());
    subStr += HD_STR;

    char num[4] = {0};
    // sprintf(num, "%d", 2);
    snprintf(num, 4, "%d", 2);
    subStr += num;
    printf("sbustr: %s\n", subStr.c_str());
}

void test3(uComData icmddata)
{
    if((NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH + 2) == icmddata.dataSize) {
        BYTE startPos = icmddata.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH];
        BYTE len      = icmddata.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH + 1];
        BYTE isFirst = m_settingSavingData[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] == 0xFF;
        printf("commandDataPack send ACC setting saving notify startPos: %d, len: %d\n", startPos, len);

        printf("commandDataPack send ACC setting saving notify isFirst: %d\n", isFirst);

        BYTE m_initACCSettingData[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] = { 0 };

        // use SYS notice setting data when first time set
        BYTE *pData = isFirst ? m_initACCSettingData : m_settingSavingData;

        // mediamanager used start postion
        if (0 == startPos) {
            memcpy(icmddata.data + len, pData + len, NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH - len);
        }

        // sxm used start postion
        if (10 == startPos) {
            memcpy(icmddata.data, pData, 6);
        }

        m_settingSavingData[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] = 0;
        icmddata.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] = 0;
        icmddata.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH + 1] = 0;
        icmddata.dataSize = NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH;
        memcpy(m_settingSavingData, icmddata.data, NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH);

        for (int i = 0; i < 22; ++i) {
            printf("%d", m_settingSavingData[i]);
        }
        printf("\nover\n\n");
    }
    else {
        printf("commandDataPack send ACC setting saving notify icmddata.dataSize: %d error\n", icmddata.dataSize);
        return;
    }
}



int main(int argc, char *argv[])
{
    // test2();
    uComData tmpData1;
    memset(tmpData1.data, 0, sizeof(tmpData1.data));
    tmpData1.dataSize = 24;
    tmpData1.data[0] = 2;
    tmpData1.data[1] = 2;
    tmpData1.data[2] = 2;
    tmpData1.data[3] = 2;
    tmpData1.data[4] = 2;
    tmpData1.data[5] = 2;
    tmpData1.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] = 0;
    tmpData1.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH + 1] = 6;

    uComData tmpData2;
    memset(tmpData2.data, 0, sizeof(tmpData2.data));
    tmpData2.dataSize = 24;
    tmpData2.data[10] = 3;
    tmpData2.data[11] = 3;
    tmpData2.data[12] = 3;
    tmpData2.data[13] = 3;
    tmpData2.data[14] = 3;
    tmpData2.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] = 10;
    tmpData2.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH + 1] = 5;

    uComData tmpData4;
    memset(tmpData4.data, 0, sizeof(tmpData1.data));
    tmpData4.dataSize = 24;
    tmpData4.data[0] = 4;
    tmpData4.data[1] = 4;
    tmpData4.data[2] = 4;
    tmpData4.data[3] = 4;
    tmpData4.data[4] = 4;
    tmpData4.data[5] = 4;
    tmpData4.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] = 0;
    tmpData4.data[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH + 1] = 6;

    memset(m_settingSavingData, 0, sizeof(m_settingSavingData));
    m_settingSavingData[NHMEDIA_INIT_ACC_SETTING_DATA_LENGTH] = 0xFF;

    test3(tmpData1);
    sleep(1);
    test3(tmpData2);
    sleep(1);
    test3(tmpData4);
}
