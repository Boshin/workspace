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
 * @file BaseSuit.h
 * @brief Declaration file of class BaseSuit.
 *
 */

#ifndef MEDIAMANAGER_FTEST_CLIENT_BASE_SUIT_H_
#define MEDIAMANAGER_FTEST_CLIENT_BASE_SUIT_H_

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <stdio.h>

namespace merbok {

class BaseSuit
{
public:
    BaseSuit() { }

    virtual ~BaseSuit() { }

public:
    virtual void setUp()
    {
        printf("BaseSuit::setUp this %p\n", this);
    }

public:
    virtual void createTestCase() = 0;

protected:
    virtual void evaluateResult()
    {
        printf("BaseSuit::evaluateResult\n");
    }


};  // BaseSuit

}   // namespace merbok

#endif // MEDIAMANAGER_FTEST_CLIENT_BASE_SUIT_H_
