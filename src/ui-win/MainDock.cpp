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

#include <Windows.h>
#include "MainDock.h"
#include "ui-win/ObjAttr.h"
#include "ui-win/ManipAttr.h"
#include "ui-win/LightAttr.h"
#include "common/Config.h"
#include "resource/resource.h"
#include "resource/ResHelper.h"

namespace ui {
namespace win {

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

    INT_PTR CALLBACK MainDock::dialogProc(const HWND hWnd, const UINT msg, WPARAM /*wParam*/, const LPARAM lParam) {
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
        mBaseAttrUi = new ObjAttr();
        mManipAttrUi = new ManipAttr();
        mLightAttrUi = new LightAttr();
        mAnimRotateRollup = new AnimRotateRollup();
        mAnimTransRollup = new AnimTransView();
        mAnimVisRollup = new AnimVisView();
    }

    MainDock::~MainDock() {
        unRegisterCallbacks();
        destroy();

        delete mBaseAttrUi;
        delete mManipAttrUi;
        delete mLightAttrUi;
        delete mAnimRotateRollup;
        delete mAnimTransRollup;
        delete mAnimVisRollup;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void MainDock::objectsDeleted(Tab<INode*> * nodeTab) {
        if (mCurrNode) {
            const int count = nodeTab->Count();
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
        const int selectedCount = mIp->GetSelNodeCount();
        if (selectedCount == 0)
            setNoSelection();
        else if (selectedCount == 1)
            setOneSelection();
        else if (selectedCount > 1)
            setMultiSelection(selectedCount);
    }

    void MainDock::setNoSelection() {
        mBaseAttrUi->show(nullptr);
        mLightAttrUi->show(nullptr);
    }

    void MainDock::setOneSelection() {
        mCurrNode = mIp->GetSelNode(0);
        assert(mCurrNode);

        mBaseAttrUi->show(mMdBaseAttr.linkNode(mCurrNode) ? &mMdBaseAttr : nullptr);
        mLightAttrUi->show(mMdLight.linkNode(mCurrNode) ? &mMdLight : nullptr);
    }

    void MainDock::setMultiSelection(int /*count*/) {
        mBaseAttrUi->show(nullptr);
        mLightAttrUi->show(nullptr);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void MainDock::setActive(const bool state) {
        state ? registerCallbacks() : unRegisterCallbacks();
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

    bool MainDock::create(const HWND parent) {
        mDock.setup(CreateDialogParam(ResHelper::hInstance, MAKEINTRESOURCE(IDD_DOCK),
                                      parent, dialogProc,
                                      reinterpret_cast<LPARAM>(this)));

        if (!mDock) {
            return false;
        }
        mIRollupWindow = GetIRollup(GetDlgItem(mDock.hwnd(), IDC_DOCK_ROLLUPS));
        mBaseAttrUi->create(mIRollupWindow);
        mManipAttrUi->create(mIRollupWindow);
        mLightAttrUi->create(mIRollupWindow);
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
        desktopResolution(w, h);
        setSize(0, h - h / 4);

        mDock.show();
        mIRollupWindow->Show();
        return true;
    }

    void MainDock::destroy() {
        saveConfig();
        if (mDock) {
            mBaseAttrUi->destroy();
            mManipAttrUi->destroy();
            mLightAttrUi->destroy();
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
        if (!posIgnore) {
            return r;
        }
        r.right = r.right - r.left;
        r.left = 0;
        r.bottom = r.bottom - r.top;
        r.top = 0;
        return r;
    }

    void MainDock::setSize(const int width, const int height) {
        RECT r;
        GetWindowRect(mDock.hwnd(), &r);

        r.right = r.right - r.left;
        r.left = 0;
        r.bottom = r.bottom - r.top + 10;
        r.top = 10;

        if (width) {
            r.right = r.left + width;
        }
        if (height) {
            r.bottom = r.top + height;
        }
        mDock.setRect(r);

        r.bottom = r.bottom - r.top;
        r.top = 0;
        mRollupArea.setRect(r);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void MainDock::desktopResolution(int & horizontal, int & vertical) {
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

        conf->setValue(TOTEXT(mBaseAttrUI), mBaseAttrUi->isOpen());
        conf->setValue(TOTEXT(mManipAttrUI), mManipAttrUi->isOpen());
        conf->setValue(TOTEXT(mLightAttrUI), mLightAttrUi->isOpen());
        conf->setValue(TOTEXT(mAnimTransRollup), mAnimTransRollup->isOpen());
        conf->setValue(TOTEXT(mAnimRotateRollup), mAnimRotateRollup->isOpen());
        conf->setValue(TOTEXT(mAnimVisRollup), mAnimVisRollup->isOpen());

        conf->endGroup();
        conf->safeFree();
    }

    void MainDock::loadConfig() {
        Config * conf = Config::instance();
        conf->beginGroup(TOTEXT(MainDockUI));

        mBaseAttrUi->setOpen(conf->value(TOTEXT(mBaseAttrUI), true));
        mManipAttrUi->setOpen(conf->value(TOTEXT(mManipAttrUI), true));
        mLightAttrUi->setOpen(conf->value(TOTEXT(mLightAttrUI), true));
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
}
