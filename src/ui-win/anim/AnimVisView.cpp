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

#include "AnimVisView.h"
#include "common/Logger.h"
#include "ui-win/Utils.h"
#include "resource/ResHelper.h"
#include "presenters/Datarefs.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK AnimVisView::panelProc(HWND /*hWnd*/, UINT msg, WPARAM wParam, LPARAM /*lParam*/) {
        switch (msg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case CHK_ENABLE: {
                        mData.mEnable = cChkEnable.isChecked();
                        mData.saveToNode();
                        break;
                    }
                    case LIST_VALUES: {
                        if (HIWORD(wParam) == LBN_SELCHANGE) {
                            selectionChanged();
                        }
                        break;
                    }
                    case BTN_ADD_SHOW: addShowItem();
                        break;
                    case BTN_ADD_HIDE: addHideItem();
                        break;
                    case BTN_DELETE: deleteItem();
                        break;
                    case BTN_DATAREF: {
                        MSTR str;
                        Utils::getText(cEditDataRef, str);
                        cEditDataRef->SetText(presenters::Datarefs::selectData(str));
                        selectedDataChanged();
                        break;
                    }
                    default: break;
                }
                break;
            }
            case WM_CUSTEDIT_ENTER: {
                switch (LOWORD(wParam)) {
                    case EDIT_DATAREF: selectedDataChanged();
                        break;
                    default: break;
                }
                break;
            }
            case CC_SPINNER_CHANGE: {
                switch (LOWORD(wParam)) {
                    case SPN_VISIBILITY_V1: selectedDataChanged();
                        break;
                    case SPN_VISIBILITY_V2: selectedDataChanged();
                        break;
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

    AnimVisView::AnimVisView()
        : RollupBase(ResHelper::hInstance),
          mIp(GetCOREInterface()) {}

    AnimVisView::~AnimVisView() {
        AnimVisView::destroy();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimVisView::registerCallbacks() {
        RegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
        RegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
    }

    void AnimVisView::unRegisterCallbacks() {
        UnRegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
        UnRegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimVisView::slotObjectsDeleted(void * param, NotifyInfo * info) {
        auto * view = reinterpret_cast<AnimVisView*>(param);
        const auto * nodeTab = reinterpret_cast<Tab<INode*>*>(info->callParam);

        if (view->mData.linkedNode()) {
            const auto count = nodeTab->Count();
            for (int i = 0; i < count; ++i) {
                if (view->mData.linkedNode() == *nodeTab->Addr(i)) {
                    view->mData.clearLink();
                }
            }
        }
    }

    void AnimVisView::slotSelectionChange(void * param, NotifyInfo *) {
        auto view = reinterpret_cast<AnimVisView*>(param);
        const auto selectedCount = view->mIp->GetSelNodeCount();
        if (selectedCount == 0) {
            view->clearValues();
            view->mData.clearLink();
        }
        else if (selectedCount == 1) {
            view->cSpnValue1->SetValue(0.0f, FALSE);
            view->cSpnValue2->SetValue(1.0f, FALSE);
            view->mData.linkNode(view->mIp->GetSelNode(0));
        }
        else {
            view->clearValues();
            view->mData.clearLink();
        }
        view->toWindow();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimVisView::create(IRollupWindow * rollWin) {
        mRollupIp = rollWin;
        create();
    }

    void AnimVisView::create() {
        createRollup(DLG_ANIM_VIS, _T("Animation Visibility"), this);
        registerCallbacks();
        toWindow();
    }

    void AnimVisView::destroy() {
        unRegisterCallbacks();
        if (this->hwnd() != nullptr) {
            deleteRollup();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimVisView::clearValues() {
        cListKeys.clear();
        cEditDataRef->SetText(_T("none"));
        cSpnValue1->SetValue(0.0f, FALSE);
        cSpnValue2->SetValue(1.0f, FALSE);
    }

    void AnimVisView::setDataRefValueAsToolType() {
#if MAX_VERSION_MAJOR > 11
        cEditDataRef->SetTooltip(true, Utils::getText(cEditDataRef));
#endif
    }

    void AnimVisView::initWindow(HWND hWnd) {
        cChkEnable.setup(hWnd, CHK_ENABLE);
        cListKeys.setup(hWnd, LIST_VALUES);
        cEditDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_DATAREF));
        cBtnDataRef.setup(hWnd, BTN_DATAREF);
        cBtnAddShow.setup(hWnd, BTN_ADD_SHOW);
        cBtnAddHide.setup(hWnd, BTN_ADD_HIDE);
        cBtnDelete.setup(hWnd, BTN_DELETE);
        cSpnValue1 = SetupFloatSpinner(hWnd, SPN_VISIBILITY_V1, SPN_VISIBILITY_V1_EDIT, -1000000.0f, 1000000.0f, 0.0f, 0.01f);
        cSpnValue2 = SetupFloatSpinner(hWnd, SPN_VISIBILITY_V2, SPN_VISIBILITY_V2_EDIT, -1000000.0f, 1000000.0f, 1.0f, 0.01f);
        cStcValue1.setup(hWnd, STC_VALUE1);
        cStcValue2.setup(hWnd, STC_VALUE2);

        cChkEnable.setToolTip(sts::toString("Enable exporting the animation."));
        cBtnAddShow.setToolTip(sts::toString("Add show animation."));
        cBtnAddHide.setToolTip(sts::toString("Add hide animation."));
        cBtnDataRef.setToolTip(sts::toString("Is not supported yet."));
        cBtnDelete.setToolTip(sts::toString("Delete selected animation."));
    }

    void AnimVisView::destroyWindow(HWND /*hWnd*/) {
        cChkEnable.release();
        cListKeys.release();
        ReleaseICustEdit(cEditDataRef);
        cBtnDataRef.release();
        cBtnAddShow.release();
        cBtnAddHide.release();
        cBtnDelete.release();
        ReleaseISpinner(cSpnValue1);
        ReleaseISpinner(cSpnValue2);
        cStcValue1.release();
        cStcValue2.release();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimVisView::toWindow() {
        mCurrSelected = -1;
        if (mData.hasLink()) {
            cChkEnable.setState(mData.mEnable);
            makeUiList();
            enableControls();
        }
        else {
            disableControls();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimVisView::enableControls() {
        cChkEnable.enable();
        cListKeys.enable();
        cEditDataRef->Enable();
        cBtnDataRef.enable();
        cBtnAddShow.enable();
        cBtnAddHide.enable();
        cBtnDelete.enable();
        cSpnValue1->Enable();
        cSpnValue2->Enable();
        cStcValue1.enable();
        cStcValue2.enable();
    }

    void AnimVisView::disableControls() {
        cChkEnable.disable();
        cListKeys.disable();
        cEditDataRef->Disable();
        cBtnDataRef.disable();
        cBtnAddShow.disable();
        cBtnAddHide.disable();
        cBtnDelete.disable();
        cSpnValue1->Disable();
        cSpnValue2->Disable();
        cStcValue1.disable();
        cStcValue2.disable();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimVisView::addItem(const MdAnimVis::Key::eType type) {
        mData.mKeyList.push_back(MdAnimVis::Key(type,
                                                cSpnValue1->GetFVal(),
                                                cSpnValue2->GetFVal(),
                                                sts::toMbString(Utils::getText(cEditDataRef))));
        cListKeys.addItem(toText(mData.mKeyList.back()));
        mData.saveToNode();
    }

    void AnimVisView::deleteItem() {
        const auto index = cListKeys.currSelected();
        if (index == -1)
            return;
        cListKeys.removeCurr();
        mData.mKeyList.erase(mData.mKeyList.begin() + static_cast<size_t>(index));
        mData.saveToNode();
    }

    void AnimVisView::makeUiList() {
        const auto sCurrSelected = cListKeys.currSelected();
        cListKeys.clear();
        for (auto & curr : mData.mKeyList) {
            cListKeys.addItem(toText(curr));
        }
        mData.saveToNode();
        cListKeys.setCurrSelected(sCurrSelected);
    }

    sts::Str AnimVisView::toText(const MdAnimVis::Key & inKey) {
        sts::Str strTmp;
        strTmp.append(1, inKey.mType);
        strTmp.append(_T(" = ")).append(sts::toString(inKey.mValue1, 4)).append(_T(" "));
        strTmp.append(sts::toString(inKey.mValue2, 4));
        const auto strTmp2 = sts::toString(inKey.mDrf);
        strTmp.append(_T(" ")).append(strTmp2.empty() ? _T("none") : strTmp2);
        return strTmp;
    }

    void AnimVisView::selectedDataChanged() {
        if (mCurrSelected == -1) {
            return;
        }
        if (mCurrSelected < mData.mKeyList.size()) {
            MdAnimVis::Key & key = mData.mKeyList[static_cast<size_t>(mCurrSelected)];
            key.mValue1 = cSpnValue1->GetFVal();
            key.mValue2 = cSpnValue2->GetFVal();
            key.mDrf = sts::toMbString(Utils::getText(cEditDataRef));
        }
        makeUiList();
    }

    void AnimVisView::selectionChanged() {
        mCurrSelected = cListKeys.currSelected();
        auto list1 = sts::StrUtils::split<sts::StrUtils::Vector>(sts::toString(cListKeys.currSelectedText()), _T("="));
        if (list1.size() != 2) {
#ifndef NDEBUG
            LError << "Internal error 1";
#endif
            return;
        }
        sts::StrUtils::trim(list1[1]);
        auto list2 = sts::StrUtils::split<sts::StrUtils::Vector>(list1[1], _T(" "));
        if (list2.size() != 3) {
#ifndef NDEBUG
            LError << "Internal error 2";
#endif
            return;
        }
        cSpnValue1->SetValue(sts::toFloat(list2[0]), FALSE);
        cSpnValue2->SetValue(sts::toFloat(list2[1]), FALSE);
        cEditDataRef->SetText(xobj::toMStr(list2[2]));
        setDataRefValueAsToolType();
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
