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

#include "MainObjPbAttr.h"

#include "resource/resource.h"
#include "objects/DirectDim.h"
#include "classes-desc/ClassesDescriptions.h"
#include "objects/main/MainObj.h"
#include "objects/main/MainObjParamsWrapper.h"
#include "objects/main/param-blocks/MainObjParams.h"

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

class MainObjAttrCallback : public PBAccessor {
public:

    virtual ~MainObjAttrCallback() = default;

    void Set(PB2Value & v, ReferenceMaker * owner, const ParamID id, const int tabIndex, const TimeValue t) override {
        auto u = dynamic_cast<MainObject*>(owner);
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
                u->updateLayerGroupSpinEnabling();
                break;
            case MainObjAttr_LayerGroupDrapedLayer:
                u->updateLayerGroupSpinEnabling();
                break;
            default: break;
        }
        PBAccessor::Set(v, owner, id, tabIndex, t);
    }
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class GlobShadingDlgProc : public ParamMap2UserDlgProc {
public:

    GlobShadingDlgProc() = default;
    virtual ~GlobShadingDlgProc() = default;

    INT_PTR DlgProc(const TimeValue t, IParamMap2 * map, const HWND hWnd, const UINT msg, const WPARAM wParam, LPARAM) override {
        auto ppb = map->GetParamBlock();
        switch (msg) {
            case WM_INITDIALOG: {
                const HWND cbmBlend = GetDlgItem(hWnd, CMB_BLEND);
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
                        MainObject::updateButtonText(ppb, MainObjAttr_Texture, sts::toString(wrapper.texture().value_or(std::string())).c_str());;
                        break;
                    }
                    case BTN_TEXTURE_LIT_DEL: {
                        MainObjParamsWrapper wrapper(ppb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
                        wrapper.setTextureLit("");
                        MainObject::updateButtonText(ppb, MainObjAttr_TextureLit, sts::toString(wrapper.textureLit().value_or(std::string())).c_str());;
                        break;
                    }
                    case BTN_TEXTURE_NORMAL_DEL: {
                        MainObjParamsWrapper wrapper(ppb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
                        wrapper.setTextureNormal("");
                        MainObject::updateButtonText(ppb, MainObjAttr_TextureNormal, sts::toString(wrapper.textureNormal().value_or(std::string())).c_str());;
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

class GlobAttrDlgProc : public ParamMap2UserDlgProc {
public:

    GlobAttrDlgProc() = default;
    virtual ~GlobAttrDlgProc() = default;

    INT_PTR DlgProc(const TimeValue t, IParamMap2 * map, const HWND hWnd, const UINT msg, const WPARAM wParam, LPARAM) override {
        auto ppb = map->GetParamBlock();
        switch (msg) {
            case WM_INITDIALOG: {
                const HWND cbmLayer = GetDlgItem(hWnd, CMB_LAYERGROUP);
                const HWND cbmLayerDrap = GetDlgItem(hWnd, CMB_LAYERGROUP_DRAPED);
                const HWND cbmWetDry = GetDlgItem(hWnd, CMB_WETDRY);
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
                    ComboBox_SetCurSel(cbmLayer, static_cast<int>(wrapper.layerGroup().value_or(xobj::AttrLayerGroup()).mLayer.id()));
                    ComboBox_SetCurSel(cbmLayerDrap, static_cast<int>(wrapper.drapedLayerGroup().value_or(xobj::AttrDrapedLayerGroup()).mLayer.id()));
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
                            const auto currLayer = xobj::ELayer(static_cast<xobj::ELayer::eId>(ComboBox_GetCurSel(GetDlgItem(hWnd, CMB_LAYERGROUP))));
                            if (currLayer == xobj::ELayer::none) {
                                wrapper.setLayerGroup(std::nullopt);
                            }
                            else {
                                auto attr = wrapper.layerGroup();
                                if (attr) {
                                    attr->mLayer = currLayer;
                                    wrapper.setLayerGroup(attr);
                                }
                            }
                        }
                        break;
                    }
                    case CMB_LAYERGROUP_DRAPED: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            MainObjParamsWrapper wrapper(ppb, nullptr, t, FOREVER);
                            const auto currLayer = xobj::ELayer(static_cast<xobj::ELayer::eId>(ComboBox_GetCurSel(GetDlgItem(hWnd, CMB_LAYERGROUP_DRAPED))));
                            if (currLayer == xobj::ELayer::none) {
                                wrapper.setDrapedLayerGroup(std::nullopt);
                            }
                            else {
                                auto attr = wrapper.drapedLayerGroup();
                                if (attr) {
                                    attr->mLayer = currLayer;
                                    wrapper.setDrapedLayerGroup(attr);
                                }
                            }
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

static MainObjAttrCallback gMainObjAttrCallback;
static GlobShadingDlgProc gGlobShadingDlgProc;
static GlobAttrDlgProc gGlobAttrDlgProc;

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

ParamBlockDesc2 MainObjPbAttr::mPb(static_cast<BlockID>(eMainObjParamsBlocks::MainObjAttrParams), _T("X-Obj-Attributes"), 0, ClassesDescriptions::mainObj(),
                                   P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION + P_MULTIMAP,
                                   PbVersionAttr, eMainObjPbOrder::PbOrderAttr, RollCount,
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
                                   p_ui, RollGlobAttr, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_LODDRAPED_DIST_EDIT, SPN_LODDRAPED_DIST, 1.0f,
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

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObjPbAttr::postLoad(IParamBlock2 * paramBlock) {
    DbgAssert(paramBlock);
    DbgAssert(paramBlock->ID() == static_cast<BlockID>(eMainObjParamsBlocks::MainObjAttrParams));

    const auto baseInterface = paramBlock->GetInterface(IPARAMBLOCK2POSTLOADINFO_ID);
    if (!baseInterface) {
        // block doesn't need updating.
        return;
    }

    auto postLoadInfo = dynamic_cast<IParamBlock2PostLoadInfo*>(baseInterface);
    if (!postLoadInfo) {
        LError << "Invalid cast";
        return;
    }

    if (postLoadInfo->GetVersion() == 1) {
        LInfo << "Updating Main Obj's attributes param block 1" << "->" << PbVersionAttr;
        //------------------------------------------------------
        // updating from system units to abstract float units.

        MainObjParamsWrapper wrapper(paramBlock, nullptr, GetCOREInterface()->GetTime(), FOREVER);
        const auto masterScale = GetMasterScale(UNITS_METERS);
        auto attrDrap = wrapper.lodDrap();
        if (attrDrap) {
            attrDrap->mDistance = float(attrDrap->mDistance * masterScale);
            wrapper.setLodDrap(attrDrap);
        }
        //------------------------------------------------------
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
