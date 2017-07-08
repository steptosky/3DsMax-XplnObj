#pragma once

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*
**  Copyright (C) 2017, StepToSky and FlightFactor
**  All rights reserved
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.The name of StepToSky or the name of FlightFactor or the names of its
**    contributors may NOT be used to endorse or promote products derived from
**    this software without specific prior written permission.
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
**  Contacts: www.steptosky.com or www.flightfactor.aero
*/

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

namespace stsff {
	namespace math {

		/**************************************************************************************************************/
		// templates

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		template<typename T>
		bool isEqual(const T left, const T right) {
			return left == right;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		template<typename T>
		bool isNotEqual(const T left, const T right) {
			return left != right;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		template<typename T>
		bool isGreater(const T left, const T right) {
			return left > right;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		template<typename T>
		bool isEqualOrGreater(const T left, const T right) {
			return left >= right;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		template<typename T>
		bool isLess(const T left, const T right) {
			return left < right;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		template<typename T>
		bool isEqualOrLess(const T left, const T right) {
			return left <= right;
		}

		/**************************************************************************************************************/
		// double

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
			return fabs(left - right) <= treshhold;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
			return !isEqual(left, right, treshhold);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
			return (left - right) > treshhold;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		inline bool isEqualOrGreater(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
			return isEqual(left, right, treshhold) || left > right;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
			return (left - right) < -treshhold;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(const double left, const double right, const double treshhold = std::numeric_limits<double>::epsilon()) {
			return isEqual(left, right, treshhold) || left < right;
		}

		/**************************************************************************************************************/
		// float

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
			return fabs(left - right) <= treshhold;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
			return !isEqual(left, right, treshhold);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
			return (left - right) > treshhold;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		inline bool isEqualOrGreater(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
			return isEqual(left, right, treshhold) || left > right;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
			return (left - right) < -treshhold;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(const float left, const float right, const float treshhold = std::numeric_limits<float>::epsilon()) {
			return isEqual(left, right, treshhold) || left < right;
		}

		/**************************************************************************************************************/
		// char / wchar_t

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(const char * left, const char * right) {
			return strcmp(left, right) == 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(char * left, char * right) {
			return strcmp(left, right) == 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(const char * left, const char * right) {
			return !isEqual(left, right);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(char * left, char * right) {
			return !isEqual(left, right);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(const char * left, const char * right) {
			return strcmp(left, right) > 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(char * left, char * right) {
			return strcmp(left, right) > 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		*/
		inline bool isEqualOrGreater(const char * left, const char * right) {
			return strcmp(left, right) >= 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		inline bool isEqualOrGreater(char * left, char * right) {
			return strcmp(left, right) >= 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(const char * left, const char * right) {
			return strcmp(left, right) < 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(char * left, char * right) {
			return strcmp(left, right) < 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(const char * left, const char * right) {
			return strcmp(left, right) <= 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(char * left, char * right) {
			return strcmp(left, right) <= 0;
		}

		//-------------------------------------------------------------------------

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(const wchar_t * left, const wchar_t * right) {
			return wcscmp(left, right) == 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(wchar_t * left, wchar_t * right) {
			return wcscmp(left, right) == 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(const wchar_t * left, const wchar_t * right) {
			return !isEqual(left, right);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(wchar_t * left, wchar_t * right) {
			return !isEqual(left, right);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(const wchar_t * left, const wchar_t * right) {
			return wcscmp(left, right) > 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(wchar_t * left, wchar_t * right) {
			return wcscmp(left, right) > 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		inline bool isEqualOrGreater(const wchar_t * left, const wchar_t * right) {
			return wcscmp(left, right) >= 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		inline bool isEqualOrGreater(wchar_t * left, wchar_t * right) {
			return wcscmp(left, right) >= 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(const wchar_t * left, const wchar_t * right) {
			return wcscmp(left, right) < 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(wchar_t * left, wchar_t * right) {
			return wcscmp(left, right) < 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(const wchar_t * left, const wchar_t * right) {
			return wcscmp(left, right) <= 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(wchar_t * left, wchar_t * right) {
			return wcscmp(left, right) <= 0;
		}

		/**************************************************************************************************************/
		// string

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(const std::string & left, const std::string & right) {
			return (left.compare(right) == 0);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(const std::string & left, const std::string & right) {
			return !isEqual(left, right);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(const std::string & left, const std::string & right) {
			return left.compare(right) > 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		inline bool isEqualOrGreater(const std::string & left, const std::string & right) {
			return (left.compare(right) >= 0);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(const std::string & left, const std::string & right) {
			return left.compare(right) < 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(const std::string & left, const std::string & right) {
			return (left.compare(right) <= 0);
		}

		//-------------------------------------------------------------------------

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled, otherwise false.
		 */
		inline bool isEqual(const std::wstring & left, const std::wstring & right) {
			return (left.compare(right) == 0);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are NOT equaled, otherwise false.
		 */
		inline bool isNotEqual(const std::wstring & left, const std::wstring & right) {
			return !isEqual(left, right);
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is greater, otherwise false.
		 */
		inline bool isGreater(const std::wstring & left, const std::wstring & right) {
			return left.compare(right) > 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the values are equaled or the first value is greater, otherwise false.
		 */
		inline bool isEqualOrGreater(const std::wstring & left, const std::wstring & right) {
			return left.compare(right) >= 0;
		}

		/*!
		 * \details Compares two values.
		 * \return True if the first value is less, otherwise false.
		 */
		inline bool isLess(const std::wstring & left, const std::wstring & right) {
			return left.compare(right) < 0;
		}

		/*!
		 * \details Compare two values.
		 * \return True if the values are equaled or the first value is less, otherwise false.
		 */
		inline bool isEqualOrLess(const std::wstring & left, const std::wstring & right) {
			return left.compare(right) <= 0;
		}

		/**************************************************************************************************************/
	}
}

/**********************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************/
