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
#include "ComboBox.h"

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        /////////////////////////////////////////* Static area *////////////////////////////////////////////
        /**************************************************************************************************/

        std::optional<INT_PTR> CALLBACK ComboBox::procedure(const UINT message, const WPARAM wParam, const LPARAM) {
            if (message == WM_COMMAND && LOWORD(wParam) == controlId()) {
                if (HIWORD(wParam) == CBN_SELCHANGE) {
                    if (onSelectionChanged) {
                        onSelectionChanged(this);
                    }
                }
                return FALSE;
            }
            return std::nullopt;
        }

        /**************************************************************************************************/
        ///////////////////////////////////////////* Functions *////////////////////////////////////////////
        /**************************************************************************************************/

        void ComboBox::addItem(const MStr & text, const bool selected) {
            const auto pos = ComboBox_AddString(hwnd(), text.data());
            if (pos == CB_ERR) {
                return;
            }
            if (selected) {
                ComboBox_SetCurSel(hwnd(), pos);
            }
        }

        void ComboBox::removeItem(const MStr & text, const bool selectAfter) {
            const auto pos = ComboBox_FindStringExact(hwnd(), 0, text.data());
            if (pos == CB_ERR) {
                return;
            }
            ComboBox_DeleteString(hwnd(), pos);
            if (selectAfter) {
                const auto itemsCount = count();
                if (itemsCount == CB_ERR) {
                    return;
                }
                setSelected(itemsCount - 1);
                if (onSelectionChanged) {
                    onSelectionChanged(this);
                }
            }
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        int ComboBox::count() const {
            return ComboBox_GetCount(hwnd());
        }

        void ComboBox::clear() {
            SendMessage(hwnd(), CB_RESETCONTENT, NULL, NULL);
        }

        /**************************************************************************************************/
        ///////////////////////////////////////////* Functions *////////////////////////////////////////////
        /**************************************************************************************************/

        void ComboBox::setSelected(const int index) {
            ComboBox_SetCurSel(hwnd(), index);
        }

        bool ComboBox::setSelected(const MStr & text) {
            return (ComboBox_SelectString(hwnd(), -1, text.data()) != CB_ERR);
        }

        std::optional<int> ComboBox::selectedIndex() const {
            return ComboBox_GetCurSel(hwnd());
        }

        std::optional<MStr> ComboBox::selectedText() const {
            const auto index = selectedIndex();
            if (!index) {
                return std::nullopt;
            }
            const auto textSize = ComboBox_GetLBTextLen(hwnd(), *index);
            if (textSize == CB_ERR) {
                return std::nullopt;
            }
            MStr out;
#if MAX_VERSION_MAJOR > 14 // 2012
            ComboBox_GetLBText(hwnd(), *index, out.dataForWrite(textSize));
#else
            out.Resize(textSize);
            ComboBox_GetLBText(hwnd(), *index, out.data());
#endif
            return out;
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
