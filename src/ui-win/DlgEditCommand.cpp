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

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include <cctype>
#include <windows.h>
#include <xpln/utils/DatarefsFile.h>
#include "resource/resource.h"
#include "resource/ResHelper.h"
#include "Utils.h"
#include "common/String.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK DlgEditCommand::callBack(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
        DlgEditCommand * theDlg;
        if (message == WM_INITDIALOG) {
            theDlg = reinterpret_cast<DlgEditCommand*>(lParam);
            theDlg->mDlgMain.setup(hWnd);
            DLSetWindowLongPtr(hWnd, lParam);
        }
        else {
            if ((theDlg = DLGetWindowLongPtr<DlgEditCommand *>(hWnd)) == nullptr) {
                return FALSE;
            }
            if (message == WM_DESTROY) {
                theDlg->mDlgMain.release();
            }
        }

        //------------------------------------------------------

        switch (message) {
            case WM_INITDIALOG: {
                theDlg->initDlg(hWnd);
                break;
            }
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case BTN_OK: {
                        EndDialog(hWnd, 1);
                        break;
                    }
                    case BTN_CANCEL: {
                        EndDialog(hWnd, 0);
                        break;
                    }
                    default: break;
                }
                break;
            }
            case WM_DESTROY: {
                theDlg->destroyDlg(hWnd);
                break;
            }
            case WM_CLOSE: {
                EndDialog(hWnd, 0);
                break;
            }
            default: break;
        }
        return FALSE;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    std::optional<xobj::Command> DlgEditCommand::edit(const xobj::Command & dataref) {
        DlgEditCommand win;
        return win.create(dataref);
    }

    std::optional<xobj::Command> DlgEditCommand::create(const xobj::Command & dataref) {
        mCommand = dataref;
        const auto res = DialogBoxParam(ResHelper::hInstance, MAKEINTRESOURCE(DLG_EDIT_COMMAND),
                                        GetCOREInterface()->GetMAXHWnd(),
                                        callBack, reinterpret_cast<LPARAM>(this));
        if (res == 0) {
            return std::nullopt;
        }
        return mCommand;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgEditCommand::initDlg(const HWND hWnd) {
        CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
        mCtrlKey = GetICustEdit(GetDlgItem(hWnd, EDIT_KEY));
        mCtrlDesc = GetICustEdit(GetDlgItem(hWnd, EDIT_DESCRIPTION));
        dataToUi();
    }

    void DlgEditCommand::destroyDlg(const HWND /*hWnd*/) {
        uiToData();
        ReleaseICustEdit(mCtrlKey);
        ReleaseICustEdit(mCtrlDesc);
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgEditCommand::dataToUi() {
        MStr str = xobj::toMStr(mCommand.mKey);
        mCtrlKey->SetText(str);

        str = xobj::toMStr(mCommand.mDescription);
        mCtrlDesc->SetText(str);
    }

    void DlgEditCommand::uiToData() {
        MStr str;

        Utils::getText(mCtrlKey, str);
        mCommand.mKey = xobj::fromMStr(str);

        Utils::getText(mCtrlDesc, str);
        mCommand.mDescription = xobj::fromMStr(str);
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
