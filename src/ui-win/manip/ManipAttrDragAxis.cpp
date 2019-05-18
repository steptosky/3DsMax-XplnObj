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

#include "ManipAttrDragAxis.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include <xpln/enums/ECursor.h>
#include "ui-win/Utils.h"
#include "common/Logger.h"
#include "resource/ResHelper.h"
#include "presenters/Datarefs.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK ManipAttrDragAxis::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        ManipAttrDragAxis * theDlg;
        if (msg == WM_INITDIALOG) {
            theDlg = reinterpret_cast<ManipAttrDragAxis*>(lParam);
            DLSetWindowLongPtr(hWnd, lParam);
            theDlg->initWindow(hWnd);
        }
        else if (msg == WM_DESTROY) {
            theDlg = DLGetWindowLongPtr<ManipAttrDragAxis*>(hWnd);
            theDlg->destroyWindow(hWnd);
        }
        else {
            theDlg = DLGetWindowLongPtr<ManipAttrDragAxis *>(hWnd);
            if (!theDlg) {
                return FALSE;
            }
        }

        //--------------------------------------

        switch (msg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case BTN_DATAREF: {
                        MSTR str;
                        Utils::getText(theDlg->cEdtDataRef, str);
                        str = presenters::Datarefs::selectData(str);
                        theDlg->cEdtDataRef->SetText(str);
                        theDlg->mData.mDataref = xobj::fromMStr(str);
                        theDlg->save();
                        break;
                    }
                    case CMB_CURSOR: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            theDlg->mData.mCursor = xobj::ECursor::fromUiString(sts::toMbString(theDlg->cCmbCursor.currSelectedText()).c_str());
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
                        theDlg->mData.mDataref = sts::toMbString(Utils::getText(theDlg->cEdtDataRef));
                        theDlg->save();
                        break;
                    }
                    case EDIT_TOOLTIP: {
                        theDlg->mData.mToolType = sts::toMbString(Utils::getText(theDlg->cEdtToolType));
                        theDlg->save();
                        break;
                    }
                    default: break;
                }
                break;
            }
            case CC_SPINNER_CHANGE: {
                switch (LOWORD(wParam)) {
                    case SPN_V1: {
                        theDlg->mData.mVal1 = theDlg->mSpnVal1->GetFVal();
                        theDlg->save();
                        break;
                    }
                    case SPN_V2: {
                        theDlg->mData.mVal2 = theDlg->mSpnVal2->GetFVal();
                        theDlg->save();
                        break;
                    }
                    case SPN_X: {
                        theDlg->mData.mDirX = theDlg->mSpnX->GetFVal();
                        theDlg->save();
                        break;
                    }
                    case SPN_Y: {
                        theDlg->mData.mDirY = theDlg->mSpnY->GetFVal();
                        theDlg->save();
                        break;
                    }
                    case SPN_Z: {
                        theDlg->mData.mDirZ = theDlg->mSpnZ->GetFVal();
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

    ManipAttrDragAxis::ManipAttrDragAxis(MdManip * modelData)
        : mModelData(modelData) {
        assert(mModelData);
    }

    ManipAttrDragAxis::~ManipAttrDragAxis() {
        ManipAttrDragAxis::destroy();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrDragAxis::create(HWND inParent) {
        assert(inParent);
        mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
                                      MAKEINTRESOURCE(ROLL_MANIP_DRAGXYZ),
                                      inParent, panelProc,
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

    void ManipAttrDragAxis::destroy() {
        if (mHwnd) {
            const BOOL res = DestroyWindow(mHwnd.hwnd());
            if (!res) {
                LError << WinCode(GetLastError());
            }
            mHwnd.release();
        }
    }

    RECT ManipAttrDragAxis::rect() const {
        return mSize;
    }

    void ManipAttrDragAxis::move(const POINT & point) {
        if (mHwnd) {
            mHwnd.move(point);
            calculateSize();
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrDragAxis::calculateSize() {
        if (mHwnd) {
            mSize = mHwnd.rect();
            mWheel.move(POINT{0, mSize.bottom});
            const RECT wheelRect = mWheel.rect();
            mSize.bottom += (wheelRect.bottom - wheelRect.top);
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrDragAxis::setManip(const std::optional<xobj::AttrManip> & manip) {
        assert(manip);
        const auto data = std::get_if<xobj::AttrManipDragAxis>(&manip->mType);
        if (!data) {
            const xobj::EManipulator type = std::visit([](auto && m) { return m.mType; }, manip->mType);
            LError << "Incorrect manipulator type: " << type.toString();
            return;
        }
        mData = *data;
        mWheel.setManip(mData.mWheel);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrDragAxis::initWindow(HWND hWnd) {
        const auto callback = [this](const std::optional<xobj::AttrManipWheel> & wheel) mutable {
            mData.mWheel = wheel;
            save();
        };
        mWheel.setCallBack(callback);

        mSpnVal1 = SetupFloatSpinner(hWnd, SPN_V1, SPN_V1_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mSpnVal2 = SetupFloatSpinner(hWnd, SPN_V2, SPN_V2_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);

        mSpnX = SetupFloatSpinner(hWnd, SPN_X, SPN_X_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mSpnY = SetupFloatSpinner(hWnd, SPN_Y, SPN_Y_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mSpnZ = SetupFloatSpinner(hWnd, SPN_Z, SPN_Z_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);

        cBtnDataRef.setup(hWnd, IDC_BTN_DATAREF);
        cEdtDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_DATAREF));
        cEdtToolType = GetICustEdit(GetDlgItem(hWnd, EDIT_TOOLTIP));
        cCmbCursor.setup(hWnd, CMB_CURSOR);

        for (auto & curr : xobj::ECursor::list()) {
            cCmbCursor.addItem(sts::toString(curr.toUiString()));
        }
        cCmbCursor.setCurrSelected(0);
    }

    void ManipAttrDragAxis::destroyWindow(HWND /*hWnd*/) {
        ReleaseISpinner(mSpnVal1);
        ReleaseISpinner(mSpnVal2);
        ReleaseISpinner(mSpnX);
        ReleaseISpinner(mSpnY);
        ReleaseISpinner(mSpnZ);

        cBtnDataRef.release();
        ReleaseICustEdit(cEdtDataRef);
        ReleaseICustEdit(cEdtToolType);
        cCmbCursor.release();
    }

    void ManipAttrDragAxis::toWindow() {
        mSpnVal1->SetValue(mData.mVal1, FALSE);
        mSpnVal2->SetValue(mData.mVal2, FALSE);

        mSpnX->SetValue(mData.mDirX, FALSE);
        mSpnY->SetValue(mData.mDirY, FALSE);
        mSpnZ->SetValue(mData.mDirZ, FALSE);

        cEdtDataRef->SetText(xobj::toMStr(mData.mDataref));
        cEdtToolType->SetText(xobj::toMStr(mData.mToolType));
        cCmbCursor.setCurrSelected(sts::toString(mData.mCursor.toUiString()));
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
