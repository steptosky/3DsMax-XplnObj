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

#include <ostream>
#include <string>
#include <stdexcept>
#include <cstdint>

namespace sts {

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Data Output Stream class provide a stream operation with data which interpreted like bytes (not characters).
	 * \pre It is the best practice to write this format version too, 
	 * use \link DataStreamO::writeFormatVersion \endlink and \link DataStreamI::readAndSetFormatVersion \endlink.
	 */
	class DataStreamO {
	public:

		explicit DataStreamO(std::ostream * inStream);
		explicit DataStreamO(std::ostream * inStream, uint8_t inVersion);
		virtual ~DataStreamO();

		//-------------------------------------------------------------------------

		std::ostream & stdStreamOutput();
		const std::ostream & stdStreamOutput() const;

		//-------------------------------------------------------------------------
		// Writing operators

		/*! 
		 * \detailes This method is useful when you want specify the type explicitly. 
		 * When it can be need? You want the compiler to show a warning if the type is not expected.
		 * \code stream.setValue<float>(double(2.0)) // warning \endcode
		 */
		template<typename Type>
		void setValue(const Type & inVal) {
			*this << inVal;
		}

		DataStreamO & operator<<(char inVal);
		DataStreamO & operator<<(int8_t inVal);
		DataStreamO & operator<<(uint8_t inVal);
		DataStreamO & operator<<(int16_t inVal);
		DataStreamO & operator<<(uint16_t inVal);
		DataStreamO & operator<<(int32_t inVal);
		DataStreamO & operator<<(uint32_t inVal);
		DataStreamO & operator<<(int64_t inVal);
		DataStreamO & operator<<(uint64_t inVal);
		DataStreamO & operator<<(bool inVal);
		DataStreamO & operator<<(float inVal);
		DataStreamO & operator<<(double inVal);
		DataStreamO & operator<<(const std::string & inVal);
		DataStreamO & operator<<(const std::wstring & inVal);
		DataStreamO & operator<<(const std::u16string & inVal);
		DataStreamO & operator<<(const std::u32string & inVal);
		DataStreamO & writeRawData(const char * inVal, uint64_t len);

		//-------------------------------------------------------------------------

		void writeFormatVersion();
		uint8_t outputFormatVersion() const;
		void setOutputFormatVersion(uint8_t inVersion);

		//-------------------------------------------------------------------------

	private:

		DataStreamO(const DataStreamO & inCopy) = delete;
		DataStreamO & operator =(const DataStreamO &) = delete;

		void _writeStringV1(const std::string & inVal);
		void _writeWStringV1(const std::wstring & inVal);
		void _writeU16StringV1(const std::u16string & inVal);
		void _writeU32StringV1(const std::u32string & inVal);

		void _writeStringV0(const std::string & inVal);
		void _writeWStringV0(const std::wstring & inVal);
		void _writeU16StringV0(const std::u16string & inVal);
		void _writeU32StringV0(const std::u32string & inVal);

		DataStreamO & _writeBytesV0(const char * inVal, uint64_t len);

		std::ostream * mStream = nullptr;
		const uint8_t mActualVersion = 1;
		uint8_t mFormatVersion = mActualVersion;

	};

	/********************************************************************************************************/
	///////////////////////////////////////* Constructors/Destructor *////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Constructor default.
	 * \note This class does not take ownership for pointer to std::iostream.
	 * \param [in] inStream reference to a std::ostream.
	 */
	inline DataStreamO::DataStreamO(std::ostream * inStream)
		:
		mStream(inStream) { }

	/*!
	 * \details Constructor init version.
	 * \note This class does not take ownership for pointer to std::iostream.
	 * \param [in] inStream
	 * \param [in] inVersion format version,
	 * for more information see \link DataStreamO::setOutputFormatVersion \endlink.
	 */
	inline DataStreamO::DataStreamO(std::ostream * inStream, uint8_t inVersion)
		:
		mStream(inStream) {
		setOutputFormatVersion(inVersion);
	}

	/*! \details Destructor. */
	inline DataStreamO::~DataStreamO() { }

	/********************************************************************************************************/
	//////////////////////////////////////////////* Operators *///////////////////////////////////////////////
	/********************************************************************************************************/

	inline DataStreamO & DataStreamO::operator <<(char inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(int8_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(uint8_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(int16_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(uint16_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(int32_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(uint32_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(int64_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(uint64_t inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(bool inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(float inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	inline DataStreamO & DataStreamO::operator <<(double inVal) {
		mStream->write(reinterpret_cast<const char *>(&inVal), sizeof(inVal));
		return *this;
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Saves utf-8 string.
	 * \note Length of one string is constrained by uint32.
	 * \exception std::exception
	 */
	inline DataStreamO & DataStreamO::operator <<(const std::string & inVal) {
		switch (mFormatVersion) {
			default: _writeStringV1(inVal);
				return *this;
			case 1: _writeStringV1(inVal);
				return *this;
			case 0: _writeStringV0(inVal);
				return *this;
		}
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Saves wide string.
	 * \note Length of one string is constrained by uint32.
	 * \warning Don,t use the std::wstring for cross-platform serialization/deserialization,
	 * because wchar_t has different size on different platforms. Use std::u16string or std::u32string instead.
	 * \exception std::exception
	 */
	inline DataStreamO & DataStreamO::operator <<(const std::wstring & inVal) {
		switch (mFormatVersion) {
			default: _writeWStringV1(inVal);
				return *this;
			case 1: _writeWStringV1(inVal);
				return *this;
			case 0: _writeWStringV0(inVal);
				return *this;
		}
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Saves utf-16 string.
	 * \note Length of one string is constrained by uint32.
	 * \exception std::exception
	 */
	inline DataStreamO & DataStreamO::operator <<(const std::u16string & inVal) {
		switch (mFormatVersion) {
			default: _writeU16StringV1(inVal);
				return *this;
			case 1: _writeU16StringV1(inVal);
				return *this;
			case 0: _writeU16StringV0(inVal);
				return *this;
		}
	}

	//-------------------------------------------------------------------------

	/*!
	 * \details Saves utf-32 string.
	 * \note Length of one string is constrained by uint32.
	 * \exception std::exception
	 */
	inline DataStreamO & DataStreamO::operator <<(const std::u32string & inVal) {
		switch (mFormatVersion) {
			default: _writeU32StringV1(inVal);
				return *this;
			case 1: _writeU32StringV1(inVal);
				return *this;
			case 0: _writeU32StringV0(inVal);
				return *this;
		}
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Writes raw bytes.
	 * \param [in] inVal Bytes for write.
	 * \param [in] len number of bytes to write.
	 */
	inline DataStreamO & DataStreamO::writeRawData(const char * inVal, uint64_t len) {
		if (!inVal || len == 0)
			return *this;
		mStream->write(inVal, len);
		return *this;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	/*!
	 * \return current format version.
	 */
	inline uint8_t DataStreamO::outputFormatVersion() const {
		return mFormatVersion;
	}

	/*!
	 * \details Writes current format version to the stream.
	 */
	inline void DataStreamO::writeFormatVersion() {
		mStream->write("@v", 2);
		mStream->write(reinterpret_cast<const char*>(&mFormatVersion), sizeof(mFormatVersion));
	}

	/*!
	 * \details Sets current format version.
	 */
	inline void DataStreamO::setOutputFormatVersion(uint8_t inVersion) {
		if (inVersion > mActualVersion) {
			throw std::runtime_error(std::string(__FUNCTION__)
									 .append(" The class does not support format version:")
									 .append(std::to_string(inVersion)));
		}
		mFormatVersion = inVersion;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	/*! \details Gives reference to internal std::ostream. */
	inline std::ostream & DataStreamO::stdStreamOutput() {
		return *mStream;
	}

	/*! \details  Gives const reference to internal std::ostream.*/
	inline const std::ostream & DataStreamO::stdStreamOutput() const {
		return *mStream;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	inline void DataStreamO::_writeStringV1(const std::string & inVal) {
		uint32_t len = static_cast<uint32_t>(inVal.size());
		writeRawData("u08", 3);
		*this << len;
		if (len != 0) {
			writeRawData(inVal.data(), len);
		}
	}

	inline void DataStreamO::_writeWStringV1(const std::wstring & inVal) {
		uint32_t len = static_cast<uint32_t>(inVal.size());
		writeRawData("wst", 3);
		*this << uint8_t(sizeof(std::wstring::value_type));
		*this << len;
		if (len != 0) {
			writeRawData(reinterpret_cast<const char*>(inVal.data()), len * sizeof(std::wstring::value_type));
		}
	}

	inline void DataStreamO::_writeU16StringV1(const std::u16string & inVal) {
		uint32_t len = static_cast<uint32_t>(inVal.size());
		writeRawData("u16", 3);
		*this << len;
		if (len != 0) {
			writeRawData(reinterpret_cast<const char*>(inVal.data()), len * sizeof(std::u16string::value_type));
		}
	}

	inline void DataStreamO::_writeU32StringV1(const std::u32string & inVal) {
		uint32_t len = static_cast<uint32_t>(inVal.size());
		writeRawData("u32", 3);
		*this << len;
		if (len != 0) {
			writeRawData(reinterpret_cast<const char*>(inVal.data()), len * sizeof(std::u32string::value_type));
		}
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	inline void DataStreamO::_writeStringV0(const std::string & inVal) {
		_writeBytesV0(inVal.data(), static_cast<uint64_t>(inVal.size()));
	}

	inline void DataStreamO::_writeWStringV0(const std::wstring & inVal) {
		_writeBytesV0(
					 reinterpret_cast<const char*>(inVal.data()), static_cast<uint64_t>(sizeof(std::wstring::value_type) * inVal.size()));
	}

	inline void DataStreamO::_writeU16StringV0(const std::u16string & inVal) {
		_writeBytesV0(
					 reinterpret_cast<const char*>(inVal.data()), static_cast<uint64_t>(sizeof(std::u16string::value_type) * inVal.size()));
	}

	inline void DataStreamO::_writeU32StringV0(const std::u32string & inVal) {
		_writeBytesV0(
					 reinterpret_cast<const char*>(inVal.data()), static_cast<uint64_t>(sizeof(std::u32string::value_type) * inVal.size()));
	}

	inline DataStreamO & DataStreamO::_writeBytesV0(const char * inVal, uint64_t len) {
		*this << len;
		if (len) {
			writeRawData(inVal, len);
		}
		return *this;
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}
