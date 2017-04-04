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

#include "DataStreamI.h"
#include "DataStreamO.h"

namespace sts {

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Data Stream class provide a stream operation with data which interpreted like bytes (not characters).
	 * \pre It is the best practice to read/write format version,
	 * use \link DataStreamO::writeFormatVersion \endlink
	 * and \link DataStreamI::readAndSetFormatVersion \endlink.
	 */
	class DataStream : public DataStreamI, public DataStreamO {
	public:

		//-------------------------------------------------------------------------

		explicit DataStream(std::iostream * inStream);
		explicit DataStream(std::iostream * inStream, uint8_t inVersion);
		virtual ~DataStream();

		//-------------------------------------------------------------------------

		std::iostream & stdStream();
		const std::iostream & stdStream() const;
		void setFormatVersion(uint8_t inVersion);

		//-------------------------------------------------------------------------

	private:

		DataStream(const DataStream & incOpy) = delete;
		DataStream & operator =(const DataStream &) = delete;

		std::iostream * mStream;

	};

	/********************************************************************************************************/
	///////////////////////////////////////* Constructors/Destructor *////////////////////////////////////////
	/********************************************************************************************************/

	/*!
	 * \details Constructor init
	 * \note This class does not take ownership for pointer to std::iostream.
	 * \param [in] inStream reference to a std::iostream.
	 */
	inline DataStream::DataStream(std::iostream * inStream)
		:
		DataStreamI(inStream),
		DataStreamO(inStream),
		mStream(inStream) { }

	/*!
	 * \details Constructor init version.
	 * \note This class does not take ownership for pointer to std::iostream.
	 * \param [in] inStream
	 * \param [in] inVersion format version,
	 * for more information see \link DataStreamO::setOutputFormatVersion \endlink and \link DataStreamI::readFormatVersion \endlink.
	 */
	inline DataStream::DataStream(std::iostream * inStream, uint8_t inVersion)
		:
		DataStream(inStream) {
		setFormatVersion(inVersion);
	}

	/*! \details Destructor */
	inline DataStream::~DataStream() { }

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	/*! \details Gives reference to internal std::iostream. */
	inline std::iostream & DataStream::stdStream() {
		return *mStream;
	}

	/*! \details Gives const reference to internal std::iostream. */
	inline const std::iostream & DataStream::stdStream() const {
		return *mStream;
	}

	/*!
	 * \details Sets IO format version.
	 * \param [in] inVersion
	 */
	inline void DataStream::setFormatVersion(uint8_t inVersion) {
		setInputFormatVersion(inVersion);
		setOutputFormatVersion(inVersion);
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}
