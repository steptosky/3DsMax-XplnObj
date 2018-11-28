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
#include "CheckBox.h"

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        /////////////////////////////////////////* Static area *////////////////////////////////////////////
        /**************************************************************************************************/

        std::optional<INT_PTR> CALLBACK CheckBox::procedure(const UINT message, const WPARAM wParam, const LPARAM) {
            if (message == WM_COMMAND && LOWORD(wParam) == controlId()) {
                if (onStateChanged) {
                    onStateChanged(this, isChecked());
                }
                return FALSE;
            }
            return std::nullopt;
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void CheckBox::onParentInit(HWND owner) {
            mOwner = owner;
            Control::onParentInit(owner);
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void CheckBox::setChecked(const bool state) {
            CheckDlgButton(mOwner, controlId(), state);
        }

        bool CheckBox::isChecked() const {
            return IsDlgButtonChecked(mOwner, controlId()) != 0;
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
