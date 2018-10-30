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

#include "AnimCalc.h"
#include "resource/resource.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool AnimCalc::calculate(KeyList & inOutData, HWND parent) {
        mData = &inOutData;
        //--------------------------
        mSpnVal1.setupSpinnerChild(mDialog, IDC_V1_SPIN, IDC_V1_EDIT, -1000000.0f, 1000000.0f, 0.0f, 0.1f);
        mSpnVal2.setupSpinnerChild(mDialog, IDC_V2_SPIN, IDC_V2_EDIT, -1000000.0f, 1000000.0f, 1.0f, 0.1f);
        mBtnOk.setupChild(mDialog, IDC_BTN_OK);
        mBtnCancel.setupChild(mDialog, IDC_BTN_CANCEL);
        //--------------------------
        mDialog.onInit = [&](auto win) { win->centerBy(parent); };
        mBtnCancel.onClick = [&](auto) { mDialog.destroy(0); };
        mBtnOk.onClick = [&](auto) {
            calculateValues();
            mDialog.destroy(1);
        };
        //--------------------------
        return mDialog.create(parent, IDD_TOOL_CALC_ANIM_VALUE) != 0;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimCalc::calculateValues() {
        if (mData->size() < 2) {
            return;
        }
        const double tStart = static_cast<double>(mData->at(0).keyTime);
        const double tEnd = static_cast<double>(mData->at(mData->size() - 1).keyTime);
        const double tRange = tEnd - tStart;

        const double vStart = static_cast<double>(mSpnVal1.floatVal());
        const double vEnd = static_cast<double>(mSpnVal2.floatVal());
        const double vRange = vEnd - vStart;

        for (auto & curr : *mData) {
            curr.datarefValue = static_cast<float>((((curr.keyTime - tStart) / tRange) * vRange) + vStart);
        }
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
