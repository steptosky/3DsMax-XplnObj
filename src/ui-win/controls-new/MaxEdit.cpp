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
#include "MaxEdit.h"
#include "ui-win/Utils.h"

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        /////////////////////////////////////////* Static area *////////////////////////////////////////////
        /**************************************************************************************************/

        std::optional<INT_PTR> CALLBACK MaxEdit::procedure(const UINT message, const WPARAM wParam, const LPARAM) {
            if (message == WM_CUSTEDIT_ENTER && LOWORD(wParam) == controlId()) {
                if (mControl) {
                    if (!mControl->HasFocus()) {
                        if (onLostFocus) {
                            onLostFocus(this);
                        }
                    }
                    else {
                        if (onEnter) {
                            onEnter(this);
                        }
                    }
                }
                return FALSE;
            }
            return std::nullopt;
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void MaxEdit::onParentInit(HWND owner) {
            mControl = GetICustEdit(GetDlgItem(owner, controlId()));
            DbgAssert(mControl);
            if (mControl) {
                setHwnd(mControl->GetHwnd());
            }
            DbgAssert(hwnd());
        }

        void MaxEdit::onParentDestroy(HWND) {
            if (mControl) {
                ReleaseICustEdit(mControl);
                mControl = nullptr;
            }
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

#if MAX_VERSION_MAJOR > 10 // 2008
        void MaxEdit::setToolTip(const MStr & text, const std::uint32_t) {
            if (mControl) {
                mControl->SetTooltip(true, MStr(text));
            }
        }
#else
        void MaxEdit::setToolTip(const MStr & text, const std::uint32_t showTimeMilliseconds) {
            if (mControl) {
                Base::setToolTip(text, showTimeMilliseconds);
            }
        }
#endif

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void MaxEdit::setText(const MStr & str) {
            if (mControl) {
#if MAX_VERSION_MAJOR > 11 // 2009
                mControl->SetText(str);
#else
                mControl->SetText(MStr(str));
#endif
            }
        }

        void MaxEdit::setXObjText(const std::string & str) {
            setText(xobj::toMStr(str));
        }

        MStr MaxEdit::text() const {
            MStr str;
            if (mControl) {
                Utils::getText(mControl, str);
            }
            return str;
        }

        std::string MaxEdit::textXObj() const {
            return xobj::fromMStr(text());
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
