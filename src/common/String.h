/*
**  Copyright(C) 2017, StepToSky
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.Neither the name of StepToSky nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  Contacts: www.steptosky.com
*/

#pragma once

#pragma warning(push, 0)
#include <max.h>
#include <Path.h>
#include <strclass.h>
#pragma warning(pop)

#include "additional/utils/StringUtils.h"
#include <xpln/utils/Path.h>

/**************************************************************************************************/
/////////////////////////////////////* UNICODE UTILS *//////////////////////////////////////////////
/**************************************************************************************************/

/* 
 * String utilities
 */

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#ifdef UNICODE
#   define USE_WITH_UNICODE(x) x
#   define USE_WITHOUT_UNICODE(x)
#else
#   define USE_WITH_UNICODE(x)
#   define USE_WITHOUT_UNICODE(x) x
#endif

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

typedef TSTR TStr;
typedef MSTR MStr;

namespace xobj {

inline MSTR toMStr(const std::string & str) {
#ifdef UNICODE
    return MSTR::FromUTF8(str.data(), str.length());
#else
    return MSTR(str.c_str());
#endif
}

/*! 
 * \details temporary solution until whole code uses corrected string 
 * \todo
 */
inline MSTR toMStr(const std::wstring & str) {
#ifdef UNICODE
    return MSTR::FromUTF16(str.data(), str.length());
#else
    return MSTR(sts::toMbString(str).c_str());
#endif
}

inline std::string fromMStr(const CStr & str) {
    return std::string(str.data(), str.Length());
}

inline std::string fromMStr(const WStr & str) {
#ifdef UNICODE
    std::size_t len = 0;
    const auto utf8 = str.ToUTF8(&len);
    return std::string(utf8.data(), len);
#else
    return ::sts::toMbString(std::wstring(str.data(), str.Length()));
#endif
}

//-------------------------------------------------------------------------

inline Path fromMPath(const MaxSDK::Util::Path & path) {
#ifdef UNICODE
    const auto str = path.GetString();
    return Path(str.data(), str.Length());
#else
    return Path(sts::toWString(fromMStr(path.GetString())));
#endif
}

}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 14

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(TSTR str) { return str; }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(const char * str) { return sts::toWString(str).c_str(); }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(const wchar_t * str) { return sts::toWString(str).c_str(); }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(std::string & str) { return sts::toWString(str).c_str(); }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(std::wstring & str) { return sts::toWString(str).c_str(); }

#else

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(TSTR str) { return str; }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(const char * str) { return sts::toMbString(str).c_str(); }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(const wchar_t * str) { return sts::toMbString(str).c_str(); }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(std::string & str) { return sts::toMbString(str).c_str(); }

/*!
 * \deprecated use xobj::toMStr
 */
inline TSTR toTSTR(std::wstring & str) { return sts::toMbString(str).c_str(); }

#endif

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
