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

#pragma once

#include <memory>

#pragma warning(push, 0)
#include <notify.h>
#pragma warning(pop)

#include "ui-win/controls/RollupBase.h"
#include "AnimRotateAxisView.h"
#include "ChangeControllerWatcher.h"

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class AnimRotateRollup final : public max::ctrl::RollupBase {
    public:

        void create(IRollupWindow * rollWin);
        void destroy() override;

        AnimRotateRollup();
        virtual ~AnimRotateRollup();

    private:

        void create() override;

        IRollupWindow * getInterface() override {
            return mRollIp;
        }

        INT_PTR panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void initWindow(HWND hWnd) override;
        void destroyWindow(HWND hWnd) override;

        IRollupWindow * mRollIp = nullptr;
        Interface * mIp = nullptr;
        //-------------------------------------------------------------------------
        void nodeSelected(INode * node);
        void nodeChanged();

        static void slotSelectionChange(void * param, NotifyInfo *);

        void adjustSize();
        //-------------------------------------------------------------------------

        static void deleteView(std::unique_ptr<AnimRotateAxisView> & viewPtr) {
            if (viewPtr) {
                viewPtr->destroy();
                viewPtr.reset();
            }
        }

        void setLinearView();
        void setEulerView();

        std::unique_ptr<AnimRotateAxisView> mLinearView;
        std::unique_ptr<AnimRotateAxisView> mXView;
        std::unique_ptr<AnimRotateAxisView> mYView;
        std::unique_ptr<AnimRotateAxisView> mZView;

        ChangeControllerWatcher mWatcher;
        //-------------------------------------------------------------------------
    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
