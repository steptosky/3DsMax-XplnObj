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

#include "ui-win/DlgEditCommand.h"
#include "resource/resource.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    std::optional<xobj::Command> DlgEditCommand::edit(const xobj::Command & command) {
        DlgEditCommand win;
        return win.create(command);
    }

    std::optional<xobj::Command> DlgEditCommand::create(const xobj::Command & command) {
        mCommand = command;

        mBtnOk.setupChild(mDialog, BTN_OK);
        mBtnCancel.setupChild(mDialog, BTN_CANCEL);
        mEditKey.setupChild(mDialog, EDIT_KEY);
        mEditDescription.setupChild(mDialog, EDIT_DESCRIPTION);
        //----------------------------------
        mDialog.onInit = [&](auto win) {
            win->centerByParent();
            mEditKey.setXObjText(mCommand.mKey);
            mEditDescription.setXObjText(mCommand.mDescription);
        };
        //----------------------------------
        mBtnOk.onClick = [&](auto) { mDialog.destroy(1); };
        mBtnCancel.onClick = [&](auto) { mDialog.destroy(0); };

        mEditKey.onLostFocus = [&](auto ctrl) { mCommand.mKey = ctrl->textXObj(); };
        mEditKey.onEnter = mEditKey.onLostFocus;

        mEditDescription.onLostFocus = [&](auto ctrl) { mCommand.mDescription = ctrl->textXObj(); };
        mEditDescription.onEnter = mEditDescription.onLostFocus;
        //----------------------------------
        const auto res = mDialog.create(GetCOREInterface()->GetMAXHWnd(), DLG_EDIT_COMMAND);
        if (res == 0) {
            return std::nullopt;
        }
        return mCommand;
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
