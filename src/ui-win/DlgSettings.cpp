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

#include "DlgSettings.h"
#include "resource/resource.h"
#include "common/Config.h"
#include "gup/ObjCommon.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgSettings::open() {
        //-------------------------
        mLblSimPath.setupChild(mDialog, IDC_SIM_PATH);
        mBtnOk.setupChild(mDialog, BTN_OK);
        mChkDatarefsId.setupChild(mDialog, IDC_CHK_DATAREFS_ID);
        mChkCommandsId.setupChild(mDialog, IDC_CHK_COMMANDS_ID);
        mBtnSelectSimPath.setupChild(mDialog, BTN_SELECT_SIM_PATH);
        //-------------------------
        mBtnOk.onClick = [&](auto) { mDialog.destroy(1); };
        mBtnSelectSimPath.onClick = [&](auto) { selectSimPath(); };
        mChkDatarefsId.onStateChanged = [](auto,const bool state) { ObjCommon::instance()->pSettings.setUseDatarefsId(state); };
        mChkCommandsId.onStateChanged = [](auto, const bool state) { ObjCommon::instance()->pSettings.setUseCommandsId(state); };
        //-------------------------
        mDialog.onInit = [&](auto win) {
            win->centerByParent();
            auto config = Config::instance();
            auto & settings = ObjCommon::instance()->pSettings;
            mLblSimPath.setText(config->simDir().GetString().data());
            mChkDatarefsId.setChecked(settings.isUseDatarefsId());
            mChkCommandsId.setChecked(settings.isUseCommandsId());

            mLblSimPath.setToolTip(_T("The path where X-Plane is installed."));
            mChkDatarefsId.setToolTip(_T("Enabling ID usage for the project's DataRefs. The state is saved with the 3Ds Max's scene."));
            mChkCommandsId.setToolTip(_T("Enabling ID usage for the project's Commands. The state is saved with the 3Ds Max's scene."));
        };
        //-------------------------
        mDialog.create(GetCOREInterface()->GetMAXHWnd(), DLG_SETTINGS);
    }

    void DlgSettings::show() {
        DlgSettings dlg;
        dlg.open();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgSettings::selectSimPath() {
        const auto ip = GetCOREInterface8();
        auto config = Config::instance();

        FilterList filterList;
        filterList.Append(_M("X-Plane exec file(*X-Plane.exe)"));
        filterList.Append(_M("*X-Plane.exe"));
        MStr currDir = config->simDir().GetString();
        MStr currFile = _T("X-Plane.exe");
        if (GetCOREInterface8()->DoMaxOpenDialog(ip->GetMAXHWnd(),
                                                 _T("Select X-Plane dir by X-Plane.exe file"),
                                                 currFile, currDir, filterList)) {
            mLblSimPath.setText(currDir.data());
            config->setSimDir(currDir);
        }
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
