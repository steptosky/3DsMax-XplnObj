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

#include "SmokeObjParamBlocks.h"

#include "resource/resource.h"
#include "objects/ScaleDim.h"
#include "classes-desc/ClassesDescriptions.h"
#include "SmokeObj.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR < 15
#	define p_end end
#endif

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class SmokeObjIconSizeCallback : public PBAccessor {
public:

    virtual ~SmokeObjIconSizeCallback() = default;

    void Set(PB2Value & v, ReferenceMaker * owner, const ParamID id, const int tabIndex, const TimeValue t) override {
        auto u = dynamic_cast<SmokeObject*>(owner);
        DbgAssert(u);
        switch (id) {
            case PSmokeObjIconScale:
                u->makeIcon();
                break;
            default: break;
        }
        PBAccessor::Set(v, owner, id, tabIndex, t);
    }
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class SmokeDlgProc : public ParamMap2UserDlgProc {
public:

    SmokeDlgProc() = default;
    virtual ~SmokeDlgProc() = default;

    INT_PTR DlgProc(const TimeValue t, IParamMap2 * map, const HWND hWnd, const UINT msg, const WPARAM wParam, LPARAM) override {
        IParamBlock2 * ppb = map->GetParamBlock();
        switch (msg) {
            case WM_INITDIALOG: {
                const HWND cbmBlend = GetDlgItem(hWnd, CMB_SMOKE_TYPE);
                DbgAssert(cbmBlend);
                if (cbmBlend) {
                    //ComboBox_AddString(cbmBlend, _T("none"));
                    ComboBox_AddString(cbmBlend, _T("White"));
                    ComboBox_AddString(cbmBlend, _T("Black"));
                    ComboBox_SetCurSel(cbmBlend, ppb->GetInt(PSmokeObjAttr_SmokeType) - 1);
                }
                return TRUE;
            }
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case CMB_SMOKE_TYPE: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            ppb->SetValue(PSmokeObjAttr_SmokeType, t, ComboBox_GetCurSel(GetDlgItem(hWnd, CMB_SMOKE_TYPE)) + 1);
                        }
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

    void DeleteThis() override {}
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

static SmokeObjIconSizeCallback gSmokeIconSizeCallback;
static SmokeDlgProc gSmokeDlgProc;

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

ParamBlockDesc2 SmokeObjParamBlocks::mParams(SmokeObjParams, _T("X-Smoke"), 0, ClassesDescriptions::smokeObj(),
                                             P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                             PbVersionParams, PbOrderParams,
                                             //-------------------------------------------------------------------------
                                             // Rollouts
                                             ROLL_SMOKEOBJ_ATTR, IDS_ROLL_SMOKE_ATTR, 0, 0, &gSmokeDlgProc,
                                             //-------------------------------------------------------------------------
                                             // Params									
                                             PSmokeObjAttr_SmokeType, _T("Type"), TYPE_INT, 0, NO_IDS,
                                             p_default, 1,
                                             p_end,
                                             //-------------------------------------------------------------------------
                                             PSmokeObjAttr_Size, _T("Size"), TYPE_FLOAT, 0, NO_IDS,
                                             p_default, 1.0f,
                                             p_range, 0.0001f, 999999999.9f,
                                             p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_SMOKE_SIZE_EDIT, SPN_SMOKE_SIZE, SPIN_AUTOSCALE,
                                             p_end,
                                             //-------------------------------------------------------------------------
                                             p_end);

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

ParamBlockDesc2 SmokeObjParamBlocks::mDisplay(SmokeObjDisplay, _T("X-Smoke-Display"), 0, ClassesDescriptions::smokeObj(),
                                              P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                              PbVersionDisplay, PbOrderDisplay,
                                              //-------------------------------------------------------------------------
                                              // Rollouts
                                              ROLL_SMOKEOBJ_DISPLAY, IDS_ROLL_SMOKE_DISPLY, 0, APPENDROLL_CLOSED, NULL,
                                              //-------------------------------------------------------------------------
                                              // Display									
                                              PSmokeObjIconScale, _T("IconScale"), TYPE_FLOAT, 0, IDS_SCALE,
                                              p_default, 1.0f,
                                              p_range, 0.01f, 1000.0f,
                                              p_accessor, &gSmokeIconSizeCallback,
                                              p_dim, &gScaleDim,
                                              p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, IDC_SCALE_EDIT, IDC_SCALE_SPIN, SPIN_AUTOSCALE,
                                              p_end,
                                              //-------------------------------------------------------------------------
                                              p_end);

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
