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

#include "MainDock.h"
#include "Windows.h"
#include "ui/ObjAttr.h"
#include "ui/ManipAttr.h"
#include "ui/LightAttr.h"
#include "common/Config.h"
#include "Resource/resource.h"
#include "resource/ResHelper.h"

namespace ui {

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

void MainDock::slotObjectSelectionChange(void * param, NotifyInfo * /*info*/) {
    MainDock * objCore = reinterpret_cast<MainDock*>(param);
    objCore->selectionChanged();
}

void MainDock::slotObjectsDeleted(void * param, NotifyInfo * info) {
    MainDock * objCore = reinterpret_cast<MainDock*>(param);
    objCore->objectsDeleted(reinterpret_cast<Tab<INode*>*>(info->callParam));
}

INT_PTR MainDock::DialogProc(HWND hWnd, UINT msg, WPARAM /*wParam*/, LPARAM lParam) {
    MainDock * theDlg;
    if (msg == WM_INITDIALOG) {
        theDlg = reinterpret_cast<MainDock*>(lParam);
        DLSetWindowLongPtr(hWnd, lParam);
        theDlg->mRollupArea.setup(GetDlgItem(hWnd, IDC_DOCK_ROLLUPS));
    }
    else if (msg == WM_DESTROY) {
        theDlg = DLGetWindowLongPtr<MainDock*>(hWnd);
        theDlg->mRollupArea.release();
    }
    else {
        theDlg = DLGetWindowLongPtr<MainDock *>(hWnd);
        if (!theDlg) {
            return FALSE;
        }
    }
    return FALSE;
}

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

MainDock::MainDock() {
    mIp = GetCOREInterface();
    mCurrNode = nullptr;

    mMaxHeight = 1600;
    mMinHeight = 200;
    mMaxWidth = 185;
    mMinWidth = 185;

    mIRollupWindow = nullptr;
    mBaseAttrUI = new ObjAttr();
    mManipAttrUI = new ManipAttr();
    mLightAttrUI = new LightAttr();
    mAnimRotateRollup = new AnimRotateRollup();
    mAnimTransRollup = new AnimTransView();
    mAnimVisRollup = new AnimVisView();
}

MainDock::~MainDock() {
    unRegisterCallbacks();
    destroy();

    delete mBaseAttrUI;
    delete mManipAttrUI;
    delete mLightAttrUI;
    delete mAnimRotateRollup;
    delete mAnimTransRollup;
    delete mAnimVisRollup;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MainDock::objectsDeleted(Tab<INode*> * nodeTab) {
    if (mCurrNode) {
        int count = nodeTab->Count();
        for (int i = 0; i < count; ++i) {
            if (mCurrNode == *(nodeTab->Addr(i)))
                mCurrNode = nullptr;
        }
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MainDock::selectionChanged() {
    int selectedCount = mIp->GetSelNodeCount();
    if (selectedCount == 0)
        setNoSelection();
    else if (selectedCount == 1)
        setOneSelection();
    else if (selectedCount > 1)
        setMultiSelection(selectedCount);
}

void MainDock::setNoSelection() {
    mBaseAttrUI->show(nullptr);
    mLightAttrUI->show(nullptr);
}

void MainDock::setOneSelection() {
    mCurrNode = mIp->GetSelNode(0);
    assert(mCurrNode);

    mBaseAttrUI->show(mMdBaseAttr.linkNode(mCurrNode) ? &mMdBaseAttr : nullptr);
    mLightAttrUI->show(mMdLight.linkNode(mCurrNode) ? &mMdLight : nullptr);
}

void MainDock::setMultiSelection(int /*count*/) {
    mBaseAttrUI->show(nullptr);
    mLightAttrUI->show(nullptr);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MainDock::becomeHidden() {
    unRegisterCallbacks();
}

void MainDock::becomeShown() {
    registerCallbacks();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MainDock::registerCallbacks() {
    RegisterNotification(slotObjectSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
    RegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
}

void MainDock::unRegisterCallbacks() {
    UnRegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
    UnRegisterNotification(slotObjectSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool MainDock::create(HWND inParent) {
    mDock.setup(CreateDialogParam(ResHelper::hInstance,
                                  MAKEINTRESOURCE(IDD_DOCK),
                                  inParent,
                                  reinterpret_cast<DLGPROC>(MainDock::DialogProc),
                                  reinterpret_cast<LPARAM>(this)));

    if (!mDock) {
        return false;
    }
    mIRollupWindow = GetIRollup(GetDlgItem(mDock.hwnd(), IDC_DOCK_ROLLUPS));
    mBaseAttrUI->create(mIRollupWindow);
    mManipAttrUI->create(mIRollupWindow);
    mLightAttrUI->create(mIRollupWindow);
    mAnimTransRollup->create(mIRollupWindow);
    mAnimRotateRollup->create(mIRollupWindow);
    mAnimVisRollup->create(mIRollupWindow);

    loadConfig();

    RECT r;
    GetWindowRect(mDock.hwnd(), &r);
    mMinWidth = r.right - r.left;
    mMaxWidth = mMinWidth;

    int w = 0;
    int h = 0;
    GetDesktopResolution(w, h);
    setSize(0, h - h / 4);

    mDock.show();
    mIRollupWindow->Show();
    return true;
}

void MainDock::destroy() {
    saveConfig();
    if (mDock) {
        mBaseAttrUI->destroy();
        mManipAttrUI->destroy();
        mLightAttrUI->destroy();
        mAnimTransRollup->destroy();
        mAnimRotateRollup->destroy();
        mAnimVisRollup->destroy();
        DestroyWindow(mDock.hwnd());
        mDock.release();
    }
}

RECT MainDock::getSize(bool posIgnore) {
    RECT r;
    GetWindowRect(mDock.hwnd(), &r);
    if (!posIgnore)
        return r;
    r.right = r.right - r.left;
    r.left = 0;
    r.bottom = r.bottom - r.top;
    r.top = 0;
    return r;
}

void MainDock::setSize(int inWidth, int inHeight) {
    RECT r;
    GetWindowRect(mDock.hwnd(), &r);

    r.right = r.right - r.left;
    r.left = 0;
    r.bottom = r.bottom - r.top + 10;
    r.top = 10;

    if (inWidth)
        r.right = r.left + inWidth;
    if (inHeight)
        r.bottom = r.top + inHeight;
    mDock.setRect(r);

    r.bottom = r.bottom - r.top;
    r.top = 0;
    mRollupArea.setRect(r);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MainDock::GetDesktopResolution(int & horizontal, int & vertical) {
    RECT desktop;
    memset(&desktop, 0x00, sizeof(desktop));
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MainDock::saveConfig() {
    Config * conf = Config::instance();
    conf->beginGroup(TOTEXT(MainDockUI));

    conf->setValue(TOTEXT(mBaseAttrUI), mBaseAttrUI->isOpen());
    conf->setValue(TOTEXT(mManipAttrUI), mManipAttrUI->isOpen());
    conf->setValue(TOTEXT(mLightAttrUI), mLightAttrUI->isOpen());
    conf->setValue(TOTEXT(mAnimTransRollup), mAnimTransRollup->isOpen());
    conf->setValue(TOTEXT(mAnimRotateRollup), mAnimRotateRollup->isOpen());
    conf->setValue(TOTEXT(mAnimVisRollup), mAnimVisRollup->isOpen());

    conf->endGroup();
    conf->safeFree();
}

void MainDock::loadConfig() {
    Config * conf = Config::instance();
    conf->beginGroup(TOTEXT(MainDockUI));

    mBaseAttrUI->setOpen(conf->value(TOTEXT(mBaseAttrUI), true));
    mManipAttrUI->setOpen(conf->value(TOTEXT(mManipAttrUI), true));
    mLightAttrUI->setOpen(conf->value(TOTEXT(mLightAttrUI), true));
    mAnimTransRollup->setOpen(conf->value(TOTEXT(mAnimTransRollup), true));
    mAnimRotateRollup->setOpen(conf->value(TOTEXT(mAnimRotateRollup), true));
    mAnimVisRollup->setOpen(conf->value(TOTEXT(mAnimVisRollup), true));

    conf->endGroup();
    conf->safeFree();
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
