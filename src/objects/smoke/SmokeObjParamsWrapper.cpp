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

#include "SmokeObjParamsWrapper.h"

#pragma warning(push, 0)
#include <iparamb2.h>
#pragma warning(pop)

#include "common/Logger.h"
#include "SmokeObjParams.h"
#include "common/String.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

SmokeObjParamsWrapper::SmokeObjParamsWrapper(INode * node, const TimeValue t, const Interval & interval)
    : mInterval(interval),
      mT(t),
      mNode(node) {

    DbgAssert(node);
    DbgAssert(isSmokeObj(node));
    mPb2 = node->GetObjectRef()->GetParamBlockByID(SmokeObjParams);
    DbgAssert(mPb2);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool SmokeObjParamsWrapper::isSmokeObj(INode * inNode) {
    Object * obj = inNode->GetObjectRef();
    if (obj != nullptr) {
        return (obj->ClassID() == ClassesDescriptions::smokeObj()->ClassID()) == TRUE;
    }
    return false;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void SmokeObjParamsWrapper::setType(const xobj::ObjSmoke::eSmokeType val) {
    if (mPb2) {
        if (!mPb2->SetValue(PSmokeObjAttr_SmokeType, int(mT), val)) {
            XLError << LogNode(mNode) << "Can't save value:" << TOTEXT(PSmokeObjAttr_SmokeType);
        }
    }
    else {
        XLError << "Pointer to IParamBlock2 is nullptr";
    }
}

void SmokeObjParamsWrapper::setSize(const float val) {
    if (mPb2) {
        if (!mPb2->SetValue(PSmokeObjAttr_Size, mT, val)) {
            XLError << LogNode(mNode) << "Can't save value:" << TOTEXT(PSmokeObjAttr_Size);
        }
    }
    else {
        XLError << "Pointer to IParamBlock2 is nullptr";
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

xobj::ObjSmoke::eSmokeType SmokeObjParamsWrapper::type() {
    int val = 0;
    if (mPb2) {
        if (!mPb2->GetValue(PSmokeObjAttr_SmokeType, mT, val, mInterval)) {
            XLError << LogNode(mNode) << "Can't save value:" << TOTEXT(PSmokeObjAttr_SmokeType);
        }
    }
    else {
        XLError << "Pointer to IParamBlock2 is nullptr";
    }
    return xobj::ObjSmoke::eSmokeType(val);
}

float SmokeObjParamsWrapper::size() {
    float val = 0.0f;
    if (mPb2) {
        if (!mPb2->GetValue(PSmokeObjAttr_Size, mT, val, mInterval)) {
            XLError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(PSmokeObjAttr_Size);
        }
    }
    else {
        XLError << "Pointer to IParamBlock2 is nullptr";
    }
    return val;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
