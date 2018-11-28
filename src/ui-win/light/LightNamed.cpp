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

#include "LightNamed.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "ui-win/Utils.h"
#include "resource/ResHelper.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK LightNamed::panelProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        LightNamed * theDlg;
        if (msg == WM_INITDIALOG) {
            theDlg = reinterpret_cast<LightNamed*>(lParam);
            DLSetWindowLongPtr(hWnd, lParam);
            theDlg->initWindow(hWnd);
        }
        else if (msg == WM_DESTROY) {
            theDlg = DLGetWindowLongPtr<LightNamed*>(hWnd);
            theDlg->destroyWindow(hWnd);
        }
        else {
            theDlg = DLGetWindowLongPtr<LightNamed *>(hWnd);
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
                            const auto text = theDlg->cCmbName.currSelectedText();
                            theDlg->mData->setName(sts::toMbString(text));
                            theDlg->cName->SetText(xobj::toMStr(text));
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
                    case IDC_NAME_LIGHTNAMED: {
                        theDlg->mData->setName(sts::toMbString(Utils::getText(theDlg->cName)));
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

    std::set<std::string> LightNamed::mPreDefinedList{
        "ship_nav_left",
        "ship_nav_right",
        "ship_mast_obs",
        "ship_mast_grn",
        "ship_nav_tail",
        "ship_mast_powered",
        "carrier_datum",
        "carrier_waveoff",
        "carrier_meatball1",
        "carrier_meatball2",
        "carrier_meatball3",
        "carrier_meatball4",
        "carrier_meatball5",
        "carrier_mast_strobe",
        "carrier_deck_blue_s",
        "carrier_deck_blue_w",
        "carrier_deck_blue_n",
        "carrier_deck_blue_e",
        "carrier_pitch_lights",
        "carrier_foul_line_red",
        "carrier_foul_line_white",
        "carrier_center_white",
        "carrier_edge_white",
        "carrier_thresh_white",
        "frigate_SGSI_lo",
        "frigate_SGSI_on",
        "frigate_SGSI_hi",
        "frigate_deck_green",
        "oilrig_deck_blue",

        "town_light_60",
        "town_light_90",
        "town_light_150",
        "town_light_180",
        "town_light_220",
        "town_light_280",
        "town_light_330",
        "town_light_350",
        "town_light_omni",
        "town_tiny_light_60",
        "town_tiny_light_90",
        "town_tiny_light_150",
        "town_tiny_light_180",
        "town_tiny_light_220",
        "town_tiny_light_280",
        "town_tiny_light_330",
        "town_tiny_light_350",
        "town_tiny_light_omni",

        "obs_strobe_day",
        "obs_strobe_night",
        "obs_red_day",
        "obs_red_night",
        "hospital_helipad_blue",
    };

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    LightNamed::LightNamed() {
        mData = nullptr;
    }

    LightNamed::~LightNamed() {
        LightNamed::destroy();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightNamed::show(xobj::ObjLightNamed * data) {
        mData = data;
        toWindow();
        mHwnd.show();
    }

    void LightNamed::hide() {
        mHwnd.hide();
    }

    void LightNamed::create(const HWND parent) {
        assert(parent);
        mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
                                      MAKEINTRESOURCE(IDD_ROLL_LIGHT_NAMED_OBJ),
                                      parent, panelProc,
                                      reinterpret_cast<LPARAM>(this)));
        assert(mHwnd);
    }

    void LightNamed::destroy() {
        assert(mHwnd);
        DestroyWindow(mHwnd.hwnd());
        mHwnd.release();
        mData = nullptr;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightNamed::initWindow(const HWND hWnd) {
        cCmbName.setup(hWnd, IDC_CMB_LIGHTNAMED);
        cName = GetICustEdit(GetDlgItem(hWnd, IDC_NAME_LIGHTNAMED));
        for (auto & curr : mPreDefinedList) {
            cCmbName.addItem(sts::toString(curr));
        }
    }

    void LightNamed::destroyWindow(const HWND /*hWnd*/) {
        cCmbName.release();
        ReleaseICustEdit(cName);
    }

    void LightNamed::toWindow() {
        if (mData) {
            enableControls();

            cName->SetText(xobj::toMStr(mData->name()));
            const auto iter = mPreDefinedList.find(mData->name());
            if (iter != mPreDefinedList.end()) {
                cCmbName.setCurrSelected(sts::toString(*iter));
            }
            else {
                cCmbName.setCurrSelected(-1);
            }
        }
        else {
            disableControls();
        }
    }

    void LightNamed::toData() {
        mData->setName(sts::toMbString(Utils::getText(cName)));
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightNamed::enableControls() {
        cCmbName.enable();
        cName->Enable();
    }

    void LightNamed::disableControls() {
        cCmbName.disable();
        cName->Disable();
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
