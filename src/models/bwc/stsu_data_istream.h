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

namespace sts_bwc {
// backward compatibility

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

/*!
\details Data Input Stream class provide a stream operation with data which interpreted like bytes (not characters, e.t.c.).
         Used std::istream like internal byte buffer.
*/
class DataStreamI {
protected:

    explicit DataStreamI();
    void setStream(std::istream * inStream) { mStream = inStream; }

public:

    /*!
    \details Constructor
    \param [in] inStream reference to a std::istream.
    */
    explicit DataStreamI(std::istream & inStream);

    /*! \details  Destructor */
    virtual ~DataStreamI();

    //-------------------------------------------------------------------------

    /*! \details Gives reference to internal byte buffer. */
    std::istream & getStdIStream() { return *mStream; }

    /*! \details  Gives const reference to internal byte buffer. */
    const std::istream & getStdIStream() const { return *mStream; }

    //-------------------------------------------------------------------------

    /*! \details Error codes */
    enum class eErrors {
        ok = 0,
        //!< No errors
        readError,
        //!< Error during read operation
        readZerroBytes,
        //!< Asked to read zero bytes - it's not correct -> error
        writeError,
        //!< Error during write operation
        writeZerroBytes,
        //!< Asked to write zero bytes - it's not correct -> error
        seekError //!< Seek operation error
    };

    /*! \details Gives last error */
    const eErrors & getInputLastError() const { return mError; }

    /*! \details  Reset errors */
    void resetInputErrors() { mError = eErrors::ok; }

    //-------------------------------------------------------------------------
    /* Reads operators
    */
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

    /*! \details Reads null-terminated character sequences */
    DataStreamI & operator>>(char *& outVal);

    /*! \details Reads null-terminated character sequences */
    DataStreamI & operator>>(wchar_t *& outVal);

    DataStreamI & operator>>(std::string & outVal);
    DataStreamI & operator>>(std::wstring & outVal);

    //DataStream &operator>>(std::streambuf &outVal);
private:
    DataStreamI & readBytes(wchar_t *& outVal, uint64_t & outLen);
    DataStreamI & readBytes(char *& outVal, uint64_t & outLen);
public:

    /*!
    \details Reads raw bytes
    \param [out] outVal buffer for read bytes.
    \param [in] inLen number of bytes to read.
    */
    DataStreamI & readRawData(char * outVal, uint64_t inLen);

    /*!
    \details Skip raw bytes
    \param [in] inLen number of bytes to skip.
    */
    DataStreamI & skipRawBytes(uint64_t inLen);

    //-------------------------------------------------------------------------

private:

    DataStreamI(const DataStreamI & inCopy)
        : mStream(inCopy.mStream) {}

    DataStreamI & operator =(const DataStreamI &) { return *this; }

    eErrors mError;
    std::istream * mStream;

};

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

} // namespace sts
