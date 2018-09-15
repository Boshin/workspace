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

#include "child.h"


#ifdef ADDCASE
#undef ADDCASE
#endif
#define ADDCASE(TestObj, CaseName) (m_testMethodVec.push_back(&TestObj::CaseName))

#define ADDCASE2(TestObj, CaseName)                                                     \
    {                                                                                   \
        Method_Ptr mPtr = &TestObj::CaseName;                                           \
        ClienTestCase<TestObj> tmpcase(mPtr, #TestObj, #CaseName);                                 \
        m_testCaseVec.push_back(ClienTestCase<TestObj>(&TestObj::CaseName, #TestObj, #CaseName));  \
    }

#define ADDCASE3(TestObj, CaseName)                                                     \
    {                                                                                   \
        Method_Ptr mPtr = &TestObj::CaseName;                                           \
        ClienTestCase<TestObj> tmpcase(mPtr);                                           \
        m_testCaseVec.push_back(ClienTestCase<TestObj>(&TestObj::CaseName));            \
    }

#define ADDCASE4(TestObj, CaseName)                                                                     \
        void TestObj::createTestCase()                                                                  \
        {                                                                                               \
            m_testCaseVec.push_back(ClienTestCase<TestObj>(&TestObj::CaseName, #TestObj, #CaseName));   \
        }                                                                                               \

#define TEST_F(TestObj, CaseName)                   \
    ADDCASE4(TestObj, CaseName)                     \
    void TestObj::CaseName()                        \

namespace merbok
{
    MediaManagerSuit::MediaManagerSuit()
    {
    }

    MediaManagerSuit::~MediaManagerSuit()
    {
    }

    // void MediaManagerSuit::createTestCase()
    // {
    //     setUp();
    //     printf("MediaManagerSuit::createTestCase this %p\n", this);
    //     // ADDCASE2(MediaManagerSuit, SourceChange_Off2FM2)
    // }

    TEST_F(MediaManagerSuit, SourceChange_Off2FM2)
    {
        printf("MediaManagerSuit::SourceOff2FM2On\n");
    }
}

int main()
{
    merbok::MediaManagerSuit suit;
    suit.createTestCase();
    return 0;
}
