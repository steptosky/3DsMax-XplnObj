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

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <cassert>
#include "common/String.h"
#include "common/Logger.h"

namespace ui {
namespace ctrl {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    class Base {
    public:

#ifdef UNICODE
        typedef std::wstring String;
#else
        typedef std::string String;
#endif // UNICODE

        //--------------------------------------------------

        Base();
        explicit Base(HWND inHWnd);
        Base(HWND inParent, int inControlID);
        virtual ~Base();

        //--------------------------------------------------

        operator bool() const { return mHwnd != nullptr; }
        virtual bool setup(HWND inParent, int inControlID);
        virtual void setup(HWND inHWnd);
        virtual void release();

        //--------------------------------------------------

        HWND parent() {
            assert(mHwnd);
            return GetParent(mHwnd);
        }

        HWND parent() const {
            assert(mHwnd);
            return GetParent(mHwnd);
        }

        int controlId() const { return mControlID; }
        HWND hwnd() { return mHwnd; }
        HWND hwnd() const { return mHwnd; }

        //--------------------------------------------------

        void enable(bool state) {
            assert(mHwnd);
            EnableWindow(mHwnd, static_cast<int>(state));
        }

        void enable() {
            enable(true);
        }

        void disable() {
            enable(false);
        }

        //--------------------------------------------------

        void setToolTip(const String & text, const uint32_t showTimeMilisec = 10000);

        //--------------------------------------------------

        RECT winRect() const {
            assert(mHwnd);
            RECT r{0, 0, 0, 0};
            BOOL res = GetWindowRect(mHwnd, &r);
            if (!res) {
                XLError << WinCode(GetLastError());
            }
            return r;
        }

        RECT clientRect() const {
            assert(mHwnd);
            RECT r{0, 0, 0, 0};
            BOOL res = GetClientRect(mHwnd, &r);
            if (!res) {
                XLError << WinCode(GetLastError());
            }
            return r;
        }

        RECT rect() const {
            assert(mHwnd);
            RECT r = winRect();
            MapWindowPoints(HWND_DESKTOP, parent(), reinterpret_cast<LPPOINT>(&r), 2);
            return r;
        }

        void move(const POINT & point, bool repaint = false) {
            assert(mHwnd);
            SIZE s = size();
            BOOL res = MoveWindow(mHwnd, point.x, point.y, s.cx, s.cy, static_cast<int>(repaint));
            if (!res) {
                XLError << WinCode(GetLastError());
            }
        }

        void setRect(const RECT & rect, bool repaint = false) {
            assert(mHwnd);
            BOOL res = MoveWindow(mHwnd, rect.left, rect.top, rect.right, rect.bottom, static_cast<int>(repaint));
            if (!res) {
                XLError << WinCode(GetLastError());
            }
        }

        void setSize(const SIZE & size) {
            assert(mHwnd);
            RECT r = rect();
            r.right = r.left + size.cx;
            r.bottom = r.top + size.cy;
            setRect(r);
        }

        SIZE size() const {
            assert(mHwnd);
            RECT r = winRect();
            return SIZE{r.right - r.left, r.bottom - r.top};
        }

        //--------------------------------------------------

        void show(bool state) {
            assert(mHwnd);
            ShowWindow(mHwnd, static_cast<int>(state));
        }

        void show() {
            show(true);
        }

        void hide() {
            show(false);
        }

        //--------------------------------------------------

        void setText(const std::string & text) {
            assert(mHwnd);
            SetWindowTextA(mHwnd, text.c_str());
        }

        void setText(const std::wstring & text) {
            assert(mHwnd);
            SetWindowTextW(mHwnd, text.c_str());
        }

        String text() const;

        //--------------------------------------------------

        void setIndeterminate(bool inState, const String & text = _T(""));

        bool isIndeterminate() const {
            return mIsIndeterminate;
        }

        //--------------------------------------------------

    private:

        Base(const Base &) = delete;
        Base & operator =(const Base &) = delete;

        void init();

        int mControlID;
        HWND mHwnd;
        String mTmpIndetStr;
        bool mIsIndeterminate;

    };

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
