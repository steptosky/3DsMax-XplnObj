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

#include "MainObj.h "

#include <Windows.h>
#include <xpln/enums/ELayer.h>
#include <xpln/obj/attributes/AttrBlend.h>
#include <xpln/obj/attributes/AttrWetDry.h>

#include "resource/resource.h"
#include "MainObjectParams.h"
#include "objects/ScaleDim.h"
#include "objects/DirectDim.h"
#include "common/Logger.h"
#include "common/String.h"
#include "models/bwc/SerializationId.h"
#include "ui-win/DlgMessageBox.h"
#include "MainObjParamsWrapper.h"
#include "classes-desc/ClassesDescriptions.h"
#include "additional/math/Compare.h"
#include "MainObjIcon-gen.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR < 15
#	define p_end end
#endif

#if MAX_VERSION_MAJOR < 19
#	define TYPE_SINGLECHECKBOX TYPE_SINGLECHEKBOX
#endif

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

#define ATTR_PB_VERSION 1
#define EXP_PB_VERSION 1
#define DISPLAY_PB_VERSION 1

enum {
    // Param blocks order
    MainObjAttrParamsOrder = MainObjAttrParams,
    MainObjExpParamsOrder = MainObjExpParams,
    MainObjDisplayOrder = MainObjDisplay,
};

enum {
    // Rollouts list
    RollGlobShading,
    RollGlobAttrCockpit,
    RollGlobAttr,
    // Rollouts count
    RollCount,
};

enum eMainObjDisplay : ParamID {
    // Internal values of the display rollout
    MainObjDisp_IconScale
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class MainObjUiCallback : public PBAccessor {
public:

    virtual ~MainObjUiCallback() {}

    void Set(PB2Value & v, ReferenceMaker * owner, ParamID id, int tabIndex, TimeValue t) override {
        MainObject * u = dynamic_cast<MainObject*>(owner);
        DbgAssert(u);
        switch (id) {
            case MainObjDisp_IconScale:
                u->makeIcon();
                break;
            default: break;
        }
        PBAccessor::Set(v, owner, id, tabIndex, t);
    }
};

class MainObjAttrCallback : public PBAccessor {
public:

    virtual ~MainObjAttrCallback() {}

    void Set(PB2Value & v, ReferenceMaker * owner, ParamID id, int tabIndex, TimeValue t) override {
        MainObject * u = dynamic_cast<MainObject*>(owner);
        DbgAssert(u);
        switch (id) {
            case MainObjAttr_Texture:
                u->updateTexturesButtons();
                break;
            case MainObjAttr_TextureLit:
                u->updateTexturesButtons();
                break;
            case MainObjAttr_TextureNormal:
                u->updateTexturesButtons();
                break;
            case MainObjAttr_BlendingType:
                u->updateBlendSpinEnabling();
                break;
            case MainObjAttr_LayerGroupLayer:
                u->updateLyerGroupSpinEnabling();
                break;
            case MainObjAttr_LayerGroupDrapedLayer:
                u->updateLyerGroupSpinEnabling();
                break;
            default: break;
        }
        PBAccessor::Set(v, owner, id, tabIndex, t);
    }
};

class MainObjectPostLoadCallback : public PostLoadCallback {
public:

    virtual ~MainObjectPostLoadCallback() {}

    MainObject * pobj;

    explicit MainObjectPostLoadCallback(MainObject * p) {
        pobj = p;
    }

    void proc(ILoad * /*iload*/) override {
        pobj->makeIcon();
    }
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

static MainObjUiCallback gMainObjUiCallback;
static MainObjAttrCallback gMainObjAttrCallback;
MouseCallback MainObject::mMouseCallback;
MainObject * MainObject::mEditOb = nullptr;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class GlobShadingDlgProc : public ParamMap2UserDlgProc {
public:

    GlobShadingDlgProc() = default;
    virtual ~GlobShadingDlgProc() = default;

    INT_PTR DlgProc(TimeValue t, IParamMap2 * map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM /*lParam*/) override {
        IParamBlock2 * ppb = map->GetParamBlock();
        switch (msg) {
            case WM_INITDIALOG: {
                HWND cbmBlend = GetDlgItem(hWnd, CMB_BLEND);
                DbgAssert(cbmBlend);
                if (cbmBlend) {
                    ComboBox_AddString(cbmBlend, _T("Blend (default)"));
                    ComboBox_AddString(cbmBlend, _T("No Blend"));
                    ComboBox_AddString(cbmBlend, _T("Shadow Blend"));
                    ComboBox_SetCurSel(cbmBlend, ppb->GetInt(MainObjAttr_BlendingType));
                }
                return TRUE;
            }
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case CMB_BLEND: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            ppb->SetValue(MainObjAttr_BlendingType, t, ComboBox_GetCurSel(GetDlgItem(hWnd, CMB_BLEND)));
                        }
                        break;
                    }
                    case BTN_TEXTURE_DEL: {
                        MainObjParamsWrapper wrapper(ppb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
                        wrapper.setTexture("");
                        MainObject::updateButtonText(ppb, MainObjAttr_Texture, sts::toString(wrapper.texture()).c_str());;
                        break;
                    }
                    case BTN_TEXTURE_LIT_DEL: {
                        MainObjParamsWrapper wrapper(ppb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
                        wrapper.setTextureLit("");
                        MainObject::updateButtonText(ppb, MainObjAttr_TextureLit, sts::toString(wrapper.textureLit()).c_str());;
                        break;
                    }
                    case BTN_TEXTURE_NORMAL_DEL: {
                        MainObjParamsWrapper wrapper(ppb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
                        wrapper.setTextureNormal("");
                        MainObject::updateButtonText(ppb, MainObjAttr_TextureNormal, sts::toString(wrapper.textureNormal()).c_str());;
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

/***************************************************************************************/

class GlobAttrDlgProc : public ParamMap2UserDlgProc {
public:

    GlobAttrDlgProc() = default;
    virtual ~GlobAttrDlgProc() = default;

    INT_PTR DlgProc(TimeValue t, IParamMap2 * map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM /*lParam*/) override {
        IParamBlock2 * ppb = map->GetParamBlock();
        switch (msg) {
            case WM_INITDIALOG: {
                HWND cbmLayer = GetDlgItem(hWnd, CMB_LAYERGROUP);
                HWND cbmLayerDrap = GetDlgItem(hWnd, CMB_LAYERGROUP_DRAPED);
                HWND cbmWetDry = GetDlgItem(hWnd, CMB_WETDRY);
                DbgAssert(cbmLayer);
                DbgAssert(cbmLayerDrap);
                DbgAssert(cbmWetDry);
                if (cbmLayer && cbmLayerDrap) {
                    for (auto & val : xobj::ELayer::list()) {
                        sts::Str str = sts::toString(val.toUiString());
                        ComboBox_AddString(cbmLayer, str.c_str());
                        ComboBox_AddString(cbmLayerDrap, str.c_str());
                    }
                    MainObjParamsWrapper wrapper(ppb, nullptr, t, FOREVER);
                    ComboBox_SetCurSel(cbmLayer,static_cast<int>(wrapper.layerGroup().layer().id()));
                    ComboBox_SetCurSel(cbmLayerDrap, static_cast<int>(wrapper.drapedLayerGroup().layer().id()));
                }
                if (cbmWetDry) {
                    ComboBox_AddString(cbmWetDry, _T("none"));
                    ComboBox_AddString(cbmWetDry, _T("Wet"));
                    ComboBox_AddString(cbmWetDry, _T("Dry"));
                    ComboBox_SetCurSel(cbmWetDry, ppb->GetInt(MainObjAttr_DryWet));
                }
                return TRUE;
            }
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case CMB_WETDRY: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            ppb->SetValue(MainObjAttr_DryWet, t, ComboBox_GetCurSel(GetDlgItem(hWnd, CMB_WETDRY)));
                        }
                        break;
                    }
                    case CMB_LAYERGROUP: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            MainObjParamsWrapper wrapper(ppb, nullptr, t, FOREVER);
                            xobj::AttrLayerGroup attr = wrapper.layerGroup();
                            attr.setLayer(xobj::ELayer(static_cast<xobj::ELayer::eId>(ComboBox_GetCurSel(GetDlgItem(hWnd, CMB_LAYERGROUP)))));
                            wrapper.setLayerGroup(attr);
                        }
                        break;
                    }
                    case CMB_LAYERGROUP_DRAPED: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            MainObjParamsWrapper wrapper(ppb, nullptr, t, FOREVER);
                            xobj::AttrDrapedLayerGroup attr = wrapper.drapedLayerGroup();
                            attr.setLayer(xobj::ELayer(static_cast<xobj::ELayer::eId>(ComboBox_GetCurSel(GetDlgItem(hWnd, CMB_LAYERGROUP_DRAPED)))));
                            wrapper.setDrapedLayerGroup(attr);
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

static GlobShadingDlgProc gGlobShadingDlgProc;
static GlobAttrDlgProc gGlobAttrDlgProc;

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

static ParamBlockDesc2 gMainAttrPb(MainObjAttrParams, _T("X-Obj-Attributes"), 0, ClassesDescriptions::mainObj(),
                                   P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION + P_MULTIMAP,
                                   ATTR_PB_VERSION, MainObjAttrParamsOrder, RollCount,
                                   //-------------------------------------------------------------------------
                                   // Rollouts
                                   RollGlobShading, ROLL_MAINOBJ_GLOB_SHADING, IDS_ROLL_GLOB_SHADING, 0, 0, &gGlobShadingDlgProc,
                                   RollGlobAttrCockpit, ROLL_MAINOBJ_COCKPIT, IDS_ROLL_GLOB_COCKPIT, 0, APPENDROLL_CLOSED, NULL,
                                   RollGlobAttr, ROLL_MAINOBJ_ATTR, IDS_ROLL_GLOB_ATTR, 0, APPENDROLL_CLOSED, &gGlobAttrDlgProc,
                                   //-------------------------------------------------------------------------
                                   // Attributes Params								
                                   MainObjAttr_PathPrefix, _T("Path-prefix"), TYPE_STRING, 0, NO_IDS,
                                   p_default, _M(""),
                                   p_ui, RollGlobShading, TYPE_EDITBOX, EDT_PATH_PREFIX,
                                   p_end,
                                   //-------------
                                   MainObjAttr_Texture, _T("Texture"), TYPE_FILENAME, 0, NO_IDS,
                                   p_ui, RollGlobShading, TYPE_FILEOPENBUTTON, BTN_TEXTURE,
                                   p_file_types, IDS_TEXTURE_TYPES,
                                   p_caption, IDS_TEXTURE_CAPTION,
                                   p_accessor, &gMainObjAttrCallback,
                                   p_end,

                                   MainObjAttr_TextureLit, _T("Texture-lit"), TYPE_FILENAME, 0, NO_IDS,
                                   p_ui, RollGlobShading, TYPE_FILEOPENBUTTON, BTN_TEXTURE_LIT,
                                   p_file_types, IDS_TEXTURE_TYPES,
                                   p_caption, IDS_TEXTURE_LIT_CAPTION,
                                   p_accessor, &gMainObjAttrCallback,
                                   p_end,

                                   MainObjAttr_TextureNormal, _T("Texture-normal"), TYPE_FILENAME, 0, NO_IDS,
                                   p_ui, RollGlobShading, TYPE_FILEOPENBUTTON, BTN_TEXTURE_NORMAL,
                                   p_file_types, IDS_TEXTURE_TYPES,
                                   p_caption, IDS_TEXTURE_NORMAL_CAPTION,
                                   p_accessor, &gMainObjAttrCallback,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_NormalMetalness, _T("NormalMetalness"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobShading, TYPE_SINGLECHECKBOX, CHK_NORMAL_METALNESS,
                                   p_end,
                                   MainObjAttr_BlendGlass, _T("BlendGlass"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobShading, TYPE_SINGLECHECKBOX, CHK_BLEND_GLASS,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_TintAlbedo, _T("Tint-albedo"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 1.0f,
                                   p_dim, &gDirectDim,
                                   p_range, 0.0f, 1.0f,
                                   p_ui, RollGlobShading, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_TINT_ALBEDO_EDIT, SPN_TINT_ALBEDO, SPIN_AUTOSCALE,
                                   p_end,

                                   MainObjAttr_TintEmissive, _T("Tint-emissive"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, 0.0f, 1.0f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobShading, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_TINT_EMISS_EDIT, SPN_TINT_EMISS, SPIN_AUTOSCALE,
                                   p_end,

                                   MainObjAttr_TintEnable, _T("Enable-tint"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_enable_ctrls, 2, MainObjAttr_TintAlbedo, MainObjAttr_TintEmissive,
                                   p_ui, RollGlobShading, TYPE_SINGLECHECKBOX, CHK_TINT,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_SpecularRatio, _T("Specular-ratio"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 1.0f,
                                   p_range, 0.0f, 1.0f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobShading, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_SPECULAR_EDIT, SPN_SPECULAR, SPIN_AUTOSCALE,
                                   p_end,

                                   MainObjAttr_SpecularEnable, _T("Enable-specular"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_enable_ctrls, 1, MainObjAttr_SpecularRatio,
                                   p_ui, RollGlobShading, TYPE_SINGLECHECKBOX, CHK_SPECULAR,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_BlendingType, _T("Blend-type"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_accessor, &gMainObjAttrCallback,
                                   p_end,
                                   // 3d max 9 does not support 
                                   //p_ui, RollGlobShading, TYPE_INT_COMBOBOX,
                                   //CMB_BLEND, 3,
                                   //IDS_BLEND_ENABLE,
                                   //IDS_BLEND_DISABLE,
                                   //IDS_BLEND_ENABLE_SHADOW,
                                   //p_default, 0,
                                   //p_accessor, &gMainObjAttrCallback,
                                   //p_end,

                                   MainObjAttr_BlendingRatio, _T("Blend-ratio"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.5f,
                                   p_range, 0.0f, 1.0f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobShading, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_BLEND_EDIT, SPN_BLEND, SPIN_AUTOSCALE,
                                   p_end,
                                   /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
                                   MainObjAttr_CockpitLit, _T("Cockpit-lit"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttrCockpit, TYPE_SINGLECHECKBOX, CHK_COCKPITLIT,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_CockpitRegion_1, _T("Cockpit-region-1"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttrCockpit, TYPE_SINGLECHECKBOX, CHK_COCKPIT_REGION_1,
                                   p_enable_ctrls, 4, MainObjAttr_CockpitRegion_1_T, MainObjAttr_CockpitRegion_1_R,
                                   MainObjAttr_CockpitRegion_1_L, MainObjAttr_CockpitRegion_1_B,
                                   p_end,

                                   MainObjAttr_CockpitRegion_2, _T("Cockpit-region-2"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttrCockpit, TYPE_SINGLECHECKBOX, CHK_COCKPIT_REGION_2,
                                   p_enable_ctrls, 4, MainObjAttr_CockpitRegion_2_T, MainObjAttr_CockpitRegion_2_R,
                                   MainObjAttr_CockpitRegion_2_L, MainObjAttr_CockpitRegion_2_B,
                                   p_end,

                                   MainObjAttr_CockpitRegion_3, _T("Cockpit-region-3"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttrCockpit, TYPE_SINGLECHECKBOX, CHK_COCKPIT_REGION_3,
                                   p_enable_ctrls, 4, MainObjAttr_CockpitRegion_3_T, MainObjAttr_CockpitRegion_3_R,
                                   MainObjAttr_CockpitRegion_3_L, MainObjAttr_CockpitRegion_3_B,
                                   p_end,

                                   MainObjAttr_CockpitRegion_4, _T("Cockpit-region-4"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttrCockpit, TYPE_SINGLECHECKBOX, CHK_COCKPIT_REGION_4,
                                   p_enable_ctrls, 4, MainObjAttr_CockpitRegion_4_T, MainObjAttr_CockpitRegion_4_R,
                                   MainObjAttr_CockpitRegion_4_L, MainObjAttr_CockpitRegion_4_B,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_CockpitRegion_1_T, _T("Cockpit-region-1-t"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_1_T_EDIT, SPN_COCKPIT_1_T, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_1_R, _T("Cockpit-region-1-r"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_1_R_EDIT, SPN_COCKPIT_1_R, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_1_L, _T("Cockpit-region-1-l"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_1_L_EDIT, SPN_COCKPIT_1_L, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_1_B, _T("Cockpit-region-1-b"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_1_B_EDIT, SPN_COCKPIT_1_B, 1,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_CockpitRegion_2_T, _T("Cockpit-region-2-t"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_2_T_EDIT, SPN_COCKPIT_2_T, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_2_R, _T("Cockpit-region-2-r"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_2_R_EDIT, SPN_COCKPIT_2_R, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_2_L, _T("Cockpit-region-2-l"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_2_L_EDIT, SPN_COCKPIT_2_L, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_2_B, _T("Cockpit-region-2-b"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_2_B_EDIT, SPN_COCKPIT_2_B, 1,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_CockpitRegion_3_T, _T("Cockpit-region-3-t"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_3_T_EDIT, SPN_COCKPIT_3_T, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_3_R, _T("Cockpit-region-3-r"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_3_R_EDIT, SPN_COCKPIT_3_R, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_3_L, _T("Cockpit-region-3-l"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_3_L_EDIT, SPN_COCKPIT_3_L, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_3_B, _T("Cockpit-region-3-b"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_3_B_EDIT, SPN_COCKPIT_3_B, 1,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_CockpitRegion_4_T, _T("Cockpit-region-4-t"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_4_T_EDIT, SPN_COCKPIT_4_T, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_4_R, _T("Cockpit-region-4-r"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_4_R_EDIT, SPN_COCKPIT_4_R, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_4_L, _T("Cockpit-region-4-l"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_4_L_EDIT, SPN_COCKPIT_4_L, 1,
                                   p_end,

                                   MainObjAttr_CockpitRegion_4_B, _T("Cockpit-region-4-b"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_range, 0, 4096,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttrCockpit, TYPE_SPINNER, EDITTYPE_POS_INT,
                                   SPN_COCKPIT_4_B_EDIT, SPN_COCKPIT_4_B, 1,
                                   p_end,
                                   /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
                                   MainObjAttr_NoShadow, _T("No-shadow"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttr, TYPE_SINGLECHECKBOX, CHK_NOSHADOW,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_Tilted, _T("Tilted"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttr, TYPE_SINGLECHECKBOX, CHK_TILTED,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_SlopeLimitEnable, _T("Slope-limit"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttr, TYPE_SINGLECHECKBOX, CHK_SLOPELIMIT,
                                   p_enable_ctrls, 4, MainObjAttr_SlopeLimitMinPitch, MainObjAttr_SlopeLimitMaxPitch,
                                   MainObjAttr_SlopeLimitMinRoll, MainObjAttr_SlopeLimitMaxRoll,
                                   p_end,

                                   MainObjAttr_SlopeLimitMinPitch, _T("Slope-limit-min-pitch"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, 0.0f, 999999999.9f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_FLOAT, SPN_SLOPELIMIT_PITCHMIN_EDIT, SPN_SLOPELIMIT_PITCHMIN, SPIN_AUTOSCALE,
                                   p_end,

                                   MainObjAttr_SlopeLimitMaxPitch, _T("Slope-limit-max-pitch"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, 0.0f, 999999999.9f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_FLOAT, SPN_SLOPELIMIT_PITCHMAX_EDIT, SPN_SLOPELIMIT_PITCHMAX, SPIN_AUTOSCALE,
                                   p_end,

                                   MainObjAttr_SlopeLimitMinRoll, _T("Slope-limit-min-roll"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, 0.0f, 999999999.9f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_FLOAT, SPN_SLOPELIMIT_ROLLMIN_EDIT, SPN_SLOPELIMIT_ROLLMIN, SPIN_AUTOSCALE,
                                   p_end,

                                   MainObjAttr_SlopeLimitMaxRoll, _T("Slope-limit-max-roll"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, 0.0f, 999999999.9f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_FLOAT, SPN_SLOPELIMIT_ROLLMAX_EDIT, SPN_SLOPELIMIT_ROLLMAX, SPIN_AUTOSCALE,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_SlungLoadWeightEnable, _T("Slung-load"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttr, TYPE_SINGLECHECKBOX, CHK_SLUNGLOAD,
                                   p_enable_ctrls, 1, MainObjAttr_SlungLoadWeight,
                                   p_end,

                                   MainObjAttr_SlungLoadWeight, _T("Slung-load-weight"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, 0.0f, 999999999.9f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_FLOAT, SPN_SLUNGLOAD_EDIT, SPN_SLUNGLOAD, SPIN_AUTOSCALE,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_LayerGroupLayer, _T("Layer-group-type"), TYPE_STRING, 0, NO_IDS,
                                   p_default, 0,
                                   p_accessor, &gMainObjAttrCallback,
                                   p_end,
                                   // 3d max 9 does not support 
                                   //p_ui, RollGlobAttr, TYPE_INT_COMBOBOX,
                                   //CMB_LAYERGROUP, 0,
                                   //p_default, 0,
                                   //p_accessor, &gMainObjAttrCallback,
                                   //p_end,

                                   MainObjAttr_LayerGroupOffset, _T("Layer-group-offset"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, -5, 5,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_INT, SPN_LAYERGROUP_EDIT, SPN_LAYERGROUP, SPIN_AUTOSCALE,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_LodDrapEnable, _T("Lod-draped"), TYPE_BOOL, 0, NO_IDS,
                                   p_default, FALSE,
                                   p_ui, RollGlobAttr, TYPE_SINGLECHECKBOX, CHK_LODDRAPED,
                                   p_enable_ctrls, 1, MainObjAttr_LodDrapDistance,
                                   p_end,

                                   MainObjAttr_LodDrapDistance, _T("Lod-draped-distance"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, 0.0f, 999999999.9f,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_POS_UNIVERSE, SPN_LODDRAPED_DIST_EDIT, SPN_LODDRAPED_DIST, 1.0f,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_LayerGroupDrapedLayer, _T("Layer-group-draped-type"), TYPE_STRING, 0, NO_IDS,
                                   p_default, 0,
                                   p_accessor, &gMainObjAttrCallback,
                                   p_end,
                                   // 3d max 9 does not support 
                                   //p_ui, RollGlobAttr, TYPE_INT_COMBOBOX,
                                   //CMB_LAYERGROUP_DRAPED, 0,
                                   //p_default, 0,
                                   //p_accessor, &gMainObjAttrCallback,
                                   //p_end,

                                   MainObjAttr_LayerGroupDrapedOffset, _T("Layer-group-draped-offset"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0.0f,
                                   p_range, -5, 5,
                                   p_dim, &gDirectDim,
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_INT, SPN_LAYERGROUP_DRAPED_EDIT, SPN_LAYERGROUP_DRAPED, SPIN_AUTOSCALE,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   MainObjAttr_DryWet, _T("Wet-Dry"), TYPE_INT, 0, NO_IDS,
                                   p_default, 0,
                                   p_accessor, &gMainObjAttrCallback,
                                   p_end,
                                   // 3d max 9 does not support 
                                   //p_ui, RollGlobAttr, TYPE_INT_COMBOBOX,
                                   //CMB_WETDRY, 3,
                                   //IDS_WETDRY_DISABLE,
                                   //IDS_WETDRY_WET,
                                   //IDS_WETDRY_DRY,
                                   //p_default, 0,
                                   //p_end,
                                   //-------------------------------------------------------------------------
                                   p_end);

static ParamBlockDesc2 gMainExpPb(MainObjExpParams, _T("X-Obj-Options"), 0, ClassesDescriptions::mainObj(), P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                  EXP_PB_VERSION, MainObjExpParamsOrder,
                                  //-------------------------------------------------------------------------
                                  // Rollouts
                                  ROLL_MAINOBJ_EXP, IDS_ROLL_MAINOBJ_EXP, 0, APPENDROLL_CLOSED, NULL,
                                  //-------------------------------------------------------------------------
                                  // Export Params
                                  MainObjAttr_ExportEnable, _T("Enable-export"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, TRUE,
                                  p_end,

                                  MainObjExp_Meshes, _T("Enable-mesh"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, TRUE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_MESH,
                                  p_end,

                                  MainObjExp_Lines, _T("Enable-lines"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_LINE,
                                  p_end,

                                  MainObjExp_Lights, _T("Enable-lights"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, TRUE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_LIGHT,
                                  p_end,

                                  MainObjExp_Animation, _T("Enable-animation"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, TRUE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_ANIM,
                                  p_end,

                                  MainObjExp_Optimisation, _T("Enable-optimization"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, TRUE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_OPTIMISATION,
                                  p_end,

                                  MainObjExp_Instancing, _T("Enable-instancing"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_INSTANCING,
                                  p_end,

                                  MainObjExp_Debug, _T("Enable-debug"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_DEBUG,
                                  p_end,
                                  //-------------------------------------------------------------------------
                                  MainObj_ScaleValue, _T("Scale-val"), TYPE_FLOAT, 0, NO_IDS,
                                  p_default, 1.0f,
                                  p_range, 0.0f, 999999999.9f,
                                  p_dim, &gScaleDim,
                                  p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_SCALE_EDIT, SPN_SCALE, SPIN_AUTOSCALE,
                                  p_end,

                                  MainObj_ScaleEnabled, _T("Scale"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_enable_ctrls, 1, MainObj_ScaleValue,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_SCALE_ENABLE,
                                  p_end,
                                  //-------------------------------------------------------------------------
                                  MainObjExp_NameMeshes, _T("Name-mesh"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_MESH,
                                  p_end,

                                  MainObjExp_NameLines, _T("Name-lines"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_LINE,
                                  p_end,

                                  MainObjExp_NameLights, _T("Name-lights"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_LIGHT,
                                  p_end,

                                  MainObjExp_NameDummies, _T("Name-dummies"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_DUMMY,
                                  p_end,

                                  MainObjExp_TreeHierarchy, _T("Name-animation"), TYPE_BOOL, 0, NO_IDS,
                                  p_default, FALSE,
                                  p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_TREE, p_end,
                                  //-------------------------------------------------------------------------
                                  p_end);

static ParamBlockDesc2 gMainDispPb(MainObjDisplay, _T("X-Obj-Display"), 0, ClassesDescriptions::mainObj(), P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                   DISPLAY_PB_VERSION, MainObjDisplayOrder,
                                   //-------------------------------------------------------------------------
                                   // Rollouts
                                   ROLL_LODOBJ_DISPLAY, IDS_ROLL_LOD_DISPLAY, 0, APPENDROLL_CLOSED, NULL,
                                   //-------------------------------------------------------------------------
                                   // Display								
                                   MainObjDisp_IconScale, _T("IconScale"), TYPE_FLOAT, 0, NO_IDS,
                                   p_default, 1.0f,
                                   p_range, 0.01f, 1000.0f,
                                   p_accessor, &gMainObjUiCallback,
                                   p_dim, &gScaleDim,
                                   p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, IDC_SCALE_EDIT, IDC_SCALE_SPIN, SPIN_AUTOSCALE,
                                   p_end,
                                   //-------------------------------------------------------------------------
                                   p_end);

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

MainObject::MainObject() {
    mDesc = ClassesDescriptions::mainObj();
    mDesc->MakeAutoParamBlocks(this);
    mObjColor = Point3(1.0, 0.7, 0.4);
    makeIcon();
}

MainObject::~MainObject() {}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

CreateMouseCallBack * MainObject::GetCreateMouseCallBack() {
    return &mMouseCallback;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::updateTexturesButtons() const {
    MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, GetCOREInterface()->GetTime(), FOREVER);
    updateButtonText(mAttrParamsPb, MainObjAttr_Texture, sts::toString(wrapper.texture()).c_str());
    updateButtonText(mAttrParamsPb, MainObjAttr_TextureLit, sts::toString(wrapper.textureLit()).c_str());
    updateButtonText(mAttrParamsPb, MainObjAttr_TextureNormal, sts::toString(wrapper.textureNormal()).c_str());
}

void MainObject::updateButtonText(IParamBlock2 * pblock, ParamID param, const MCHAR * value) {
    IParamMap2 * map = pblock->GetMap(RollGlobShading);
    //DbgAssert(map);
    if (map) {
        TSTR p, f, e, name;
        ParamDef & mapdef = pblock->GetParamDef(param);
        static TCHAR cubemapFile[MAX_PATH];
        _tcscpy(cubemapFile, value ? value : _M("none"));
        TSTR tooltip = cubemapFile;
        mapdef.init_file = cubemapFile;
        TSTR tstr(cubemapFile);
        SplitFilename(tstr, &p, &f, &e);
        name = f + e;
        map->SetText(param, name);
        map->SetTooltip(param, true, tooltip);
    }
}

//-------------------------------------------------------------------------

void MainObject::updateBlendSpinEnabling() const {
    IParamMap2 * map = mAttrParamsPb->GetMap(RollGlobShading);
    //DbgAssert(map);
    if (map) {
        MainObjParamsWrapper wrapper(mAttrParamsPb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
        map->Enable(MainObjAttr_BlendingRatio, wrapper.blend() ? 1 : 0);
    }
}

//-------------------------------------------------------------------------

void MainObject::updateLyerGroupSpinEnabling() const {
    IParamMap2 * map = mAttrParamsPb->GetMap(RollGlobAttr);
    //DbgAssert(map);
    if (map) {
        MainObjParamsWrapper wrapper(mAttrParamsPb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
        map->Enable(MainObjAttr_LayerGroupOffset, wrapper.layerGroup().layer().isValid() ? 1 : 0);
        map->Enable(MainObjAttr_LayerGroupDrapedOffset, wrapper.drapedLayerGroup().layer().isValid() ? 1 : 0);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::loadMdDisplayObj(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());

    if (id != bcw::SerializationId(0x7f084ca4, 0x1f19134e)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    uint8_t version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    float iconSize;
    stream >> iconSize;
    if (!mDisplayPb->SetValue(MainObjDisp_IconScale, mIp ? mIp->GetTime() : 0, iconSize)) {
        DLError << "Can't save " << TOTEXT(MainObjDisp_IconScale) << " value to the param block";
    }
}

void MainObject::loadRawGlobAttr(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());
    if (id != bcw::SerializationId(0x37af3f56, 0x7eae24ae)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    struct {
        double noBlendRatio;
        double shadowBlendRatio;
        double specularRatio;

        double tintAlbedo;
        double tintEmissive;

        double slopePitchMax;
        double slopePitchMin;
        double slopeRollMax;
        double slopeRollhMin;

        double slungLoad;
        int layerGroup;
        double lodDrapedDist;
        int drapedLayerGroup;

        std::wstring strLayerGrpup;
        std::wstring strDrapedLayerGrpup;

        bool chkNoShadow;
        bool chkTilted;
        bool chkCockpitLit;
        bool chkDry;
        bool chkWet;

        bool chkNoBlend;
        bool chkShadowBlend;
        bool chkSpecular;
        bool chkTint;
        bool chkSlopeLimit;
        bool chkSlungLoad;
        bool chkLodDraped;

        bool chkLayerGroup;
        bool chkDrapedLayerGroup;

        std::wstring strDiffuse;
        std::wstring strLit;
        std::wstring strBump;
    } data;

    stream >> data.noBlendRatio;
    stream >> data.shadowBlendRatio;
    stream >> data.specularRatio;

    stream >> data.tintAlbedo;
    stream >> data.tintEmissive;

    stream >> data.slopePitchMax;
    stream >> data.slopePitchMin;
    stream >> data.slopeRollMax;
    stream >> data.slopeRollhMin;

    stream >> data.slungLoad;
    stream >> data.layerGroup;
    stream >> data.lodDrapedDist;
    stream >> data.drapedLayerGroup;

    stream >> data.strLayerGrpup;
    stream >> data.strDrapedLayerGrpup;

    stream >> data.chkNoShadow;
    stream >> data.chkTilted;
    stream >> data.chkCockpitLit;
    stream >> data.chkDry;
    stream >> data.chkWet;

    stream >> data.chkNoBlend;
    stream >> data.chkShadowBlend;
    stream >> data.chkSpecular;
    stream >> data.chkTint;
    stream >> data.chkSlopeLimit;
    stream >> data.chkSlungLoad;
    stream >> data.chkLodDraped;

    stream >> data.chkLayerGroup;
    stream >> data.chkDrapedLayerGroup;

    stream >> data.strDiffuse;
    stream >> data.strLit;
    stream >> data.strBump;

    //-------------------------------------------------------------------------
    if (data.chkNoBlend && data.chkShadowBlend) {
        std::string msg("\"shadow blend\" and \"no blend\" can not be enabled at same time. Only data about \"no blend\" will be loaded.");
        ui::win::DlgMessageBox::warning(nullptr, "[Backward Compatibility]", msg);
        LWarning << msg;
    }
    MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, TimeValue(0), FOREVER);
    if (data.chkNoBlend) {
        wrapper.setBlend(xobj::AttrBlend(xobj::AttrBlend::no_blend, float(data.noBlendRatio)));
    }
    else if (data.chkShadowBlend) {
        wrapper.setBlend(xobj::AttrBlend(xobj::AttrBlend::shadow_blend, float(data.noBlendRatio)));
    }
    else {
        wrapper.setBlend(xobj::AttrBlend());
    }
    //-------------------------------------------------------------------------
    if (data.chkSpecular) {
        wrapper.setSpecular(xobj::AttrSpecular(float(data.specularRatio)));
    }
    else {
        wrapper.setSpecular(xobj::AttrSpecular());
    }
    //-------------------------------------------------------------------------
    if (data.chkTint) {
        wrapper.setTint(xobj::AttrTint(float(data.tintAlbedo), float(data.tintEmissive)));
    }
    else {
        wrapper.setTint(xobj::AttrTint());
    }
    //-------------------------------------------------------------------------
    if (data.chkSlopeLimit) {
        wrapper.setSlopeLimit(xobj::AttrSlopeLimit(float(data.slopePitchMin), float(data.slopePitchMax),
                                                   float(data.slopeRollhMin), float(data.slopeRollMax)));
    }
    else {
        wrapper.setSlopeLimit(xobj::AttrSlopeLimit());
    }
    //-------------------------------------------------------------------------
    if (data.chkSlungLoad) {
        wrapper.setSlungWeight(xobj::AttrSlungLoadWeight(float(data.slungLoad)));
    }
    else {
        wrapper.setSlungWeight(xobj::AttrSlungLoadWeight());
    }
    //-------------------------------------------------------------------------
    if (data.chkLodDraped) {
        wrapper.setLodDrap(xobj::AttrLodDrap(float(data.lodDrapedDist)));
    }
    else {
        wrapper.setLodDrap(xobj::AttrLodDrap());
    }
    //-------------------------------------------------------------------------
    if (data.chkLayerGroup) {
        wrapper.setLayerGroup(xobj::AttrLayerGroup(xobj::ELayer::fromString(sts::toMbString(data.strLayerGrpup).c_str()),
                                                   int(data.layerGroup)));
    }
    else {
        wrapper.setLayerGroup(xobj::AttrLayerGroup());
    }
    //-------------------------------------------------------------------------
    if (data.chkDrapedLayerGroup) {
        wrapper.setDrapedLayerGroup(xobj::AttrDrapedLayerGroup(xobj::ELayer::fromString(sts::toMbString(data.strDrapedLayerGrpup).c_str()),
                                                               int(data.drapedLayerGroup)));
    }
    else {
        wrapper.setDrapedLayerGroup(xobj::AttrDrapedLayerGroup());
    }
    //-------------------------------------------------------------------------
    wrapper.setNoShadow(data.chkNoShadow);
    wrapper.setTilted(data.chkTilted);
    wrapper.setCockpitLit(data.chkCockpitLit);
    //-------------------------------------------------------------------------
    if (data.chkDry) {
        wrapper.setWetDry(xobj::AttrWetDry(xobj::AttrWetDry::dry));
    }
    else if (data.chkWet) {
        wrapper.setWetDry(xobj::AttrWetDry(xobj::AttrWetDry::wet));
    }
    else {
        wrapper.setWetDry(xobj::AttrWetDry());
    }
    //-------------------------------------------------------------------------
    wrapper.setTexture(sts::toMbString(data.strDiffuse));
    wrapper.setTextureLit(sts::toMbString(data.strLit));
    wrapper.setTextureNormal(sts::toMbString(data.strBump));
    //-------------------------------------------------------------------------
}

void MainObject::loadRawExpOption(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());
    if (id != bcw::SerializationId(0xf34588f, 0x1863592b)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    //--------------------------
    uint64_t flags = 0;
    double scale = 0.0;
    stream >> flags;
    stream >> scale;
    //--------------------------
    {
        bcw::SerializationId idx;
        idx.load(stream.getStdStream());
        if (idx != bcw::SerializationId(0x3b0318d2, 0x7b7e3761)) {
            LError << "Unexpected data input: " << idx.toString();
            return;
        }
        unsigned char verx;
        stream >> verx;
        if (version != 1) {
            LError << "Unexpected data version: " << version;
            return;
        }

        int32_t xVersion = 0;
        stream >> xVersion;

        std::string signature;
        uint64_t oFlags = 0;
        stream >> oFlags;
        stream >> signature;

        enum {
            EXP_NO_OPT = 0,
            EXP_MARK_MESH = 1 << 0,
            EXP_MARK_LINE = 1 << 1,
            EXP_MARK_DUMMY = 1 << 2,
            EXP_MARK_LIGHT = 1 << 3,
            EXP_MARK_TREE = 1 << 4,
            EXP_MARK_COMMENTS = 1 << 5,
            EXP_MARK_TRANSFORM = 1 << 6,
            EXP_MARK_VERTEX = 1 << 7,
            EXP_OBJ_MESH = 1 << 11,
            EXP_OBJ_LINE = 1 << 12,
            EXP_OBJ_DUMMY = 1 << 13,
            EXP_OBJ_LIGHT = 1 << 14,
            EXP_ANIMATION = 1 << 21,
            EXP_OPTIMIZATION = 1 << 22,
            EXP_CHECK_INSTANCE = 1 << 23,
            EXP_DEBUG = 1 << 24,
        };

        if (mExpPb) {
            MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, TimeValue(0), FOREVER);
            //-------------------------------------------------------------------------
            wrapper.setMeshExport((oFlags & EXP_OBJ_MESH) == EXP_OBJ_MESH);
            wrapper.setLinesExport(false);
            wrapper.setLightsExport((oFlags & EXP_OBJ_LIGHT) == EXP_OBJ_LIGHT);
            wrapper.setAnimationExport((oFlags & EXP_ANIMATION) == EXP_ANIMATION);
            //-------------------------------------------------------------------------
            wrapper.setNameMesh((oFlags & EXP_MARK_MESH) == EXP_MARK_MESH);
            wrapper.setNameLines((oFlags & EXP_MARK_LINE) == EXP_MARK_LINE);
            wrapper.setNameLights((oFlags & EXP_MARK_LIGHT) == EXP_MARK_LIGHT);
            wrapper.setNameDummies((oFlags & EXP_MARK_DUMMY) == EXP_MARK_DUMMY);
            wrapper.setTreeHierarchy((oFlags & EXP_MARK_TREE) == EXP_MARK_TREE);
            //-------------------------------------------------------------------------
            if (!stsff::math::isEqual(scale, 1.0)) {
                wrapper.setScale(true, float(scale));
            }
            //-------------------------------------------------------------------------
        }
    }
}

void MainObject::load186(std::vector<char> & inByteArray) {
    LMessage << "Object: <" << sts::toMbString(GetObjectName()) << "> has got data from previous version.";
    std::stringbuf buf(std::string(reinterpret_cast<char*>(inByteArray.data()), inByteArray.size()));
    sts_bwc::DataStream stream(buf);

    std::streampos pos = stream.getStdStream().tellg();
    uint8_t version;
    stream >> version; // 64 >= when incorrect version
    stream.getStdStream().seekg(pos);
    if (version != 0) {
        loadRawExpOption(stream);
        loadRawGlobAttr(stream);
    }
    else {
        uint8_t verx;
        stream >> verx;
        loadRawExpOption(stream);
        loadRawGlobAttr(stream);
        loadMdDisplayObj(stream);
    }
}

IOResult MainObject::Load(ILoad * iload) {
    ULONG temp;
    uint32_t dataSize = 0;
    IOResult res;
    std::vector<char> ba;
    while ((res = iload->OpenChunk()) == IO_OK) {
        switch (iload->CurChunkID()) {
            case 0:
                iload->Read(&dataSize, sizeof(dataSize), &temp);
                break;
            case 1: {
                if (dataSize != 0) {
                    ba.resize(dataSize);
                    iload->Read(ba.data(), dataSize, &temp);
                }
                break;
            }
            default: break;
        }
        iload->CloseChunk();
        if (res != IO_OK)
            return res;
    }
    if (!ba.empty()) {
        try {
            load186(ba);
        }
        catch (std::exception & e) {
            std::string msg("Can't load data of the <");
            msg.append(sts::toMbString(GetObjectName())).append("> reason: <").append(e.what()).append(">.");
            DLError << msg;
        }
        catch (...) {
            std::string msg("Can't load data of the <");
            msg.append(sts::toMbString(GetObjectName())).append("> reason: <unknown exception>.");
            DLError << msg;
        }
    }

    iload->RegisterPostLoadCallback(new MainObjectPostLoadCallback(this));
    return IO_OK;
}

IOResult MainObject::Save(ISave * /*isave*/) {
    return IO_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::BeginEditParams(IObjParam * ip, ULONG flags, Animatable * prev) {
    mIp = ip;
    mEditOb = this;
    mDesc->BeginEditParams(ip, this, flags, prev);
    updateTexturesButtons();
    updateBlendSpinEnabling();
    updateLyerGroupSpinEnabling();
}

void MainObject::EndEditParams(IObjParam * ip, ULONG flags, Animatable * next) {
    mEditOb = nullptr;
    mIp = nullptr;
    mDesc->EndEditParams(ip, this, flags, next);
    ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState MainObject::Eval(TimeValue) { return ObjectState(this); }
Object * MainObject::ConvertToType(TimeValue, Class_ID) { return nullptr; }
int MainObject::CanConvertToType(Class_ID) { return FALSE; }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::GetWorldBoundBox(TimeValue, INode * inode, ViewExp *, Box3 & box) {
    Matrix3 tm = inode->GetObjectTM(GetCOREInterface()->GetTime());
    box = mIconMesh.getBoundingBox() * tm;
}

void MainObject::GetLocalBoundBox(TimeValue, INode *, ViewExp *, Box3 & box) {
    box = mIconMesh.getBoundingBox();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 14
const MCHAR * MainObject::GetObjectName() { return _T("X-Obj"); }
#else
TCHAR * MainObject::GetObjectName() { return _T("X-Obj"); }
#endif

void MainObject::InitNodeName(TSTR & s) { s = _T("X-Obj-"); }

//-------------------------------------------------------------------------

Class_ID MainObject::ClassID() { return mDesc->ClassID(); }
SClass_ID MainObject::SuperClassID() { return mDesc->SuperClassID(); }
void MainObject::GetClassName(TSTR & s) { s = mDesc->ClassName(); }

RefTargetHandle MainObject::Clone(RemapDir & remap) {
    MainObject * newob = new MainObject();
    newob->ReplaceReference(MainObjAttrParamsOrder, mAttrParamsPb->Clone(remap));
    newob->ReplaceReference(MainObjExpParamsOrder, mExpPb->Clone(remap));
    newob->ReplaceReference(MainObjDisplayOrder, mDisplayPb->Clone(remap));
    BaseClone(this, newob, remap);
    return (newob);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * MainObject::SubAnim(int i) { return GetParamBlock(i); }

TSTR MainObject::SubAnimName(int i) {
    switch (i) {
        case MainObjAttrParamsOrder: return _T("Attributes");
        case MainObjExpParamsOrder: return _T("Options");
        case MainObjDisplayOrder: return _T("Display");
        default: return _T("");
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::GetParamBlockIndex(int id) { return id; }
int MainObject::NumParamBlocks() { return 3; }

IParamBlock2 * MainObject::GetParamBlock(int i) {
    switch (i) {
        case MainObjAttrParamsOrder: return mAttrParamsPb;
        case MainObjExpParamsOrder: return mExpPb;
        case MainObjDisplayOrder: return mDisplayPb;
        default: return nullptr;
    }
}

IParamBlock2 * MainObject::GetParamBlockByID(BlockID id) {
    switch (id) {
        case MainObjAttrParams: return mAttrParamsPb;
        case MainObjExpParams: return mExpPb;
        case MainObjDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::NumRefs() { return 3; }

RefTargetHandle MainObject::GetReference(int i) {
    switch (i) {
        case MainObjAttrParamsOrder: return mAttrParamsPb;
        case MainObjExpParamsOrder: return mExpPb;
        case MainObjDisplayOrder: return mDisplayPb;
        default: return nullptr;
    }
}

void MainObject::SetReference(int i, RefTargetHandle rtarg) {
    switch (i) {
        case MainObjAttrParamsOrder: {
            mAttrParamsPb = static_cast<IParamBlock2*>(rtarg);
            break;
        }
        case MainObjExpParamsOrder: {
            mExpPb = static_cast<IParamBlock2*>(rtarg);
            break;
        }
        case MainObjDisplayOrder: {
            mDisplayPb = static_cast<IParamBlock2*>(rtarg);
            break;
        }
        default: break;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 16
RefResult MainObject::NotifyRefChanged(const Interval & /*changeInt*/, RefTargetHandle /*hTarget*/,
                                       PartID & /*partID*/, RefMessage message, BOOL /*propagate*/) {
#else
RefResult MainObject::NotifyRefChanged(Interval /*changeInt*/, RefTargetHandle /*hTarget*/,
                                       PartID & /*partID*/, RefMessage message) {
#endif
    switch (message) {
        case REFMSG_CHANGE:
            if (mEditOb == this) {
                gMainAttrPb.InvalidateUI(mAttrParamsPb->LastNotifyParamID());
                gMainExpPb.InvalidateUI(mExpPb->LastNotifyParamID());
                gMainDispPb.InvalidateUI(mDisplayPb->LastNotifyParamID());
            }
            break;
        default: break;
    }
    return REF_SUCCEED;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::HitTest(TimeValue t, INode * inode, int type, int crossing, int flags, IPoint2 * p, ViewExp * vpt) {
    HitRegion hitRegion;
    DWORD savedLimits;
    int res = 0;
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    MakeHitRegion(hitRegion, type, crossing, 4, p);
    gw->setRndLimits(((savedLimits = gw->getRndLimits()) | GW_PICK) & ~GW_ILLUM);
    Matrix3 m = inode->GetObjectTM(t);
    gw->setTransform(m);
    gw->clearHitCode();
    if (mIconMesh.select(gw, mtl, &hitRegion, flags & HIT_ABORTONHIT)) {
        return TRUE;
    }
    gw->clearHitCode();
    gw->setRndLimits(savedLimits);
    return res;
}

//-------------------------------------------------------------------------

int MainObject::UsesWireColor() { return TRUE; }

//-------------------------------------------------------------------------

int MainObject::Display(TimeValue t, INode * inode, ViewExp * vpt, int /*flags*/) {
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    Color color(inode->GetWireColor());
    mObjColor.x = color.r;
    mObjColor.y = color.g;
    mObjColor.z = color.b;
    gw->setTransform(inode->GetNodeTM(t));
    //-------------------------------------------------------------------------
    DWORD rlim = gw->getRndLimits();
    gw->setRndLimits(GW_WIREFRAME | GW_EDGES_ONLY | GW_BACKCULL | (rlim & GW_Z_BUFFER));

    if (inode->Selected()) {
        gw->setColor(LINE_COLOR, GetSelColor());
    }
    else if (!inode->IsFrozen() && !inode->Dependent()) {
        gw->setColor(LINE_COLOR, mObjColor);
    }
    mIconMesh.render(gw, mtl, nullptr, COMP_ALL);
    gw->setRndLimits(rlim);
    //-------------------------------------------------------------------------
    return 0;
}

//-------------------------------------------------------------------------

void MainObject::makeIcon() {
    float size = 1.0f;
    Interval interval = FOREVER;
    if (!mDisplayPb->GetValue(MainObjDisp_IconScale, mIp ? mIp->GetTime() : 0, size, interval)) {
        LError << "Can't retrieve scale value from param block";
    }

    if (stsff::math::isEqual(mLastIconScale, size, 0.001f) && mIconMesh.getNumVerts() != 0) {
        return;
    }

    float masterScale = static_cast<float>(GetMasterScale(UNITS_METERS));
    if (masterScale != -1.0f) {
        masterScale = 1.0f / masterScale;
        size = size * masterScale;
        if (size < 0.00001f) {
            size = 0.00001f;
            LError << "The icon scale is too small";
        }
    }

    mLastIconScale = size;
    MainObjIcon::fillMesh(mIconMesh, size);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
