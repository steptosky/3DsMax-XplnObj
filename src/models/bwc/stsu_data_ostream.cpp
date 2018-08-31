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

#include "stsu_data_ostream.h"

namespace sts_bwc {
// backward compatibility

/********************************************************************************************************/
///////////////////////////////////////* Constructors/Destructor *////////////////////////////////////////
/********************************************************************************************************/

DataStreamO::DataStreamO(std::ostream & inStream)
    : mStream(&inStream),
      mError(DataStreamO::eErrors::ok) {}

DataStreamO::DataStreamO()
    : mStream(nullptr),
      mError(DataStreamO::eErrors::ok) {}

DataStreamO::~DataStreamO() { }

/********************************************************************************************************/
//////////////////////////////////////////////* Operators *///////////////////////////////////////////////
/********************************************************************************************************/

DataStreamO & DataStreamO::operator<<(char inVal) {
    if (!mStream->write(&inVal, sizeof(char))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(int8_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(int8_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(uint8_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(uint8_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(int16_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(int16_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(uint16_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(uint16_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(int32_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(int32_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(uint32_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(uint32_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(int64_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(int64_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(uint64_t inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(uint64_t))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(bool inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(bool))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(float inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(float))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(double inVal) {
    if (!mStream->write(reinterpret_cast<char *>(&inVal), sizeof(double))) {
        mError = eErrors::writeError;
    }
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(const char * inVal) {
    if (!inVal) {
        *this << static_cast<uint64_t>(0);
        mError = eErrors::writeZerroBytes;
        return *this;
    }
    return writeBytes(inVal, static_cast<uint64_t>(strlen(inVal)));
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(const wchar_t * inVal) {
    if (!inVal) {
        *this << static_cast<uint64_t>(0);
        mError = eErrors::writeZerroBytes;
        return *this;
    }
    return writeBytes(reinterpret_cast<const char *>(inVal), static_cast<uint64_t>(sizeof(wchar_t) * wcslen(inVal)));
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(const std::string & inVal) {
    return writeBytes(inVal.data(), static_cast<uint64_t>(inVal.size()));
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::operator<<(const std::wstring & inVal) {
    return writeBytes(reinterpret_cast<const char*>(inVal.data()), static_cast<uint64_t>(sizeof(wchar_t) * inVal.size()));
}

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

DataStreamO & DataStreamO::writeBytes(const char * inVal, uint64_t len) {
    *this << len;
    if (len)
        writeRawData(inVal, len);
    return *this;
}

//-------------------------------------------------------------------------

DataStreamO & DataStreamO::writeRawData(const char * inVal, uint64_t len) {
    if (!mStream->write(inVal, len)) {
        mError = eErrors::writeError;
    }
    return *this;
}

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

} // namespace sts
