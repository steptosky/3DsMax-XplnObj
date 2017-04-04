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
#include <map>
#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <float.h>
#include <limits>
#include "sts/io/DataStream.h"
#include <iostream>

#ifndef TOTEXT
#define TOTEXT(setX) #setX
#endif

namespace sts {

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	/*!
	 * \details Implementation of the application settings, based on key/value paradigm.
	 * Supports 2 formats, text (.ini) and its own binary format which also can be stored/restored with std::stream.
	 */
	class Settings {
	public:

		typedef std::vector<std::string> keyList;

		enum eFormats {
			txt,
			bin,
		};

		//-------------------------------------------------------

		explicit Settings(eFormats inFormat = txt);
		Settings(const Settings & inCopy);
		Settings(Settings && inMove);
		virtual ~Settings();

		//-------------------------------------------------------

		Settings & operator =(const Settings & inCopy);
		Settings & operator =(Settings && inMove);

		//-------------------------------------------------------

		bool hasGroup(const std::string & inGroupName, bool inFullTreePath = false) const;
		void beginGroup(const std::string & inGroupName);
		void endGroup();
		std::string actualGroup(bool inFullTreePath = false) const;

		//-------------------------------------------------------

		uint32_t beginReadArray(const std::string & inName);
		void beginWriteArray(const std::string & inName, int32_t inSize = -1);
		void arrayIndex(uint32_t inIndex);
		void endArray();

		//-------------------------------------------------------

		std::string value(const std::string & inKey, const char * inDefault) const;
		std::string value(const std::string & inKey, const std::string & inDefault) const;
		bool value(const std::string & inKey, bool inDefault) const;
		int8_t value(const std::string & inKey, int8_t inDefault) const;
		uint8_t value(const std::string & inKey, uint8_t inDefault) const;
		int16_t value(const std::string & inKey, int16_t inDefault) const;
		uint16_t value(const std::string & inKey, uint16_t inDefault) const;
		int32_t value(const std::string & inKey, int32_t inDefault) const;
		uint32_t value(const std::string & inKey, uint32_t inDefault) const;
		int64_t value(const std::string & inKey, int64_t inDefault) const;
		uint64_t value(const std::string & inKey, uint64_t inDefault) const;
		float value(const std::string & inKey, float inDefault) const;
		double value(const std::string & inKey, double inDefault) const;

		//-------------------------------------------------------

		void setValue(const std::string & inKey, const char * inValue);
		void setValue(const std::string & inKey, const std::string & inValue);
		void setValue(const std::string & inKey, bool inValue);
		void setValue(const std::string & inKey, int8_t inValue);
		void setValue(const std::string & inKey, uint8_t inValue);
		void setValue(const std::string & inKey, int16_t inValue);
		void setValue(const std::string & inKey, uint16_t inValue);
		void setValue(const std::string & inKey, int32_t inValue);
		void setValue(const std::string & inKey, uint32_t inValue);
		void setValue(const std::string & inKey, int64_t inValue);
		void setValue(const std::string & inKey, uint64_t inValue);
		void setValue(const std::string & inKey, float inValue);
		void setValue(const std::string & inKey, double inValue);

		//-------------------------------------------------------

		keyList keysList() const;
		bool containsKey(const std::string & inKey) const;
		bool remove(const std::string & inKey);
		void clear();

		eFormats format() const;

		//-------------------------------------------------------

		bool load(const std::string & inFileName);
		bool save(const std::string & inFileName) const;

		bool load(std::istream & inStream);
		bool save(std::ostream & outStream) const;

		/*!
		 * \exception std::runtime_error
		 */
		friend std::istream & operator >>(std::istream & inStream, Settings & outSettings);
		friend std::ostream & operator <<(std::ostream & outStream, const Settings & inSettings);

		//-------------------------------------------------------

	private:

		void _init();
		const std::string & _value(const std::string & inKey) const;
		std::string _checkLineForGroup(const std::string & inLine) const;

		//-------------------------------------------------------

		bool _loadBin(const std::string & inFileName);
		bool _saveBin(const std::string & inFileName) const;

		bool _loadTxt(const std::string & inFileName);
		bool _saveTxt(const std::string & inFileName) const;

		//-------------------------------------------------------

		template<typename T>
		void _setIntVlalue(const std::string & inKey, const T & inVal);

		//-------------------------------------------------------

		typedef std::map<std::string, std::string> Values;
		typedef std::map<std::string, Values> Categories;

		eFormats mFormat;

		// tree
		std::string mActualTreePath;
		Categories * mRoot;
		Values * mActualValues;

		// arrays
		uint32_t mCurrArrayIndex;
		int32_t mMaxArrayIndex;
		uint32_t mCurrArraySize;
		std::string mCurrArrayName;
		bool mIsReadArrayOperationInProgress;
		bool mIsWriteArrayOperationInProgress;
		bool mIsNeedWriteArraySize;

		void _arraysOperationsReset();

		// bin io
		const unsigned char ioVersion() const;
		const unsigned ioIdSize() const;
		const char * ioId() const;

	};

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

#include "Settings.inl"

}
