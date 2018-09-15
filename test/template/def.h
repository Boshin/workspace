#ifndef MMFTEST_DEF_H_
#define MMFTEST_DEF_H_

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <stdio.h>
#include <string>

namespace merbok
{
template<typename OBJ>
struct ClienTestCase
{
    typedef void(OBJ::*Method_Ptr)();

    Method_Ptr       m_methodPtr;
    std::string      m_objName;
    std::string      m_caseName;

    ClienTestCase(Method_Ptr methodPtr, const std::string& objName, const std::string& caseName)
        : m_methodPtr(methodPtr)
        , m_objName(objName)
        , m_caseName(caseName)
        {
            printf("m_objName: %s, m_caseName: %s\n", m_objName.c_str(), m_caseName.c_str());
        }

    ClienTestCase(Method_Ptr methodPtr)
        : m_methodPtr(methodPtr) { }
};
}
#endif /* MMFTEST_DEF_H_ */
