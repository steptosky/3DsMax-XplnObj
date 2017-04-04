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

#include <string>
#include <cstring>
#include <wchar.h>
#include <cmath>
#include <cstdint>
#include <limits>

/**********************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************/

namespace sts {

	/**********************************************************************************************************************/
	// This overloaded methods most important for templates. ///////////////////////////////////////////////////////////////
	/**********************************************************************************************************************/
	// templates

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	template<typename T>
	bool isEqual(const T left, const T right) {
		return left == right;
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	template<typename T>
	bool isNotEqual(const T left, const T right) {
		return left != right;
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	template<typename T>
	bool isEqualOrGreater(const T left, const T right) {
		return left >= right;
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	template<typename T>
	[[deprecated("Use isEqualOrGreater instead")]]
	bool isEqualOrMore(const T left, const T right) {
		return isEqualOrGreater(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	template<typename T>
	bool isEqualOrLess(const T left, const T right) {
		return left <= right;
	}

	/***************************************************************************************/
	// double

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
		return (fabs(left - right) <= treshhold);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
		return !isEqual(left, right, treshhold);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	inline bool isEqualOrGreater(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
		return (isEqual(left, right) || (left - right) > treshhold);
	}

	/*!
	 * \deprecated Use Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
		return isEqualOrGreater(left, right, treshhold);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
		return (isEqual(left, right) || (left - right) < -treshhold);
	}

	/**********************************************************************************************************************/
	// float

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
		return (fabs(left - right) <= treshhold);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
		return !isEqual(left, right, treshhold);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	inline bool isEqualOrGreater(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
		return (isEqual(left, right) || (left - right) > treshhold);
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
		return isEqualOrGreater(left, right, treshhold);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
		return (isEqual(left, right) || (left - right) < -treshhold);
	}

	/**********************************************************************************************************************/
	// char / wchar_t

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(const char * left, const char * right) {
		return (strcmp(left, right) == 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(char * left, char * right) {
		return (strcmp(left, right) == 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(const char * left, const char * right) {
		return !isEqual(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(char * left, char * right) {
		return !isEqual(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	*/
	inline bool isEqualOrGreater(const char * left, const char * right) {
		return (strcmp(left, right) >= 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	inline bool isEqualOrGreater(char * left, char * right) {
		return (strcmp(left, right) >= 0);
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(const char * left, const char * right) {
		return isEqualOrGreater(left, right);
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(char * left, char * right) {
		return isEqualOrGreater(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(const char * left, const char * right) {
		return (strcmp(left, right) <= 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(char * left, char * right) {
		return (strcmp(left, right) <= 0);
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(const wchar_t * left, const wchar_t * right) {
		return (wcscmp(left, right) == 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(wchar_t * left, wchar_t * right) {
		return (wcscmp(left, right) == 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(const wchar_t * left, const wchar_t * right) {
		return !isEqual(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(wchar_t * left, wchar_t * right) {
		return !isEqual(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	inline bool isEqualOrGreater(const wchar_t * left, const wchar_t * right) {
		return (wcscmp(left, right) >= 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	inline bool isEqualOrGreater(wchar_t * left, wchar_t * right) {
		return (wcscmp(left, right) >= 0);
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(const wchar_t * left, const wchar_t * right) {
		return isEqualOrGreater(left, right);
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(wchar_t * left, wchar_t * right) {
		return isEqualOrGreater(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(const wchar_t * left, const wchar_t * right) {
		return (wcscmp(left, right) <= 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(wchar_t * left, wchar_t * right) {
		return (wcscmp(left, right) <= 0);
	}

	/**********************************************************************************************************************/
	// string

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(const std::string & left, const std::string & right) {
		return (left.compare(right) == 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(const std::string & left, const std::string & right) {
		return !isEqual(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	inline bool isEqualOrGreater(const std::string & left, const std::string & right) {
		return (left.compare(right) >= 0);
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(const std::string & left, const std::string & right) {
		return isEqualOrGreater(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(const std::string & left, const std::string & right) {
		return (left.compare(right) <= 0);
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled, otherwise false.
	 */
	inline bool isEqual(const std::wstring & left, const std::wstring & right) {
		return (left.compare(right) == 0);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are NOT equaled, otherwise false.
	 */
	inline bool isNotEqual(const std::wstring & left, const std::wstring & right) {
		return !isEqual(left, right);
	}

	/*!
	 * \details Compares two values.
	 * \return True if values are equaled or first value greater, otherwise false.
	 */
	inline bool isEqualOrGreater(const std::wstring & left, const std::wstring & right) {
		return (left.compare(right) >= 0);
	}

	/*!
	 * \deprecated Use isEqualOrGreater instead
	 */
	[[deprecated("Use isEqualOrGreater instead")]]
	inline bool isEqualOrMore(const std::wstring & left, const std::wstring & right) {
		return isEqualOrGreater(left, right);
	}

	/*!
	 * \details Compare two values.
	 * \return True if values are equaled or first value less, otherwise false.
	 */
	inline bool isEqualOrLess(const std::wstring & left, const std::wstring & right) {
		return (left.compare(right) <= 0);
	}

	/**********************************************************************************************************************/
}

/**********************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************/

/**********************************************************************************************************************/
////////////////////////////////////////// ALL NEXT IS DEPRECATED //////////////////////////////////////////////////////
/**********************************************************************************************************************/
#include <cfloat>

#ifndef STS_DOUBLE_THRESHOLD
//#define STS_DOUBLE_THRESHOLD	0.00000000000000005		//!< This is threshold for comparison double data.
#define STS_DOUBLE_THRESHOLD	DBL_EPSILON		//!< This is threshold for comparison double data.
#endif

#ifndef STS_FLOAT_THRESHOLD
//#define STS_FLOAT_THRESHOLD		0.000001f				//!< This is threshold for comparison float data.
#define STS_FLOAT_THRESHOLD		FLT_EPSILON				//!< This is threshold for comparison float data.
#endif

/**********************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************/

namespace sts {

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const bool left, const bool right) { return left == right; }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const uint8_t left, const uint8_t right) { return left == right; }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const int8_t left, const int8_t right) { return left == right; }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const uint16_t left, const uint16_t right) { return left == right; }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const int16_t left, const int16_t right) { return left == right; }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const uint32_t left, const uint32_t right) { return left == right; }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const int32_t left, const int32_t right) { return left == right; }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const uint64_t left, const uint64_t right) { return left == right; }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const int64_t left, const int64_t right) { return left == right; }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const double & left, const double & right, const double & treshhold = STS_DOUBLE_THRESHOLD) { return !(fabs(left - right) > treshhold); }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const float & left, const float & right, const float & treshhold = STS_FLOAT_THRESHOLD) { return !(fabs(left - right) > treshhold); }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const char * left, const char * right) { return (!strcmp(left, right)); }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const wchar_t * left, const wchar_t * right) { return (!wcscmp(left, right)); }

	//---------------------------------------------------------------------------------------

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const std::string & left, const std::string & right) { return (!left.compare(right)); }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const std::string * left, const std::string * right) { return (!left->compare(*right)); }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const std::wstring & left, const std::wstring & right) { return (!left.compare(right)); }

	[[deprecated("Use family of sts::isEqual")]]
	inline bool compare(const std::wstring * left, const std::wstring * right) { return (!left->compare(*right)); }

}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
