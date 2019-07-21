/*
**  Copyright(C) 2019, StepToSky
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

#include "AnimRotateRollup.h"
#include "common/Logger.h"
#include "resource/resource.h"
#include "resource/ResHelper.h"
#include "common/NodeUtils.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    /////////////////////////////////////////* Static area *////////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK AnimRotateRollup::panelProc(HWND /*hWnd*/, UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/) {
        return 0;
    }

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    AnimRotateRollup::AnimRotateRollup()
        : RollupBase(ResHelper::hInstance),
          mIp(GetCOREInterface()) {}

    AnimRotateRollup::~AnimRotateRollup() {
        AnimRotateRollup::destroy();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimRotateRollup::create(IRollupWindow * rollWin) {
        mRollIp = rollWin;
        create();
    }

    void AnimRotateRollup::create() {
        createRollup(IDD_ROLL_ANIM_OBJ, _T("Animation Rotate"), this);
        setLinearView();
    }

    void AnimRotateRollup::destroy() {
        if (hwnd() != nullptr) {
            deleteRollup();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimRotateRollup::initWindow(HWND hWnd) {
        mXView = std::make_unique<AnimRotateAxisView>(MdAnimRot::X);
        mYView = std::make_unique<AnimRotateAxisView>(MdAnimRot::Y);
        mZView = std::make_unique<AnimRotateAxisView>(MdAnimRot::Z);
        mLinearView = std::make_unique<AnimRotateAxisView>(MdAnimRot::LINEAR);
        if (!mXView->create(hWnd)) {
            XLError << "Can't create dialog for X rotation animation";
        }
        if (!mYView->create(hWnd)) {
            XLError << "Can't create dialog for Y rotation animation";
        }
        if (!mZView->create(hWnd)) {
            XLError << "Can't create dialog for Z rotation animation";
        }
        if (!mLinearView->create(hWnd)) {
            XLError << "Can't create dialog for linear rotation animation";
        }
        RegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
        RegisterNotification(slotSelectionChange, this, NOTIFY_ANIMATE_OFF);
        RegisterNotification(slotSelectionChange, this, NOTIFY_ANIMATE_ON);
        mWatcher.setCallback(std::bind(&AnimRotateRollup::nodeChanged, this));
    }

    void AnimRotateRollup::destroyWindow(HWND /*hWnd*/) {
        UnRegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
        UnRegisterNotification(slotSelectionChange, this, NOTIFY_ANIMATE_OFF);
        UnRegisterNotification(slotSelectionChange, this, NOTIFY_ANIMATE_ON);
        mWatcher.removeReference();
        deleteView(mLinearView);
        deleteView(mXView);
        deleteView(mYView);
        deleteView(mZView);
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimRotateRollup::nodeChanged() {
        const auto node = mIp->GetSelNode(0);
        nodeSelected(node);
    }

    void AnimRotateRollup::slotSelectionChange(void * param, NotifyInfo *) {
        auto view = reinterpret_cast<AnimRotateRollup*>(param);
        const auto selectedCount = view->mIp->GetSelNodeCount();
        if (selectedCount == 0) {
            view->nodeSelected(nullptr);
            view->mWatcher.removeReference();
        }
        else if (selectedCount == 1) {
            const auto node = view->mIp->GetSelNode(0);
            view->nodeSelected(node);
            view->mWatcher.createReference(node);
        }
        else {
            view->nodeSelected(nullptr);
            view->mWatcher.removeReference();
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimRotateRollup::setLinearView() {
        mLinearView->active(true);
        mXView->active(false);
        mYView->active(false);
        mZView->active(false);
        adjustSize();
    }

    void AnimRotateRollup::setEulerView() {
        mLinearView->active(false);
        mXView->active(true);
        mYView->active(true);
        mZView->active(true);
        adjustSize();
    }

    void AnimRotateRollup::nodeSelected(INode * node) {
        if (!node) {
            return;
        }
        if (NodeUtils::hasLinearRotateController(node)) {
            setLinearView();
        }
        else {
            setEulerView();
        }
    }

    void AnimRotateRollup::adjustSize() {
        int dlgHeight = 0;
        if (mLinearView->isActive()) {
            const RECT size = mLinearView->clientRect();
            dlgHeight = size.bottom - size.top;
        }
        else {
            RECT size = mXView->clientRect();
            dlgHeight = (size.bottom - size.top) * 3;

            size.top += size.bottom - size.top;
            size.left = 0;
            mYView->setWindowPos(size);

            size.top += size.top;
            size.left = 0;
            mZView->setWindowPos(size);
        }
        IRollupPanel * panel = mRollIp->GetPanel(hwnd());
        panel->SetDlgHeight(dlgHeight);
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
