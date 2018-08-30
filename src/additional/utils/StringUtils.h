#pragma once

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

#include <cstddef>
#include <cstring>
#include <cassert>
#include <sstream>
#include <vector>
#include <list>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "Export.h"

#ifndef DOUBLE_PRECISION_PRINT
#	define DOUBLE_PRECISION_PRINT 8
#endif

#ifndef FLOAT_PRECISION_PRINT
#	define FLOAT_PRECISION_PRINT 6
#endif

namespace sts {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

namespace internal {

    //---------------------------------------------------------------
    // todo use C++ 17 'if' in future
    template<typename T>
    struct TrimSymbols {
        static const std::basic_string<T> & get() {
            static std::basic_string<std::string::value_type> s;
            assert(false); // Is not implemented for the type.
            return std::basic_string<T>();
        }
    };

    template<>
    struct TrimSymbols<std::string::value_type> {
        static const std::basic_string<std::string::value_type> & get() {
            static std::basic_string<std::string::value_type> s(" \t\f\v\r\n");
            return s;
        }
    };

    template<>
    struct TrimSymbols<std::wstring::value_type> {
        static const std::basic_string<std::wstring::value_type> & get() {
            static std::basic_string<std::wstring::value_type> s(L" \t\f\v\r\n");
            return s;
        }
    };

    template<>
    struct TrimSymbols<std::u16string::value_type> {
        static const std::basic_string<std::u16string::value_type> & get() {
            static std::basic_string<std::u16string::value_type> s(u" \t\f\v\r\n");
            return s;
        }
    };

    template<>
    struct TrimSymbols<std::u32string::value_type> {
        static const std::basic_string<std::u32string::value_type> & get() {
            static std::basic_string<std::u32string::value_type> s(U" \t\f\v\r\n");
            return s;
        }
    };

    //---------------------------------------------------------------
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details This is static class with utility methods for the STL string.
 * \tparam CharType type of the std::basic_string character.
 */
template<typename CharType>
class StringUtil {
public:

    //---------------------------------------------------------------
    // @{

    typedef std::list<std::basic_string<CharType>> List;     //!< STL List 
    typedef std::vector<std::basic_string<CharType>> Vector; //!< STL Vector

    // @}
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // @{

    /*!
     * \details Splits the string with specified separators.
     * \code 
     * // Usage example:
     * typedef std::vector<std::string> StrCont;
     * StrCont outData;
     * sts::MbStrUtils::split(myString, ":", outData);
     * \endcode
     * \note value_type of the Container must have "sub-string" constructor (i.e type(const std::string &str, size_t pos, size_t len)).
     * \tparam Container type of the container for storing the result, must have the method 'emplace_back'.
     * \param [in] str string for splitting
     * \param [in] separators the symbols which will be used as separator.
     * \param [out] container the container for storing the result.
     */
    template<class Container>
    static void split(const std::basic_string<CharType> & str, const std::basic_string<CharType> & separators, Container & container) {
        size_t found = str.find_first_of(separators);
        size_t lastFound = 0;
        while (found != std::basic_string<CharType>::npos) {
            if (lastFound != found) {
                container.emplace_back(str, lastFound, found - lastFound);
            }
            found = str.find_first_of(separators, lastFound = found + 1);
        }

        if (lastFound != str.size()) {
            container.emplace_back(str, lastFound, str.size() - lastFound);
        }
    }

    /*!
     * \details Splits the string with specified separators to the specified container.
     * \code 
     * // Usage example:
     * typedef std::vector<std::string> StrCont;
     * StrCont outData = sts::MbStrUtils::split<StrCont>(myString, ":");
     * \endcode
     * \tparam Container type of the container for storing the result, must have the method 'emplace_back'.
     * \param [in] str string for splitting
     * \param [in] separators the symbols which will be used as separator.
     * \return specified container with the result.
     */
    template<class Container>
    static Container split(const std::basic_string<CharType> & str, const std::basic_string<CharType> & separators) {
        Container out;
        split<Container>(str, separators, out);
        return out;
    }

    /*!
     * \details Splits the string with specified separators to std::list.
     * \code 
     * // Usage example:
     * sts::MbStrUtils::List outData = sts::MbStrUtils::splitToList(myString, ":");
     * \endcode
     * \param [in] str string for splitting
     * \param [in] separators the symbols which will be used as separator.
     * \return std::list with the result.
     */
    static List splitToList(const std::basic_string<CharType> & str, const std::basic_string<CharType> & separators) {
        List out;
        split<List>(str, separators, out);
        return out;
    }

    /*!
     * \details Splits the string with specified separators to std::vector.
     * \code 
     * // Usage example:
     * sts::MbStrUtils::Vector outData = sts::MbStrUtils::splitToVector(myString, ":");
     * \endcode
     * \param [in] str string for splitting
     * \param [in] separators the symbols which will be used as separator.
     * \return std::vector with the result.
     */
    static Vector splitToVector(const std::basic_string<CharType> & str, const std::basic_string<CharType> & separators) {
        Vector out;
        split<Vector>(str, separators, out);
        return out;
    }

    // @}
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // @{

    /*!
     * \details Trims left.
     * \param [in, out] str string for trimming.
     * \param [in] symbols the symbols that must be removed from start.
     */
    static void trimLeft(std::basic_string<CharType> & str, const std::basic_string<CharType> & symbols) {
        str.erase(0, str.find_first_not_of(symbols));
    }

    /*!
     * \details Trims left.
     * \param [in, out] str string for trimming.
     */
    static void trimLeft(std::basic_string<CharType> & str) {
        trimLeft(str, internal::TrimSymbols<CharType>::get());
    }

    /*!
     * \details Makes string copy and trims left.
     * \param [in] str string for trimming.
     * \param [in] symbols the symbols that must be removed from start.
     * \return new trimmed string.
     */
    static std::basic_string<CharType> trimLeftCopy(const std::basic_string<CharType> & str, const std::basic_string<CharType> & symbols) {
        std::basic_string<CharType> out(str);
        trimLeft(out, symbols);
        return out;
    }

    /*!
     * \details Makes string copy and trims left.
     * \param [in] str string for trimming.
     * \return new trimmed string.
     */
    static std::basic_string<CharType> trimLeftCopy(const std::basic_string<CharType> & str) {
        return trimLeftCopy(str, internal::TrimSymbols<CharType>::get());
    }

    //---------------------------------------------------------------

    /*!
     * \details Trims right.
     * \param [in, out] str string for trimming.
     * \param [in] symbols the symbols that must be removed from end.
     */
    static void trimRight(std::basic_string<CharType> & str, const std::basic_string<CharType> & symbols) {
        str.erase(str.find_last_not_of(symbols) + 1);
    }

    /*!
     * \details Trims right.
     * \param [in, out] str string for trimming.
     */
    static void trimRight(std::basic_string<CharType> & str) {
        trimRight(str, internal::TrimSymbols<CharType>::get());
    }

    /*!
     * \details Makes string copy and trims right.
     * \param [in] str string for trimming.
     * \param [in] symbols the symbols that must be removed from start.
     * \return new trimmed string.
     */
    static std::basic_string<CharType> trimRightCopy(const std::basic_string<CharType> & str, const std::basic_string<CharType> & symbols) {
        std::basic_string<CharType> out(str);
        trimRight(out, symbols);
        return out;
    }

    /*!
     * \details Makes string copy and trims right.
     * \param [in] str string for trimming.
     * \return new trimmed string.
     */
    static std::basic_string<CharType> trimRightCopy(const std::basic_string<CharType> & str) {
        return trimRightCopy(str, internal::TrimSymbols<CharType>::get());
    }

    //---------------------------------------------------------------

    /*!
     * \details Trims left and right.
     * \param [in, out] str string for trimming.
     * \param [in] symbols the symbols that must be removed from start and end.
     */
    static void trim(std::basic_string<CharType> & str, const std::basic_string<CharType> & symbols) {
        trimLeft(str, symbols);
        trimRight(str, symbols);
    }

    /*!
     * \details Trims left and right.
     * \param [in, out] str string for trimming
     */
    static void trim(std::basic_string<CharType> & str) {
        trim(str, internal::TrimSymbols<CharType>::get());
    }

    /*!
     * \details Makes string copy and trims left and right.
     * \param [in] str string for trimming.
     * \param [in] symbols the symbols that must be removed from start and end.
     * \return new trimmed string.
     */
    static std::basic_string<CharType> trimCopy(const std::basic_string<CharType> & str, const std::basic_string<CharType> & symbols) {
        std::basic_string<CharType> out(str);
        trim(out, symbols);
        return out;
    }

    /*!
     * \details Makes string copy and trims left and right.
     * \param [in] str string for trimming
     * \return new trimmed string.
     */
    static std::basic_string<CharType> trimCopy(const std::basic_string<CharType> & str) {
        return trimCopy(str, internal::TrimSymbols<CharType>::get());
    }

    // @}
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // @{

    /*!
     * \details Makes new string and replaces symbols.
     * \param [in] str string for replacing.
     * \param [in] what what symbols must be replaced.\n
     *             <b>Attention:</b> this parameter is symbols array, not a word which can be replaced
     * \param [in] to symbols which will be used for replace "what" symbols.
     * \return new string with replaces the symbols.
     */
    static std::basic_string<CharType> replaceCopy(const std::basic_string<CharType> & str,
                                                   const std::basic_string<CharType> & what,
                                                   const std::basic_string<CharType> & to) {
        std::basic_string<CharType> outStr;
        bool foundForReplace = false;
        for (auto & itThis : str) {
            for (auto itWhat : what) {
                if (itThis == itWhat) {
                    outStr.append(to);
                    foundForReplace = true;
                    break;
                }
            }

            if (foundForReplace == true) {
                foundForReplace = false;
                continue;
            }

            outStr.append(1, itThis);
        }
        return outStr;
    }

    /*!
     * \deprecated Use replaceCopy instead
     */
    [[deprecated("Use replaceCopy instead")]]
    static void replace(std::basic_string<CharType> & str,
                        const std::basic_string<CharType> & what,
                        const std::basic_string<CharType> & to) {
        str = replaceCopy(str, what, to);
    }

    /*!
     * \details Makes new string and replaces symbols.
     * \param [in] str string for replacing.
     * \param [in] what what the symbol should be replaced.
     * \param [in] to symbols which will be used for replace "what" symbol.
     * \return new string with replaces the symbols.
     */
    static std::basic_string<CharType> replaceCopy(const std::basic_string<CharType> & str,
                                                   const CharType & what,
                                                   const std::basic_string<CharType> & to) {
        std::basic_string<CharType> outStr;
        for (auto & it : str) {
            if (it == what)
                outStr.append(to);
            else
                outStr.append(1, it);
        }
        return outStr;
    }

    /*!
     * \deprecated Use replaceCopy instead
     */
    [[deprecated("Use replaceCopy instead")]]
    static void replace(std::basic_string<CharType> & str,
                        const CharType & what,
                        const std::basic_string<CharType> & to) {
        str = replaceCopy(str, what, to);
    }

    /*!
     * \details Replaces symbols.
     * \pre Use this function always when you can, because it is fast and doesn't make any copies.
     * \param [in, out] str string for replacing.
     * \param [in] what what symbol should be replaced.
     * \param [in] to symbol which will be used for replace "what" symbol.
     */
    static void replace(std::basic_string<CharType> & str,
                        const CharType what,
                        const CharType to) {

        for (auto & it : str) {
            if (it == what)
                it = to;
        }
    }

    /*!
     * \details Makes new string and replaces symbols.
     * \pre Use this function always when you can, 
     *      because it is fast and doesn't make any copies inside the algorithm.
     * \param [in] str string for replacing.
     * \param [in] what what symbol should be replaced.
     * \param [in] to symbol which will be used for replace "what" symbol.
     * \return new string with replaces the symbols.
     */
    static std::basic_string<CharType> replaceCopy(const std::basic_string<CharType> & str,
                                                   const CharType & what,
                                                   const CharType & to) {
        std::basic_string<CharType> out(str);
        replace(out, what, to);
        return out;
    }

    /*!
     * \details Replaces symbols.
     * \param [in, out] str string for char replacing.
     * \param [in] what what symbols should be replaced.\n
     *             <b>Attention:</b> this parameter is symbols array, not a word which should be replaced
     * \param [in] to symbol which will be used for replace "what" symbols.
     */
    static void replace(std::basic_string<CharType> & str,
                        const std::basic_string<CharType> & what,
                        const CharType to) {

        for (auto & it : str) {
            for (auto & itWhat : what) {
                if (it == itWhat) {
                    it = to;
                    break;
                }
            }
        }
    }

    /*!
     * \details Makes new string and replaces symbols.
     * \param [in] str string for char replacing.
     * \param [in] what what symbols should be replaced.\n
     *             <b>Attention:</b> this parameter is symbols array, not a word which should be replaced
     * \param [in] to symbol which will be used for replace "what" symbols.
     * \return new string with replaces the symbols.
     */
    static std::basic_string<CharType> replaceCopy(const std::basic_string<CharType> & str,
                                                   const std::basic_string<CharType> & what,
                                                   const CharType & to) {
        std::basic_string<CharType> out(str);
        replace(out, what, to);
        return out;
    }

    // @}
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // @{

    /*!
     * \details Check whether the string starts with another one.
     * \param [in] str
     * \param [in] with
     */
    static bool startsWith(const std::basic_string<CharType> & str,
                           const std::basic_string<CharType> & with) {
        return startsWith(str, with.c_str(), with.length());
    }

    /*!
     * \details Check whether the string starts with another one.
     * \param [in] str
     * \param [in] with
     * \param [in] withLen length of inWith string
     */
    static bool startsWith(const std::basic_string<CharType> & str,
                           const CharType * with, size_t withLen) {
        if (with == nullptr || withLen == 0) {
            return false;
        }
        if (str.length() < withLen) {
            return false;
        }
        for (size_t i = 0; i < withLen; ++i) {
            if (with[i] != str[i]) {
                return false;
            }
        }
        return true;
    }

    /*!
     * \details Check whether the string ends with another one.
     * \param [in] str
     * \param [in] with
     */
    static bool endsWith(const std::basic_string<CharType> & str,
                         const std::basic_string<CharType> & with) {
        return endsWith(str, with.c_str(), with.length());
    }

    /*!
     * \details Check whether the string ends with another one.
     * \param [in] str
     * \param [in] with
     * \param [in] withLen length of with string
     */
    static bool endsWith(const std::basic_string<CharType> & str,
                         const CharType * with, size_t withLen) {
        if (with == nullptr || withLen == 0) {
            return false;
        }
        size_t len1 = str.length();
        if (len1 < withLen) {
            return false;
        }
        for (size_t i = 0; i < withLen; ++i) {
            if (with[i] != str[i + (len1 - withLen)]) {
                return false;
            }
        }
        return true;
    }

    // @}
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // @{

    /*!
     * \details Joins values into the string.
     * \param [in] args list of arguments. You can use all the types which are supported by the stringstream.
     * \return result string
     * \todo does not work properly for char16_t and char32_t see the tests for more information.
     */
    template<typename... Args>
    static std::basic_string<CharType> join(const Args &... args) {
        std::basic_ostringstream<CharType> stream;
        int unpack[]{0, ((stream << args), 0)...};
        return stream.str();
    }

    /*!
     * \details Joins strings into one string. 
     *          This can be used instead of 'append' chain like std::string("str1").append("str2").append("str3")
     * \param [in] args list of arguments. You can use all the types which are supported by the std::basic_string::append() method.
     * \return result string
     */
    template<typename... Args>
    static std::basic_string<CharType> joinStr(const Args &... args) {
        std::basic_string<CharType> stream;
        int unpack[]{0, ((stream.append(args)), 0)...};
        return stream;
    }

    /*!
     * \details Joins values of the container into the string.
     * \param [in] c container with data.
     * \param [in] separator
     * \return result string
     * \todo does not work properly for char16_t and char32_t see the tests for more information.
     */
    template<typename Container>
    static std::basic_string<CharType> joinContainer(const Container & c, const std::basic_string<CharType> & separator) {
        std::basic_ostringstream<CharType> stream;
        for (const auto & v : c) {
            stream << v << separator;
        }
        // remove last separator
        auto res = stream.str();
        res.resize(res.length() - separator.length());
        return res;
    }

    // @}
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

typedef StringUtil<char> MbStrUtils;
typedef StringUtil<wchar_t> WStrUtils;
typedef StringUtil<char16_t> U16StrUtils;
typedef StringUtil<char32_t> U32StrUtils;

#ifdef UNICODE
typedef std::wstring Str;
typedef WStrUtils StrUtils;
#else
	typedef std::string Str;
	typedef MbStrUtils StrUtils;
#endif // UNICODE

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

inline int8_t toInt8(const std::string & str) { return int8_t(atoi(str.data())); }
inline uint8_t toUint8(const std::string & str) { return uint8_t(atoi(str.data())); }
inline int8_t toInt8(const std::wstring & str) { return int8_t(wcstol(str.data(), nullptr, 0)); }
inline uint8_t toUint8(const std::wstring & str) { return uint8_t(wcstoul(str.data(), nullptr, 0)); }

inline int16_t toInt16(const std::string & str) { return int16_t(atoi(str.data())); }
inline uint16_t toUint16(const std::string & str) { return uint16_t(atoi(str.data())); }
inline int16_t toInt16(const std::wstring & str) { return int16_t(wcstol(str.data(), nullptr, 0)); }
inline uint16_t toUint16(const std::wstring & str) { return uint16_t(wcstoul(str.data(), nullptr, 0)); }

inline int32_t toInt32(const std::string & str) { return int32_t(atoi(str.data())); }
inline uint32_t toUint32(const std::string & str) { return uint32_t(atoi(str.data())); }
inline int32_t toInt32(const std::wstring & str) { return int32_t(wcstol(str.data(), nullptr, 0)); }
inline uint32_t toUint32(const std::wstring & str) { return uint32_t(wcstoul(str.data(), nullptr, 0)); }

inline int64_t toInt64(const std::string & str) { return int64_t(atol(str.data())); }
inline uint64_t toUint64(const std::string & str) { return uint64_t(atol(str.data())); }
inline int64_t toInt64(const std::wstring & str) { return int64_t(wcstoll(str.data(), nullptr, 0)); }
inline uint64_t toUint64(const std::wstring & str) { return uint64_t(wcstoull(str.data(), nullptr, 0)); }

inline double toDouble(const std::string & str) { return double(atof(str.data())); }
inline float toFloat(const std::string & str) { return float(atof(str.data())); }
inline double toDouble(const std::wstring & str) { return double(wcstod(str.data(), nullptr)); }
inline float toFloat(const std::wstring & str) { return float(wcstof(str.data(), nullptr)); }

/****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/

UtilsExp std::wstring toWString(const std::string & str);
UtilsExp std::wstring toWString(const char * str);
UtilsExp std::string toMbString(const std::wstring & str);
UtilsExp std::string toMbString(const wchar_t * str);

//-------------------------------------------------------------------------

inline std::string & toMbString(std::string & str) { return str; }
inline std::string toMbString(const std::string & str) { return str; }
inline std::string toMbString(const char * str) { return std::string(str); }

inline std::wstring & toWString(std::wstring & str) { return str; }
inline std::wstring toWString(const std::wstring & str) { return str; }
inline std::wstring toWString(const wchar_t * str) { return std::wstring(str); }

//-------------------------------------------------------------------------

inline std::string toMbString(const int8_t & v) { return std::to_string(v); }
inline std::string toMbString(const uint8_t v) { return std::to_string(v); }
inline std::string toMbString(const int16_t v) { return std::to_string(v); }
inline std::string toMbString(const uint16_t v) { return std::to_string(v); }
inline std::string toMbString(const int32_t v) { return std::to_string(v); }
inline std::string toMbString(const uint32_t v) { return std::to_string(v); }
inline std::string toMbString(const int64_t v) { return std::to_string(v); }
inline std::string toMbString(const uint64_t v) { return std::to_string(v); }
UtilsExp std::string toMbString(const double v, uint8_t precision);
UtilsExp std::string toMbString(const long double v, uint8_t precision);
UtilsExp std::string toMbString(const float v, uint8_t precision);
inline std::string toMbString(const long double v) { return std::to_string(v); }
inline std::string toMbString(const double v) { return std::to_string(v); }
inline std::string toMbString(const float v) { return std::to_string(v); }

//-------------------------------------------------------------------------

inline std::wstring toWString(const int8_t v) { return std::to_wstring(v); }
inline std::wstring toWString(const uint8_t v) { return std::to_wstring(v); }
inline std::wstring toWString(const int16_t v) { return std::to_wstring(v); }
inline std::wstring toWString(const uint16_t v) { return std::to_wstring(v); }
inline std::wstring toWString(const int32_t v) { return std::to_wstring(v); }
inline std::wstring toWString(const uint32_t v) { return std::to_wstring(v); }
inline std::wstring toWString(const int64_t v) { return std::to_wstring(v); }
inline std::wstring toWString(const uint64_t v) { return std::to_wstring(v); }
UtilsExp std::wstring toWString(const double v, uint8_t precision);
UtilsExp std::wstring toWString(const long double v, uint8_t precision);
UtilsExp std::wstring toWString(const float v, uint8_t precision);
inline std::wstring toWString(const long double v) { return std::to_wstring(v); }
inline std::wstring toWString(const double v) { return std::to_wstring(v); }
inline std::wstring toWString(const float v) { return std::to_wstring(v); }

/****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/

//inline void toLower(std::wstring & str) { transform(str.begin(), str.end(), str.begin(), tolower); }
//inline void toLower(std::string & str) { transform(str.begin(), str.end(), str.begin(), tolower); }
//inline void toUpper(std::wstring & str) { transform(str.begin(), str.end(), str.begin(), toupper); }
//inline void toUpper(std::string & str) { transform(str.begin(), str.end(), str.begin(), toupper); }

/****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/

#ifdef UNICODE
inline std::wstring toString(const std::string & str) { return toWString(str); }
inline std::wstring & toString(std::wstring & str) { return toWString(str); }
inline std::wstring toString(const std::wstring & str) { return toWString(str); }
inline std::wstring toString(const char * str) { return toWString(str); }
inline std::wstring toString(const wchar_t * str) { return toWString(str); }

inline std::wstring toString(const int8_t v) { return toWString(v); }
inline std::wstring toString(const uint8_t v) { return toWString(v); }
inline std::wstring toString(const int16_t v) { return toWString(v); }
inline std::wstring toString(const uint16_t v) { return toWString(v); }
inline std::wstring toString(const int32_t v) { return toWString(v); }
inline std::wstring toString(const uint32_t v) { return toWString(v); }
inline std::wstring toString(const int64_t v) { return toWString(v); }
inline std::wstring toString(const uint64_t v) { return toWString(v); }
inline std::wstring toString(const long double v, uint8_t precision) { return toWString(v, precision); }
inline std::wstring toString(const double v, uint8_t precision) { return toWString(v, precision); }
inline std::wstring toString(const float v, uint8_t precision) { return toWString(v, precision); }
inline std::wstring toString(const long double v) { return toWString(v); }
inline std::wstring toString(const double v) { return toWString(v); }
inline std::wstring toString(const float v) { return toWString(v); }
#else
	inline std::string & toString(std::string & str) { return toMbString(str); }
	inline std::string toString(const std::string & str) { return toMbString(str); }
	inline std::string toString(const std::wstring & str) { return toMbString(str); }
	inline std::string toString(const char * str) { return toMbString(str); }
	inline std::string toString(const wchar_t * str) { return toMbString(str); }

	inline std::string toString(const int8_t v) { return toMbString(v); }
	inline std::string toString(const uint8_t v) { return toMbString(v); }
	inline std::string toString(const int16_t v) { return toMbString(v); }
	inline std::string toString(const uint16_t v) { return toMbString(v); }
	inline std::string toString(const int32_t v) { return toMbString(v); }
	inline std::string toString(const uint32_t v) { return toMbString(v); }
	inline std::string toString(const int64_t v) { return toMbString(v); }
	inline std::string toString(const uint64_t v) { return toMbString(v); }
	inline std::string toString(const long double v, uint8_t precision) { return toMbString(v, precision); }
	inline std::string toString(const double v, uint8_t precision) { return toMbString(v, precision); }
	inline std::string toString(const float v, uint8_t precision) { return toMbString(v, precision); }
	inline std::string toString(const long double v) { return toMbString(v); }
	inline std::string toString(const double v) { return toMbString(v); }
	inline std::string toString(const float v) { return toMbString(v); }
#endif // UNICODE

/****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/

}
