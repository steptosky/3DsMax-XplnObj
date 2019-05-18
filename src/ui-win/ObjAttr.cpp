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
#include "Utils.h"
#include "resource/ResHelper.h"
#include "presenters/Datarefs.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR ObjAttr::panelProc(HWND /*hWnd*/, UINT msg, WPARAM wParam, LPARAM /*lParam*/) {
        switch (msg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case BTN_LIGHTLEVEL_DRF: {
                        MSTR str;
                        Utils::getText(cEdtLightLevelDrf, str);
                        cEdtLightLevelDrf->SetText(presenters::Datarefs::selectData(str));
                        dropFromUiLightLevel();
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
        cLblShinyRatio.setup(hWnd, SPN_SHINY_LABEL);
        cSpnShinyRatio = SetupFloatSpinner(hWnd, SPN_SHINY, SPN_SHINY_EDIT, 0.0f, 1.0f, 0.5f, 0.01f);

        cChkCockpit.setup(hWnd, CHK_COCKPIT);
        cCmbCockpit.setup(hWnd, CMB_COCKPIT);

        cChkHard.setup(hWnd, CHK_HARD);
        cChkHardDeck.setup(hWnd, CHK_HARD_DECK);
        cCmbHard.setup(hWnd, CMB_HARD);

        cCmbBlend.setup(hWnd, CMB_BLEND);
        cLblBlendRatio.setup(hWnd, SPN_BLEND_LABEL);
        cSpnBlendRatio = SetupFloatSpinner(hWnd, SPN_BLEND, SPN_BLEND_EDIT, 0.0f, 1.0f, 0.5f, 0.01f);

        cChkPolyOffset.setup(hWnd, CHK_POLYOFFSET);
        cLblPolyOstDist.setup(hWnd, SPN_POLYOFFSET_DIST_LABEL);
        cSpnPolyOstDist = SetupFloatSpinner(hWnd, SPN_POLYOFFSET_DIST, SPN_POLYOFFSET_DIST_EDIT, 0.0f, 999999.9f, 0.0f, 0.5f);

        cChkLightLevel.setup(hWnd, CHK_LIGHTLEVEL);
        cLblLightLevelVal1.setup(hWnd, SPN_LIGHTLEVEL_V1_LABEL);
        cLblLightLevelVal2.setup(hWnd, SPN_LIGHTLEVEL_V2_LABEL);
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
        cLblShinyRatio.release();
        ReleaseISpinner(cSpnShinyRatio);

        cChkCockpit.release();
        cCmbCockpit.release();

        cChkHard.release();
        cChkHardDeck.release();
        cCmbHard.release();

        cCmbBlend.release();
        cLblBlendRatio.release();
        ReleaseISpinner(cSpnBlendRatio);

        cChkPolyOffset.release();
        cLblPolyOstDist.release();
        ReleaseISpinner(cSpnPolyOstDist);

        cChkLightLevel.release();
        cLblLightLevelVal1.release();
        cLblLightLevelVal2.release();
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

    void ObjAttr::enableBool(const bool state) {
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
        mData.mIsTree = cChkSunLight.isChecked();
        mData.mIsTwoSided = cChkTwoSided.isChecked();
        mData.mIsDraped = cChkDraped.isChecked();
        mData.mIsCastShadow = cChkShadow.isChecked();
        mData.mIsSolidForCamera = cChkSolidCamera.isChecked();
        mData.mIsDraw = cChkDraw.isChecked();
        mMdData->saveToNode(mData);
    }

    void ObjAttr::loadToUiBool() {
        cChkSunLight.setState(mData.mIsTree);
        cChkTwoSided.setState(mData.mIsTwoSided);
        cChkDraped.setState(mData.mIsDraped);
        cChkShadow.setState(mData.mIsCastShadow);
        cChkSolidCamera.setState(mData.mIsSolidForCamera);
        cChkDraw.setState(mData.mIsDraw);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ObjAttr::enableShiny(const bool state) {
        if (!state) {
            cChkShiny.disable();
            cLblShinyRatio.disable();
            cSpnShinyRatio->Disable();
        }
        else {
            const auto isChecked = cChkShiny.isChecked();
            cChkShiny.enable();
            cLblShinyRatio.enable(isChecked);
            cSpnShinyRatio->Enable(mData.mShiny.has_value());
        }
    }

    void ObjAttr::dropFromUiShiny() {
        if (!cChkShiny.isChecked()) {
            mData.mShiny = std::nullopt;
        }
        else {
            mData.mShiny = xobj::AttrShiny(cSpnShinyRatio->GetFVal());
        }
        mMdData->saveToNode(mData);
    }

    void ObjAttr::loadToUiShiny() {
        cChkShiny.setState(mData.mShiny.has_value());
        if (mData.mShiny.has_value()) {
            cSpnShinyRatio->SetValue(mData.mShiny->mRatio, FALSE);
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ObjAttr::enableCockpit(const bool state) {
        if (!state) {
            cChkCockpit.disable();
            cCmbCockpit.disable();
        }
        else {
            cChkCockpit.enable();
            cCmbCockpit.enable(mData.mCockpit.has_value());
        }
    }

    void ObjAttr::dropFromUiCockpit() {
        if (!cChkCockpit.isChecked()) {
            mData.mCockpit = std::nullopt;
        }
        else {
            mData.mCockpit = xobj::AttrCockpit(static_cast<xobj::AttrCockpit::eType>(cCmbCockpit.currSelected()));
        }
        mMdData->saveToNode(mData);
    }

    void ObjAttr::loadToUiCockpit() {
        cChkCockpit.setState(mData.mCockpit.has_value());
        if (mData.mCockpit.has_value()) {
            cCmbCockpit.setCurrSelected(static_cast<int>(mData.mCockpit->mType));
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ObjAttr::enableHard(const bool state) {
        if (!state) {
            cChkHard.disable();
            cChkHardDeck.disable();
            cCmbHard.disable();
        }
        else {
            cChkHard.enable();
            cChkHardDeck.enable(mData.mHard.has_value());
            cCmbHard.enable(mData.mHard.has_value());
        }
    }

    void ObjAttr::dropFromUiHard() {
        if (!cChkHard.isChecked()) {
            mData.mHard = std::nullopt;
        }
        else {
            xobj::AttrHard attr(xobj::ESurface::fromUiString(sts::toMbString(cCmbHard.currSelectedText()).c_str()),
                                cChkHardDeck.isChecked());
            mData.mHard = attr;
        }
        mMdData->saveToNode(mData);
    }

    void ObjAttr::loadToUiHard() {
        cChkHard.setState(mData.mHard.has_value());
        if (mData.mHard.has_value()) {
            cChkHardDeck.setState(mData.mHard->mIsDeck);
            cCmbHard.setCurrSelected(static_cast<int>(mData.mHard->mSurface.id()));
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ObjAttr::enableBlend(const bool state) {
        if (!state) {
            cCmbBlend.disable();
            cLblBlendRatio.disable();
            cSpnBlendRatio->Disable();
        }
        else {
            const auto isChecked = mData.mBlend.has_value();
            cCmbBlend.enable();
            cLblBlendRatio.enable(isChecked);
            cSpnBlendRatio->Enable(isChecked);
        }
    }

    void ObjAttr::dropFromUiBlend() {
        int currSel = cCmbBlend.currSelected();
        xobj::AttrBlend attr(static_cast<xobj::AttrBlend::eType>(currSel),
                             cSpnBlendRatio->GetFVal());
        if (attr.mType == xobj::AttrBlend::blend || currSel == 0) {
            mData.mBlend = std::nullopt;
        }
        else {
            mData.mBlend = attr;
        }
        mMdData->saveToNode(mData);
    }

    void ObjAttr::loadToUiBlend() {
        if (mData.mBlend.has_value()) {
            cCmbBlend.setCurrSelected(static_cast<int>(mData.mBlend->mType));
            cSpnBlendRatio->SetValue(mData.mBlend->mRatio, FALSE);
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ObjAttr::enablePolyOffset(const bool state) {
        if (!state) {
            cChkPolyOffset.disable();
            cLblPolyOstDist.disable();
            cSpnPolyOstDist->Disable();
        }
        else {
            const auto isChecked = cChkPolyOffset.isChecked();
            cChkPolyOffset.enable();
            cLblPolyOstDist.enable(isChecked);
            cSpnPolyOstDist->Enable(mData.mPolyOffset.has_value());
        }
    }

    void ObjAttr::dropFromUiPolyOffset() {
        if (!cChkPolyOffset.isChecked()) {
            mData.mPolyOffset = std::nullopt;
        }
        else {
            mData.mPolyOffset = xobj::AttrPolyOffset(cSpnPolyOstDist->GetFVal());
        }
        mMdData->saveToNode(mData);
    }

    void ObjAttr::loadToUiPolyOffset() {
        cChkPolyOffset.setState(mData.mPolyOffset.has_value());
        if (mData.mPolyOffset.has_value()) {
            cSpnPolyOstDist->SetValue(mData.mPolyOffset->mOffset, FALSE);
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ObjAttr::enableLightLevel(const bool state) {
        if (!state) {
            cChkLightLevel.disable();
            cLblLightLevelVal1.disable();
            cLblLightLevelVal2.disable();
            cSpnLightLevelVal1->Disable();
            cSpnLightLevelVal2->Disable();
            cEdtLightLevelDrf->Disable();
            cBtnLightLevelDrf.disable();
        }
        else {
            const auto isChecked = cChkLightLevel.isChecked();
            cChkLightLevel.enable();
            cLblLightLevelVal1.enable(isChecked);
            cLblLightLevelVal2.enable(isChecked);
            cSpnLightLevelVal1->Enable(mData.mLightLevel.has_value());
            cSpnLightLevelVal2->Enable(mData.mLightLevel.has_value());
            cEdtLightLevelDrf->Enable(mData.mLightLevel.has_value());
            cBtnLightLevelDrf.enable(mData.mLightLevel.has_value());
        }
    }

    void ObjAttr::dropFromUiLightLevel() {
        if (!cChkLightLevel.isChecked()) {
            mData.mLightLevel = std::nullopt;
        }
        else {
            mData.mLightLevel = xobj::AttrLightLevel(cSpnLightLevelVal1->GetFVal(), cSpnLightLevelVal2->GetFVal(),
                                                     sts::toMbString(Utils::getText(cEdtLightLevelDrf)));;
        }
        mMdData->saveToNode(mData);
    }

    void ObjAttr::loadToUiLightLevel() {
        cChkLightLevel.setState(mData.mLightLevel.has_value());
        if (mData.mLightLevel.has_value()) {
            cEdtLightLevelDrf->SetText(xobj::toMStr(mData.mLightLevel->mDataref));
            cSpnLightLevelVal1->SetValue(mData.mLightLevel->mVal1, FALSE);
            cSpnLightLevelVal2->SetValue(mData.mLightLevel->mVal2, FALSE);
        }
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
