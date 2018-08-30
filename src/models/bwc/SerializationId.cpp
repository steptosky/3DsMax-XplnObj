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

#include "SerializationId.h"

namespace bcw {
// backward compatibility

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

SerializationId::SerializationId() {
    mData[0] = 0;
    mData[1] = 0;
}

SerializationId::SerializationId(std::istream & inStream) {
    load(inStream);
}

SerializationId::SerializationId(const SerializationId & cid) {
    mData[0] = cid.mData[0];
    mData[1] = cid.mData[1];
}

SerializationId::SerializationId(uint32_t a, uint32_t b) {
    mData[0] = a;
    mData[1] = b;
}

SerializationId::~SerializationId() {}

/**************************************************************************************************/
///////////////////////////////////////////* Operators *////////////////////////////////////////////
/**************************************************************************************************/

bool SerializationId::operator==(const SerializationId & cid) const {
    return (mData[0] == cid.mData[0] && mData[1] == cid.mData[1]);
}

bool SerializationId::operator!=(const SerializationId & cid) const {
    return (mData[0] != cid.mData[0] || mData[1] != cid.mData[1]);
}

SerializationId & SerializationId::operator=(const SerializationId & cid) {
    mData[0] = cid.mData[0];
    mData[1] = cid.mData[1];
    return *this;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool SerializationId::isValid() const {
    return (mData[0] != 0 || mData[1] != 0);
}

void SerializationId::setA(uint32_t a) {
    mData[0] = a;
}

void SerializationId::setB(uint32_t b) {
    mData[1] = b;
}

uint32_t SerializationId::a() const {
    return mData[0];
}

uint32_t SerializationId::b() const {
    return mData[1];
}

void SerializationId::save(std::ostream & ourStream) const {
    ourStream.write("@", 1);
    ourStream.write(reinterpret_cast<const char*>(mData), sizeof(mData));
}

bool SerializationId::load(std::istream & inStream) {
    char id;
    inStream.read(&id, 1);
    if (id != '@') {
        *this = SerializationId();
        return false;
    }
    inStream.read(reinterpret_cast<char*>(mData), sizeof(mData));
    return true;
}

std::string SerializationId::toString() const {
    char buffer[32];
    size_t n = 0;
#ifdef _MSC_VER
    n = sprintf_s(buffer, 32, "@ %x|%x", mData[0], mData[1]);
#else
		n = sprintf(buffer, "@ %x|%x", mData[0], mData[1]);
#endif
    return std::string(buffer, n);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
