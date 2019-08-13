/*
**  Copyright(C) 2019, StepToSky
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

#include "ChangeControllerWatcher.h"

namespace ui::win {

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

ChangeControllerWatcher::~ChangeControllerWatcher() {
    DeleteAllRefsFromMe();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ChangeControllerWatcher::setCallback(std::function<void()> callback) {
    mCallback = std::move(callback);
}

void ChangeControllerWatcher::createReference(const RefTargetHandle hTarget) {
    ReplaceReference(0, hTarget);
}

void ChangeControllerWatcher::removeReference() {
    if (mRef0) {
        DeleteReference(0);
        mRef0 = nullptr;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int ChangeControllerWatcher::NumRefs() {
    return 1;
}

RefTargetHandle ChangeControllerWatcher::GetReference(const int i) {
    DbgAssert(i == 0);
    if (i == 0) {
        return mRef0;
    }
    return nullptr;
}

void ChangeControllerWatcher::SetReference(const int i, const RefTargetHandle rTarget) {
    if (i == 0) {
        mRef0 = rTarget;
    }
    DbgAssert(i == 0);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 16
RefResult ChangeControllerWatcher::NotifyRefChanged(const Interval & /*changeInt*/, RefTargetHandle /*hTarget*/,
                                                    PartID & /*partId*/, const RefMessage message, BOOL /*propagate*/) {
#else
RefResult ChangeControllerWatcher::NotifyRefChanged(Interval /*changeInt*/, RefTargetHandle /*hTarget*/,
                                       PartID& /*partId*/, const RefMessage message) {
#endif

    // if (message == REFMSG_TARGET_DELETED) {
    //     // The item we monitor has been deleted -- we're done...
    //     return REF_SUCCEED;
    // }
    if (message == REFMSG_CONTROLREF_CHANGE) {
        if (mCallback) {
            mCallback();
        }
    }
    return REF_SUCCEED;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

}
