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

#include "MdManip.h"
#include "models/io/NodeIO.h"
#include "common/String.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

MdManip::MdManip() {
    reset();
}

MdManip::~MdManip() {
    clearLink();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MdManip::reset() {}

void MdManip::cloneData(INode * from, INode * to) {
    ManipIO::cloneData(from, to);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MdManip::clearLink() {
    mNode = nullptr;
    reset();
}

bool MdManip::linkNode(INode * node) {
    clearLink();
    if (node == nullptr) {
        return false;
    }

    if (!ManipIO::canApply(node)) {
        return false;
    }

    mNode = node;
    return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MdManip::saveToNode(INode * node, const xobj::AttrManipBase & inManip) const {
    if (node) {
        try {
            ManipIO::saveToNode(node, &inManip);
        }
        catch (std::exception & e) {
            LCritical << "Can't save data to <" << sts::toMbString(node->GetName())
                    << "> object. Reason: <" << e.what() << ">";
        }
    }
}

bool MdManip::loadFromNode(INode * node, ManipIO::IManipIo * callBack) {
    if (node) {
        try {
            return ManipIO::loadFromNode(node, callBack);
        }
        catch (std::exception & e) {
            LCritical << "Can't load data from <" << sts::toMbString(node->GetName())
                    << "> object. Reason: <" << e.what() << ">";
        }
    }
    return false;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
