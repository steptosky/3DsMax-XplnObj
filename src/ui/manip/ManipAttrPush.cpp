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

#include "ManipAttrPush.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include <xpln/enums/ECursor.h>
#include "ui/UiUtilities.h"
#include "resource/resource.h"
#include "common/Logger.h"
#include "resource/ResHelper.h"
#include "ui/Factory.h"

namespace ui {

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

INT_PTR ManipAttrPush::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ManipAttrPush * theDlg;
    if (msg == WM_INITDIALOG) {
        theDlg = reinterpret_cast<ManipAttrPush*>(lParam);
        DLSetWindowLongPtr(hWnd, lParam);
        theDlg->initWindow(hWnd);
    }
    else if (msg == WM_DESTROY) {
        theDlg = DLGetWindowLongPtr<ManipAttrPush*>(hWnd);
        theDlg->destroyWindow(hWnd);
    }
    else {
        theDlg = DLGetWindowLongPtr<ManipAttrPush *>(hWnd);
        if (!theDlg) {
            return FALSE;
        }
    }

    //--------------------------------------

    switch (msg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case BTN_DATAREF: {
                    Factory::showNotImplemented();
                    break;
                }
                case CMB_CURSOR: {
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        theDlg->mData.setCursor(xobj::ECursor::fromUiString(sts::toMbString(theDlg->cCmbCursor.currSelectedText()).c_str()));
                        theDlg->save();
                    }
                    break;
                }
                default: break;
            }
            break;
        }
        case WM_CUSTEDIT_ENTER: {
            switch (LOWORD(wParam)) {
                case EDIT_DATAREF: {
                    theDlg->mData.setDataref(sts::toMbString(UiUtilities::getText(theDlg->cEdtDataRef)));
                    theDlg->save();
                    break;
                }
                case EDIT_TOOLTIP: {
                    theDlg->mData.setToolTip(sts::toMbString(UiUtilities::getText(theDlg->cEdtToolType)));
                    theDlg->save();
                    break;
                }
                default: break;
            }
            break;
        }
        case CC_SPINNER_CHANGE: {
            switch (LOWORD(wParam)) {
                case SPN_UP: {
                    theDlg->mData.setUp(theDlg->mSpnUp->GetFVal());
                    theDlg->save();
                    break;
                }
                case SPN_DOWN: {
                    theDlg->mData.setDown(theDlg->mSpnDown->GetFVal());
                    theDlg->save();
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

ManipAttrPush::ManipAttrPush(MdManip * modelData)
    : mModelData(modelData) {
    assert(mModelData);
}

ManipAttrPush::~ManipAttrPush() {
    ManipAttrPush::destroy();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttrPush::create(HWND inParent) {
    assert(inParent);
    mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
                                  MAKEINTRESOURCE(ROLL_MANIP_PUSH),
                                  inParent,
                                  reinterpret_cast<DLGPROC>(panelProc),
                                  reinterpret_cast<LPARAM>(this)));
    assert(mHwnd);
    if (mHwnd) {
        toWindow();
        mWheel.create(inParent);
        calculateSize();
        mHwnd.show(true);
    }
    else {
        LError << WinCode(GetLastError());
    }
}

void ManipAttrPush::destroy() {
    if (mHwnd) {
        BOOL res = DestroyWindow(mHwnd.hwnd());
        if (!res) {
            LError << WinCode(GetLastError());
        }
        mHwnd.release();
    }
}

RECT ManipAttrPush::rect() const {
    return mSize;
}

void ManipAttrPush::move(const POINT & point) {
    if (mHwnd) {
        mHwnd.move(point);
        calculateSize();
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttrPush::calculateSize() {
    if (mHwnd) {
        mSize = mHwnd.rect();
        mWheel.move(POINT{0, mSize.bottom});
        RECT wheelRect = mWheel.rect();
        mSize.bottom += (wheelRect.bottom - wheelRect.top);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttrPush::setManip(const xobj::AttrManipBase & manip) {
    if (manip.type() != mData.type()) {
        LError << "Incorrect manipulator: " << manip.type().toString();
        return;
    }
    mData = static_cast<const xobj::AttrManipPush &>(manip);
    mWheel.setManip(mData.wheel());
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttrPush::initWindow(HWND hWnd) {
    std::function<void(const xobj::AttrManipWheel &)> callback = [this](const xobj::AttrManipWheel & wheel) mutable {
        mData.setWheel(wheel);
        mModelData->saveToNode(mData);
    };
    mWheel.setCallBack(callback);

    mSpnUp = SetupFloatSpinner(hWnd, SPN_UP, SPN_UP_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
    mSpnDown = SetupFloatSpinner(hWnd, SPN_DOWN, SPN_DOWN_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);

    cBtnDataRef.setup(hWnd, IDC_BTN_DATAREF);
    cEdtDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_DATAREF));
    cEdtToolType = GetICustEdit(GetDlgItem(hWnd, EDIT_TOOLTIP));
    cCmbCursor.setup(hWnd, CMB_CURSOR);

    for (auto & curr : xobj::ECursor::list()) {
        cCmbCursor.addItem(sts::toString(curr.toUiString()));
    }
    cCmbCursor.setCurrSelected(0);
}

void ManipAttrPush::destroyWindow(HWND /*hWnd*/) {
    ReleaseISpinner(mSpnUp);
    ReleaseISpinner(mSpnDown);

    cBtnDataRef.release();
    ReleaseICustEdit(cEdtDataRef);
    ReleaseICustEdit(cEdtToolType);
    cCmbCursor.release();
}

void ManipAttrPush::toWindow() {
    mSpnUp->SetValue(mData.up(), FALSE);
    mSpnDown->SetValue(mData.down(), FALSE);

    UiUtilities::setText(cEdtDataRef, sts::toString(mData.dataref()));

    UiUtilities::setText(cEdtToolType, sts::toString(mData.toolTip()));
    cCmbCursor.setCurrSelected(sts::toString(mData.cursor().toUiString()));
}

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

}
