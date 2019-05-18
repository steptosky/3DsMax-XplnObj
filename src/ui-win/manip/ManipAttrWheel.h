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

#include <functional>
#include <xpln/obj/manipulators/embeddable/AttrManipWheel.h>
#include "common/String.h"
#include "ui-win/controls/Base.h"
#include "ui-win/controls/CheckBox.h"
#include "ui-win/IWindow.h"

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class ManipAttrWheel : public IWindow {
    public:

        ManipAttrWheel() = default;
        virtual ~ManipAttrWheel();

        //-------------------------------------------------------------------------

        void create(HWND inParent) override;
        void destroy() override;

        //-------------------------------------------------------------------------

        RECT rect() const override;
        void move(const POINT & point) override;

        //-------------------------------------------------------------------------

        void setManip(const std::optional<xobj::AttrManipWheel> & manip) {
            mData = manip;
            toWindow();
        }

        void setCallBack(std::function<void(const std::optional<xobj::AttrManipWheel> &)> & inFn) {
            mCallback = inFn;
        }

        //-------------------------------------------------------------------------

    private:

        static INT_PTR panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        void initWindow(HWND hWnd);
        void destroyWindow(HWND hWnd);

        void toWindow();
        void enablingControls();
        void dataChanged();

        std::function<void(const std::optional<xobj::AttrManipWheel> &)> mCallback;
        std::optional<xobj::AttrManipWheel> mData;
        ctrl::Base mHwnd;
        ctrl::CheckBox mChkEnable;
        ISpinnerControl * mSpnDelta = nullptr;

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
