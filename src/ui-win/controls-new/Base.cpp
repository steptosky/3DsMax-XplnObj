/*
**  Copyright(C) 2018, StepToSky
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

#include <windows.h>
#include "Base.h"
#include "resource/ResHelper.h"

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        /////////////////////////////////////////* Static area *////////////////////////////////////////////
        /**************************************************************************************************/

        INT_PTR CALLBACK Base::procedure(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
            if (message == WM_INITDIALOG) {
                const auto currentObject = reinterpret_cast<Base*>(lParam);
                currentObject->mHwnd = hWnd;
                SetWindowLongPtr(hWnd, GWLP_USERDATA, static_cast<LONG_PTR>(lParam));
            }
            auto * currentObject = reinterpret_cast<Base*>(static_cast<LONG_PTR>(::GetWindowLongPtr(hWnd, GWLP_USERDATA)));
            if (!currentObject) {
                return FALSE;
            }
            return currentObject->procedure(message, wParam, lParam).value_or(FALSE);
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        HWND Base::parent() {
            DbgAssert(mHwnd);
            return GetParent(mHwnd);
        }

        void Base::enable(const bool state) {
            if (mHwnd) {
                EnableWindow(mHwnd, static_cast<int>(state));
            }
        }

        void Base::show(const bool state) {
            if (mHwnd) {
                ShowWindow(mHwnd, static_cast<int>(state));
            }
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void Base::setRect(const RECT & rect, const bool repaint) {
            if (mHwnd) {
                MoveWindow(mHwnd, rect.left, rect.top, rect.right, rect.bottom, static_cast<int>(repaint));
            }
        }

        RECT Base::winRect() const {
            RECT r{0, 0, 0, 0};
            if (mHwnd) {
                GetWindowRect(mHwnd, &r);
            }
            return r;
        }

        RECT Base::clientRect() const {
            RECT r{0, 0, 0, 0};
            if (mHwnd) {
                GetClientRect(mHwnd, &r);
            }
            return r;
        }

        void Base::move(const POINT & point, const bool repaint) {
            if (mHwnd) {
                const auto s = size();
                MoveWindow(mHwnd, point.x, point.y, s.cx, s.cy, static_cast<int>(repaint));
            }
        }

        void Base::setSize(const SIZE & size) {
            RECT r = winRect();
            r.right = r.left + size.cx;
            r.bottom = r.top + size.cy;
            setRect(r);
        }

        SIZE Base::size() const {
            const auto r = winRect();
            return SIZE{r.right - r.left, r.bottom - r.top};
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void Base::setText(const MStr & text) {
            if (mHwnd) {
                SetWindowText(mHwnd, text.data());
            }
        }

        void Base::setXObjText(const std::string & text) {
            if (mHwnd) {
                SetWindowTextA(mHwnd, text.data());
            }
        }

        MStr Base::text() const {
            MStr out;
            if (mHwnd) {
                const auto textSize = GetWindowTextLength(mHwnd);
#if MAX_VERSION_MAJOR > 14 // 2012
                GetWindowText(mHwnd, out.dataForWrite(textSize), textSize);
#else
                out.Resize(textSize);
                GetWindowText(mHwnd, out.data(), textSize);
#endif
            }
            return out;
        }

        std::string Base::textXObj() const {
            return xobj::fromMStr(text());
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void Base::setToolTip(const MStr & text, const std::uint32_t showTimeMilliseconds) {
            assert(mHwnd);

            const HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, nullptr,
                                               WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                               mHwnd, nullptr, nullptr, nullptr);

            SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

            TTTOOLINFO ti;
            ti.cbSize = sizeof(TTTOOLINFO);
            ti.uFlags = TTF_SUBCLASS;
            ti.hwnd = mHwnd;
            ti.hinst = ResHelper::hInstance;
            ti.lpszText = const_cast<LPTSTR>(text.data());

            GetClientRect(mHwnd, &ti.rect);

            SendMessage(hwndTT, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
            SendMessage(hwndTT, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM((showTimeMilliseconds), (0)));
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
