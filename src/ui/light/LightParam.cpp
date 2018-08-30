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

#include "LightParam.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include <xpln/enums/ELightParams.h>
#include "resource/resource.h"
#include "ui/UiUtilities.h"
#include "resource/ResHelper.h"

namespace ui {

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

INT_PTR LightParam::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    LightParam * theDlg;
    if (msg == WM_INITDIALOG) {
        theDlg = reinterpret_cast<LightParam*>(lParam);
        DLSetWindowLongPtr(hWnd, lParam);
        theDlg->initWindow(hWnd);
    }
    else if (msg == WM_DESTROY) {
        theDlg = DLGetWindowLongPtr<LightParam*>(hWnd);
        theDlg->destroyWindow(hWnd);
    }
    else {
        theDlg = DLGetWindowLongPtr<LightParam *>(hWnd);
        if (!theDlg) {
            return FALSE;
        }
    }

    //--------------------------------------

    switch (msg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_CMB_LIGHTNAMED: {
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        if (theDlg->isUiCustom()) {
                            theDlg->setCustFieldEnable(true);
                            theDlg->mData->setLightId(xobj::ELightParams(xobj::ELightParams::light_params_custom));
                            theDlg->eventParamChanged(true);
                        }
                        else {
                            theDlg->setCustFieldEnable(false);
                            theDlg->mData->setLightId(xobj::ELightParams::fromUiString(sts::toMbString(theDlg->cCmbName.currSelectedText()).c_str()));
                            theDlg->eventParamChanged(true);
                        }
                    }
                    break;
                }
                default: break;
            }
            break;
        }
        case WM_CUSTEDIT_ENTER: {
            switch (LOWORD(wParam)) {
                case IDC_EDIT_PARAMS: {
                    theDlg->mData->setAdditionalParams(sts::toMbString(UiUtilities::getText(theDlg->cEdtAdditional)));
                    theDlg->eventParamChanged(true);
                    break;
                }
                case IDC_EDIT_CUSTOM: {
                    theDlg->mData->setLightName(sts::toMbString(UiUtilities::getText(theDlg->cEdtCustomName)));
                    theDlg->eventParamChanged(true);
                    break;
                }
                default: break;
            }
            break;
        }
        default: break;
    }
    return FALSE;
}

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

LightParam::LightParam() {
    mData = nullptr;
}

LightParam::~LightParam() {
    LightParam::destroy();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void LightParam::show(xobj::ObjLightParam * inData) {
    mData = inData;
    toWindow();
    mHwnd.show();
}

void LightParam::hide() {
    mHwnd.hide();
}

void LightParam::create(HWND inParent) {
    assert(inParent);
    mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
                                  MAKEINTRESOURCE(IDD_ROLL_LIGHT_PARAM_OBJ),
                                  inParent,
                                  reinterpret_cast<DLGPROC>(panelProc),
                                  reinterpret_cast<LPARAM>(this)));
    assert(mHwnd);
}

void LightParam::destroy() {
    assert(mHwnd);
    DestroyWindow(mHwnd.hwnd());
    mHwnd.release();
    mData = nullptr;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void LightParam::initWindow(HWND hWnd) {
    cCmbName.setup(hWnd, IDC_CMB_LIGHTNAMED);
    cStcCust.setup(hWnd, IDC_STATIC_CUST);
    cEdtAdditional = GetICustEdit(GetDlgItem(hWnd, IDC_EDIT_PARAMS));
    cEdtCustomName = GetICustEdit(GetDlgItem(hWnd, IDC_EDIT_CUSTOM));

    assert(cEdtAdditional);
    assert(cCmbName);

    for (auto & curr : xobj::ELightParams::list()) {
        cCmbName.addItem(sts::toString(curr.toUiString()));
    }
    cCmbName.setCurrSelected(0);
}

void LightParam::destroyWindow(HWND /*hWnd*/) {
    cCmbName.release();
    ReleaseICustEdit(cEdtCustomName);
    ReleaseICustEdit(cEdtAdditional);
}

void LightParam::toWindow() {
    if (mData) {
        enableControls();

        if (mData->lightId() == xobj::ELightParams(xobj::ELightParams::light_params_custom)) {
            setCustFieldEnable(true);
            UiUtilities::setText(cEdtCustomName, sts::toString(mData->lightName()));
        }
        else {
            setCustFieldEnable(false);
        }
        cCmbName.setCurrSelected(sts::toString(mData->lightId().toUiString()));
        UiUtilities::setText(cEdtAdditional, sts::toString(mData->additionalParams()));
    }
    else {
        disableControls();
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void LightParam::enableControls() {
    cCmbName.enable();
    setCustFieldEnable(true);
    cEdtAdditional->Enable();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void LightParam::disableControls() {
    cCmbName.disable();
    setCustFieldEnable(false);
    cEdtAdditional->Disable();
}

void LightParam::setCustFieldEnable(bool status) {
    cEdtCustomName->Enable(status);
    cStcCust.enable(status);
}

bool LightParam::isUiCustom() const {
    return cCmbName.currSelectedText() == sts::toString(xobj::ELightParams(xobj::ELightParams::light_params_custom).toUiString());
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
