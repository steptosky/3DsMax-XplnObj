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

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#include <cstring>
#include <string>
#include <stdint.h>

#ifndef DOUBLE_PRECISION_PRINT
#define DOUBLE_PRECISION_PRINT 8
#endif

#ifndef FLOAT_PRECISION_PRINT
#define FLOAT_PRECISION_PRINT 6
#endif

#include <assert.h>
#include <algorithm>

namespace sts {

	/****************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************************************************************************/

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

	static std::wstring toWString(const std::string & str);
	static std::wstring toWString(const char * str);
	static std::string toMbString(const std::wstring & str);
	static std::string toMbString(const wchar_t * str);

	[[deprecated]] inline std::string toAString(const std::wstring & str) { return toMbString(str); }
	[[deprecated]] inline std::string toAString(const wchar_t * str) { return toMbString(str); }

	//-------------------------------------------------------------------------

	inline std::string & toMbString(std::string & str) { return str; }
	inline std::string toMbString(const std::string & str) { return str; }
	inline std::string toMbString(const char * str) { return std::string(str); }

	[[deprecated]] inline std::string & toAString(std::string & str) { return str; }
	[[deprecated]] inline std::string toAString(const std::string & str) { return str; }
	[[deprecated]] inline std::string toAString(const char * str) { return std::string(str); }

	inline std::wstring & toWString(std::wstring & str) { return str; }
	inline std::wstring toWString(const std::wstring & str) { return str; }
	inline std::wstring toWString(const wchar_t * str) { return std::wstring(str); }

	//-------------------------------------------------------------------------

	inline std::string toMbString(const int8_t & v) { return std::to_string(v); }
	inline std::string toMbString(const uint8_t & v) { return std::to_string(v); }
	inline std::string toMbString(const int16_t & v) { return std::to_string(v); }
	inline std::string toMbString(const uint16_t & v) { return std::to_string(v); }
	inline std::string toMbString(const int32_t & v) { return std::to_string(v); }
	inline std::string toMbString(const uint32_t & v) { return std::to_string(v); }
	inline std::string toMbString(const int64_t & v) { return std::to_string(v); }
	inline std::string toMbString(const uint64_t & v) { return std::to_string(v); }
	static std::string toMbString(const double & v, uint8_t precision);
	static std::string toMbString(const long double & v, uint8_t precision);
	static std::string toMbString(const float & v, uint8_t precision);
	inline std::string toMbString(const long double & v) { return std::to_string(v); }
	inline std::string toMbString(const double & v) { return std::to_string(v); }
	inline std::string toMbString(const float & v) { return std::to_string(v); }

	[[deprecated]] inline std::string toAString(const int8_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const uint8_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const int16_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const uint16_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const int32_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const uint32_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const int64_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const uint64_t & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const double & v, uint8_t precision) { return toMbString(v, precision); }
	[[deprecated]] inline std::string toAString(const float & v, uint8_t precision) { return toMbString(v, precision); }
	[[deprecated]] inline std::string toAString(const double & v) { return std::to_string(v); }
	[[deprecated]] inline std::string toAString(const float & v) { return std::to_string(v); }

	//-------------------------------------------------------------------------

	inline std::wstring toWString(const int8_t & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const uint8_t & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const int16_t & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const uint16_t & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const int32_t & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const uint32_t & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const int64_t & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const uint64_t & v) { return std::to_wstring(v); }
	static std::wstring toWString(const double & v, uint8_t precision);
	static std::wstring toWString(const long double & v, uint8_t precision);
	static std::wstring toWString(const float & v, uint8_t precision);
	inline std::wstring toWString(const long double & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const double & v) { return std::to_wstring(v); }
	inline std::wstring toWString(const float & v) { return std::to_wstring(v); }

	/****************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************************************************************************/

	inline void toLower(std::wstring & str) { std::transform(str.begin(), str.end(), str.begin(), tolower); }
	inline void toLower(std::string & str) { std::transform(str.begin(), str.end(), str.begin(), tolower); }
	inline void toUpper(std::wstring & str) { std::transform(str.begin(), str.end(), str.begin(), toupper); }
	inline void toUpper(std::string & str) { std::transform(str.begin(), str.end(), str.begin(), toupper); }

	/****************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************************************************************************/

#ifdef UNICODE
	inline std::wstring toString(const std::string & str) { return toWString(str); }
	inline std::wstring & toString(std::wstring & str) { return toWString(str); }
	inline std::wstring toString(const std::wstring & str) { return toWString(str); }
	inline std::wstring toString(const char * str) { return toWString(str); }
	inline std::wstring toString(const wchar_t * str) { return toWString(str); }

	inline std::wstring toString(const int8_t & v) { return toWString(v); }
	inline std::wstring toString(const uint8_t & v) { return toWString(v); }
	inline std::wstring toString(const int16_t & v) { return toWString(v); }
	inline std::wstring toString(const uint16_t & v) { return toWString(v); }
	inline std::wstring toString(const int32_t & v) { return toWString(v); }
	inline std::wstring toString(const uint32_t & v) { return toWString(v); }
	inline std::wstring toString(const int64_t & v) { return toWString(v); }
	inline std::wstring toString(const uint64_t & v) { return toWString(v); }
	inline std::wstring toString(const long double & v, uint8_t precision) { return toWString(v, precision); }
	inline std::wstring toString(const double & v, uint8_t precision) { return toWString(v, precision); }
	inline std::wstring toString(const float & v, uint8_t precision) { return toWString(v, precision); }
	inline std::wstring toString(const long double & v) { return toWString(v); }
	inline std::wstring toString(const double & v) { return toWString(v); }
	inline std::wstring toString(const float & v) { return toWString(v); }
#else
	inline std::string & toString(std::string & str) { return toMbString(str); }
	inline std::string toString(const std::string & str) { return toMbString(str); }
	inline std::string toString(const std::wstring & str) { return toMbString(str); }
	inline std::string toString(const char * str) { return toMbString(str); }
	inline std::string toString(const wchar_t * str) { return toMbString(str); }

	inline std::string toString(const int8_t & v) { return toMbString(v); }
	inline std::string toString(const uint8_t & v) { return toMbString(v); }
	inline std::string toString(const int16_t & v) { return toMbString(v); }
	inline std::string toString(const uint16_t & v) { return toMbString(v); }
	inline std::string toString(const int32_t & v) { return toMbString(v); }
	inline std::string toString(const uint32_t & v) { return toMbString(v); }
	inline std::string toString(const int64_t & v) { return toMbString(v); }
	inline std::string toString(const uint64_t & v) { return toMbString(v); }
	inline std::string toString(const long double & v, uint8_t precision) { return toMbString(v, precision); }
	inline std::string toString(const double & v, uint8_t precision) { return toMbString(v, precision); }
	inline std::string toString(const float & v, uint8_t precision) { return toMbString(v, precision); }
	inline std::string toString(const long double & v) { return toMbString(v); }
	inline std::string toString(const double & v) { return toMbString(v); }
	inline std::string toString(const float & v) { return toMbString(v); }
#endif // UNICODE

	/****************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************************************************************************/

#include "_inl_/StringConverters.inl"

}
