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

#include <xpln/obj/manipulators/AttrManipDragAxis.h>
#include "ManipInterface.h"
#include "ui-win/controls/Combo.h"
#include "ui-win/controls/Base.h"
#include "models/MdManip.h"
#include "ManipAttrWheel.h"

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class ManipAttrDragAxis : public ManipInterface {
    public:

        explicit ManipAttrDragAxis(MdManip * modelData);
        virtual ~ManipAttrDragAxis();

        //-------------------------------------------------------------------------

        void create(HWND inParent) override;
        void destroy() override;
        RECT rect() const override;
        void move(const POINT & point) override;

        void setManip(const std::optional<xobj::AttrManip> & manip) override;

        //-------------------------------------------------------------------------

    private:

        static INT_PTR panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        void initWindow(HWND hWnd);
        void destroyWindow(HWND hWnd);
        void calculateSize();

        void toWindow();

        void save() const { mModelData->saveToNode(xobj::AttrManip(mData)); }

        RECT mSize;

        MdManip * mModelData;
        xobj::AttrManipDragAxis mData;
        ManipAttrWheel mWheel;

        ctrl::Base mHwnd;

        ISpinnerControl * mSpnX = nullptr;
        ISpinnerControl * mSpnY = nullptr;
        ISpinnerControl * mSpnZ = nullptr;

        ISpinnerControl * mSpnVal1 = nullptr;
        ISpinnerControl * mSpnVal2 = nullptr;

        ctrl::Base cBtnDataRef;
        ICustEdit * cEdtDataRef = nullptr;
        ICustEdit * cEdtToolType = nullptr;
        ctrl::Combo cCmbCursor;
    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
