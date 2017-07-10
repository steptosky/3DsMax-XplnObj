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

#include <istream>
#include <string>
#include <cstdint>
#include <stdexcept>

namespace sts {

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Data Input Stream class provide a stream operation with data which interpreted like bytes (not characters).
	 * Used std::istream like internal byte buffer.
	 */
	class DataStreamI {
	public:

		explicit DataStreamI(std::istream * inStream);
		explicit DataStreamI(std::istream * inStream, uint8_t inVersion);
		virtual ~DataStreamI();

		//-------------------------------------------------------------------------

		std::istream & stdStreamInput();
		const std::istream & stdStreamInput() const;

		//-------------------------------------------------------------------------
		// Reading operators

		/*!
		 * \detailes This method is useful when you want specify the type explicitly.
		 * When it can be need? You want the compiler to show a warning if the type is not expected.
		 * \code double v;  stream.value<float>(v) // warning \endcode
		 */
		template<typename Type>
		void value(Type & outVal) {
			*this >> outVal;
		}

		/*!
		 * \detailes This method is useful when you want specify the type explicitly.
		 * When it can be need? You want the compiler to show a warning if the type is not expected.
		 * \code double v = stream.value<float>() // warning \endcode
		 */
		template<typename Type>
		Type value() {
			Type v;
			value(v);
			return v;
		}

		DataStreamI & operator>>(char & outVal);
		DataStreamI & operator>>(int8_t & outVal);
		DataStreamI & operator>>(uint8_t & outVal);
		DataStreamI & operator>>(int16_t & outVal);
		DataStreamI & operator>>(uint16_t & outVal);
		DataStreamI & operator>>(int32_t & outVal);
		DataStreamI & operator>>(uint32_t & outVal);
		DataStreamI & operator>>(int64_t & outVal);
		DataStreamI & operator>>(uint64_t & outVal);
		DataStreamI & operator>>(bool & outVal);
		DataStreamI & operator>>(float & outVal);
		DataStreamI & operator>>(double & outVal);
		DataStreamI & operator>>(std::string & outVal);
		DataStreamI & operator>>(std::wstring & outVal);
		DataStreamI & operator>>(std::u16string & outVal);
		DataStreamI & operator>>(std::u32string & outVal);
		DataStreamI & readRawData(char * outVal, uint64_t inLen);
		DataStreamI & skipRawBytes(uint64_t inLen);

		//-------------------------------------------------------------------------

		uint8_t inputFormatVersion() const;
		void setInputFormatVersion(uint8_t inVersion);
		uint8_t readFormatVersion();
		void readAndSetFormatVersion();

		//-------------------------------------------------------------------------

	private:

		DataStreamI(const DataStreamI & inCopy) = delete;
		DataStreamI & operator =(const DataStreamI &) = delete;

		std::istream * mStream = nullptr;
		const uint8_t mActualVersion = 1;
		uint8_t mFormatVersion = mActualVersion;

		void _readStringV1(std::string & outVal);
		void _readWStringV1(std::wstring & outVal);
		void _readU16StringV1(std::u16string & outVal);
		void _readU32StringV1(std::u32string & outVal);

		void _readStringV0(std::string & outVal);
		void _readWStringV0(std::wstring & outVal);
		void _readU16StringV0(std::u16string & outVal);
		void _readU32StringV0(std::u32string & outVal);

	};

	/********************************************************************************************************/
	///////////////////////////////////////* Constructors/Destructor *////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Constructor default.
	 * \note This class does not take ownership for pointer to std::iostream.
	 * \param [in] inStream reference to a std::istream.
	 */
	inline DataStreamI::DataStreamI(std::istream * inStream)
		: mStream(inStream) {}

	/*!
	 * \details Constructor init version.
	 * \note This class does not take ownership for pointer to std::iostream.
	 * \param [in] inStream
	 * \param [in] inVersion format version,
	 * for more information see \link DataStreamI::readAndSetFormatVersion \endlink.
	 */
	inline DataStreamI::DataStreamI(std::istream * inStream, uint8_t inVersion)
		:
		mStream(inStream) {
		setInputFormatVersion(inVersion);
	}

	/*! \details  Destructor. */
	inline DataStreamI::~DataStreamI() { }

	/********************************************************************************************************/
	//////////////////////////////////////////////* Operators *///////////////////////////////////////////////
	/********************************************************************************************************/

	inline DataStreamI & DataStreamI::operator >>(char & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(int8_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(uint8_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(int16_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(uint16_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(int32_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(uint32_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(int64_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(uint64_t & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(bool & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(float & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamI & DataStreamI::operator >>(double & outVal) {
		mStream->read(reinterpret_cast<char *>(&outVal), sizeof(outVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Retrievals utf-8 string.
	 * \exception std::exception
	 */
	inline DataStreamI & DataStreamI::operator >>(std::string & outVal) {
		switch (mFormatVersion) {
			default: _readStringV1(outVal);
				return *this;
			case 1: _readStringV1(outVal);
				return *this;
			case 0: _readStringV0(outVal);
				return *this;
		}
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Retrievals wide string.
	 * \warning Don't use the std::wstring for cross-platform serialization/deserialization,
	 * because wchar_t has different size on different platforms, use std::u16string or std::u32string instead.
	 * \exception std::exception
	 */
	inline DataStreamI & DataStreamI::operator >>(std::wstring & outVal) {
		switch (mFormatVersion) {
			default: _readWStringV1(outVal);
				return *this;
			case 1: _readWStringV1(outVal);
				return *this;
			case 0: _readWStringV0(outVal);
				return *this;
		}
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Retrievals utf-16 string.
	 * \exception std::exception
	 */
	inline DataStreamI & DataStreamI::operator >>(std::u16string & outVal) {
		switch (mFormatVersion) {
			default: _readU16StringV1(outVal);
				return *this;
			case 1: _readU16StringV1(outVal);
				return *this;
			case 0: _readU16StringV0(outVal);
				return *this;
		}
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Retrievals utf-32 string.
	 * \exception std::exception
	 */
	inline DataStreamI & DataStreamI::operator >>(std::u32string & outVal) {
		switch (mFormatVersion) {
			default: _readU32StringV1(outVal);
				return *this;
			case 1: _readU32StringV1(outVal);
				return *this;
			case 0: _readU32StringV0(outVal);
				return *this;
		}
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Reads raw bytes.
	 * \param [out] outVal buffer for read bytes.
	 * \param [in] inLen number of bytes to read.
	 */
	inline DataStreamI & DataStreamI::readRawData(char * outVal, uint64_t inLen) {
		if (inLen != 0) {
			mStream->read(outVal, inLen);
		}
		return *this;
	}

	/*!
	 * \details Skip raw bytes.
	 * \param [in] inLen number of bytes to skip.
	 * \exception std::exception
	 */
	inline DataStreamI & DataStreamI::skipRawBytes(uint64_t inLen) {
		mStream->seekg(inLen, mStream->cur);
		return *this;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	/*!
	 * \return current format version.
	 */
	inline uint8_t DataStreamI::inputFormatVersion() const {
		return mFormatVersion;
	}

	/*!
	 * \details Sets current format version.
	 * \param [in] inVersion
	 * \exception std::exception
	 */
	inline void DataStreamI::setInputFormatVersion(uint8_t inVersion) {
		if (inVersion > mActualVersion) {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" The class does not support format version :")
									 .append(std::to_string(inVersion)));
		}
		mFormatVersion = inVersion;
	}

	/*!
	 * \details Reads format version from the stream.
	 * \exception std::exception
	 */
	inline uint8_t DataStreamI::readFormatVersion() {
		char id[2];
		mStream->read(id, 2);
		if (id[0] != '@' || id[1] != 'v') {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" Incorrect version marker, expected:@v got:")
									 .append(std::string(id, 2)));
		}
		uint8_t version = 0;
		mStream->read(reinterpret_cast<char*>(&version), sizeof(version));
		return version;
	}

	/*!
	 * \details Reads and sets format version from the stream.
	 * \exception std::exception
	 */
	inline void DataStreamI::readAndSetFormatVersion() {
		setInputFormatVersion(readFormatVersion());
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	/*! \details Gives reference to internal std::istream. */
	inline std::istream & DataStreamI::stdStreamInput() {
		return *mStream;
	}

	/*! \details  Gives const reference to internal std::istream */
	inline const std::istream & DataStreamI::stdStreamInput() const {
		return *mStream;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	inline void DataStreamI::_readStringV1(std::string & outVal) {
		char mark[3];
		readRawData(mark, sizeof(mark));
		std::string marker(mark, sizeof(mark));
		if (marker[0] != 'u' || marker[1] != '0' || marker[2] != '8') {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" Incorrect string marker, expected:u08 got:")
									 .append(marker));
		}
		uint32_t len;
		*this >> len;
		if (len != 0) {
			char * str = new char[len];
			readRawData(str, len);
			outVal.resize(len);
			outVal.replace(0, len, str, len);
			delete[]str;
		}
		else {
			outVal.clear();
		}
	}

	inline void DataStreamI::_readWStringV1(std::wstring & outVal) {
		char mark[3];
		readRawData(mark, sizeof(mark));
		std::string marker(mark, sizeof(mark));
		if (marker[0] != 'w' || marker[1] != 's' || marker[2] != 't') {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" Incorrect string marker, expected:wst got:")
									 .append(marker));
		}
		uint32_t len = 0;
		uint8_t typeSize = 0;
		*this >> typeSize;
		if (typeSize != uint8_t(sizeof(std::wstring::value_type))) {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" Incorrect std::wstring type size, expected:")
									 .append(std::to_string(sizeof(std::wstring::value_type)))
									 .append(" bytes got:")
									 .append(std::to_string(typeSize))
									 .append(" bytes"));
		}
		*this >> len;
		if (len != 0) {
			std::wstring::value_type * str = new std::wstring::value_type[len];
			readRawData(reinterpret_cast<char*>(str), len * sizeof(std::wstring::value_type));
			outVal.resize(len);
			outVal.replace(0, len, str, len);
			delete[]str;
		}
		else {
			outVal.clear();
		}
	}

	inline void DataStreamI::_readU16StringV1(std::u16string & outVal) {
		char mark[3];
		readRawData(mark, sizeof(mark));
		std::string marker(mark, sizeof(mark));
		if (marker[0] != 'u' || marker[1] != '1' || marker[2] != '6') {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" Incorrect string marker, expected:u16 got:")
									 .append(marker));
		}
		uint32_t len;
		*this >> len;
		if (len != 0) {
			std::u16string::value_type * str = new std::u16string::value_type[len];
			readRawData(reinterpret_cast<char*>(str), len * sizeof(std::u16string::value_type));
			outVal.resize(len);
			outVal.replace(0, len, str, len);
			delete[]str;
		}
		else {
			outVal.clear();
		}
	}

	inline void DataStreamI::_readU32StringV1(std::u32string & outVal) {
		char mark[3];
		readRawData(mark, sizeof(mark));
		std::string marker(mark, sizeof(mark));
		if (marker[0] != 'u' || marker[1] != '3' || marker[2] != '2') {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" Incorrect string marker, expected:u32 got:")
									 .append(marker));
		}
		uint32_t len;
		*this >> len;
		if (len != 0) {
			std::u32string::value_type * str = new std::u32string::value_type[len];
			readRawData(reinterpret_cast<char*>(str), len * sizeof(std::u32string::value_type));
			outVal.resize(len);
			outVal.replace(0, len, str, len);
			delete[]str;
		}
		else {
			outVal.clear();
		}
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
	/********************************************************************************************************/

	inline void DataStreamI::_readStringV0(std::string & outVal) {
		uint64_t len;
		*this >> len;
		if (len > 10485760) {
			throw std::runtime_error(std::string(__FUNCTION__).append(" String size is too large"));
		}
		if (len != 0) {
			char * val = new char[len];
			mStream->read(val, len);
			outVal = std::string(val, len);
			delete[] val;
		}
	}

	inline void DataStreamI::_readWStringV0(std::wstring & outVal) {
		std::string str;
		_readStringV0(str);
		outVal = std::wstring(reinterpret_cast<const std::wstring::value_type*>(str.data()), str.size() / sizeof(std::wstring::value_type));
	}

	inline void DataStreamI::_readU16StringV0(std::u16string & outVal) {
		std::string str;
		_readStringV0(str);
		outVal = std::u16string(reinterpret_cast<const std::u16string::value_type*>(str.data()), str.size() / sizeof(std::u16string::value_type));
	}

	inline void DataStreamI::_readU32StringV0(std::u32string & outVal) {
		std::string str;
		_readStringV0(str);
		outVal = std::u32string(reinterpret_cast<const std::u32string::value_type*>(str.data()), str.size() / sizeof(std::u32string::value_type));
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}
