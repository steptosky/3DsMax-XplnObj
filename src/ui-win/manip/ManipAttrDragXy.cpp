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

#include "ManipAttrDragXy.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include <xpln/enums/ECursor.h>
#include "ui-win/Utils.h"
#include "resource/resource.h"
#include "common/Logger.h"
#include "resource/ResHelper.h"
#include "presenters/Datarefs.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK ManipAttrDragXy::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        ManipAttrDragXy * theDlg;
        if (msg == WM_INITDIALOG) {
            theDlg = reinterpret_cast<ManipAttrDragXy*>(lParam);
            DLSetWindowLongPtr(hWnd, lParam);
            theDlg->initWindow(hWnd);
        }
        else if (msg == WM_DESTROY) {
            theDlg = DLGetWindowLongPtr<ManipAttrDragXy*>(hWnd);
            theDlg->destroyWindow(hWnd);
        }
        else {
            theDlg = DLGetWindowLongPtr<ManipAttrDragXy *>(hWnd);
            if (!theDlg) {
                return FALSE;
            }
        }

        //--------------------------------------

        switch (msg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case BTN_X_DATAREF: {
                        MSTR str;
                        Utils::getText(theDlg->cEdtXDataRef, str);
                        str = presenters::Datarefs::selectData(str);
                        theDlg->cEdtXDataRef->SetText(str);
                        theDlg->mData.setXDataref(xobj::fromMStr(str));
                        theDlg->save();
                        break;
                    }
                    case BTN_Y_DATAREF: {
                        MSTR str;
                        Utils::getText(theDlg->cEdtYDataRef, str);
                        str = presenters::Datarefs::selectData(str);
                        theDlg->cEdtYDataRef->SetText(str);
                        theDlg->mData.setYDataref(xobj::fromMStr(str));
                        theDlg->save();
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
                    case EDIT_X_DATAREF: {
                        theDlg->mData.setXDataref(sts::toMbString(Utils::getText(theDlg->cEdtXDataRef)));
                        theDlg->save();
                        break;
                    }
                    case EDIT_Y_DATAREF: {
                        theDlg->mData.setYDataref(sts::toMbString(Utils::getText(theDlg->cEdtYDataRef)));
                        theDlg->save();
                        break;
                    }
                    case EDIT_TOOLTIP: {
                        theDlg->mData.setToolTip(sts::toMbString(Utils::getText(theDlg->cEdtToolType)));
                        theDlg->save();
                        break;
                    }
                    default: break;
                }
                break;
            }
            case CC_SPINNER_CHANGE: {
                switch (LOWORD(wParam)) {
                    case SPN_X: {
                        theDlg->mData.setX(theDlg->mSpnX->GetFVal());
                        theDlg->save();
                        break;
                    }
                    case SPN_MIN: {
                        theDlg->mData.setXMin(theDlg->mSpnXMin->GetFVal());
                        theDlg->save();
                        break;
                    }
                    case SPN_MAX: {
                        theDlg->mData.setXMax(theDlg->mSpnXMax->GetFVal());
                        theDlg->save();
                        break;
                    }
                    case SPN_Y: {
                        theDlg->mData.setY(theDlg->mSpnY->GetFVal());
                        theDlg->save();
                        break;
                    }
                    case SPN_MIN2: {
                        theDlg->mData.setYMin(theDlg->mSpnYMin->GetFVal());
                        theDlg->save();
                        break;
                    }
                    case SPN_MAX2: {
                        theDlg->mData.setYMax(theDlg->mSpnYMax->GetFVal());
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

    ManipAttrDragXy::ManipAttrDragXy(MdManip * modelData)
        : mModelData(modelData) {
        assert(mModelData);
    }

    ManipAttrDragXy::~ManipAttrDragXy() {
        ManipAttrDragXy::destroy();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrDragXy::create(HWND inParent) {
        assert(inParent);
        mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
                                      MAKEINTRESOURCE(ROLL_MANIP_DRAGXY),
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

    void ManipAttrDragXy::destroy() {
        if (mHwnd) {
            BOOL res = DestroyWindow(mHwnd.hwnd());
            if (!res) {
                LError << WinCode(GetLastError());
            }
            mHwnd.release();
        }
    }

    RECT ManipAttrDragXy::rect() const {
        RECT r{0, 0, 0, 0};
        if (mHwnd) {
            r = mHwnd.rect();
        }
        return r;
    }

    void ManipAttrDragXy::move(const POINT & point) {
        if (mHwnd) {
            mHwnd.move(point);
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrDragXy::setManip(const xobj::AttrManipBase & manip) {
        if (manip.type() != mData.type()) {
            LError << "Incorrect manipulator: " << manip.type().toString();
            return;
        }
        mData = static_cast<const xobj::AttrManipDragXy &>(manip);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ManipAttrDragXy::initWindow(HWND hWnd) {
        mSpnX = SetupFloatSpinner(hWnd, SPN_X, SPN_X_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mSpnXMin = SetupFloatSpinner(hWnd, SPN_MIN, SPN_MIN_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mSpnXMax = SetupFloatSpinner(hWnd, SPN_MAX, SPN_MAX_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        cBtnXDataRef.setup(hWnd, BTN_X_DATAREF);
        cEdtXDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_X_DATAREF));

        mSpnY = SetupFloatSpinner(hWnd, SPN_Y, SPN_Y_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mSpnYMin = SetupFloatSpinner(hWnd, SPN_MIN2, SPN_MIN_EDIT2, -10000.0f, 10000.0f, 0.0f, 0.1f);
        mSpnYMax = SetupFloatSpinner(hWnd, SPN_MAX2, SPN_MAX_EDIT2, -10000.0f, 10000.0f, 0.0f, 0.1f);
        cBtnYDataRef.setup(hWnd, BTN_Y_DATAREF);
        cEdtYDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_Y_DATAREF));

        cEdtToolType = GetICustEdit(GetDlgItem(hWnd, EDIT_TOOLTIP));
        cCmbCursor.setup(hWnd, CMB_CURSOR);

        for (auto & curr : xobj::ECursor::list()) {
            cCmbCursor.addItem(sts::toString(curr.toUiString()));
        }
        cCmbCursor.setCurrSelected(0);
    }

    void ManipAttrDragXy::destroyWindow(HWND /*hWnd*/) {
        ReleaseISpinner(mSpnX);
        ReleaseISpinner(mSpnXMin);
        ReleaseISpinner(mSpnXMax);
        cBtnXDataRef.release();
        ReleaseICustEdit(cEdtXDataRef);

        ReleaseISpinner(mSpnY);
        ReleaseISpinner(mSpnYMin);
        ReleaseISpinner(mSpnYMax);
        cBtnYDataRef.release();
        ReleaseICustEdit(cEdtYDataRef);

        ReleaseICustEdit(cEdtToolType);
        cCmbCursor.release();
    }

    void ManipAttrDragXy::toWindow() {
        mSpnX->SetValue(mData.x(), FALSE);
        mSpnXMin->SetValue(mData.xMin(), FALSE);
        mSpnXMax->SetValue(mData.xMax(), FALSE);
        cEdtXDataRef->SetText(xobj::toMStr(mData.xDataref()));

        mSpnY->SetValue(mData.y(), FALSE);
        mSpnYMin->SetValue(mData.yMin(), FALSE);
        mSpnYMax->SetValue(mData.yMax(), FALSE);
        cEdtYDataRef->SetText(xobj::toMStr(mData.yDataref()));

        cEdtToolType->SetText(xobj::toMStr(mData.toolTip()));
        cCmbCursor.setCurrSelected(sts::toString(mData.cursor().toUiString()));
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
