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

#pragma once

#pragma warning(push, 0)
#include <max.h>
#include <notify.h>
#pragma warning(pop)

#include "models/MdObjAttr.h"
#include "models/MdLight.h"
#include "ui/anim/AnimRotateRollup.h"
#include "ui/anim/AnimTransView.h"
#include "ui/anim/AnimVisView.h"
#include "ui/controls/Base.h"
#include "models/MdManip.h"

namespace ui {

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

class ObjAttr;
class ManipAttr;
class LightAttr;

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

class MainDock {

    MainDock(const MainDock &) = delete;
    MainDock & operator =(const MainDock &) = delete;

public:

    MainDock();
    virtual ~MainDock();

    HWND getHwnd() {
        return mDock.hwnd();
    }

    bool create(HWND inParent);
    void destroy();

    //---------------------------------------------

    void becomeHidden();
    void becomeShown();

    //---------------------------------------------

    int getMaxHeight() const {
        return mMaxHeight;
    }

    int getMinHeight() const {
        return mMinHeight;
    }

    int getMaxWidth() const {
        return mMaxWidth;
    }

    int getMinWidth() const {
        return mMinWidth;
    }

    void setSize(int inWidth, int inHeight);
    RECT getSize(bool posIgnore = false);

    //---------------------------------------------

private:

    //--------------------------------------

    void registerCallbacks();
    void unRegisterCallbacks();

    static void slotObjectSelectionChange(void * param, NotifyInfo * info);
    static void slotObjectsDeleted(void * param, NotifyInfo * info);

    //--------------------------------------

    void selectionChanged();
    void setNoSelection();
    void setOneSelection();
    void setMultiSelection(int count);

    void objectsDeleted(Tab<INode*> * nodeTab);

    //--------------------------------------

    static INT_PTR DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    //--------------------------------------

    void saveConfig();
    void loadConfig();

    //--------------------------------------

    int mMaxHeight;
    int mMinHeight;
    int mMaxWidth;
    int mMinWidth;

    //--------------------------------------

    void GetDesktopResolution(int & horizontal, int & vertical);

    //--------------------------------------

    Interface * mIp = nullptr;

    ObjAttr * mBaseAttrUI = nullptr;
    ManipAttr * mManipAttrUI = nullptr;
    LightAttr * mLightAttrUI = nullptr;

    AnimRotateRollup * mAnimRotateRollup = nullptr;
    AnimTransView * mAnimTransRollup = nullptr;
    AnimVisView * mAnimVisRollup = nullptr;

    MdObjAttr mMdBaseAttr;
    MdLight mMdLight;

    INode * mCurrNode = nullptr;
    win::Base mDock;
    win::Base mRollupArea;

    IRollupWindow * mIRollupWindow = nullptr;
};

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

}
