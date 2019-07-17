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

#include "common/String.h"
#include "models/MdAnimVis.h"
#include "ui-win/controls/CheckBox.h"
#include "ui-win/controls/List.h"
#include "ui-win/controls/RollupBase.h"

#pragma warning(push, 0)
#include <notify.h>
#pragma warning(pop)

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class AnimVisView : public max::ctrl::RollupBase {
    public:

        void create(IRollupWindow * rollWin);
        void destroy() override;

        AnimVisView();
        virtual ~AnimVisView();

    private:

        void create() override;

        IRollupWindow * getInterface() override { return mRollupIp; }

        INT_PTR panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void initWindow(HWND hWnd) override;
        void destroyWindow(HWND hWnd) override;
        //----------------------------------------------------

        MdAnimVis mData;
        void toWindow();
        void enableControls();
        void disableControls();
        void clearValues();

        void addHideItem() { addItem(MdAnimVis::Key::HIDE); }
        void addShowItem() { addItem(MdAnimVis::Key::SHOW); }

        void addItem(MdAnimVis::Key::eType type);
        void deleteItem();
        void selectionChanged();
        void selectedDataChanged();
        void makeUiList();
        static sts::Str toText(const MdAnimVis::Key & inKey);
        void setDataRefValueAsToolType();

        int mCurrSelected = 0;
        ctrl::List cListKeys;
        ctrl::Base cBtnDataRef;
        ctrl::Base cBtnAddShow;
        ctrl::Base cBtnAddHide;
        ctrl::Base cBtnDelete;
        ctrl::Base cStcValue1;
        ctrl::Base cStcValue2;
        ctrl::CheckBox cChkEnable;
        ICustEdit * cEditDataRef = nullptr;
        ISpinnerControl * cSpnValue1 = nullptr;
        ISpinnerControl * cSpnValue2 = nullptr;

        IRollupWindow * mRollupIp = nullptr;
        Interface * mIp = nullptr;
        //-------------------------------------------------------------------------

        void registerCallbacks();
        void unRegisterCallbacks();

        static void slotSelectionChange(void * param, NotifyInfo *);
        static void slotObjectsDeleted(void * param, NotifyInfo * info);
        //-------------------------------------------------------------------------

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
