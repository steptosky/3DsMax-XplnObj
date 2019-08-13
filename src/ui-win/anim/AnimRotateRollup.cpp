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

#include "AnimRotateRollup.h"
#include "common/Logger.h"
#include "resource/resource.h"
#include "resource/ResHelper.h"

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
        : RollupBase(ResHelper::hInstance) {}

    AnimRotateRollup::~AnimRotateRollup() {
        AnimRotateRollup::destroy();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimRotateRollup::create(IRollupWindow * rollWin) {
        mIp = rollWin;
        create();
    }

    void AnimRotateRollup::create() {
        createRollup(IDD_ROLL_ANIM_OBJ, _T("Animation Rotate"), this);
        adjustSize();
    }

    void AnimRotateRollup::destroy() {
        if (this->hwnd() != nullptr) {
            deleteRollup();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void AnimRotateRollup::initWindow(HWND hWnd) {
        mXView = new AnimRotateAxisView(MdAnimRot::X);
        mYView = new AnimRotateAxisView(MdAnimRot::Y);
        mZView = new AnimRotateAxisView(MdAnimRot::Z);
        if (!mXView->create(hWnd)) {
            XLError << "Can't create dialog for X rotation animation";
        }
        if (!mYView->create(hWnd)) {
            XLError << "Can't create dialog for Y rotation animation";
        }
        if (!mZView->create(hWnd)) {
            XLError << "Can't create dialog for Z rotation animation";
        }
    }

    void AnimRotateRollup::destroyWindow(HWND /*hWnd*/) {
        mXView->destroy();
        mYView->destroy();
        mZView->destroy();
        delete mXView;
        delete mYView;
        delete mZView;
    }

    void AnimRotateRollup::adjustSize() {
        RECT size = mXView->clientRect();
        const auto height = (size.bottom - size.top) * 3;

        size.top += size.bottom - size.top;
        size.left = 0;
        mYView->setWindowPos(size);

        size.top += size.top;
        size.left = 0;
        mZView->setWindowPos(size);

        IRollupPanel * panel = mIp->GetPanel(hwnd());
        panel->SetDlgHeight(height);
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
