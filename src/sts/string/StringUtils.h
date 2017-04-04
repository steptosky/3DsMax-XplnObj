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

#include <stddef.h>
#include <string>
#include <vector>
#include <list>

#ifndef STS_STRING_TRIM_W
#   define STS_STRING_TRIM_W L" \t\f\v\r\n"
#endif
#ifndef STS_STRING_TRIM_A
#   define STS_STRING_TRIM_A " \t\f\v\r\n"
#endif

#ifdef UNICODE
#   define STS_STRING_TRIM STS_STRING_TRIM_W
#else
#   define STS_STRING_TRIM STS_STRING_TRIM_A
#endif // UNICODE

namespace sts {

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

#ifdef UNICODE
	typedef std::wstring Str;
#else
	typedef std::string Str;
#endif // UNICODE

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	/*!
	 * \details This is static class with utilities methods for the STL string.
	 */
	template<typename CharType>
	class StringUtils {
	public:

		//-------------------------------------------------------------------------

		typedef std::list<Str> List; //!< STL List of unicode dependent string
		typedef std::list<std::string> ListMb; //!< STL List of multibyte string (utf8)
		typedef std::list<std::wstring> ListW; //!< STL List of wide string (wchar_t)
		typedef std::vector<Str> Vector; //!< STL Vector of unicode dependent string
		typedef std::vector<std::string> VectorMb; //!< STL Vector of multibyte string (utf8)
		typedef std::vector<std::wstring> VectorW; //!< STL Vector of wide string (wchar_t)

		//-------------------------------------------------------------------------

		/*!
		 * \details Splits the string by specified symbols.
		 * \note value_type of the Container must have "sub-string" constructor (i.e type(const std::string &str, size_t pos, size_t len)).
		 * \param [in] inStr string for splitting
		 * \param [in] inSymbols the symbols which will be used as separator.
		 * \param [out] outList array for storing the result.
		*/
		template<class Container>
		static void split(const std::basic_string<CharType> & inStr, const std::basic_string<CharType> & inSymbols, Container & outList) {
			std::size_t found = inStr.find_first_of(inSymbols);
			std::size_t lastFound = 0;

			while (found != std::basic_string<CharType>::npos) {
				if (lastFound != found)
					outList.emplace_back(inStr, lastFound, found - lastFound);
				found = inStr.find_first_of(inSymbols, lastFound = found + 1);
			}

			if (lastFound != inStr.size()) // Critical end
				outList.emplace_back(inStr, lastFound, inStr.size() - lastFound);
		}

		/*!
		 * \details Splits the string by specified symbols.
		 * \param [in] inStr string for splitting
		 * \param [in] inSymbols the symbols which will be used as separator.
		 * \return array with result.
		 */
		template<class Container>
		static Container split(const std::basic_string<CharType> & inStr, const std::basic_string<CharType> & inSymbols) {
			Container out;
			split<Container>(inStr, inSymbols, out);
			return out;
		}

		//-------------------------------------------------------------------------

		/*!
		 * \details Trims left
		 * \param [in, out] inOutStr string for trimming
		 * \param [in] inDelim symbols that must be removed from start, you may use STS_STRING_TRIM macro.
		 */
		static void trimLeft(std::basic_string<CharType> & inOutStr, const std::basic_string<CharType> & inDelim) {
			inOutStr.erase(0, inOutStr.find_first_not_of(inDelim));
		}

		/*!
		 * \details Trims right
		 * \param [in, out] inOutStr string for trimming
		 * \param [in] inDelim symbols that must be removed from end, you may use STS_STRING_TRIM macro.
		 */
		static void trimRight(std::basic_string<CharType> & inOutStr, const std::basic_string<CharType> & inDelim) {
			inOutStr.erase(inOutStr.find_last_not_of(inDelim) + 1);
		}

		/*!
		 * \details Trims left and right
		 * \param [in, out] inOutStr string for trimming
		 * \param [in] inDelim symbols that must be removed from start and end, you may use STS_STRING_TRIM macro.
		 */
		static void trim(std::basic_string<CharType> & inOutStr, const std::basic_string<CharType> & inDelim) {
			trimLeft(inOutStr, inDelim);
			trimRight(inOutStr, inDelim);
		}

		//-------------------------------------------------------------------------

		/*!
		 * \details Replaces symbols.
		 * \param [in, out] inOutStr string for replacing.
		 * \param [in] inWhat what symbols must be replaced. \n
		 * (<b>Attention:</b> this parameter is symbols array, not a word which can be replaced)
		 * \param [in] inTo symbols which will be used for replace "inWhat" symbols.
		 */
		static void replace(std::basic_string<CharType> & inOutStr,
							const std::basic_string<CharType> & inWhat,
							const std::basic_string<CharType> & inTo) {

			std::basic_string<CharType> outStr;
			bool foundForReplace = false;
			for (auto & itThis : inOutStr) {
				for (auto itWhat : inWhat) {
					if (itThis == itWhat) {
						outStr.append(inTo);
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
			inOutStr = std::move(outStr);
		}

		/*!
		 * \details Replaces symbols.
		 * \param [in, out] inOutStr string for replacing.
		 * \param [in] inWhat what the symbol should be replaced.
		 * \param [in] inTo symbols which will be used for replace "inWhat" symbol.
		 */
		static void replace(std::basic_string<CharType> & inOutStr,
							const CharType inWhat,
							const std::basic_string<CharType> & inTo) {

			std::basic_string<CharType> outStr;
			for (auto & itThis : inOutStr) {
				if (itThis == inWhat)
					outStr.append(inTo);
				else
					outStr.append(1, itThis);
			}
			inOutStr = std::move(outStr);
		}

		/*!
		 * \details Replaces symbols.
		 * \pre Use this function always when you can, because it is fast and doesn't make any copies.
		 * \param [in, out] inOutStr string for replacing.
		 * \param [in] inWhat what symbol should be replaced.
		 * \param [in] inTo symbol which will be used for replace "inWhat" symbol.
		 */
		static void replace(std::basic_string<CharType> & inOutStr,
							const CharType inWhat,
							const CharType inTo) {

			for (auto & itThis : inOutStr) {
				if (itThis == inWhat)
					itThis = inTo;
			}
		}

		/*!
		 * \details Replaces symbols.
		 * \param [in, out] inOutStr string for char replacing.
		 * \param [in] inWhat what symbols should be replaced. \n
		 * (<b>Attention:</b> this parameter is symbols array, not a word which can be replaced)
		 * \param [in] inTo symbol which will be used for replace "inWhat" symbols.
		 */
		static void replace(std::basic_string<CharType> & inOutStr,
							const std::basic_string<CharType> & inWhat,
							const CharType inTo) {

			for (auto & itThis : inOutStr) {
				for (auto & itWhat : inWhat) {
					if (itThis == itWhat) {
						itThis = inTo;
						break;
					}
				}
			}
		}

		//-------------------------------------------------------------------------

	};

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	typedef StringUtils<char> MbStrUtils;
	typedef StringUtils<wchar_t> WStrUtils;

#ifdef UNICODE
	typedef WStrUtils StrUtils;
#else
	typedef MbStrUtils StrUtils;
#endif // UNICODE

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

}
