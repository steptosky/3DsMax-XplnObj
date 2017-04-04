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

#include "stsu_data_istream.h"

namespace sts_bwc { // backward compatibility

	/********************************************************************************************************/
	///////////////////////////////////////* Constructors/Destructor *////////////////////////////////////////
	/********************************************************************************************************/

	DataStreamI::DataStreamI(std::istream & inStream)
		:
		mStream(&inStream),
		mError(DataStreamI::eErrors::ok) {}

	DataStreamI::DataStreamI()
		:
		mStream(nullptr),
		mError(DataStreamI::eErrors::ok) { }

	DataStreamI::~DataStreamI() { }

	/********************************************************************************************************/
	//////////////////////////////////////////////* Operators *///////////////////////////////////////////////
	/********************************************************************************************************/

	DataStreamI & DataStreamI::operator>>(char & outVal) {
		outVal = 0;
		if (!mStream->read(&outVal, sizeof(char))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(int8_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(int8_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(uint8_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(uint8_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(int16_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(int16_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(uint16_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(uint16_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(int32_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(int32_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(uint32_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(uint32_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(int64_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(int64_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(uint64_t & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(uint64_t))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(bool & outVal) {
		outVal = 0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(bool))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(float & outVal) {
		outVal = 0.0f;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(float))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(double & outVal) {
		outVal = 0.0;
		if (!mStream->read(reinterpret_cast<char *>(&outVal), sizeof(double))) {
			outVal = 0;
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(char *& outVal) {
		uint64_t len = 0;
		return readBytes(outVal, len);
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(wchar_t *& outVal) {
		uint64_t len = 0;
		return readBytes(outVal, len);
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(std::string & outVal) {
		uint64_t len;
		char * str;
		readBytes(str, len);
		outVal = std::string(str, len);
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::operator>>(std::wstring & outVal) {
		uint64_t len;
		char * str;
		readBytes(str, len);
		outVal = std::wstring(reinterpret_cast<wchar_t*>(str), len / sizeof(wchar_t));
		return *this;
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
	/********************************************************************************************************/

	DataStreamI & DataStreamI::readBytes(wchar_t *& outVal, uint64_t & outLen) {
		outVal = nullptr;
		outLen = 0;

		uint64_t byteCount;
		*this >> byteCount;
		if (byteCount == 0) {
			mError = eErrors::readZerroBytes;
			return *this;
		}
		uint64_t len = byteCount / sizeof(wchar_t);
		outVal = new wchar_t[len + 1];
		if (!mStream->read(reinterpret_cast<char *>(outVal), byteCount)) {
			mError = eErrors::readError;
			delete[] outVal;
			return *this;
		}
		outVal[len] = L'\0';
		outLen = byteCount;
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::readBytes(char *& outVal, uint64_t & outLen) {
		outVal = nullptr;
		outLen = 0;

		uint64_t len;
		*this >> len;
		if (len == 0) {
			mError = eErrors::readZerroBytes;
			return *this;
		}
		outVal = new char[len + 1];
		if (!mStream->read(outVal, len)) {
			mError = eErrors::readError;
			delete[] outVal;
			return *this;
		}
		outVal[len] = '\0';
		outLen = len;
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::readRawData(char * outVal, uint64_t inLen) {
		if (!mStream->read(outVal, inLen)) {
			mError = eErrors::readError;
		}
		return *this;
	}

	//-------------------------------------------------------------------------

	DataStreamI & DataStreamI::skipRawBytes(uint64_t inLen) {
		if (!mStream->seekg(inLen, mStream->cur)) {
			mError = eErrors::seekError;
		}
		return *this;
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}// namespace sts
