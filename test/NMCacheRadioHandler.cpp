/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "NMCacheRadioHandler.h"
#include "NMCacheRadioInfo.h"
#include "NMCacheRadioData.h"
#include "NMCacheRadioEncode.h"
#include "NMCacheRadioDecode.h"
#include "NMCacheRadioLog.h"
#include "NMRunnableLooper.h"
#include "NMRunnableTimer.h"
#include "NMTunerParser.h"
#include "NMTask.h"
#include "contentresolver/NCContentResolver.h"

namespace nutshell
{
namespace media
{

    void NMCacheRadioHandler::initialize(NMRunnableLooper *Looper)
    {

        NMLOGD("NMCacheRadioHandler::initialize() m_pCacheData[%p], m_pCacheInfo[%p], m_pCacheEncode[%p], m_pCacheDecode[%p]",
                m_pCacheData, m_pCacheInfo, m_pCacheEncode, m_pCacheDecode);
        m_pCacheData->initialize(this);
        m_pCacheInfo->initialize(this);
        m_pCacheEncode->initialize(m_pCacheData, this);
        m_pCacheDecode->initialize(m_pCacheData, this, Looper);
        m_workLooper = Looper;

    }

    void NMCacheRadioHandler::start()
    {

        NMLOGD("NMCacheRadioHandler::start()");
        m_reqPlayStatus = NMCacheRadioPlayStatus_Start;
        m_curStatus.playStatus = NMCacheRadioPlayStatus_Play;
        m_isInSeekOrScanStatus = FALSE;
        m_isInAutoPauseStatus = FALSE;
        m_isInStationListUpStatus = FALSE;
        m_isInVRStatus = FALSE;
        m_seekAutoPause = FALSE;
        if (NULL != m_pCacheData) {
            m_pCacheData->clearCacheBuffer();
        }
        else {
            NMLOGE("NMCacheRadioHandler::start() m_pCacheData is NULL");
        }

        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->clearCacheInfo();
        }
        else {
            NMLOGE("NMCacheRadioHandler::start() m_pCacheInfo is NULL");
        }

        if (NULL != m_pCacheEncode) {
            m_pCacheEncode->start();
        }
        else {
            NMLOGE("NMCacheRadioHandler::start() m_pCacheEncode is NULL");
        }

        if (NULL != m_pCacheDecode) {
            m_pCacheDecode->start();
        }
        else {
            NMLOGE("NMCacheRadioHandler::start() m_pCacheDecode is NULL");
        }

        if (NULL != m_pTimer) {
            m_pTimer->start();
        }

        if (NULL != m_pCacheData) {
            m_pCacheData->forceDispArtNumChanged(NMCacheRadio_Order); // for start later than reqcachemode
        }

    }

    void NMCacheRadioHandler::stop()
    {

        NMLOGD("NMCacheRadioHandler::stop()");

        m_reqPlayStatus = NMCacheRadioPlayStatus_Stop;

        m_isInSeekOrScanStatus = FALSE;

        reqLiveMode();

        if (NULL != m_pTimer) {
            m_pTimer->stop();
        }

        if (NULL != m_pCacheEncode) {
            m_pCacheEncode->stop();
        }
        else {
            NMLOGE("NMCacheRadioHandler::stop() m_pCacheEncode is NULL");
        }

        if (NULL != m_pCacheDecode) {
            m_pCacheDecode->stop();
        }
        else {
            NMLOGE("NMCacheRadioHandler::stop() m_pCacheDecode is NULL");
        }

        if (NULL != m_pCacheData) {
            m_pCacheData->clearCacheBuffer();
        }
        else {
            NMLOGE("NMCacheRadioHandler::stop() m_pCacheData is NULL");
        }

        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->clearCacheInfo();
        }
        else {
            NMLOGE("NMCacheRadioHandler::stop() m_pCacheInfo is NULL");
        }

        m_curStatus.playStatus = NMCacheRadioPlayStatus_Play;

        updateCacheRadioStatusForUI(0, 0);

        reInitializeMember();

        NMLOGD("NMCacheRadioHandler::stop()   on live end");

    }

    void NMCacheRadioHandler::reqCacheMode()
    {

        NMLOGD("NMCacheRadioHandler::reqCacheMode()");
        if ((NMCacheRadioPlayStatus_Stop == m_reqPlayStatus) || (NMTunerMode_CacheMode == m_curTunerMode)) {
            return;
        }
        NMCacheRadioReq reqInfo;
        reqInfo.recordMin = (((m_curRecordTime / 60) / 10) << 4) + (((m_curRecordTime / 60) % 10) & 0x0f);
        reqInfo.recordSec = (((m_curRecordTime % 60) / 10) << 4) + (((m_curRecordTime % 60) % 10) & 0x0f);
        reqInfo.playMin = (((m_curPlayTime / 60) / 10) << 4) + (((m_curPlayTime / 60) % 10) & 0x0f);
        reqInfo.playSec = (((m_curPlayTime % 60) / 10) << 4) + (((m_curPlayTime % 60) % 10) & 0x0f);
        if (m_playInfo.nmInfo.stBitData4.byDigitalSound) {
            reqInfo.tunerSoundMod = 0x02;
        }
        else {
            reqInfo.tunerSoundMod = 0x01;
        }
        if (NULL != m_pTunerParser) {
            m_pTunerParser->reqSwitchTunerMode(reqInfo);
        }

    }

    void NMCacheRadioHandler::reqLiveMode()
    {

        NMLOGD("NMCacheRadioHandler::reqLiveMode()");
        if ((m_isInSeekOrScanStatus) && (NMCacheRadioPlayStatus_Stop != m_reqPlayStatus)) {
            return; // becuse source off have to execute reqLiveMode
        }
        m_isInreqLiveStatus = TRUE;
        NMCacheRadioReq reqInfo;
        reqInfo.recordMin = (((m_curRecordTime / 60) / 10) << 4) + (((m_curRecordTime / 60) % 10) & 0x0f);
        reqInfo.recordSec = (((m_curRecordTime % 60) / 10) << 4) + (((m_curRecordTime % 60) % 10) & 0x0f);
        reqInfo.playMin = (((m_curPlayTime / 60) / 10) << 4) + (((m_curPlayTime / 60) % 10) & 0x0f);
        reqInfo.playSec = (((m_curPlayTime % 60) / 10) << 4) + (((m_curPlayTime % 60) % 10) & 0x0f);
        reqInfo.tunerSoundMod = 0x00;
        if (NULL != m_pTunerParser) {
            m_pTunerParser->reqSwitchTunerMode(reqInfo);
        }

    }

    void NMCacheRadioHandler::reqCacheRadioPlay()
    {

        NMLOGD("NMCacheRadioHandler::reqCacheRadioPlay()");
        if ((m_isInSeekOrScanStatus)
            || (TRUE == m_isInAutoPauseStatus)
            || (TRUE == m_isInStationListUpStatus)
            || (NMTunerMode_CacheMode != m_curTunerMode)
            || (NMCacheRadioPlayStatus_Stop == m_reqPlayStatus)
            || (NMCacheRadioPlayStatus_Skip == m_reqPlayStatus)) {
            return;
        }

        if (NULL != m_pCacheDecode) {
            m_reqPlayStatus = NMCacheRadioPlayStatus_Play;
            m_pCacheDecode->play();
            if (m_isInVRStatus) {
                m_autoPausePlayStatus = NMCacheRadioPlayStatus_Play;
            }
        }
        else {
            NMLOGE("NMCacheRadioHandler::reqCacheRadioPlay() m_pCacheDecode is NULL");
        }

    }

    void NMCacheRadioHandler::reqCacheRadioPause()
    {

        NMLOGD("NMCacheRadioHandler::reqCacheRadioPause()");
        if ((m_isInSeekOrScanStatus == TRUE)
            || (m_isInStationListUpStatus == TRUE)
            || (NMCacheRadioPlayStatus_Stop == m_reqPlayStatus)
            || (NMCacheRadioPlayStatus_Skip == m_reqPlayStatus)) {
            return;
        }

        if (NMTunerMode_LiveMode == m_curTunerMode) {
            reqCacheMode();
        }

        if (NULL != m_pCacheDecode) {
            m_reqPlayStatus = NMCacheRadioPlayStatus_Pause;
            muteFrontScreen();
            m_pCacheDecode->pause(NMPauseNotify);
            if (m_isInVRStatus) {
                m_autoPausePlayStatus = NMCacheRadioPlayStatus_Pause;
            }
        }
        else {
            NMLOGE("NMCacheRadioHandler::reqCacheRadioPause() m_pCacheDecode is NULL");
        }

    }

    void NMCacheRadioHandler::reqCacheRadioNativePause()
    {
        NMLOGD("NMCacheRadioHandler::reqCacheRadioNativePause()");
        if (NULL != m_pCacheDecode) {
            muteFrontScreen();
            m_pCacheDecode->pause(NMNoPauseNotify);
        }
    }

    // tuner service
    void NMCacheRadioHandler::reqCacheRadioStationChange()
    {

        NMLOGD("NMCacheRadioHandler::reqCacheRadioStationChange()");

        if (NMTunerMode_CacheMode == m_curTunerMode) {
            reqCacheRadioNativePause();
        }

        if (NULL != m_pCacheEncode) {
            m_pCacheEncode->pause();
        }

        if (NULL != m_pTimer) {
            m_pTimer->stop();
        }
// for emulator gtest
#ifndef GTESTUP
        if (NMCacheRadioPlayStatus_Stop != m_reqPlayStatus) {
            if (NULL != m_pCacheEncode) {
                m_pCacheEncode->flushEncoderBuffer();
            }
            else {
                NMLOGE("NMCacheRadioHandler::reqCacheRadioStationChange() m_pCacheEncode is NULL");
            }

            if (NULL != m_pCacheDecode) {
                m_pCacheDecode->flushDecoderBuffer();
            }
            else {
                NMLOGE("NMCacheRadioHandler::reqCacheRadioStationChange() m_pCacheDecode is NULL");
            }
        }
#endif
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->clearCacheInfo();
        }
        else {
            NMLOGE("NMCacheRadioHandler::reqCacheRadioStationChange() m_pCacheInfo is NULL");
        }

        if (NULL != m_pCacheData) {
            m_pCacheData->clearCacheBuffer();
        }
        else {
            NMLOGE("NMCacheRadioHandler::reqCacheRadioStationChange() m_pCacheData is NULL");
        }

        reInitializeMember();

        if (NULL != m_pCacheEncode) {
            m_pCacheEncode->play();
        }

        if ((NMTunerMode_CacheMode == m_curTunerMode)
            && (NMCacheRadioPlayStatus_Stop != m_reqPlayStatus)) {
            if (NULL != m_pCacheData) {
                m_pCacheData->forceDispArtNumChanged(NMCacheRadio_Reverse);
            }
        }

        if (NMTunerMode_LiveMode == m_curTunerMode) {
            updateStatus(TRUE); // must before timer start

            if (NULL != m_pTimer) {
                m_pTimer->start();
            }
        }
        NMLOGD("NMCacheRadioHandler::reqCacheRadioStationChange() end");

    }

    void NMCacheRadioHandler::reqCacheRadioFF(const NMTunerCommand& cmd)
    {

        NMLOGD("NMCacheRadioHandler::reqCacheRadioFF()");
        if ((TRUE == m_isInSeekOrScanStatus)
            || (NMTunerMode_CacheMode != m_curTunerMode)
            || (NMCacheRadioPlayStatus_Stop == m_reqPlayStatus)
            || (TRUE == m_isInAutoPauseStatus)
            || (TRUE == m_isInVRStatus)
            || (NMCacheRadioPlayStatus_Skip == m_reqPlayStatus)
            || (TRUE == m_isInStationListUpStatus)) {
            return;
        }

        NMLOGD("NMCacheRadioHandler::reqCacheRadioFF(%d)", cmd);
        if (NMTunerCommand_Start == cmd) {
            NMLOGD("NMCacheRadioHandler::reqCacheRadioFF   NMTunerCommand_Start == cmd");
            m_reqPlayStatus = NMCacheRadioPlayStatus_FF;
            reqCacheRadioNativePause();
            cacheRadioPlayStatusChanged(NMCacheRadioPlayStatus_FF);
        }
        else {
            NMLOGD("NMCacheRadioHandler::reqCacheRadioFF   NMTunerCommand_Stop == cmd");
            if (NULL != m_pCacheDecode) {
                m_pCacheDecode->flushDecoderBuffer();
                m_pCacheDecode->play();
                m_reqPlayStatus = NMCacheRadioPlayStatus_Play;
            }
            else {
                NMLOGE("NMCacheRadioHandler::reqCacheRadioFF() m_pCacheDecode is NULL");
            }
        }

    }

    void NMCacheRadioHandler::reqCacheRadioREW(const NMTunerCommand &cmd)
    {

        NMLOGD("NMCacheRadioHandler::reqCacheRadioREW(%d)", cmd);
        if ((TRUE == m_isInSeekOrScanStatus)
            || (NMCacheRadioPlayStatus_Stop == m_reqPlayStatus)
            || (TRUE == m_isInAutoPauseStatus)
            || (TRUE == m_isInVRStatus)
            || (NMCacheRadioPlayStatus_Skip == m_reqPlayStatus)
            || (TRUE == m_isInStationListUpStatus)) {
            return;
        }

        if (NMTunerMode_LiveMode == m_curTunerMode) {
            if (NMTunerCommand_Start == cmd) {
                m_reqPlayStatus = NMCacheRadioPlayStatus_REW;
                reqCacheMode();
            }
            else {
                NMLOGD("NMCacheRadioHandler::reqCacheRadioREW(%d)    in live mode", cmd);
                m_reqPlayStatus = NMCacheRadioPlayStatus_Play;
                m_pCacheDecode->play();
                cacheRadioPlayStatusChanged(NMCacheRadioPlayStatus_Play);
            }
        }
        else {
            if (NMTunerCommand_Start == cmd) {
                m_reqPlayStatus = NMCacheRadioPlayStatus_REW;
                NMLOGD("NMCacheRadioHandler::m_reqPlayStatus = %d", m_reqPlayStatus);
                reqCacheRadioNativePause();
                cacheRadioPlayStatusChanged(NMCacheRadioPlayStatus_REW);
            }
            else {
                if (NULL != m_pCacheDecode) {
                    m_pCacheDecode->flushDecoderBuffer();
                    m_pCacheDecode->play();
                    m_reqPlayStatus = NMCacheRadioPlayStatus_Play;
                }
                else {
                    NMLOGE("NMCacheRadioHandler::reqCacheRadioREW() m_pCacheDecode is NULL");
                }
            }
        }

    }

    void NMCacheRadioHandler::reqCacheRadioSkip(const NMTunerDirection &direction)
    {
// for emulator gtest
#ifndef GTESTUP
        NMLOGD("NMCacheRadioHandler::reqCacheRadioSkip(%d)", direction);
        if ((TRUE == m_isInSeekOrScanStatus)
            || (NMCacheRadioPlayStatus_Stop == m_reqPlayStatus)
            || (TRUE == m_isInAutoPauseStatus)
            || (TRUE == m_isInSkipStatus)
            || (TRUE == m_isInVRStatus)
            || (TRUE == m_isInStationListUpStatus)) {
            return;
        }

        if (NMCacheRadioPlayStatus_Skip != m_reqPlayStatus) {
            if ((NMTunerMode_LiveMode == m_curTunerMode) && (NMTunerDirection_Down == direction)) {
                m_isInSkipStatus = TRUE;
                reqCacheMode();
            }
            else {
                m_skipDirection = direction;
                cacheRadioPlayStatusChanged(NMCacheRadioPlayStatus_Skip);
                m_isInSkipStatus = TRUE;
                reqCacheRadioNativePause();
                m_reqPlayStatus = NMCacheRadioPlayStatus_Skip;
            }
        }
        else {
            NMLOGD("NMCacheRadioHandler::reqCacheRadioSkip()   m_reqPlayStatus conflict");
        }
#endif
    }

    // tuner service
    void NMCacheRadioHandler::onSkip(const NMTunerDirection &direction)
    {

        NMLOGD("NMCacheRadioHandler::onSkip(%d)", direction);
        m_isInSkipStatus = FALSE;
        if (NULL != m_pCacheDecode) {
            m_pCacheDecode->flushDecoderBuffer();
        }

        if (NULL != m_pCacheData) {
            bool isFinished = m_pCacheData->caculatePlayPoint(skip_operation, direction);
            if (!isFinished) {
                m_reqPlayStatus = NMCacheRadioPlayStatus_Pause;
                // fix end
                reqLiveMode();
                return;
            }
        }
        else {
            NMLOGE("NMCacheRadioHandler::onSkip() m_pCacheData is NULL");
        }

    }

    void NMCacheRadioHandler::onCacheRadioGraceNoteAlbumUpdate(const NMGraceNoteCoverArtWithArtistText& albumBuf)
    {

        if (TRUE == m_isInSeekOrScanStatus) {
            return;
        }
        NMLOGD("NMCacheRadioHandler::onCacheRadioGraceNoteAlbumUpdate()");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->onCacheRadioGraceNoteAlbumUpdate(albumBuf);
        }
        else {
            NMLOGE("NMCacheRadioHandler::onCacheRadioGraceNoteAlbumUpdate() m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::onCacheRadioGraceNoteGenreUpdate(const NMGraceNoteCoverArtWithArtistText& genreBuf)
    {

        if (TRUE == m_isInSeekOrScanStatus) {
            return;
        }
        NMLOGD("NMCacheRadioHandler::onCacheRadioGraceNoteGenreUpdate()");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->onCacheRadioGraceNoteGenreUpdate(genreBuf);
        }
        else {
            NMLOGE("NMCacheRadioHandler::onCacheRadioGraceNoteGenreUpdate() m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::onCacheRadioPrimaryImageArtworkDispUpdate(const NMHdRadioArtworkDisp& artWorkDisp)
    {

        if (TRUE == m_isInSeekOrScanStatus) {
            return;
        }
        NMLOGD("NMCacheRadioHandler::onCacheRadioPrimaryImageArtworkDispUpdate()");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->onCacheRadioPrimaryImageArtworkDispUpdate(artWorkDisp);
        }
        else {
            NMLOGE("NMCacheRadioHandler::onCacheRadioPrimaryImageArtworkDispUpdate() m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::onCacheRadioPrimaryImageUpdate(const NMHdRadioImageFormat& primaryImage)
    {

        if (TRUE == m_isInSeekOrScanStatus) {
            return;
        }
        NMLOGD("NMCacheRadioHandler::onCacheRadioPrimaryImageUpdate()");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->onCacheRadioPrimaryImageUpdate(primaryImage);
        }
        else {
            NMLOGE("NMCacheRadioHandler::onCacheRadioPrimaryImageUpdate() m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::onCacheRadioStationLogoUpdate(const std::string& stationLogo)
    {

        if (TRUE == m_isInSeekOrScanStatus) {
            return;
        }
        NMLOGD("NMCacheRadioHandler::onCacheRadioStationLogoUpdate()");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->onCacheRadioStationLogoUpdate(stationLogo);
        }
        else {
            NMLOGE("NMCacheRadioHandler::onCacheRadioStationLogoUpdate() m_pCacheInfo is NULL");
        }

    }

// tuner service
    void NMCacheRadioHandler::reqTunerSeekOrScan()
    {

        NMLOGD("NMCacheRadioHandler::reqTunerSeekOrScan()");

        if (NULL != m_pTimer) {
            m_pTimer->stop();
        }

        if (NMTunerMode_CacheMode == m_curTunerMode) {
            if (NMCacheRadioPlayStatus_Stop != m_reqPlayStatus) {
                reqCacheRadioNativePause();
            }
        }

        if (NULL != m_pCacheEncode) {
            m_pCacheEncode->pause();
        }

        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->clearCacheInfo();
        }
        else {
            NMLOGE("NMCacheRadioHandler::reqTunerSeekOrScan() m_pCacheInfo is NULL");
        }

        if (NULL != m_pCacheData) {
            m_pCacheData->clearCacheBuffer();
        }
        else {
            NMLOGE("NMCacheRadioHandler::reqTunerSeekOrScan() m_pCacheData is NULL");
        }

        reInitializeMember();

        if ((NMTunerMode_CacheMode == m_curTunerMode)
            && (NMCacheRadioPlayStatus_Stop != m_reqPlayStatus)) {
            if (NULL != m_pCacheData) {
                m_pCacheData->forceDispArtNumChanged(NMCacheRadio_Reverse);
            }
        }
        if (NMTunerMode_LiveMode == m_curTunerMode) {
            updateStatus(TRUE);
        }
        NMLOGD("NMCacheRadioHandler::reqTunerSeekOrScan()   end");

    }

    // stub looper
    void NMCacheRadioHandler::onTunerPlayInfoUpdate(const NMCacheRadioPlayInfo& playInfo)
    {
// for emulator gtest
#ifndef GTESTUP
        NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate()");

        if ((TRUE == m_seekAutoPause) && (NMTunerStatus_Normal == playInfo.nmInfo.byRadioStatus)) {
            m_seekAutoPause = FALSE;
            onDispInfoChanged(NMCacheRadioInfo_PlayInfo, 1);
        }

        // can not add stop judgement
        if (NMTunerStatus_BandScan == playInfo.nmInfo.byRadioStatus) {
            m_isInStationListUpStatus = TRUE;
            m_playInfo.nmInfo.byRadioStatus = NMTunerStatus_BandScan;
            NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate()    band scan");
        }
        else if ((NMTunerStatus_FastSeekUp == playInfo.nmInfo.byRadioStatus)
            || (NMTunerStatus_FastSeekDown == playInfo.nmInfo.byRadioStatus)
            || (NMTunerStatus_SeekUp == playInfo.nmInfo.byRadioStatus)
            || (NMTunerStatus_SeekDown == playInfo.nmInfo.byRadioStatus)
            || (NMTunerStatus_ScanUp == playInfo.nmInfo.byRadioStatus)
            || (NMTunerStatus_ScanUpReceiving == playInfo.nmInfo.byRadioStatus)
            || (NMTunerStatus_ScanDown == playInfo.nmInfo.byRadioStatus)
            || (NMTunerStatus_ScanDownReceiving == playInfo.nmInfo.byRadioStatus)) {
            if (FALSE == m_isInSeekOrScanStatus) {
                NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate()   in seek mode   FALSE == m_isInSeekOrScanStatus");
                if (NMTunerMode_CacheMode == m_curTunerMode) {
                    reqLiveMode(); // for stop seek flash Cache mode before Live mode
                }
                reqTunerSeekOrScan();
                m_isInSeekOrScanStatus = TRUE; // must after reqCacheRadioStationChange
            }
            if (NMTunerMode_CacheMode == m_curTunerMode) {
                if (m_playInfo.nmInfo.stFreq != playInfo.nmInfo.stFreq) {
                    onDispInfoChanged(NMCacheRadioInfo_PlayInfo, 1);
                }
            }
        }
        else if (NMTunerStatus_Invalid != playInfo.nmInfo.byRadioStatus) {
            if ((TRUE == m_isInSeekOrScanStatus)
                || (TRUE == m_isInStationListUpStatus)
                || (playInfo.nmInfo.stFreq != m_playInfo.nmInfo.stFreq)
                || (playInfo.nmInfo.stBitData5.byMultiCh != m_playInfo.nmInfo.stBitData5.byMultiCh)) {
                if (NMTunerMode_CacheMode == m_curTunerMode) {
                    reqLiveMode();
                }
                NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate()  Frequent or MultiCh Changed or seek finished");
                m_isInSeekOrScanStatus = FALSE;
                m_isInStationListUpStatus = FALSE;
                m_playInfo.nmInfo.stFreq = playInfo.nmInfo.stFreq;
                m_playInfo.nmInfo.stBitData5.byMultiCh = playInfo.nmInfo.stBitData5.byMultiCh;
                NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate()   fre change & m_isInSeekOrScanStatus == FALSE");
                if (!m_isFirstChange) {
                    reqCacheRadioStationChange();
                }
                m_isFirstChange = FALSE;
            }
            else {
                bool isEqual = FALSE;
                int result = memcmp(&(playInfo.nmInfo), &(m_playInfo.nmInfo), sizeof(NMTunerPlayInfo));
                if (!result) {
                    NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate() nmInfo compare is equal");
                    if ((m_playInfo.psdInfo.trackName == playInfo.psdInfo.trackName)
                        && (m_playInfo.psdInfo.artistName == playInfo.psdInfo.artistName)
                        && (m_playInfo.psdInfo.albumName == playInfo.psdInfo.albumName)
                        && (m_playInfo.psdInfo.genreName == playInfo.psdInfo.genreName)) {
                        NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate() psdInfo compare is equal");
                        if ((m_playInfo.isAcquiringSignal == playInfo.isAcquiringSignal)
                            && (m_playInfo.isPsNameGet == playInfo.isPsNameGet)
                            && (m_playInfo.psName == playInfo.psName)) {
                            NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate() other compare is equal");
                            isEqual = TRUE;
                        }
                    }
                }

                if (!isEqual) {
                    m_playInfo = playInfo;
                    if (NULL != m_pCacheInfo) {
                        NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate()   after post  isNoHdRadiosignal[%d]", playInfo.nmInfo.stBitData7.byIsHD);
                        m_pCacheInfo->cacheRadioStoreInfoUpdate(&m_playInfo);
                    }
                    else {
                        NMLOGE("NMCacheRadioHandler::onTunerPlayInfoUpdate() m_pCacheInfo is NULL");
                    }
                }
            }
        }
        else {
            m_isInSeekOrScanStatus = FALSE;
            reqCacheRadioStationChange();
            // do nothing
        }
        NMLOGD("NMCacheRadioHandler::onTunerPlayInfoUpdate() end");
#endif
    }

    // tuner service
    void NMCacheRadioHandler::getCacheRadioPlayInfo(NMCacheRadioPlayInfo& playInfo)
    {

        NMLOGD("NMCacheRadioHandler::getCacheRadioPlayInfo()");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->getDispPlayInfo(playInfo);
        }
        else {
            NMLOGE("NMCacheRadioHandler::getCacheRadioPlayInfo() m_pCacheInfo is NULL");
        }

    }

// work thread
    void NMCacheRadioHandler::getCacheRadioStatus(NMCacheRadioStatus& playStatus)
    {

        playStatus = m_curStatus;

    }

    void NMCacheRadioHandler::getCacheRadioPlayStatus(NMCacheRadioPlayStatus &curStatus)
    {

        NMLOGD("NMCacheRadioHandler::getCacheRadioPlayStatus()");
        curStatus = m_reqPlayStatus;

    }

// encode
    void NMCacheRadioHandler::onStoreInfoOverWrite(NMCacheInfoType infoType, unsigned short num)
    {

        NMLOGD("NMCacheRadioHandler::onStoreInfoOverWrite()");
        if (NULL != m_pCacheInfo) {
            if (NULL != m_workLooper) {
                m_workLooper->postRunnable(NMTaskBind(m_pCacheInfo, &NMCacheRadioInfo::onOverWriteStoreInfo, infoType, num));
            }
            else {
                NMLOGE("NMCacheRadioHandler::onStoreInfoOverWrite() m_workLooper is NULL");
            }
        }
        else {
            NMLOGE("NMCacheRadioHandler::onStoreInfoOverWrite() m_pCacheInfo is NULL");
        }

    }

// decode
    void NMCacheRadioHandler::onDispInfoChanged(NMCacheInfoType infoType, unsigned short num)
    {

        NMLOGD("NMCacheRadioHandler::onDispInfoChanged()  infoType[%d] num[%d]", infoType, num);
        if (NULL != m_pCacheInfo) {
            if (NULL != m_workLooper) {
                m_workLooper->postRunnable(NMTaskBind(m_pCacheInfo, &NMCacheRadioInfo::changeDispInfo, infoType, num));
            }
            else {
                NMLOGE("NMCacheRadioHandler::onDispInfoChanged() m_workLooper is NULL");
            }
        }
        else {
            NMLOGE("NMCacheRadioHandler::onDispInfoChanged() m_pCacheInfo is NULL");
        }

    }

    // live mode to cache mode disp the current playinfo
    void NMCacheRadioHandler::changeDispInfoToCurNum()
    {

        NMLOGD("NMCacheRadioHandler::changeDispInfoToCurNum()");
        if (NULL != m_pCacheInfo) {
            if (NULL != m_pCacheData) {
                m_pCacheData->forceDispArtNumChanged(NMCacheRadio_Order);
            }
        }
        else {
            NMLOGE("NMCacheRadioHandler::changeDispInfoToCurNum() m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::changeStoringInfoNum(NMCacheInfoType infoType, unsigned short num) // handle looper
    {

        NMLOGD("NMCacheRadioHandler::changeStoringInfoNum()");
        if (NULL != m_pCacheData) {
            m_pCacheData->changeStoringInfoNum(infoType, num);
        }
        else {
            NMLOGE("NMCacheRadioHandler::changeStoringInfoNum() m_pCacheData is NULL");
        }
        NMLOGD("NMCacheRadioHandler::changeStoringInfoNum() end");

    }

    void NMCacheRadioHandler::timeOut()
    {
        if (NULL != m_workLooper) {
            m_workLooper->postRunnableAtFront(NMTaskBind(this, &NMCacheRadioHandler::updateStatus, FALSE));
        }
    }

// tuner service & timer
    void NMCacheRadioHandler::updateStatus(bool forceUpdate = FALSE)
    {

        if (NULL == m_pCacheData) {
            NMLOGE("NMCacheRadioHandler::updateStatus() m_pCacheData is NULL");
            return;
        }

        // handle FF and REW command
        if (m_timeSquenceStatus <= m_timeCuntFast) {
            m_timeCuntFast = 0;
            if ((NMCacheRadioPlayStatus_REW == m_reqPlayStatus) || (NMCacheRadioPlayStatus_FF == m_reqPlayStatus)) {
                NMLOGD("NMCacheRadioHandler::updateStatus()   onFF || onRew");
                if (NMCacheRadioPlayStatus_REW == m_reqPlayStatus) {
                    m_pCacheData->caculatePlayPoint(fast_operation, NMTunerDirection_Down);
                }
                else {
                    bool isFinished = m_pCacheData->caculatePlayPoint(fast_operation, NMTunerDirection_Up);
                    if (!isFinished) {
                        // for skip to end play button status not chang to pause
                        // end
                        reqLiveMode();
                        return;
                    }
                }
                if (NULL != m_pTimer) {
                    m_timeCuntStatus = 0;
                    m_timeCuntDiag = 0;
                    m_timeCuntMicon = 0;
                    forceUpdate = TRUE;
                    m_pTimer->restart();
                }
                m_curStatus.playStatus = m_reqPlayStatus;
            }
        }
        else {
            m_timeCuntFast++;
        }

        // skip handler
        if (NMCacheRadioPlayStatus_Skip == m_reqPlayStatus) {
            onSkip(m_skipDirection);
            forceUpdate = TRUE;
        }


        // Live mode frequence is 900ms
        // Cache mode frequence is 400ms
        if ((m_timeSquenceStatus <= m_timeCuntStatus) || (TRUE == forceUpdate)) {
            uint32_t storeFrameCunt = 0;
            m_pCacheData->getStoreFrameNum(storeFrameCunt);
            uint32_t playFrameCunt = 0;
            m_pCacheData->getPlayFrameNum(playFrameCunt);

            if ((NMTunerMode_CacheMode == m_curTunerMode) || (TRUE == forceUpdate)) {
                m_timeCuntStatus = 0;
                updateCacheRadioStatusForUI(storeFrameCunt, playFrameCunt);
            }
            else { // Live Mode
                unsigned short curTime = ((storeFrameCunt + 22) * NMCacheRadio_TimeUint / 1000);
                if (NMCacheRadio_LiveTimeNotifySequence < (curTime - m_curPlayTime)) {
                    unsigned short moveFrameCnt = NMCacheRadio_OneSecondFramCnt;
                    if (((playFrameCunt + moveFrameCnt + 22) * NMCacheRadio_TimeUint / 1000) < (m_curPlayTime + NMCacheRadio_LiveTimeNotifySequence)) {
                        ++moveFrameCnt;
                    }

                    m_pCacheData->moveFrameNumbers(moveFrameCnt);
                    m_timeCuntStatus = 3;
                    m_pCacheData->getPlayFrameNum(playFrameCunt);
                    updateCacheRadioStatusForUI(storeFrameCunt, playFrameCunt);
                }
                else if ((NMCacheRadio_LiveTimeNotifySequence == (curTime - m_curPlayTime))
                    || ((NMCacheRadio_MAXSTORETIME <= curTime) && (NMCacheRadio_MAXSTORETIME > m_curPlayTime))) {

                    if (NULL != m_pCacheData) {
                        m_pCacheData->moveToLivePosition();
                    }
                    m_pCacheData->getPlayFrameNum(playFrameCunt);
                    m_timeCuntStatus = 0;
                    updateCacheRadioStatusForUI(storeFrameCunt, playFrameCunt);
                }
                else {

                }
            }
        }
        else {
            m_timeCuntStatus++;
        }

        // for diag cache time frequence is 500ms
        if ((4 <= m_timeCuntDiag) || (TRUE == forceUpdate)) {
            m_timeCuntDiag = 0;
            unsigned short cachetime = 0;
            cachetime = m_curRecordTime - m_curPlayTime;
            m_diagCacheTime.set_cachetime(cachetime);
            m_diagProxy.notifyCacheTime(m_diagCacheTime);
            NMLOGD("RecordTime[%d], PlayTime[%d], cachetime[%d]", m_curRecordTime, m_curPlayTime, cachetime);
        }
        else {
            m_timeCuntDiag++;
        }

        // send command to micon frequence is 1s
        // BCD
        if ((NMCacheRadioPlayStatus_Stop != m_reqPlayStatus)
            && (NMCacheRadioPlayStatus_Start != m_reqPlayStatus)
            && (NMTunerMode_CacheMode == m_curTunerMode)
            && (FALSE == m_isInreqLiveStatus)) { // must use m_reqPlayStatus, otherwise may switch to cache, during call live mode
            if ((9 <= m_timeCuntMicon) || (TRUE == forceUpdate)) {
                m_timeCuntMicon = 0;
                if (NMCacheRadioPlayStatus_Stop != m_reqPlayStatus) {
                    NMCacheRadioReq reqInfo;
                    reqInfo.recordMin = (((m_curRecordTime / 60) / 10) << 4) + (((m_curRecordTime / 60) % 10) & 0x0f);
                    reqInfo.recordSec = (((m_curRecordTime % 60) / 10) << 4) + (((m_curRecordTime % 60) % 10) & 0x0f);
                    reqInfo.playMin = (((m_curPlayTime / 60) / 10) << 4) + (((m_curPlayTime / 60) % 10) & 0x0f);
                    reqInfo.playSec = (((m_curPlayTime % 60) / 10) << 4) + (((m_curPlayTime % 60) % 10) & 0x0f);
                    if (m_playInfo.nmInfo.stBitData4.byDigitalSound) {
                        reqInfo.tunerSoundMod = 0x02;
                    }
                    else {
                        reqInfo.tunerSoundMod = 0x01;
                    }
                    if (NULL != m_pTunerParser) {
                        // NMLOGD("NMCacheRadioHandler::updateStatus()  changed tuner mode");
                        m_pTunerParser->reqSwitchTunerMode(reqInfo);
                    }
                }
            }
            else {
                m_timeCuntMicon++;
            }
        }

        // request play after skip
        if ((NMCacheRadioPlayStatus_Skip == m_reqPlayStatus) && (TRUE == forceUpdate)) {
            if (NULL != m_pCacheDecode) {
                if ((FALSE == m_isInAutoPauseStatus)
                    && (FALSE == m_isInVRStatus)) {
                    m_reqPlayStatus = NMCacheRadioPlayStatus_Play;
                    m_pCacheDecode->play();
                }
            }
            else {
                NMLOGE("NMCacheRadioHandler::onSkip() m_pCacheDecode is NULL");
            }
        }

    }

// timer && tunerservice
    void NMCacheRadioHandler::updateCacheRadioStatusForUI(uint32_t storeFrameCunt, uint32_t playFrameCunt)
    {

        // caculate record time and play time

        m_curRecordTime = (storeFrameCunt + 22) * NMCacheRadio_TimeUint / 1000;
        // ( + 22)make the play tiem has more chance to reach top time when playing automaticually
        uint32_t tmpCurPlayTime = (playFrameCunt + 22) * NMCacheRadio_TimeUint / 1000;
        NMLOGD("storeFrameCunt[%d], playFrameCunt[%d], ", storeFrameCunt, playFrameCunt);

        if (NMCacheRadio_MAXSTORETIME <= m_curRecordTime) {
            if (NMTunerMode_LiveMode == m_curTunerMode) {
                if (tmpCurPlayTime > m_curPlayTime) {
                    m_curPlayTime = tmpCurPlayTime;
                }
            }
            else {
                unsigned short cachetime = 0;
                if (NULL != m_pCacheData) {
                    m_pCacheData->getCacheTime(cachetime);
                    cachetime = cachetime * NMCacheRadio_TimeUint / 1000;
                }

                if (NMCacheRadioPlayStatus_Play == m_reqPlayStatus) { // reach the top play time
                    if (2 >= m_topPlayTimeCun) { // it only has 2 times(1.6s) to reach the top play time
                        ++m_topPlayTimeCun;
                        if ((1 >= cachetime) && (m_curPlayTime < tmpCurPlayTime)) { // readuce the thread to change play time
                            m_curPlayTime = tmpCurPlayTime;
                        }
                    }
                // need not update play
                // NMLOGD("NMCacheRadioHandler::updateCacheRadioStatusForUI() m_curRecordTime[%d]", m_curRecordTime);
                }
                else if (NMCacheRadioPlayStatus_Pause == m_reqPlayStatus) { // pause
                    if (m_curPlayTime > tmpCurPlayTime) {
                        m_curPlayTime = tmpCurPlayTime;
                    }
                }
                else {
                    m_curPlayTime = tmpCurPlayTime;
                }
            }
        }
        else {
            m_curPlayTime = tmpCurPlayTime;
        }
        m_curStatus.progressBar.curRecordTime = m_curRecordTime;
        m_curStatus.progressBar.curPlayTime = m_curPlayTime;
        m_curStatus.progressBar.maxRecordTime = NMCacheRadio_MAXSTORETIME;

        // update
        if ((NULL != m_pCacheInfo) && (NULL != m_workLooper)) {
            m_workLooper->postRunnableAtFront(NMTaskBind(m_pCacheInfo, &NMCacheRadioInfo::updateCacheRadioStatus));
        }
        else {
            NMLOGE("NMCacheRadioHandler::updateCacheRadioStatusForUI() m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::getCacheRadioPrimaryImageArtworkDisp(NMHdRadioArtworkDisp& artWorkDisp)
    {

        // NMLOGD("NMCacheRadioHandler::getCacheRadioPrimaryImageArtworkDisp");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->getCacheRadioPrimaryImageArtworkDisp(artWorkDisp);
        }
        else {
            NMLOGE("NMCacheRadioHandler::getCacheRadioPrimaryImageArtworkDisp m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::getCacheRadioPrimaryImage(NMHdRadioImageFormat& imageFormat)
    {

        // NMLOGD("NMCacheRadioHandler::getCacheRadioPrimaryImage");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->getCacheRadioPrimaryImage(imageFormat);
        }
        else {
            NMLOGE("NMCacheRadioHandler::getCacheRadioPrimaryImage m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::getCacheRadioAlbumCoverArt(NMGraceNoteCoverArtWithArtistText& albumBuf)
    {

        // NMLOGD("NMCacheRadioHandler::getCacheRadioAlbumCoverArt");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->getCacheRadioAlbumCoverArt(albumBuf);
        }
        else {
            NMLOGE("NMCacheRadioHandler::getCacheRadioAlbumCoverArt m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::getCacheRadioGenreCoverArt(NMGraceNoteCoverArtWithArtistText& genreBuf)
    {

        // NMLOGD("NMCacheRadioHandler::getCacheRadioGenreCoverArt");
        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->getCacheRadioGenreCoverArt(genreBuf);
        }
        else {
            NMLOGE("NMCacheRadioHandler::getCacheRadioGenerCoverArt m_pCacheInfo is NULL");
        }

    }

    void NMCacheRadioHandler::getCacheRadioStationLogo(std::string& stationLogo)
    {

        // NMLOGD("NMCacheRadioHandler::getCacheRadioStationLogo");

        if (NULL != m_pCacheInfo) {
            m_pCacheInfo->getCacheRadioStationLogo(stationLogo);
        }
        else {
            NMLOGE("NMCacheRadioHandler::getCacheRadioStationLogo m_pCacheInfo is NULL");
        }

    }

// timer & tuner service & decode
    void NMCacheRadioHandler::cacheRadioPlayStatusChanged(NMCacheRadioPlayStatus playStatus)
    {

        NMLOGD("NMCacheRadioHandler::cacheRadioPlayStatusChanged playStatus[%d], m_reqPlayStatus[%d]", playStatus, m_reqPlayStatus);

            NMLOGD("NMCacheRadioHandler::cacheRadioPlayStatusChanged m_curStatus.playStatus[%d]", m_curStatus.playStatus);
            m_curStatus.playStatus = playStatus;
            if (NULL != m_pCacheInfo) {
                m_pCacheInfo->updateCacheRadioStatus();
            }

        NMLOGD("NMCacheRadioHandler::cacheRadioPlayStatusChanged end");

    }

    void NMCacheRadioHandler::setCacheRadioAudioToken(int32_t tokenId)
    {

        NMLOGD("NMCacheRadioHandler::setCacheRadioAudioToken()");
        if (NULL != m_pCacheDecode) {
            m_pCacheDecode->setCacheRadioAudioToken(tokenId);
        }

    }

// code decode
    void NMCacheRadioHandler::terribleErrorCtrl()
    {

        NMLOGD("NMCacheRadioHandler::terribleErrorCtrl()");
        if (NULL != m_workLooper) {
            if (NULL != m_pCacheInfo) {
                m_workLooper->postRunnable(NMTaskBind(m_pCacheInfo, &NMCacheRadioInfo::notifyCacheRadioUnRecoverErr));
            }
        }

    }

    void NMCacheRadioHandler::tunerModeChanged(NMTunerModeType tunerMode)
    {

        NMLOGD("NMCacheRadioHandler::tunerModeChanged() tunerde[%d] m_curTunerMode[%d]", tunerMode, m_curTunerMode);
        m_isInreqLiveStatus = TRUE;
        if (NMTunerMode_CacheMode == tunerMode) {
            if ((NMTunerMode_LiveMode == m_curTunerMode)
                || (NMCacheRadioPlayStatus_Start == m_reqPlayStatus)) {
                if (NMCacheRadio_MAXSTORETIME <= m_curRecordTime) {
                    if (NULL != m_pCacheData) {
                        m_pCacheData->moveToLivePosition();
                    }
                }
                m_curTunerMode = NMTunerMode_CacheMode;
                m_timeSquenceStatus = NMCacheRadio_CacheStatusTimeSquence;
                m_timeCuntStatus = 0;
                changeDispInfoToCurNum();
                updateStatus(TRUE);
                if (NULL != m_pTimer) {
                    m_pTimer->restart();
                }
                if (TRUE == m_isInSeekOrScanStatus) {
                    m_reqPlayStatus = NMCacheRadioPlayStatus_Pause;  // live switch to cache while seeking
                    return; // when you seek, can not changed to play status
                }
                if (TRUE == m_isInSkipStatus) {
                    m_isInSkipStatus = FALSE;
                    if ((NMCacheRadioPlayStatus_Stop != m_reqPlayStatus)
                        && (NMCacheRadioPlayStatus_REW != m_reqPlayStatus)) {
                        reqCacheRadioPlay();
                    }
                }
            }
        }
        else {
            if (NMTunerMode_CacheMode == m_curTunerMode) {
                if (NULL != m_pCacheData) {
                    m_pCacheData->moveToLivePosition();
                }
                m_timeCuntStatus = 0;
                m_isInSkipStatus = FALSE;
                m_curTunerMode = NMTunerMode_LiveMode;
                m_curStatus.playStatus = NMCacheRadioPlayStatus_Play;
                m_timeSquenceStatus = NMCacheRadio_LiveStatusTimeSquence;
                updateStatus(TRUE);
                reqCacheRadioNativePause();
                if (NMCacheRadioPlayStatus_Stop != m_reqPlayStatus) {
                    if (NULL != m_pTimer) {
                        m_pTimer->restart();
                    }
                }
            }
        }

    }

// tuner service
    void NMCacheRadioHandler::onStartAutoPause(bool isPauseByVR)
    {

        NMLOGD("NMCacheRadioHandler::onStartAutoPause()  m_isAutoPauseOn = %d, isPauseByVR = %d", m_isAutoPauseOn, isPauseByVR);
        if (NMCacheRadioPlayStatus_Stop == m_reqPlayStatus) {
            return;
        }
        if (!isPauseByVR) {
            m_isInAutoPauseStatus = TRUE;
        }
        if (NMTunerMode_CacheMode == m_curTunerMode) {
            if (TRUE == m_isInStationListUpStatus) {
                NMLOGD("NMCacheRadioHandler::onStartAutoPause m_isInStationListUpStatus == TRUE");
                m_autoPausePlayStatus = NMCacheRadioPlayStatus_Play;
            }
            else if (TRUE == m_isInSeekOrScanStatus) {
                m_isInSeekOrScanStatus = FALSE;
                m_seekAutoPause = TRUE;
                reqCacheRadioStationChange();
                m_autoPausePlayStatus = NMCacheRadioPlayStatus_Play;
            }
            else {
                if (FALSE == isPauseByVR) {
                    m_autoPausePlayStatus = NMCacheRadioPlayStatus_Play;
                }
                else { // VR autoPause
                    m_autoPausePlayStatus = m_reqPlayStatus;
                }
            }

            if (NMTunerSwitch_Off != m_isAutoPauseOn) {
                reqCacheRadioNativePause();
            }
            m_isInVRStatus = isPauseByVR;
        }
    }

// tuner service
    void NMCacheRadioHandler::onStopAutoPause()
    {

        NMLOGD("NMCacheRadioHandler::onStopAutoPause()   m_isAutoPauseOn = %d", m_isAutoPauseOn);
        NMLOGD("NMCacheRadioHandler::onStopAutoPause()   m_curTunerMode = %d", m_curTunerMode);
        NMLOGD("NMCacheRadioHandler::onStopAutoPause()   m_autoPausePlayStatus = %d", m_autoPausePlayStatus);
        if (NMCacheRadioPlayStatus_Stop == m_reqPlayStatus) {
            return;
        }
        m_isInAutoPauseStatus = FALSE;
        m_isInVRStatus = FALSE;
        if (NMTunerMode_CacheMode == m_curTunerMode) {
            if (NMCacheRadioPlayStatus_Pause != m_autoPausePlayStatus) {
                m_autoPauseTime = 0;
                NMLOGD("NMCacheRadioHandler::onStopAutoPause()   play");
                reqCacheRadioPlay();
            }
            else {
                if (NMCacheRadioPlayStatus_Pause == m_autoPausePlayStatus) {
                    reqCacheRadioNativePause();
                    cacheRadioPlayStatusChanged(NMCacheRadioPlayStatus_Pause);
                }
            }
        }

    }

    void NMCacheRadioHandler::reInitializeMember()
    {

        NMLOGD("NMCacheRadioHandler::reInitializeMember()");
        m_curRecordTime = 0;
        m_curPlayTime = 0;
        m_timeCuntDiag = 0;
        m_timeCuntMicon = 0;
        m_timeCuntStatus = 0;
        m_timeCuntFast = 0;
        m_topPlayTimeCun = 0;
        m_isInSkipStatus = FALSE;
        m_curStatus.progressBar.curPlayTime = 0;
        m_curStatus.progressBar.curRecordTime = 0;

        // no need reinitialize playinfo
        NMLOGD("NMCacheRadioHandler::reInitializeMember()   end");

    }

    void NMCacheRadioHandler::setAutoPauseCfg(NMTunerSwitch autoPauseStatus)
    {

        NMLOGD("NMCacheRadioHandler::setAutoPauseCfg() autoPauseStatus = %d", autoPauseStatus);
        m_isAutoPauseOn = autoPauseStatus;

    }

    void NMCacheRadioHandler::muteFrontScreen()
    {

        NMLOGD("NMCacheRadioHandler::muteFrontScreen()");
#ifndef EMULATOR
        NCString status("on");
        NCUri uri("content://settings/system");
        NCContentResolver resolver;
        NCContentValues value;
        value.put("avsetting_mute_short", status);
        resolver.update(uri, value, NCString(), NCObjList<NCString>());
#endif
    }

    NMCacheRadioHandler::NMCacheRadioHandler(NMTunerParser *tunerParser)
    {

        NMLOGD("NMCacheRadioHandler::NMCacheRadioHandler()");

        m_pTunerParser = tunerParser;
        m_curTunerMode = NMTunerMode_LiveMode;
        m_curRecordTime = 0;
        m_curPlayTime = 0;
        m_timeCuntDiag = 0;
        m_timeCuntMicon = 0;
        m_timeCuntStatus = 0;
        m_timeCuntFast = 0;
        m_isInSeekOrScanStatus = FALSE;
        m_isInStationListUpStatus = FALSE;
        m_isInAutoPauseStatus = FALSE;
        m_isInVRStatus = FALSE;
        m_curHDStatus = 0;
        m_isInSkipStatus = FALSE;
        m_topPlayTimeCun = 0;
        m_timeSquenceStatus = NMCacheRadio_LiveStatusTimeSquence;
        m_skipDirection = NMTunerDirection_Up;
        m_pCacheData = new NMCacheRadioData();
        m_pCacheInfo = new NMCacheRadioInfo();
        m_pCacheEncode = new NMCacheRadioEncode();
        m_pCacheDecode = new NMCacheRadioDecode();

        memset(&m_curStatus, 0, sizeof(NMCacheRadioStatus));
        m_curStatus.playStatus = NMCacheRadioPlayStatus_Play;
        m_curStatus.progressBar.maxRecordTime = NMCacheRadio_MAXSTORETIME;

        m_playInfo.psdInfo.isGetTrackName  = FALSE;
        m_playInfo.psdInfo.trackName       = "";
        m_playInfo.psdInfo.isGetArtistName = FALSE;
        m_playInfo.psdInfo.artistName      = "";
        m_playInfo.psdInfo.isGetAlbumName  = FALSE;
        m_playInfo.psdInfo.albumName       = "";
        m_playInfo.psdInfo.isGetGenreName  = FALSE;
        m_playInfo.psdInfo.genreName       = "";
        memset(&m_playInfo.nmInfo, 0, sizeof(NMTunerPlayInfo));
        m_playInfo.isAcquiringSignal = FALSE;
        m_playInfo.isPsNameGet = FALSE;
        m_playInfo.psName = "";

        m_isAutoPauseOn = NMTunerSwitch_On;
        m_autoPauseTime = 0;
        m_workLooper = NULL;
        m_isFirstChange = TRUE;
        m_isInreqLiveStatus = FALSE;

        m_pTimer = new NMRunnableTimer(NMTaskBind(this, &NMCacheRadioHandler::timeOut), NMCacheRadio_StatusTimeInterval, NC_TRUE);

    }

    NMCacheRadioHandler::~NMCacheRadioHandler()
    {

        NMLOGD("NMCacheRadioHandler::~NMCacheRadioHandler()");

        stop();

        if (NULL != m_pTimer) {
            delete m_pTimer;
            m_pTimer = NULL;
        }
        if (NULL != m_pCacheEncode) {
            delete m_pCacheEncode;
            m_pCacheEncode = NULL;
        }
        if (NULL != m_pCacheDecode) {
            delete m_pCacheDecode;
            m_pCacheDecode = NULL;
        }
        if (NULL != m_pCacheInfo) {
            delete m_pCacheInfo;
            m_pCacheInfo = NULL;
        }
        if (NULL != m_pCacheData) {
            delete m_pCacheData;
            m_pCacheData = NULL;
        }

        NMLOGD("NMCacheRadioHandler::~NMCacheRadioHandler() end");

    }

}
}
/* EOF */