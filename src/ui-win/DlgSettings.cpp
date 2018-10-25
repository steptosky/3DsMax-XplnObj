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

#pragma warning(push, 0)
#include <max.h>
#include <3dsmaxport.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "common/Logger.h"
#include "resource/ResHelper.h"
#include "common/Config.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK DlgSettings::callBack(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
        DlgSettings * theDlg;
        if (message == WM_INITDIALOG) {
            theDlg = reinterpret_cast<DlgSettings*>(lParam);
            theDlg->mDlgMain.setup(hWnd);
            DLSetWindowLongPtr(hWnd, lParam);
        }
        else {
            if ((theDlg = DLGetWindowLongPtr<DlgSettings *>(hWnd)) == nullptr) {
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
                    case BTN_SELECT_SIM_PATH: {
                        theDlg->selectSimPath();
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

    void DlgSettings::open() {
        DialogBoxParam(ResHelper::hInstance, MAKEINTRESOURCE(DLG_SETTINGS),
                       GetCOREInterface()->GetMAXHWnd(),
                       callBack, reinterpret_cast<LPARAM>(this));
    }

    void DlgSettings::show() {
        DlgSettings dlg;
        dlg.open();
    }

    void DlgSettings::initDlg(const HWND hWnd) {
        CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
        mLblSimPath.setup(hWnd, IDC_SIM_PATH);
        mBtnOk.setup(hWnd, BTN_OK);
        mBtnSelectSimPath.setup(hWnd, BTN_SELECT_SIM_PATH);

        auto config = Config::instance();
        mLblSimPath.setText(config->simDir().GetString().data());
    }

    void DlgSettings::destroyDlg(HWND /*hWnd*/) {
        mLblSimPath.release();
        mBtnOk.release();
        mBtnSelectSimPath.release();
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
