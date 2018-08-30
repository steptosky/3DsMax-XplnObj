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

#include "stsu_data_istream.h"
#include "stsu_data_ostream.h"
#include <sstream>

namespace sts_bwc {
// backward compatibility

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

/*!
\details Data Stream class provide a stream operation with data which interpreted like bytes (not characters, e.t.c.).
         Used std::iostream like internal byte buffer.
*/
class DataStream : public DataStreamI, public DataStreamO {
public:

    //-------------------------------------------------------------------------
    /*! 
    \details Constructor
    \param [in] inStream reference to a std::iostream.
    \note std::iostream object deletion is your responsibility.
    */
    explicit DataStream(std::iostream & inStream);

    /*! 
    \details Constructor
    \param [in] inStreamBuf reference to a std::stringbuf. Will be created std::stringstream with this buffer for internal using.
    \note std::stringbuf object deletion is NOT your responsibility.
    */
    explicit DataStream(std::stringbuf & inStreamBuf);

    /*
    \details Default Constructor
    \remark std::stringstream will be created for internal using.
    */
    DataStream();

    /*! \details Destructor */
    virtual ~DataStream();

    //-------------------------------------------------------------------------

    /*! \details Gives reference to internal byte buffer. */
    std::iostream & getStdStream() { return mStream; }

    /*! \details Gives const reference to internal byte buffer. */
    const std::iostream & getStdStream() const { return mStream; }

    //-------------------------------------------------------------------------

private:

    DataStream(const DataStream & incOpy);
    DataStream & operator =(const DataStream &) { return *this; }

    std::iostream & mStream;
    bool mIsStreamOwn;

};

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

} // namespace sts
