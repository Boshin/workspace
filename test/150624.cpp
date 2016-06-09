#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define MM "12345"

#define NMTUNER_SXM_CHANNELNAME_SIZE_MAX 12;

typedef unsigned char BYTE;
typedef int           INT32;

using namespace std;

#if 0
struct NMTunerMixPresetInfo
{
    BYTE                    presetChannelNo;      // Preset channel Number
    BYTE                    band;                 // Band type
    bool                    isBlank;              // Blank preset flag
    union
    {
        struct {
            INT32                frequency;       // AM/FM frequency value info
            BYTE                 multiCastType;   // Multicast type
            bool                 isHDRadio;       // HD radio
        }tuner;
        struct {
            INT32                sid;             // XM sid
            INT32                channelNo;       // SXM channel number
            bool                 isSmartFavorite; // Smart favorite flag
            BYTE                 channelName[]; // SXM channel name
        }sxm;
    };

    NMTunerMixPresetInfo()
    {
        presetChannelNo = 0;
        band            = 0xFF;
        isBlank         = true;
        tuner.frequency       = -1;
        tuner.multiCastType   = 0;
        tuner.isHDRadio       = false;
        sxm.sid             = -1;
        sxm.channelNo       = -1;
        sxm.isSmartFavorite = false;
        memset(sxm.channelName, 0, sizeof(sxm.channelName));
    }
};

void getItem(NMTunerMixPresetInfo& info)
{
    static NMTunerMixPresetInfo mixInfo;
    mixInfo.presetChannelNo     = 10;
    mixInfo.band                = 0x81;
    mixInfo.isBlank             = false;
    mixInfo.tuner.frequency     = 8790;
    mixInfo.tuner.multiCastType = 11;
    mixInfo.tuner.isHDRadio     = true;

    info = mixInfo;
}

void getVec(vector<NMTunerMixPresetInfo>& infoVec)
{
    static NMTunerMixPresetInfo mixInfo1;
    mixInfo1.presetChannelNo     = 10;
    mixInfo1.band                = 0x81;
    mixInfo1.isBlank             = false;
    mixInfo1.tuner.frequency     = 8790;
    mixInfo1.tuner.multiCastType = 11;
    mixInfo1.tuner.isHDRadio     = true;

    static NMTunerMixPresetInfo mixInfo2;

    mixInfo2.presetChannelNo     = 11;
    mixInfo2.band                = 0x93;
    mixInfo2.isBlank             = false;
    mixInfo2.sxm.sid           = 7;
    mixInfo2.sxm.channelNo     = 71;
    mixInfo2.sxm.isSmartFavorite     = true;

    infoVec.push_back(mixInfo1);
    infoVec.push_back(mixInfo2);


}

int main(int argc, char *argv[])
{
    // char str[] = {"12345"};
    // printf("sizeof(MM): %d\n", sizeof(MM));

    // if(0 == strncmp(MM, str, sizeof(MM)))
    // {
    //     printf("equal\n");
    // }

    // strncpy(NULL, MM, sizeof(MM));
    NMTunerMixPresetInfo Info;
    getItem(Info);

    printf("presetChannelNo: %d, band: %02X, isBlank: %d, frequency: %d, multiCastType: %d, isHDRadio: %d\n",
        Info.presetChannelNo, Info.band, Info.isBlank, Info.tuner.frequency,
        Info.tuner.multiCastType, Info.tuner.isHDRadio );

    vector<NMTunerMixPresetInfo> infoVec;
    getVec(infoVec);

    for(BYTE i = 0; i < infoVec.size(); ++i)
    {
        if (0x81 == infoVec[i].band)
        {
            printf("presetChannelNo: %d, band: %02X, isBlank: %d, frequency: %d, multiCastType: %d, isHDRadio: %d\n",
                infoVec[i].presetChannelNo, infoVec[i].band, infoVec[i].isBlank, infoVec[i].tuner.frequency,
                infoVec[i].tuner.multiCastType, infoVec[i].tuner.isHDRadio );
        }
        else if(0x93 == infoVec[i].band)
        {
            printf("presetChannelNo: %d, band: %02X, isBlank: %d, sid: %d, channelNo: %d, isSmartFavorite: %d\n",
                infoVec[i].presetChannelNo, infoVec[i].band, infoVec[i].isBlank, infoVec[i].sxm.sid,
                infoVec[i].sxm.channelNo, infoVec[i].sxm.isSmartFavorite);
        }
    }
}
#else
struct NMTunerMixPresetInfo
{
    BYTE                    presetChannelNo;      // Preset channel Number
    BYTE                    band;                 // Band type
    bool                    isBlank;              // Blank preset flag
    union
    {
        struct {
            INT32                frequency;       // AM/FM frequency value info
            BYTE                 multiCastType;   // Multicast type
            bool                 isHDRadio;       // HD radio
        };
        struct {
            INT32                sid;             // XM sid
            INT32                channelNo;       // SXM channel number
            bool                 isSmartFavorite; // Smart favorite flag
            BYTE                 channelName[]; // SXM channel name
        };
    };

    NMTunerMixPresetInfo()
    {
        presetChannelNo = 0;
        band            = 0xFF;
        isBlank         = true;
        frequency       = -1;
        multiCastType   = 0;
        isHDRadio       = false;
        sid             = -1;
        channelNo       = -1;
        isSmartFavorite = false;
        memset(channelName, 0, sizeof(channelName));
    }
};

void getItem(NMTunerMixPresetInfo& info)
{
    static NMTunerMixPresetInfo mixInfo;
    mixInfo.presetChannelNo     = 10;
    mixInfo.band                = 0x81;
    mixInfo.isBlank             = false;
    mixInfo.frequency     = 8790;
    mixInfo.multiCastType = 11;
    mixInfo.isHDRadio     = true;

    info = mixInfo;
}

void getVec(vector<NMTunerMixPresetInfo>& infoVec)
{
    static NMTunerMixPresetInfo mixInfo1;
    mixInfo1.presetChannelNo     = 10;
    mixInfo1.band                = 0x81;
    mixInfo1.isBlank             = false;
    mixInfo1.frequency     = 8790;
    mixInfo1.multiCastType = 11;
    mixInfo1.isHDRadio     = true;

    static NMTunerMixPresetInfo mixInfo2;

    mixInfo2.presetChannelNo     = 11;
    mixInfo2.band                = 0x93;
    mixInfo2.isBlank             = false;
    mixInfo2.sid                 = 7;
    mixInfo2.channelNo           = 71;
    mixInfo2.isSmartFavorite     = true;

    memcpy(mixInfo2.channelName, MM, sizeof(MM));

    infoVec.push_back(mixInfo1);
    infoVec.push_back(mixInfo2);
}

int main(int argc, char *argv[])
{
    // char str[] = {"12345"};
    // printf("sizeof(MM): %d\n", sizeof(MM));

    // if(0 == strncmp(MM, str, sizeof(MM)))
    // {
    //     printf("equal\n");
    // }

    // strncpy(NULL, MM, sizeof(MM));
    NMTunerMixPresetInfo Info;
    getItem(Info);

    printf("presetChannelNo: %d, band: %02X, isBlank: %d, frequency: %d, multiCastType: %d, isHDRadio: %d\n",
        Info.presetChannelNo, Info.band, Info.isBlank, Info.frequency,
        Info.multiCastType, Info.isHDRadio );

    vector<NMTunerMixPresetInfo> infoVec;
    getVec(infoVec);

    for(BYTE i = 0; i < infoVec.size(); ++i)
    {
        if (0x81 == infoVec[i].band)
        {
            printf("presetChannelNo: %d, band: %02X, isBlank: %d, frequency: %d, multiCastType: %d, isHDRadio: %d\n",
                infoVec[i].presetChannelNo, infoVec[i].band, infoVec[i].isBlank, infoVec[i].frequency,
                infoVec[i].multiCastType, infoVec[i].isHDRadio );
        }
        else if(0x93 == infoVec[i].band)
        {
            printf("presetChannelNo: %d, band: %02X, isBlank: %d, sid: %d, channelNo: %d, isSmartFavorite: %d\n",
                infoVec[i].presetChannelNo, infoVec[i].band, infoVec[i].isBlank, infoVec[i].sid,
                infoVec[i].channelNo, infoVec[i].isSmartFavorite);

            printf("infoVec[i].channelName: %s\n", infoVec[i].channelName);
        }
    }
}
#endif