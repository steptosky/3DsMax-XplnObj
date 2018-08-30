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

#include "ObjAttr.h"
#include "models/MdObjAttr.h"
#include "UiUtilities.h"
#include "resource/resource.h"
#include "common/Config.h"
#include "resource/ResHelper.h"
#include "Factory.h"

namespace ui {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

INT_PTR ObjAttr::panelProc(HWND /*hWnd*/, UINT msg, WPARAM wParam, LPARAM /*lParam*/) {
    switch (msg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case BTN_LIGHTLEVEL_DRF: {
                    Factory::showNotImplemented();;
                    break;
                }
                case CMB_COCKPIT: {
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        dropFromUiCockpit();
                    }
                    break;
                }
                case CMB_HARD: {
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        dropFromUiHard();
                    }
                    break;
                }
                case CMB_BLEND: {
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        dropFromUiBlend();
                        enableBlend(true);
                    }
                    break;
                }
                    //----------------------
                case CHK_TREE: {
                    dropFromUiBool();
                    enableBool(true);
                    break;
                }
                case CHK_TWO_SIDED: {
                    dropFromUiBool();
                    enableBool(true);
                    break;
                }
                case CHK_DRAPED: {
                    dropFromUiBool();
                    enableBool(true);
                    break;
                }
                case CHK_SHADOW: {
                    dropFromUiBool();
                    enableBool(true);
                    break;
                }
                case CHK_SOLIDCAMERA: {
                    dropFromUiBool();
                    enableBool(true);
                    break;
                }
                case CHK_DRAW: {
                    dropFromUiBool();
                    enableBool(true);
                    break;
                }
                    //----------------------
                case CHK_SHINY: {
                    dropFromUiShiny();
                    enableShiny(true);
                    break;
                }
                case CHK_COCKPIT: {
                    dropFromUiCockpit();
                    enableCockpit(true);
                    break;
                }
                case CHK_HARD: {
                    dropFromUiHard();
                    enableHard(true);
                    break;
                }
                case CHK_HARD_DECK: {
                    dropFromUiHard();
                    enableHard(true);
                    break;
                }
                case CHK_POLYOFFSET: {
                    dropFromUiPolyOffset();
                    enablePolyOffset(true);
                    break;
                }
                case CHK_LIGHTLEVEL: {
                    dropFromUiLightLevel();
                    enableLightLevel(true);
                    break;
                }
                    //----------------------
                default: break;
            }
            break;
        }
        case WM_CUSTEDIT_ENTER: {
            if (LOWORD(wParam) == EDT_LIGHTLEVEL_DRF) {
                dropFromUiLightLevel();
            }
            break;
        }
        case CC_SPINNER_CHANGE: {
            switch (LOWORD(wParam)) {
                case SPN_SHINY: {
                    dropFromUiShiny();
                    break;
                }
                case SPN_BLEND: {
                    dropFromUiBlend();
                    break;
                }
                case SPN_POLYOFFSET_DIST: {
                    dropFromUiPolyOffset();
                    break;
                }
                case SPN_LIGHTLEVEL_V1: {
                    dropFromUiLightLevel();
                    break;
                }
                case SPN_LIGHTLEVEL_V2: {
                    dropFromUiLightLevel();
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

ObjAttr::ObjAttr()
    : RollupBase(ResHelper::hInstance) {}

ObjAttr::~ObjAttr() {
    ObjAttr::destroy();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::show(MdObjAttr * inOutData) {
    mData.reset();
    mMdData = inOutData;
    if (mMdData) {
        mMdData->loadFromNode(mData);
    }
    toWindow();
}

void ObjAttr::create(IRollupWindow * rollWin) {
    mIp = rollWin;
    create();
}

void ObjAttr::create() {
    createRollup(IDD_ROLL_BASEATTR_OBJ, _T("X Base attributes"), this);
    show(nullptr);
}

void ObjAttr::destroy() {
    if (this->hwnd() != nullptr) {
        deleteRollup();
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::initWindow(HWND hWnd) {
    cChkSunLight.setup(hWnd, CHK_TREE);
    cChkTwoSided.setup(hWnd, CHK_TWO_SIDED);
    cChkDraped.setup(hWnd, CHK_DRAPED);
    cChkShadow.setup(hWnd, CHK_SHADOW);
    cChkSolidCamera.setup(hWnd, CHK_SOLIDCAMERA);
    cChkDraw.setup(hWnd, CHK_DRAW);

    cChkShiny.setup(hWnd, CHK_SHINY);
    cSpnShinyRatio = SetupFloatSpinner(hWnd, SPN_SHINY, SPN_SHINY_EDIT, 0.0f, 1.0f, 0.5f, 0.01f);

    cChkCockpit.setup(hWnd, CHK_COCKPIT);
    cCmbCockpit.setup(hWnd, CMB_COCKPIT);

    cChkHard.setup(hWnd, CHK_HARD);
    cChkHardDeck.setup(hWnd, CHK_HARD_DECK);
    cCmbHard.setup(hWnd, CMB_HARD);

    cCmbBlend.setup(hWnd, CMB_BLEND);
    cSpnBlendRatio = SetupFloatSpinner(hWnd, SPN_BLEND, SPN_BLEND_EDIT, 0.0f, 1.0f, 0.5f, 0.01f);

    cChkPolyOffset.setup(hWnd, CHK_POLYOFFSET);
    cSpnPolyOstDist = SetupFloatSpinner(hWnd, SPN_POLYOFFSET_DIST, SPN_POLYOFFSET_DIST_EDIT, 0.0f, 999999.9f, 0.0f, 0.5f);

    cChkLightLevel.setup(hWnd, CHK_LIGHTLEVEL);
    cSpnLightLevelVal1 = SetupFloatSpinner(hWnd, SPN_LIGHTLEVEL_V1, SPN_LIGHTLEVEL_V1_EDIT, 0.0f, 999999.9f, 1.0f, 0.01f);
    cSpnLightLevelVal2 = SetupFloatSpinner(hWnd, SPN_LIGHTLEVEL_V2, SPN_LIGHTLEVEL_V2_EDIT, 0.0f, 999999.9f, 1.0f, 0.01f);
    cEdtLightLevelDrf = GetICustEdit(GetDlgItem(hWnd, EDT_LIGHTLEVEL_DRF));
    cBtnLightLevelDrf.setup(hWnd, BTN_LIGHTLEVEL_DRF);

    cCmbBlend.addItem(_T("Blend (default)"));
    cCmbBlend.addItem(_T("No Blend"));
    cCmbBlend.addItem(_T("Shadow Blend"));
    cCmbBlend.setCurrSelected(0);

    cCmbCockpit.addItem(_T("Cockpit"));
    cCmbCockpit.addItem(_T("Region 1"));
    cCmbCockpit.addItem(_T("Region 2"));
    cCmbCockpit.addItem(_T("Region 3"));
    cCmbCockpit.addItem(_T("Region 4"));
    cCmbCockpit.setCurrSelected(0);

    for (auto s : xobj::ESurface::list()) {
        cCmbHard.addItem(sts::toString(s.toUiString()));
    }
}

void ObjAttr::destroyWindow(HWND /*hWnd*/) {
    cChkSunLight.release();
    cChkTwoSided.release();
    cChkDraped.release();
    cChkShadow.release();
    cChkSolidCamera.release();
    cChkDraw.release();

    cChkShiny.release();
    ReleaseISpinner(cSpnShinyRatio);

    cChkCockpit.release();
    cCmbCockpit.release();

    cChkHard.release();
    cChkHardDeck.release();
    cCmbHard.release();

    cCmbBlend.release();
    ReleaseISpinner(cSpnBlendRatio);

    cChkPolyOffset.release();
    ReleaseISpinner(cSpnPolyOstDist);

    cChkLightLevel.release();
    ReleaseISpinner(cSpnLightLevelVal1);
    ReleaseISpinner(cSpnLightLevelVal2);
    ReleaseICustEdit(cEdtLightLevelDrf);
    cBtnLightLevelDrf.release();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::toWindow() {
    if (mMdData) {
        loadToUiBool();
        loadToUiShiny();
        loadToUiCockpit();
        loadToUiHard();
        loadToUiBlend();
        loadToUiPolyOffset();
        loadToUiLightLevel();

        enableBool(true);
        enableShiny(true);
        enableCockpit(true);
        enableHard(true);
        enableBlend(true);
        enablePolyOffset(true);
        enableLightLevel(true);
    }
    else {
        enableBool(false);
        enableShiny(false);
        enableCockpit(false);
        enableHard(false);
        enableBlend(false);
        enablePolyOffset(false);
        enableLightLevel(false);
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::enableBool(bool state) {
    if (!state) {
        cChkSunLight.disable();
        cChkTwoSided.disable();
        cChkDraped.disable();
        cChkShadow.disable();
        cChkSolidCamera.disable();
        cChkDraw.disable();
    }
    else {
        cChkSunLight.enable();
        cChkTwoSided.enable();
        cChkDraped.enable();
        cChkShadow.enable();
        cChkSolidCamera.enable();
        cChkDraw.enable();
    }
}

void ObjAttr::dropFromUiBool() {
    mData.setTree(cChkSunLight.isChecked());
    mData.setTwoSided(cChkTwoSided.isChecked());
    mData.setDraped(cChkDraped.isChecked());
    mData.setCastShadow(cChkShadow.isChecked());
    mData.setSolidForCamera(cChkSolidCamera.isChecked());
    mData.setDraw(cChkDraw.isChecked());
    mMdData->saveToNode(mData);
}

void ObjAttr::loadToUiBool() {
    cChkSunLight.setState(mData.isTree());
    cChkTwoSided.setState(mData.isTwoSided());
    cChkDraped.setState(mData.isDraped());
    cChkShadow.setState(mData.isCastShadow());
    cChkSolidCamera.setState(mData.isSolidForCamera());
    cChkDraw.setState(mData.isDraw());
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::enableShiny(bool state) {
    if (!state) {
        cChkShiny.disable();
        cSpnShinyRatio->Disable();
    }
    else {
        cChkShiny.enable();
        cSpnShinyRatio->Enable(mData.shiny());
    }
}

void ObjAttr::dropFromUiShiny() {
    xobj::AttrShiny attr(cSpnShinyRatio->GetFVal());
    attr.setEnabled(cChkShiny.isChecked());
    mData.setShiny(attr);
    mMdData->saveToNode(mData);
}

void ObjAttr::loadToUiShiny() {
    cChkShiny.setState(mData.shiny());
    cSpnShinyRatio->SetValue(mData.shiny().ratio(), FALSE);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::enableCockpit(bool state) {
    if (!state) {
        cChkCockpit.disable();
        cCmbCockpit.disable();
    }
    else {
        cChkCockpit.enable();
        cCmbCockpit.enable(mData.cockpit());
    }
}

void ObjAttr::dropFromUiCockpit() {
    xobj::AttrCockpit attr(static_cast<xobj::AttrCockpit::eType>(cCmbCockpit.currSelected()));
    attr.setEnabled(cChkCockpit.isChecked());
    mData.setCockpit(attr);
    mMdData->saveToNode(mData);
}

void ObjAttr::loadToUiCockpit() {
    cChkCockpit.setState(mData.cockpit());
    cCmbCockpit.setCurrSelected(static_cast<int>(mData.cockpit().type()));
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::enableHard(bool state) {
    if (!state) {
        cChkHard.disable();
        cChkHardDeck.disable();
        cCmbHard.disable();
    }
    else {
        cChkHard.enable();
        cChkHardDeck.enable(mData.hard());
        cCmbHard.enable(mData.hard());
    }
}

void ObjAttr::dropFromUiHard() {
    xobj::AttrHard attr(xobj::ESurface::fromUiString(sts::toMbString(cCmbHard.currSelectedText()).c_str()), cChkHardDeck.isChecked());
    attr.setEnabled(cChkHard.isChecked());
    mData.setHard(attr);
    mMdData->saveToNode(mData);
}

void ObjAttr::loadToUiHard() {
    cChkHard.setState(mData.hard());
    cChkHardDeck.setState(mData.hard().isDeck());
    cCmbHard.setCurrSelected(static_cast<int>(mData.hard().surface().id()));
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::enableBlend(bool state) {
    if (!state) {
        cCmbBlend.disable();
        cSpnBlendRatio->Disable();
    }
    else {
        cCmbBlend.enable();
        cSpnBlendRatio->Enable(mData.blend());
    }
}

void ObjAttr::dropFromUiBlend() {
    int currSel = cCmbBlend.currSelected();
    xobj::AttrBlend attr(static_cast<xobj::AttrBlend::eType>(currSel),
                         cSpnBlendRatio->GetFVal());
    attr.setEnabled(currSel != 0);
    mData.setBlend(attr);
    mMdData->saveToNode(mData);
}

void ObjAttr::loadToUiBlend() {
    cCmbBlend.setCurrSelected(static_cast<int>(mData.blend().type()));
    cSpnBlendRatio->SetValue(mData.blend().ratio(), FALSE);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::enablePolyOffset(bool state) {
    if (!state) {
        cChkPolyOffset.disable();
        cSpnPolyOstDist->Disable();
    }
    else {
        cChkPolyOffset.enable();
        cSpnPolyOstDist->Enable(mData.polyOffset());
    }
}

void ObjAttr::dropFromUiPolyOffset() {
    xobj::AttrPolyOffset attr(cSpnPolyOstDist->GetFVal());
    attr.setEnabled(cChkPolyOffset.isChecked());
    mData.setPolyOffset(attr);
    mMdData->saveToNode(mData);
}

void ObjAttr::loadToUiPolyOffset() {
    cChkPolyOffset.setState(mData.polyOffset());
    cSpnPolyOstDist->SetValue(mData.polyOffset().offset(), FALSE);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttr::enableLightLevel(bool state) {
    if (!state) {
        cChkLightLevel.disable();
        cSpnLightLevelVal1->Disable();
        cSpnLightLevelVal2->Disable();
        cEdtLightLevelDrf->Disable();
        cBtnLightLevelDrf.disable();
    }
    else {
        cChkLightLevel.enable();
        cSpnLightLevelVal1->Enable(mData.lightLevel());
        cSpnLightLevelVal2->Enable(mData.lightLevel());
        cEdtLightLevelDrf->Enable(mData.lightLevel());
        cBtnLightLevelDrf.enable(mData.lightLevel());
    }
}

void ObjAttr::dropFromUiLightLevel() {
    xobj::AttrLightLevel attr(cSpnLightLevelVal1->GetFVal(),
                              cSpnLightLevelVal2->GetFVal(),
                              sts::toMbString(UiUtilities::getText(cEdtLightLevelDrf)));
    attr.setEnabled(cChkLightLevel.isChecked());
    mData.setLightLevel(attr);
    mMdData->saveToNode(mData);
}

void ObjAttr::loadToUiLightLevel() {
    cChkLightLevel.setState(mData.lightLevel());
    UiUtilities::setText(cEdtLightLevelDrf, sts::toString(mData.lightLevel().dataref()));
    cSpnLightLevelVal1->SetValue(mData.lightLevel().val1(), FALSE);
    cSpnLightLevelVal2->SetValue(mData.lightLevel().val2(), FALSE);
}

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
}
