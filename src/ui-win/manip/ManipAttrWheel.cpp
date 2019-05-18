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

#include "ManipAttrWheel.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "common/Logger.h"
#include "resource/ResHelper.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK ManipAttrWheel::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        ManipAttrWheel * theDlg;
        if (msg == WM_INITDIALOG) {
            theDlg = reinterpret_cast<ManipAttrWheel*>(lParam);
            DLSetWindowLongPtr(hWnd, lParam);
            theDlg->initWindow(hWnd);
        }
        else if (msg == WM_DESTROY) {
            theDlg = DLGetWindowLongPtr<ManipAttrWheel*>(hWnd);
            theDlg->destroyWindow(hWnd);
        }
        else {
            theDlg = DLGetWindowLongPtr<ManipAttrWheel *>(hWnd);
            if (!theDlg) {
                return FALSE;
            }
        }

        //--------------------------------------

        switch (msg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case CHK_ENABLE: {
                        theDlg->mChkEnable.isChecked() ? theDlg->mData.emplace() : theDlg->mData.reset();
                        theDlg->dataChanged();
                        break;
                    }
                    default: break;
                }
                break;
            }
            case CC_SPINNER_CHANGE: {
                switch (LOWORD(wParam)) {
                    case SPN_DELTA: {
                        if (theDlg->mData) {
                            theDlg->mData->mWheelDelta = theDlg->mSpnDelta->GetFVal();
                            theDlg->dataChanged();
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            default: break;
        }
        return 0;
    }

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    ManipAttrWheel::~ManipAttrWheel() {
        ManipAttrWheel::destroy();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrWheel::create(HWND inParent) {
        assert(inParent);
        mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
                                      MAKEINTRESOURCE(ROLL_MANIP_WHEEL),
                                      inParent, panelProc,
                                      reinterpret_cast<LPARAM>(this)));
        assert(mHwnd);
        if (mHwnd) {
            toWindow();
            mHwnd.show(true);
        }
        else {
            LError << WinCode(GetLastError());
        }
    }

    void ManipAttrWheel::destroy() {
        if (mHwnd) {
            const BOOL res = DestroyWindow(mHwnd.hwnd());
            if (!res) {
                LError << WinCode(GetLastError());
            }
            mHwnd.release();
        }
    }

    RECT ManipAttrWheel::rect() const {
        RECT r{0, 0, 0, 0};
        if (mHwnd) {
            r = mHwnd.rect();
        }
        return r;
    }

    void ManipAttrWheel::move(const POINT & point) {
        if (mHwnd) {
            mHwnd.move(point);
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrWheel::initWindow(HWND hWnd) {
        mSpnDelta = SetupFloatSpinner(hWnd, SPN_DELTA, SPN_DELTA_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mChkEnable.setup(hWnd, CHK_ENABLE);
    }

    void ManipAttrWheel::destroyWindow(HWND /*hWnd*/) {
        ReleaseISpinner(mSpnDelta);
        mChkEnable.release();
    }

    void ManipAttrWheel::toWindow() {
        if (mHwnd) {
            if (mData) {
                mSpnDelta->SetValue(mData->mWheelDelta, FALSE);
                mChkEnable.setState(true);
            }
            else {
                mChkEnable.setState(false);
            }
            enablingControls();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrWheel::enablingControls() {
        if (mHwnd) {
            mData ? mSpnDelta->Enable() : mSpnDelta->Disable();
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrWheel::dataChanged() {
        enablingControls();
        if (mCallback) {
            mCallback(mData);
        }
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
