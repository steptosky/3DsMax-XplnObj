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

#include <xpln/obj/attributes/AttrSet.h>
#include "ui-win/controls/RollupBase.h"
#include "ui-win/controls/CheckBox.h"
#include "ui-win/controls/Base.h"
#include "ui-win/controls/Combo.h"
#include "models/MdObjAttr.h"

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class ObjAttr : public max::ctrl::RollupBase {
    public:

        void create(IRollupWindow * rollWin);
        void destroy() override;

        void show(MdObjAttr * inOutData);

        ObjAttr();
        virtual ~ObjAttr();

    private:

        void create() override;

        IRollupWindow * getInterface() override {
            return mIp;
        }

        INT_PTR panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void initWindow(HWND hWnd) override;
        void destroyWindow(HWND hWnd) override;

        void toWindow();

        xobj::AttrSet mData;
        MdObjAttr * mMdData = nullptr;
        IRollupWindow * mIp = nullptr;

        //-------------------------------------------------------------------------

        ctrl::CheckBox cChkSunLight;
        ctrl::CheckBox cChkTwoSided;
        ctrl::CheckBox cChkDraped;
        ctrl::CheckBox cChkShadow;
        ctrl::CheckBox cChkSolidCamera;
        ctrl::CheckBox cChkDraw;

        void enableBool(bool state);
        void dropFromUiBool();
        void loadToUiBool();

        //-------------------------------------------------------------------------

        ctrl::CheckBox cChkShiny;
        ctrl::Base cLblShinyRatio;
        ISpinnerControl * cSpnShinyRatio = nullptr;

        void enableShiny(bool state);
        void dropFromUiShiny();
        void loadToUiShiny();

        //-------------------------------------------------------------------------

        ctrl::CheckBox cChkCockpit;
        ctrl::Combo cCmbCockpit;

        void enableCockpit(bool state);
        void dropFromUiCockpit();
        void loadToUiCockpit();

        //-------------------------------------------------------------------------

        ctrl::CheckBox cChkHard;
        ctrl::CheckBox cChkHardDeck;
        ctrl::Combo cCmbHard;

        void enableHard(bool state);
        void dropFromUiHard();
        void loadToUiHard();

        //-------------------------------------------------------------------------

        ctrl::Combo cCmbBlend;
        ctrl::Base cLblBlendRatio;
        ISpinnerControl * cSpnBlendRatio = nullptr;

        void enableBlend(bool state);
        void dropFromUiBlend();
        void loadToUiBlend();

        //-------------------------------------------------------------------------

        ctrl::CheckBox cChkPolyOffset;
        ctrl::Base cLblPolyOstDist;
        ISpinnerControl * cSpnPolyOstDist = nullptr;

        void enablePolyOffset(bool state);
        void dropFromUiPolyOffset();
        void loadToUiPolyOffset();

        //-------------------------------------------------------------------------

        ICustEdit * cEdtLightLevelDrf = nullptr;
        ctrl::CheckBox cChkLightLevel;
        ctrl::Base cBtnLightLevelDrf;
        ctrl::Base cLblLightLevelVal1;
        ctrl::Base cLblLightLevelVal2;
        ISpinnerControl * cSpnLightLevelVal1 = nullptr;
        ISpinnerControl * cSpnLightLevelVal2 = nullptr;

        void enableLightLevel(bool state);
        void dropFromUiLightLevel();
        void loadToUiLightLevel();

        //-------------------------------------------------------------------------

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
