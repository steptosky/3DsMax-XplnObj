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

#include <iostream>
#include <stdint.h>
#include <cstring>

namespace sts_bwc { // backward compatibility

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	\details Data Output Stream class provide a stream operation with data which interpreted like bytes (not characters, e.t.c.).
	         Used std::iostream like internal byte buffer.
	*/
	class DataStreamO {
	protected:

		explicit DataStreamO();
		void setStream(std::ostream * inStream) { mStream = inStream; }

	public:

		/*!
		\details Constructor
		\param [in] inStream reference to a std::ostream.
		*/
		explicit DataStreamO(std::ostream & inStream);

		/*! \details Destructor */
		virtual ~DataStreamO();

		//-------------------------------------------------------------------------

		/*! \details Gives reference to internal byte buffer. */
		std::ostream & getStdOStream() { return *mStream; }

		/*! \details  Gives const reference to internal byte buffer.*/
		const std::ostream & getStdOStream() const { return *mStream; }

		//-------------------------------------------------------------------------

		/*! \details Error codes */
		enum class eErrors {
			ok = 0,//!< No errors
			readError,//!< Error during read operation
			readZerroBytes,//!< Asked to read zero bytes - it's not correct -> error
			writeError,//!< Error during write operation
			writeZerroBytes,//!< Asked to write zero bytes - it's not correct -> error
			seekError//!< Seek operation error
		};

		/*! \details Gives last error */
		const eErrors & getOutputLastError() const { return mError; }

		/*! \details Reset errors */
		void resetOutputErrors() { mError = eErrors::ok; }

		//-------------------------------------------------------------------------
		/* Writes operators
		*/
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

		/*! \details Writes null-terminated character sequences */
		DataStreamO & operator<<(const char * inVal);

		/*! \details  Writes null-terminated character sequences */
		DataStreamO & operator<<(const wchar_t * inVal);

		DataStreamO & operator<<(const std::string & inVal);
		DataStreamO & operator<<(const std::wstring & inVal);
		//DataStream &operator<<(std::streambuf &inVal);        

		/*!
		\details Writes raw bytes
		\param [in] inVal Bytes for write.
		\param [in] len number of bytes to write.
		*/
		DataStreamO & writeRawData(const char * inVal, uint64_t len);

		//-------------------------------------------------------------------------

	private:

		DataStreamO(const DataStreamO & inCopy)
			: mStream(inCopy.mStream) {}

		DataStreamO & operator =(const DataStreamO &) { return *this; }

		DataStreamO & writeBytes(const char * inVal, uint64_t len);

		eErrors mError;
		std::ostream * mStream;

	};

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}// namespace sts
