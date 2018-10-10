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

#include "resource/resource.h"
#include "ui-win/UiUtilities.h"
#include "resource/ResHelper.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK LightParam::panelProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
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
                            auto text = theDlg->cCmbName.currSelectedText();
                            const auto iter = theDlg->mPreDefinedList.find(sts::toMbString(text));
                            DbgAssert(iter != theDlg->mPreDefinedList.end());

                            theDlg->mData->setName(sts::toMbString(iter->first));
                            theDlg->mData->setRawParams(sts::toMbString(iter->second));
                            UiUtilities::setText(theDlg->cEdtCustomName, sts::toString(iter->first));
                            UiUtilities::setText(theDlg->cEdtAdditional, sts::toString(iter->second));
                            theDlg->eventParamChanged(true);
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
                        theDlg->mData->setRawParams(sts::toMbString(UiUtilities::getText(theDlg->cEdtAdditional)));
                        theDlg->eventParamChanged(true);
                        break;
                    }
                    case IDC_EDIT_CUSTOM: {
                        theDlg->mData->setName(sts::toMbString(UiUtilities::getText(theDlg->cEdtCustomName)));
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
    /////////////////////////////////////////* Static area *////////////////////////////////////////////
    /**************************************************************************************************/

    // https://developer.x-plane.com/?article=airplane-parameterized-light-guide
    std::map<std::string, std::string> LightParam::mPreDefinedList{
            {"airplane_landing_core", "$direction:a+10.0 index size"},
            {"airplane_landing_glow", "$direction:a+10.0 index size"},
            {"airplane_landing_flare", "$direction:a+10.0 index size"},
            {"airplane_landing_sp", "$rgb index size $width"},

            {"airplane_taxi_core", "$direction:a+10.0 index"},
            {"airplane_taxi_glow", "$direction:a+10.0 index size"},
            {"airplane_taxi_flare", "$direction:a+10.0 index size"},
            {"airplane_taxi_sp", "$rgb index size $width"},

            {"airplane_spot_core", "$direction:a+10.0 index size"},
            {"airplane_spot_glow", "$direction:a+10.0 index size"},
            {"airplane_spot_flare", "$direction:a+10.0 index size"},
            {"airplane_spot_sp", "$rgb index size $width"},

            {"airplane_generic_core", "$direction:a+10.0 index size"},
            {"airplane_generic_glow", "$direction:a+10.0 index size"},
            {"airplane_generic_flare", "$direction:a+10.0 index size"},
            {"airplane_generic_sp", "$rgb index size $width"},

            {"airplane_beacon_rotate", "period phase"},
            {"airplane_beacon_rotate_sp", "period phase size $width"},

            {"airplane_beacon_strobe", "size index"},
            {"airplane_beacon_strobe_sp", "size index"},

            {"airplane_strobe_omni", "0 0 0 index size"},
            {"airplane_strobe_dir", "0 0 0 index size $direction 1"},
            {"airplane_strobe_sp", "$rgb index size $direction $width"},

            {"airplane_nav_tail_size", "size focus"},
            {"airplane_nav_left_size", "size focus"},
            {"airplane_nav_right_size", "size focus"},
            {"airplane_nav_sp", "$rgb index size $direction $width"},

            {"airplane_panel_sp", "$rgb index size $direction $width"},
            {"airplane_inst_sp", "$rgb index size $direction $width"},
    };

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

    void LightParam::show(xobj::ObjLightParam * data) {
        mData = data;
        toWindow();
        mHwnd.show();
    }

    void LightParam::hide() {
        mHwnd.hide();
    }

    void LightParam::create(const HWND parent) {
        assert(parent);
        mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
                                      MAKEINTRESOURCE(IDD_ROLL_LIGHT_PARAM_OBJ),
                                      parent, panelProc,
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

    void LightParam::initWindow(const HWND hWnd) {
        cCmbName.setup(hWnd, IDC_CMB_LIGHTNAMED);
        cStcCust.setup(hWnd, IDC_STATIC_CUST);
        cEdtAdditional = GetICustEdit(GetDlgItem(hWnd, IDC_EDIT_PARAMS));
        cEdtCustomName = GetICustEdit(GetDlgItem(hWnd, IDC_EDIT_NAME));

        assert(cEdtAdditional);
        assert(cCmbName);

        for (auto & curr : mPreDefinedList) {
            cCmbName.addItem(sts::toString(curr.first));
        }
    }

    void LightParam::destroyWindow(const HWND /*hWnd*/) {
        cCmbName.release();
        ReleaseICustEdit(cEdtCustomName);
        ReleaseICustEdit(cEdtAdditional);
    }

    void LightParam::toWindow() {
        if (mData) {
            enableControls();

            UiUtilities::setText(cEdtCustomName, sts::toString(mData->name()));
            UiUtilities::setText(cEdtAdditional, sts::toString(mData->params()));
            const auto iter = mPreDefinedList.find(mData->name());
            if (iter != mPreDefinedList.end()) {
                cCmbName.setCurrSelected(sts::toString(iter->first));
            }
            else {
                cCmbName.setCurrSelected(-1);
            }
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
        cEdtCustomName->Enable();
        cEdtAdditional->Enable();
    }

    void LightParam::disableControls() {
        cCmbName.disable();
        cEdtCustomName->Disable();
        cEdtAdditional->Disable();
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
