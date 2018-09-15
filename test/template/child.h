/**
 * Copyright @ 2017 - 2018 Suntec Software(Shanghai) Co., Ltd.
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

/**
 * @file MediaManagerSuit.h
 * @brief Declaration file of class MediaManagerSuit.
 *
 */

#ifndef MEDIAMANAGER_FTEST_CLIENT_MEDIAMANAGER_SUIT_H_
#define MEDIAMANAGER_FTEST_CLIENT_MEDIAMANAGER_SUIT_H_

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <vector>
#include "def.h"
#include "base.h"


#ifdef OBJ_METHOD_PTR
#undef OBJ_METHOD_PTR
#endif
#define OBJ_METHOD_PTR(OBJ) typedef void(OBJ::*Method_Ptr)();

namespace merbok {

class MediaManagerSuit : public BaseSuit
{
public:
    MediaManagerSuit();
    ~MediaManagerSuit();

public:
    virtual void createTestCase();

public: // test case
    void SourceChange_Off2FM2();

private:
    OBJ_METHOD_PTR(MediaManagerSuit);
    std::vector<Method_Ptr>                             m_testMethodVec;
    std::vector<ClienTestCase<MediaManagerSuit> >        m_testCaseVec;
};  // MediaManagerSuit

}   // namespace merbok

#endif // MEDIAMANAGER_FTEST_CLIENT_MEDIAMANAGER_SUIT_H_
