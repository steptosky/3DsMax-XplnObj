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

#include <map>
#include <sts/signals/Signal.h>
#include "common/String.h"
#include "ui-win/controls/RollupBase.h"
#include "ui-win/controls/Combo.h"
#include "models/MdManip.h"
#include "ui-win/manip/ManipInterface.h"

#pragma warning(push, 0)
#include <notify.h>
#pragma warning(pop)

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class ManipAttr : public max::ctrl::RollupBase, ManipIO::IManipIo, public sts::signals::AutoDisconnect {
    public:

        ManipAttr();
        virtual ~ManipAttr();

        void create(IRollupWindow * rollWin);
        void destroy() override;

    private:

        void create() override;
        void createSubWindow();
        void destroyAllSubWindows();
        void recalculateSize();

        IRollupWindow * getInterface() override {
            return mIRollup;
        }

        INT_PTR panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void initWindow(HWND hWnd) override;
        void destroyWindow(HWND hWnd) override;

        ctrl::Combo cCmbManipType;

        void enableControls();
        void disableControls();

        MdManip mData;
        IRollupWindow * mIRollup = nullptr;
        SIZE mOriginRollupSize;
        ManipInterface * currSubWin = nullptr;
        Interface * mIp = nullptr;

        //-------------------------------------------------------------------------

        void setCurrManip(const std::optional<xobj::AttrManip> & manip) {
            mCurrManip = manip;
        }

        template<typename T>
        T * currentManip() {
            return dynamic_cast<T*>(mCurrManip);
        }

        std::optional<xobj::AttrManip> mCurrManip;

        //-------------------------------------------------------------------------

        std::map<const sts::Str, ManipInterface *> mChildren;

        //-------------------------------------------------------------------------

        void registerCallbacks();
        void unRegisterCallbacks();

        static void slotSelectionChange(void * param, NotifyInfo *);
        static void slotObjectsDeleted(void * param, NotifyInfo * info);
        static void slotNodesClone(void * param, NotifyInfo *);

        //-------------------------------------------------------------------------

        void setNoSelected();
        void setNodeSelected(INode * node);

        //-------------------------------------------------------------------------

        void gotAttrManip(const xobj::AttrManipAxisKnob & inManip) override;
        void gotAttrManip(const xobj::AttrManipAxisSwitchLeftRight & inManip) override;
        void gotAttrManip(const xobj::AttrManipAxisSwitchUpDown & inManip) override;
        void gotAttrManip(const xobj::AttrManipCmd & inManip) override;
        void gotAttrManip(const xobj::AttrManipCmdAxis & inManip) override;
        void gotAttrManip(const xobj::AttrManipCmdKnob & inManip) override;
        void gotAttrManip(const xobj::AttrManipCmdSwitchLeftRight & inManip) override;
        void gotAttrManip(const xobj::AttrManipCmdSwitchUpDown & inManip) override;
        void gotAttrManip(const xobj::AttrManipDelta & inManip) override;
        void gotAttrManip(const xobj::AttrManipDragAxis & inManip) override;
        void gotAttrManip(const xobj::AttrManipDragAxisPix & inManip) override;
        void gotAttrManip(const xobj::AttrManipDragXy & inManip) override;
        void gotAttrManip(const xobj::AttrManipNoop & inManip) override;
        void gotAttrManip(const xobj::AttrManipPanel & inManip) override;
        void gotAttrManip(const xobj::AttrManipPush & inManip) override;
        void gotAttrManip(const xobj::AttrManipRadio & inManip) override;
        void gotAttrManip(const xobj::AttrManipToggle & inManip) override;
        void gotAttrManip(const xobj::AttrManipWrap & inManip) override;
        void gotNoManip() override;

        //-------------------------------------------------------------------------

        template<typename T, typename W>
        void createSubWin(const T & inManip) {
            auto item = mChildren.find(sts::toString(inManip.mType.toUiString()));
            assert(item != mChildren.end());
            setCurrManip(xobj::AttrManip(inManip));
            W * ui = new W(&mData);
            delete item->second;
            item->second = ui;
            currSubWin = ui;
            ui->setManip(xobj::AttrManip(inManip));
            ui->create(hwnd());
            cCmbManipType.setCurrSelected(sts::toString(inManip.mType.toUiString()));
            recalculateSize();
        }

        //-------------------------------------------------------------------------

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
