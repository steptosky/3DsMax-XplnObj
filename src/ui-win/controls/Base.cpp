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

#include "Base.h"
#include "commctrl.h"

namespace ui {
namespace ctrl {

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    Base::Base() {
        init();
    }

    Base::Base(HWND inHWnd) {
        init();
        Base::setup(inHWnd);
    }

    Base::Base(HWND inParent, int inControlID) {
        init();
        Base::setup(inParent, inControlID);
    }

    Base::~Base() { }

    void Base::init() {
        mHwnd = nullptr;
        mControlID = 0;
    }

    void Base::release() {
        init();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool Base::setup(HWND inParent, int inControlID) {
        mControlID = inControlID;
        mHwnd = GetDlgItem(inParent, inControlID);
        return static_cast<bool>(*this); // there is the operator bool
    }

    void Base::setup(HWND inHWnd) {
        mHwnd = inHWnd;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    Base::String Base::text() const {
        assert(mHwnd);
        int textSize = GetWindowTextLength(mHwnd);
        TCHAR * text = new TCHAR[textSize + 1];
        GetWindowText(mHwnd, text, textSize + 1);
        String t(text);
        delete[] text;
        return t;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void Base::setToolTip(const String & text, const uint32_t showTimeMilisec) {
        assert(mHwnd);
        // Create a tooltip.
        HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                     WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                     mHwnd, NULL, NULL, NULL);

        SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        // Set up "tool" information. In this case, the "tool" is the entire parent window.

        TTTOOLINFO ti = {0};
        ti.cbSize = sizeof(TTTOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = mHwnd;
        ti.hinst = nullptr;

        size_t length = text.length() + 1;
        ti.lpszText = new TCHAR[length];
        _tcscpy_s(ti.lpszText, length, text.c_str());

        GetClientRect(mHwnd, &ti.rect);
        // Associate the tooltip with the "tool" window.
        SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        SendMessage(hwndTT, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM((showTimeMilisec), (0)));
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void Base::setIndeterminate(bool inState, const String & text) {
        mIsIndeterminate = inState;
        if (inState) {
            mTmpIndetStr = this->text();
            setText(text);
        }
        else {
            setText(mTmpIndetStr.c_str());
        }
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
