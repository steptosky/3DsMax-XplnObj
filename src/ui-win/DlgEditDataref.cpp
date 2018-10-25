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

    INT_PTR CALLBACK DlgEditDataref::callBack(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
        DlgEditDataref * theDlg;
        if (message == WM_INITDIALOG) {
            theDlg = reinterpret_cast<DlgEditDataref*>(lParam);
            theDlg->mDlgMain.setup(hWnd);
            DLSetWindowLongPtr(hWnd, lParam);
        }
        else {
            if ((theDlg = DLGetWindowLongPtr<DlgEditDataref *>(hWnd)) == nullptr) {
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

    std::optional<xobj::Dataref> DlgEditDataref::edit(const xobj::Dataref & dataref) {
        DlgEditDataref win;
        return win.create(dataref);
    }

    std::optional<xobj::Dataref> DlgEditDataref::create(const xobj::Dataref & dataref) {
        mDataref = dataref;
        const auto res = DialogBoxParam(ResHelper::hInstance, MAKEINTRESOURCE(DLG_EDIT_DATAREF),
                                        GetCOREInterface()->GetMAXHWnd(),
                                        callBack, reinterpret_cast<LPARAM>(this));
        if (res == 0) {
            return std::nullopt;
        }
        return mDataref;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgEditDataref::initDlg(const HWND hWnd) {
        CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
        mCtrChkWritable.setup(hWnd, CHK_WRITABLE);

        mCtrlKey = GetICustEdit(GetDlgItem(hWnd, EDIT_KEY));
        mCtrlValueType = GetICustEdit(GetDlgItem(hWnd, EDIT_VALUE_TYPE));
        mCtrlValueUnits = GetICustEdit(GetDlgItem(hWnd, EDIT_VALUE_UNITS));
        mCtrlDesc = GetICustEdit(GetDlgItem(hWnd, EDIT_DESCRIPTION));

        dataToUi();
    }

    void DlgEditDataref::destroyDlg(const HWND /*hWnd*/) {
        uiToData();

        ReleaseICustEdit(mCtrlKey);
        ReleaseICustEdit(mCtrlValueType);
        ReleaseICustEdit(mCtrlValueUnits);
        ReleaseICustEdit(mCtrlDesc);

        mCtrChkWritable.release();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgEditDataref::dataToUi() {
        MStr str = xobj::toMStr(mDataref.mKey);
        mCtrlKey->SetText(str);

        str = xobj::toMStr(mDataref.mValueType);
        mCtrlValueType->SetText(str);

        str = xobj::toMStr(mDataref.mValueUnits);
        mCtrlValueUnits->SetText(str);

        str = xobj::toMStr(mDataref.mDescription);
        mCtrlDesc->SetText(str);

        mCtrChkWritable.setState(mDataref.mWritable);
    }

    void DlgEditDataref::uiToData() {
        MStr str;

        Utils::getText(mCtrlKey, str);
        mDataref.mKey = xobj::fromMStr(str);

        Utils::getText(mCtrlValueType, str);
        mDataref.mValueType = xobj::fromMStr(str);

        Utils::getText(mCtrlValueUnits, str);
        mDataref.mValueUnits = xobj::fromMStr(str);

        Utils::getText(mCtrlDesc, str);
        mDataref.mDescription = xobj::fromMStr(str);

        mDataref.mWritable = mCtrChkWritable.isChecked();

        mDataref.fillEmptyFields();
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
