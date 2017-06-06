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

#pragma warning(push, 0)
#include <iparamb2.h>
#include <iparamm2.h>
#include <Windows.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "MainObjectParams.h"
#include "ScaleDim.h"
#include "DirectDim.h"
#include "common/Logger.h"
#include "common/String.h"
#include "sts/utilities/Compare.h"
#include <xpln/enums/ELayer.h>
#include "models/bwc/SerializationId.h"
#include <xpln/obj/attributes/AttrBlend.h>
#include <xpln/obj/attributes/AttrWetDry.h>
#include "ui/DlgMessageBox.h"
#include "MainObjParamsWrapper.h"
#include "resource/ResHelper.h"
#include "classes-desc/ClassesDescriptions.h"

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
									// 3d max 9 doesnot support 
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
									// 3d max 9 doesnot support 
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
									// 3d max 9 doesnot support 
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
									// 3d max 9 doesnot support 
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

								MainObjExp_Optimisation, _T("Enable-optimisation"), TYPE_BOOL, 0, NO_IDS,
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
	ClassesDescriptions::mainObj()->MakeAutoParamBlocks(this);
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
		ui::DlgMessageBox::warning(nullptr, "[Backward Compatibility]", msg);
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
			if (!sts::isEqual(scale, 1.0)) {
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
	ClassesDescriptions::mainObj()->BeginEditParams(ip, this, flags, prev);
	updateTexturesButtons();
	updateBlendSpinEnabling();
	updateLyerGroupSpinEnabling();
}

void MainObject::EndEditParams(IObjParam * ip, ULONG flags, Animatable * next) {
	mEditOb = nullptr;
	mIp = nullptr;
	ClassesDescriptions::mainObj()->EndEditParams(ip, this, flags, next);
	ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState MainObject::Eval(TimeValue /*t*/) {
	return ObjectState(this);
}

Object * MainObject::ConvertToType(TimeValue /*t*/, Class_ID /*obtype*/) {
	return nullptr;
}

int MainObject::CanConvertToType(Class_ID /*obtype*/) {
	return FALSE;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::GetWorldBoundBox(TimeValue /*t*/, INode * inode, ViewExp * /*vpt*/, Box3 & box) {
	Matrix3 tm = inode->GetObjectTM(GetCOREInterface()->GetTime());
	box = mIconMesh.getBoundingBox() * tm;
}

void MainObject::GetLocalBoundBox(TimeValue /*t*/, INode * /*inode*/, ViewExp * /*vpt*/, Box3 & box) {
	box = mIconMesh.getBoundingBox();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 14

const MCHAR * MainObject::GetObjectName() {
	return _T("X-Obj");
}

#else

TCHAR * MainObject::GetObjectName() {
	return _T("X-Obj");
}

#endif

//-------------------------------------------------------------------------

Class_ID MainObject::ClassID() {
	return ClassesDescriptions::mainObj()->ClassID();
}

SClass_ID MainObject::SuperClassID() {
	return HELPER_CLASS_ID;
}

void MainObject::GetClassName(TSTR & s) {
	s = _T("X-Obj");
}

RefTargetHandle MainObject::Clone(RemapDir & remap) {
	MainObject * newob = new MainObject();
	newob->ReplaceReference(MainObjAttrParamsOrder, mAttrParamsPb->Clone(remap));
	newob->ReplaceReference(MainObjExpParamsOrder, mExpPb->Clone(remap));
	newob->ReplaceReference(MainObjDisplayOrder, mDisplayPb->Clone(remap));
	//newob->ivalid.SetEmpty();
	BaseClone(this, newob, remap);
	return (newob);
}

void MainObject::InitNodeName(TSTR & s) {
	s = _T("X-Obj-");
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * MainObject::SubAnim(int i) {
	switch (i) {
		case MainObjAttrParamsOrder: return mAttrParamsPb;
		case MainObjExpParamsOrder: return mExpPb;
		case MainObjDisplayOrder: return mDisplayPb;
		default: return nullptr;
	}
}

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

int MainObject::GetParamBlockIndex(int id) {
	return id;
}

int MainObject::NumParamBlocks() {
	return 3;
}

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

int MainObject::NumRefs() {
	return 3;
}

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
	Matrix3 m;
	GraphicsWindow * gw = vpt->getGW();
	Material * mtl = gw->getMaterial();
	MakeHitRegion(hitRegion, type, crossing, 4, p);
	gw->setRndLimits(((savedLimits = gw->getRndLimits()) | GW_PICK) & ~GW_ILLUM);
	m = inode->GetObjectTM(t);
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

int MainObject::UsesWireColor() {
	return TRUE;
}

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

	if (sts::isEqual(mLastIconScale, size, 0.001f) && mIconMesh.getNumVerts() != 0) {
		return;
	}

	// Because box size is 2 meters we make it 1 meter, if the icon is changed the scale must be corrected.
	size *= 0.5f;

	float masterScale = static_cast<float>(GetMasterScale(UNITS_METERS));
	if (masterScale != -1.0f) {
		masterScale = 1.0f / masterScale;
		size = size * masterScale;
		if (size < 0.000001f) {
			size = 0.000001f;
			LError << "The icon scale is too small";
		}
	}

	mLastIconScale = size;

	mIconMesh.setNumVerts(93);
	mIconMesh.setNumFaces(87);
	mIconMesh.setVert(0, size * Point3(-1.0, -1.0, -0.853554));
	mIconMesh.setVert(1, size * Point3(1.0, -1.0, -0.853554));
	mIconMesh.setVert(2, size * Point3(-1.0, 1.0, -0.853554));
	mIconMesh.setVert(3, size * Point3(1.0, 1.0, -0.853554));
	mIconMesh.setVert(4, size * Point3(-1.0, -1.0, 0.146446));
	mIconMesh.setVert(5, size * Point3(1.0, -1.0, 0.146446));
	mIconMesh.setVert(6, size * Point3(-1.0, 1.0, 0.146446));
	mIconMesh.setVert(7, size * Point3(1.0, 1.0, 0.146446));
	mIconMesh.setVert(8, size * Point3(-1.0, -1.0, 0.146446));
	mIconMesh.setVert(9, size * Point3(-1.0, 1.0, 0.146447));
	mIconMesh.setVert(10, size * Point3(-1.70711, 1.0, 0.853553));
	mIconMesh.setVert(11, size * Point3(-1.70711, -1.0, 0.853553));
	mIconMesh.setVert(12, size * Point3(1.0, -1.0, 0.146446));
	mIconMesh.setVert(13, size * Point3(-1.0, -1.0, 0.146446));
	mIconMesh.setVert(14, size * Point3(-1.0, -1.70711, 0.853553));
	mIconMesh.setVert(15, size * Point3(1.0, -1.70711, 0.853553));
	mIconMesh.setVert(16, size * Point3(-1.0, 1.0, 0.146446));
	mIconMesh.setVert(17, size * Point3(1.0, 1.0, 0.146447));
	mIconMesh.setVert(18, size * Point3(1.0, 1.70711, 0.853553));
	mIconMesh.setVert(19, size * Point3(-1.0, 1.70711, 0.853553));
	mIconMesh.setVert(20, size * Point3(1.0, 1.0, 0.146446));
	mIconMesh.setVert(21, size * Point3(1.0, -1.0, 0.146447));
	mIconMesh.setVert(22, size * Point3(1.70711, -1.0, 0.853553));
	mIconMesh.setVert(23, size * Point3(1.70711, 1.0, 0.853553));
	mIconMesh.setVert(24, size * Point3(-0.239903, 0.187985, 0.146446));
	mIconMesh.setVert(25, size * Point3(-0.346179, 0.303623, 0.146446));
	mIconMesh.setVert(26, size * Point3(-0.39022, 0.242289, 0.146446));
	mIconMesh.setVert(27, size * Point3(-0.314366, 0.153872, 0.146446));
	mIconMesh.setVert(28, size * Point3(0.138931, -0.292677, 0.146446));
	mIconMesh.setVert(29, size * Point3(0.239858, -0.2822, 0.146446));
	mIconMesh.setVert(30, size * Point3(0.30836, -0.251197, 0.146446));
	mIconMesh.setVert(31, size * Point3(0.354243, -0.192828, 0.146446));
	mIconMesh.setVert(32, size * Point3(0.372805, -0.110937, 0.146446));
	mIconMesh.setVert(33, size * Point3(0.342367, -0.0159931, 0.146446));
	mIconMesh.setVert(34, size * Point3(0.255318, 0.0408782, 0.146446));
	mIconMesh.setVert(35, size * Point3(0.320794, 0.0981927, 0.146446));
	mIconMesh.setVert(36, size * Point3(0.342482, 0.173909, 0.146446));
	mIconMesh.setVert(37, size * Point3(0.317943, 0.256686, 0.146446));
	mIconMesh.setVert(38, size * Point3(0.250091, 0.315292, 0.146446));
	mIconMesh.setVert(39, size * Point3(0.135079, 0.334329, 0.146446));
	mIconMesh.setVert(40, size * Point3(-0.0996502, 0.334329, 0.146446));
	mIconMesh.setVert(41, size * Point3(-0.0996502, -0.292677, 0.146446));
	mIconMesh.setVert(42, size * Point3(-0.016506, 0.259988, 0.146446));
	mIconMesh.setVert(43, size * Point3(0.108531, 0.259988, 0.146446));
	mIconMesh.setVert(44, size * Point3(0.203689, 0.251253, 0.146446));
	mIconMesh.setVert(45, size * Point3(0.246361, 0.218867, 0.146446));
	mIconMesh.setVert(46, size * Point3(0.261294, 0.164097, 0.146446));
	mIconMesh.setVert(47, size * Point3(0.245292, 0.109549, 0.146446));
	mIconMesh.setVert(48, size * Point3(0.197713, 0.0784459, 0.146446));
	mIconMesh.setVert(49, size * Point3(0.118772, 0.0712014, 0.146446));
	mIconMesh.setVert(50, size * Point3(-0.016506, 0.0712014, 0.146446));
	mIconMesh.setVert(51, size * Point3(-0.016506, -0.00313936, 0.146446));
	mIconMesh.setVert(52, size * Point3(0.128156, -0.00313936, 0.146446));
	mIconMesh.setVert(53, size * Point3(0.219691, -0.0136012, 0.146446));
	mIconMesh.setVert(54, size * Point3(0.269593, -0.0498927, 0.146446));
	mIconMesh.setVert(55, size * Point3(0.287704, -0.110738, 0.146446));
	mIconMesh.setVert(56, size * Point3(0.275317, -0.163475, 0.146446));
	mIconMesh.setVert(57, size * Point3(0.219775, -0.206805, 0.146446));
	mIconMesh.setVert(58, size * Point3(0.139451, -0.218336, 0.146446));
	mIconMesh.setVert(59, size * Point3(-0.016506, -0.218336, 0.146446));
	mIconMesh.setVert(60, size * Point3(0.487388, -0.256034, 0.146446));
	mIconMesh.setVert(61, size * Point3(0.613984, -0.303437, 0.146446));
	mIconMesh.setVert(62, size * Point3(0.711014, -0.280763, 0.146446));
	mIconMesh.setVert(63, size * Point3(0.771416, -0.21489, 0.146446));
	mIconMesh.setVert(64, size * Point3(0.790483, -0.0929933, 0.146446));
	mIconMesh.setVert(65, size * Point3(0.790483, 0.334329, 0.146446));
	mIconMesh.setVert(66, size * Point3(0.707338, 0.334329, 0.146446));
	mIconMesh.setVert(67, size * Point3(0.707338, -0.0974256, 0.146446));
	mIconMesh.setVert(68, size * Point3(0.683056, -0.195326, 0.146446));
	mIconMesh.setVert(69, size * Point3(0.61322, -0.229096, 0.146446));
	mIconMesh.setVert(70, size * Point3(0.546605, -0.202716, 0.146446));
	mIconMesh.setVert(71, size * Point3(0.51953, -0.104869, 0.146446));
	mIconMesh.setVert(72, size * Point3(0.445189, -0.115124, 0.146446));
	mIconMesh.setVert(73, size * Point3(-0.203336, 0.0197559, 0.146446));
	mIconMesh.setVert(74, size * Point3(-0.239903, -0.148474, 0.146446));
	mIconMesh.setVert(75, size * Point3(-0.314366, -0.11436, 0.146446));
	mIconMesh.setVert(76, size * Point3(-0.288437, 0.0197559, 0.146446));
	mIconMesh.setVert(77, size * Point3(-0.346179, -0.264111, 0.146446));
	mIconMesh.setVert(78, size * Point3(-0.39022, -0.202777, 0.146446));
	mIconMesh.setVert(79, size * Point3(-0.502717, -0.305577, 0.146446));
	mIconMesh.setVert(80, size * Point3(-0.76553, 0.187985, 0.146446));
	mIconMesh.setVert(81, size * Point3(-0.691067, 0.153872, 0.146446));
	mIconMesh.setVert(82, size * Point3(-0.659254, 0.303623, 0.146446));
	mIconMesh.setVert(83, size * Point3(-0.615214, 0.242289, 0.146446));
	mIconMesh.setVert(84, size * Point3(-0.502717, 0.345088, 0.146446));
	mIconMesh.setVert(85, size * Point3(-0.502717, 0.273682, 0.146446));
	mIconMesh.setVert(86, size * Point3(-0.802097, 0.0197559, 0.146446));
	mIconMesh.setVert(87, size * Point3(-0.716996, 0.0197559, 0.146446));
	mIconMesh.setVert(88, size * Point3(-0.691067, -0.11436, 0.146446));
	mIconMesh.setVert(89, size * Point3(-0.76553, -0.148474, 0.146446));
	mIconMesh.setVert(90, size * Point3(-0.615214, -0.202777, 0.146446));
	mIconMesh.setVert(91, size * Point3(-0.659254, -0.264111, 0.146446));
	mIconMesh.setVert(92, size * Point3(-0.502717, -0.23417, 0.146446));
	mIconMesh.faces[0].setVerts(5, 4, 0);
	mIconMesh.faces[0].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[0].setSmGroup(8);
	mIconMesh.faces[1].setVerts(0, 1, 5);
	mIconMesh.faces[1].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[1].setSmGroup(8);
	mIconMesh.faces[2].setVerts(7, 5, 1);
	mIconMesh.faces[2].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[2].setSmGroup(16);
	mIconMesh.faces[3].setVerts(1, 3, 7);
	mIconMesh.faces[3].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[3].setSmGroup(16);
	mIconMesh.faces[4].setVerts(6, 7, 3);
	mIconMesh.faces[4].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[4].setSmGroup(32);
	mIconMesh.faces[5].setVerts(3, 2, 6);
	mIconMesh.faces[5].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[5].setSmGroup(32);
	mIconMesh.faces[6].setVerts(4, 6, 2);
	mIconMesh.faces[6].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[6].setSmGroup(64);
	mIconMesh.faces[7].setVerts(2, 0, 4);
	mIconMesh.faces[7].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[7].setSmGroup(64);
	mIconMesh.faces[8].setVerts(11, 8, 9);
	mIconMesh.faces[8].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[8].setSmGroup(4);
	mIconMesh.faces[9].setVerts(9, 10, 11);
	mIconMesh.faces[9].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[9].setSmGroup(4);
	mIconMesh.faces[10].setVerts(15, 12, 13);
	mIconMesh.faces[10].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[10].setSmGroup(4);
	mIconMesh.faces[11].setVerts(13, 14, 15);
	mIconMesh.faces[11].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[11].setSmGroup(4);
	mIconMesh.faces[12].setVerts(19, 16, 17);
	mIconMesh.faces[12].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[12].setSmGroup(4);
	mIconMesh.faces[13].setVerts(17, 18, 19);
	mIconMesh.faces[13].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[13].setSmGroup(4);
	mIconMesh.faces[14].setVerts(23, 20, 21);
	mIconMesh.faces[14].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[14].setSmGroup(4);
	mIconMesh.faces[15].setVerts(21, 22, 23);
	mIconMesh.faces[15].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[15].setSmGroup(4);
	mIconMesh.faces[16].setVerts(59, 41, 28);
	mIconMesh.faces[16].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[16].setSmGroup(1);
	mIconMesh.faces[17].setVerts(28, 58, 59);
	mIconMesh.faces[17].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[17].setSmGroup(1);
	mIconMesh.faces[18].setVerts(60, 70, 71);
	mIconMesh.faces[18].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[18].setSmGroup(1);
	mIconMesh.faces[19].setVerts(71, 72, 60);
	mIconMesh.faces[19].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[19].setSmGroup(1);
	mIconMesh.faces[20].setVerts(76, 73, 24);
	mIconMesh.faces[20].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[20].setSmGroup(1);
	mIconMesh.faces[21].setVerts(24, 27, 76);
	mIconMesh.faces[21].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[21].setSmGroup(1);
	mIconMesh.faces[22].setVerts(27, 24, 25);
	mIconMesh.faces[22].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[22].setSmGroup(1);
	mIconMesh.faces[23].setVerts(25, 26, 27);
	mIconMesh.faces[23].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[23].setSmGroup(1);
	mIconMesh.faces[24].setVerts(85, 26, 25);
	mIconMesh.faces[24].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[24].setSmGroup(1);
	mIconMesh.faces[25].setVerts(25, 84, 85);
	mIconMesh.faces[25].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[25].setSmGroup(1);
	mIconMesh.faces[26].setVerts(76, 75, 74);
	mIconMesh.faces[26].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[26].setSmGroup(1);
	mIconMesh.faces[27].setVerts(74, 73, 76);
	mIconMesh.faces[27].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[27].setSmGroup(1);
	mIconMesh.faces[28].setVerts(75, 78, 77);
	mIconMesh.faces[28].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[28].setSmGroup(1);
	mIconMesh.faces[29].setVerts(77, 74, 75);
	mIconMesh.faces[29].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[29].setSmGroup(1);
	mIconMesh.faces[30].setVerts(92, 79, 77);
	mIconMesh.faces[30].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[30].setSmGroup(1);
	mIconMesh.faces[31].setVerts(77, 78, 92);
	mIconMesh.faces[31].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[31].setSmGroup(1);
	mIconMesh.faces[32].setVerts(87, 81, 80);
	mIconMesh.faces[32].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[32].setSmGroup(1);
	mIconMesh.faces[33].setVerts(80, 86, 87);
	mIconMesh.faces[33].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[33].setSmGroup(1);
	mIconMesh.faces[34].setVerts(81, 83, 82);
	mIconMesh.faces[34].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[34].setSmGroup(1);
	mIconMesh.faces[35].setVerts(82, 80, 81);
	mIconMesh.faces[35].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[35].setSmGroup(1);
	mIconMesh.faces[36].setVerts(85, 84, 82);
	mIconMesh.faces[36].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[36].setSmGroup(1);
	mIconMesh.faces[37].setVerts(82, 83, 85);
	mIconMesh.faces[37].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[37].setSmGroup(1);
	mIconMesh.faces[38].setVerts(87, 86, 89);
	mIconMesh.faces[38].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[38].setSmGroup(1);
	mIconMesh.faces[39].setVerts(89, 88, 87);
	mIconMesh.faces[39].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[39].setSmGroup(1);
	mIconMesh.faces[40].setVerts(88, 89, 91);
	mIconMesh.faces[40].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[40].setSmGroup(1);
	mIconMesh.faces[41].setVerts(91, 90, 88);
	mIconMesh.faces[41].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[41].setSmGroup(1);
	mIconMesh.faces[42].setVerts(92, 90, 91);
	mIconMesh.faces[42].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[42].setSmGroup(1);
	mIconMesh.faces[43].setVerts(91, 79, 92);
	mIconMesh.faces[43].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[43].setSmGroup(1);
	mIconMesh.faces[44].setVerts(66, 67, 64);
	mIconMesh.faces[44].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[44].setSmGroup(1);
	mIconMesh.faces[45].setVerts(64, 65, 66);
	mIconMesh.faces[45].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[45].setSmGroup(1);
	mIconMesh.faces[46].setVerts(70, 60, 61);
	mIconMesh.faces[46].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[46].setSmGroup(1);
	mIconMesh.faces[47].setVerts(61, 69, 70);
	mIconMesh.faces[47].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[47].setSmGroup(1);
	mIconMesh.faces[48].setVerts(69, 61, 62);
	mIconMesh.faces[48].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[48].setSmGroup(1);
	mIconMesh.faces[49].setVerts(62, 68, 69);
	mIconMesh.faces[49].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[49].setSmGroup(1);
	mIconMesh.faces[50].setVerts(68, 63, 64);
	mIconMesh.faces[50].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[50].setSmGroup(1);
	mIconMesh.faces[51].setVerts(64, 67, 68);
	mIconMesh.faces[51].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[51].setSmGroup(1);
	mIconMesh.faces[52].setVerts(62, 63, 68);
	mIconMesh.faces[52].setEdgeVisFlags(1, 1, 1);
	mIconMesh.faces[52].setSmGroup(1);
	mIconMesh.faces[53].setVerts(42, 40, 41);
	mIconMesh.faces[53].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[53].setSmGroup(1);
	mIconMesh.faces[54].setVerts(41, 59, 51);
	mIconMesh.faces[54].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[54].setSmGroup(1);
	mIconMesh.faces[55].setVerts(41, 51, 50);
	mIconMesh.faces[55].setEdgeVisFlags(0, 1, 0);
	mIconMesh.faces[55].setSmGroup(1);
	mIconMesh.faces[56].setVerts(42, 41, 50);
	mIconMesh.faces[56].setEdgeVisFlags(0, 0, 1);
	mIconMesh.faces[56].setSmGroup(1);
	mIconMesh.faces[57].setVerts(49, 50, 51);
	mIconMesh.faces[57].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[57].setSmGroup(1);
	mIconMesh.faces[58].setVerts(51, 52, 49);
	mIconMesh.faces[58].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[58].setSmGroup(1);
	mIconMesh.faces[59].setVerts(40, 42, 43);
	mIconMesh.faces[59].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[59].setSmGroup(1);
	mIconMesh.faces[60].setVerts(43, 39, 40);
	mIconMesh.faces[60].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[60].setSmGroup(1);
	mIconMesh.faces[61].setVerts(48, 49, 52);
	mIconMesh.faces[61].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[61].setSmGroup(1);
	mIconMesh.faces[62].setVerts(52, 53, 48);
	mIconMesh.faces[62].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[62].setSmGroup(1);
	mIconMesh.faces[63].setVerts(47, 48, 34);
	mIconMesh.faces[63].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[63].setSmGroup(1);
	mIconMesh.faces[64].setVerts(34, 35, 47);
	mIconMesh.faces[64].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[64].setSmGroup(1);
	mIconMesh.faces[65].setVerts(53, 34, 48);
	mIconMesh.faces[65].setEdgeVisFlags(1, 1, 1);
	mIconMesh.faces[65].setSmGroup(1);
	mIconMesh.faces[66].setVerts(34, 53, 54);
	mIconMesh.faces[66].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[66].setSmGroup(1);
	mIconMesh.faces[67].setVerts(54, 33, 34);
	mIconMesh.faces[67].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[67].setSmGroup(1);
	mIconMesh.faces[68].setVerts(46, 36, 37);
	mIconMesh.faces[68].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[68].setSmGroup(1);
	mIconMesh.faces[69].setVerts(37, 45, 46);
	mIconMesh.faces[69].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[69].setSmGroup(1);
	mIconMesh.faces[70].setVerts(45, 37, 38);
	mIconMesh.faces[70].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[70].setSmGroup(1);
	mIconMesh.faces[71].setVerts(38, 44, 45);
	mIconMesh.faces[71].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[71].setSmGroup(1);
	mIconMesh.faces[72].setVerts(44, 38, 39);
	mIconMesh.faces[72].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[72].setSmGroup(1);
	mIconMesh.faces[73].setVerts(39, 43, 44);
	mIconMesh.faces[73].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[73].setSmGroup(1);
	mIconMesh.faces[74].setVerts(46, 47, 35);
	mIconMesh.faces[74].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[74].setSmGroup(1);
	mIconMesh.faces[75].setVerts(35, 36, 46);
	mIconMesh.faces[75].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[75].setSmGroup(1);
	mIconMesh.faces[76].setVerts(32, 33, 54);
	mIconMesh.faces[76].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[76].setSmGroup(1);
	mIconMesh.faces[77].setVerts(54, 55, 32);
	mIconMesh.faces[77].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[77].setSmGroup(1);
	mIconMesh.faces[78].setVerts(31, 32, 55);
	mIconMesh.faces[78].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[78].setSmGroup(1);
	mIconMesh.faces[79].setVerts(55, 56, 31);
	mIconMesh.faces[79].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[79].setSmGroup(1);
	mIconMesh.faces[80].setVerts(30, 31, 56);
	mIconMesh.faces[80].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[80].setSmGroup(1);
	mIconMesh.faces[81].setVerts(56, 57, 30);
	mIconMesh.faces[81].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[81].setSmGroup(1);
	mIconMesh.faces[82].setVerts(57, 29, 30);
	mIconMesh.faces[82].setEdgeVisFlags(1, 1, 1);
	mIconMesh.faces[82].setSmGroup(1);
	mIconMesh.faces[83].setVerts(28, 29, 57);
	mIconMesh.faces[83].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[83].setSmGroup(1);
	mIconMesh.faces[84].setVerts(57, 58, 28);
	mIconMesh.faces[84].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[84].setSmGroup(1);
	mIconMesh.faces[85].setVerts(3, 1, 0);
	mIconMesh.faces[85].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[85].setSmGroup(0);
	mIconMesh.faces[86].setVerts(0, 2, 3);
	mIconMesh.faces[86].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[86].setSmGroup(0);

	//--------------------------------

	mIconMesh.InvalidateGeomCache();
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
