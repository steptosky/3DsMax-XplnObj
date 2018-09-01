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

#include "LightAttr.h"
#include "models/MdLight.h"
#include "common/Logger.h"

#include "light/LightCust.h"
#include "light/LightNamed.h"
#include "light/LightParam.h"
#include "light/LightSpillCust.h"
#include "resource/resource.h"
#include "resource/ResHelper.h"

#define UI_LIGHT_NONE "None"
#define UI_LIGHT_CUSTOM "Custom"
#define UI_LIGHT_PARAM "Param"
#define UI_LIGHT_NAMED "Named"
#define UI_LIGHT_SPILL "Spill"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR LightAttr::panelProc(HWND /*hWnd*/, UINT msg, WPARAM wParam, LPARAM /*lParam*/) {
        switch (msg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case IDC_CMB_LIGHTTYPE: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            destroyAllSubWindows();
                            recalculateSize();
                            createSubWindow();
                        }
                    }
                    default: break;;
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

    LightAttr::LightAttr()
        : RollupBase(ResHelper::hInstance) {
        mData = nullptr;
        currSubWin = nullptr;

        mChildren.insert(std::make_pair(_T(UI_LIGHT_NAMED), nullptr));
        mChildren.insert(std::make_pair(_T(UI_LIGHT_PARAM), nullptr));
        mChildren.insert(std::make_pair(_T(UI_LIGHT_CUSTOM), nullptr));
        mChildren.insert(std::make_pair(_T(UI_LIGHT_SPILL), nullptr));
    }

    LightAttr::~LightAttr() {
        LightAttr::destroy();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightAttr::show(MdLight * inOutData) {
        mData = inOutData;
        toWindow();
    }

    void LightAttr::create(IRollupWindow * rollWin) {
        mIp = rollWin;
        create();
    }

    void LightAttr::create() {
        createRollup(IDD_ROLL_LIGHT_MAIN_OBJ, _T("X Light attributes"), this);
        GetWindowRect(this->hwnd(), &mThisSize);
        createSubWindow();
        show(nullptr);
    }

    void LightAttr::destroy() {
        if (this->hwnd() != nullptr) {
            destroyAllSubWindows();
            deleteRollup();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightAttr::initWindow(HWND hWnd) {
        cCmbLightType.setup(hWnd, IDC_CMB_LIGHTTYPE);
        cCmbLightType.addItem(_T(UI_LIGHT_NONE));
        cCmbLightType.addItem(_T(UI_LIGHT_NAMED));
        cCmbLightType.addItem(_T(UI_LIGHT_PARAM));
        cCmbLightType.addItem(_T(UI_LIGHT_CUSTOM));
        cCmbLightType.addItem(_T(UI_LIGHT_SPILL));
        cCmbLightType.setCurrSelected(0);
    }

    void LightAttr::destroyWindow(HWND /*hWnd*/) {
        cCmbLightType.release();
    }

    void LightAttr::toWindow() {
        destroyAllSubWindows();
        recalculateSize();
        if (mData) {
            enableControls();
            mData->loadFromNode(this);
        }
        else {
            disableControls();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightAttr::enableControls() {
        cCmbLightType.enable();
    }

    void LightAttr::disableControls() {
        cCmbLightType.disable();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightAttr::createSubWindow() {
        if (mData != nullptr) {
            sts::Str currItem = cCmbLightType.currSelectedText();

            //-------------------------------------------------
            if (currItem == _T(UI_LIGHT_NONE)) {
                setCurrLight(nullptr);
            }
                //-------------------------------------------------
            else if (currItem == _T(UI_LIGHT_NAMED)) {
                gotLight(xobj::ObjLightNamed());
            }
                //-------------------------------------------------
            else if (currItem == _T(UI_LIGHT_CUSTOM)) {
                gotLight(xobj::ObjLightCustom());
            }
                //-------------------------------------------------
            else if (currItem == _T(UI_LIGHT_PARAM)) {
                gotLight(xobj::ObjLightParam());
            }
                //-------------------------------------------------
            else if (currItem == _T(UI_LIGHT_SPILL)) {
                gotLight(xobj::ObjLightSpillCust());
            }
            //-------------------------------------------------
        }
    }

    void LightAttr::recalculateSize() {
        if (currSubWin == nullptr) {
            mIp->SetPageDlgHeight(mIp->GetPanelIndex(this->hwnd()), mThisSize.bottom - mThisSize.top);
            return;
        }

        RECT size = currSubWin->rect();
        size.top = size.top + (mThisSize.bottom - mThisSize.top);
        size.bottom = size.bottom + (mThisSize.bottom - mThisSize.top);
        MoveWindow(currSubWin->hwnd(), 0, mThisSize.bottom - mThisSize.top, size.right - size.left, size.bottom - size.top, FALSE);
        mIp->SetPageDlgHeight(mIp->GetPanelIndex(this->hwnd()), mThisSize.bottom - mThisSize.top + size.bottom - size.top);
        mIp->UpdateLayout();
    }

    void LightAttr::destroyAllSubWindows() {
        for (auto & curr : mChildren) {
            if (curr.second != nullptr) {
                delete curr.second;
                curr.second = nullptr;
            }
        }
        currSubWin = nullptr;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void LightAttr::onParamChanged(bool) {
        if (mData) {
            mData->saveToNode(*currentLight<xobj::ObjAbstract>());
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void LightAttr::gotLight(const xobj::ObjLightCustom & inLight) {
        auto item = mChildren.find(_T(UI_LIGHT_CUSTOM));
        if (item != mChildren.end()) {
            setCurrLight(inLight.clone());
            LightCust * ui = new LightCust;
            delete item->second;
            item->second = ui;
            ui->eventParamChanged.connect(this, &LightAttr::onParamChanged);
            ui->create(this->hwnd());
            ui->show(currentLight<xobj::ObjLightCustom>());
            currSubWin = &ui->getCBase();
            cCmbLightType.setCurrSelected(_T(UI_LIGHT_CUSTOM));
            recalculateSize();
        }
        else {
            LError << "Internal error. There is no 'Custom' in the container.";
        }
    }

    void LightAttr::gotLight(const xobj::ObjLightNamed & inLight) {
        auto item = mChildren.find(_T(UI_LIGHT_NAMED));
        if (item != mChildren.end()) {
            setCurrLight(inLight.clone());
            LightNamed * ui = new LightNamed;
            delete item->second;
            item->second = ui;
            ui->eventParamChanged.connect(this, &LightAttr::onParamChanged);
            ui->create(this->hwnd());
            ui->show(currentLight<xobj::ObjLightNamed>());
            currSubWin = &ui->getCBase();
            cCmbLightType.setCurrSelected(_T(UI_LIGHT_NAMED));
            recalculateSize();
        }
        else {
            LError << "Internal error. There is no 'Named' in the container.";
        }
    }

    void LightAttr::gotLight(const xobj::ObjLightParam & inLight) {
        auto item = mChildren.find(_T(UI_LIGHT_PARAM));
        if (item != mChildren.end()) {
            setCurrLight(inLight.clone());
            LightParam * ui = new LightParam;
            delete item->second;
            item->second = ui;
            ui->eventParamChanged.connect(this, &LightAttr::onParamChanged);
            ui->create(this->hwnd());
            ui->show(currentLight<xobj::ObjLightParam>());
            currSubWin = &ui->getCBase();
            cCmbLightType.setCurrSelected(_T(UI_LIGHT_PARAM));
            recalculateSize();
        }
        else {
            LError << "Internal error. There is no 'Param' in the container.";
        }
    }

    void LightAttr::gotLight(const xobj::ObjLightPoint & /*inLight*/) {}

    void LightAttr::gotLight(const xobj::ObjLightSpillCust & inLight) {
        auto item = mChildren.find(_T(UI_LIGHT_SPILL));
        if (item != mChildren.end()) {
            setCurrLight(inLight.clone());
            LightSpillCust * ui = new LightSpillCust;
            delete item->second;
            item->second = ui;
            ui->eventParamChanged.connect(this, &LightAttr::onParamChanged);
            ui->create(this->hwnd());
            ui->show(currentLight<xobj::ObjLightSpillCust>());
            currSubWin = &ui->getCBase();
            cCmbLightType.setCurrSelected(_T(UI_LIGHT_SPILL));
            recalculateSize();
        }
        else {
            LError << "Internal error. There is no 'Spill Custom' in the container.";
        }
    }

    void LightAttr::gotNoLight() {
        setCurrLight(nullptr);
        cCmbLightType.setCurrSelected(_T(UI_LIGHT_NONE));
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
