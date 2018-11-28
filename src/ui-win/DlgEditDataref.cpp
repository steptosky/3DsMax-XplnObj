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

#include "ui-win/DlgEditDataref.h"
#include "resource/resource.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    std::optional<xobj::Dataref> DlgEditDataref::edit(const xobj::Dataref & dataref) {
        DlgEditDataref win;
        return win.create(dataref);
    }

    std::optional<xobj::Dataref> DlgEditDataref::create(const xobj::Dataref & dataref) {
        mDataref = dataref;

        mChkIsWritable.setupChild(mDialog, CHK_WRITABLE);
        mBtnOk.setupChild(mDialog, BTN_OK);
        mBtnCancel.setupChild(mDialog, BTN_CANCEL);
        mEditKey.setupChild(mDialog, EDIT_KEY);
        mEditValueType.setupChild(mDialog, EDIT_VALUE_TYPE);
        mEditValueUnits.setupChild(mDialog, EDIT_VALUE_UNITS);
        mEditDescription.setupChild(mDialog, EDIT_DESCRIPTION);
        //----------------------------------
        mDialog.onInit = [&](auto win) {
            win->centerByParent();
            mEditKey.setXObjText(mDataref.mKey);
            mEditValueType.setXObjText(mDataref.mValueType);
            mEditValueUnits.setXObjText(mDataref.mValueUnits);
            mEditDescription.setXObjText(mDataref.mDescription);
            mChkIsWritable.setChecked(mDataref.mWritable);
        };
        //----------------------------------
        mChkIsWritable.onStateChanged = [&](auto, auto state) { mDataref.mWritable = state; };
        mBtnOk.onClick = [&](auto) { mDialog.destroy(1); };
        mBtnCancel.onClick = [&](auto) { mDialog.destroy(0); };

        mEditKey.onLostFocus = [&](auto ctrl) { mDataref.mKey = ctrl->textXObj(); };
        mEditKey.onEnter = mEditKey.onLostFocus;

        mEditValueType.onLostFocus = [&](auto ctrl) { mDataref.mValueType = ctrl->textXObj(); };
        mEditValueType.onEnter = mEditValueType.onLostFocus;

        mEditValueUnits.onLostFocus = [&](auto ctrl) { mDataref.mValueUnits = ctrl->textXObj(); };
        mEditValueUnits.onEnter = mEditValueUnits.onLostFocus;

        mEditDescription.onLostFocus = [&](auto ctrl) { mDataref.mDescription = ctrl->textXObj(); };
        mEditDescription.onEnter = mEditDescription.onLostFocus;
        //----------------------------------

        const auto res = mDialog.create(GetCOREInterface()->GetMAXHWnd(), DLG_EDIT_DATAREF);
        if (res == 0) {
            return std::nullopt;
        }
        mDataref.fillEmptyFields();
        return mDataref;
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
