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

#include "ListBox.h"
#include <WindowsX.h>

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        /////////////////////////////////////////* Static area *////////////////////////////////////////////
        /**************************************************************************************************/

        std::optional<INT_PTR> CALLBACK ListBox::procedure(const UINT message, const WPARAM wParam, const LPARAM) {
            if (message == WM_COMMAND && LOWORD(wParam) == controlId()) {
                if (HIWORD(wParam) == LBN_SELCHANGE) {
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

        void ListBox::addItem(const MStr & text, const bool selected) {
            const auto pos = ListBox_AddString(hwnd(), text.data());
            if (selected) {
                ListBox_SetCurSel(hwnd(), pos);
            }
        }

        /**************************************************************************************************/
        ///////////////////////////////////////////* Functions *////////////////////////////////////////////
        /**************************************************************************************************/

        MStr ListBox::itemText(const int index) const {
            MStr result;
            if (index >= 0) {
                const auto textLen = ListBox_GetTextLen(hwnd(), index);
#if MAX_VERSION_MAJOR > 14 // 2012
                ListBox_GetText(hwnd(), index, result.dataForWrite(textLen));
#else
                result.Resize(textLen);
                ListBox_GetText(hwnd(), index, result.data());
#endif
            }
            return result;
        }

        void ListBox::removeItem(const int index) {
            ListBox_DeleteString(hwnd(), index);
        }

        int ListBox::count() const {
            return ListBox_GetCount(hwnd());
        }

        void ListBox::clear() {
            SendMessage(hwnd(), LB_RESETCONTENT, NULL, NULL);
        }

        /**************************************************************************************************/
        ///////////////////////////////////////////* Functions *////////////////////////////////////////////
        /**************************************************************************************************/

        void ListBox::setSelected(const int index) {
            ListBox_SetCurSel(hwnd(), index);
        }

        std::optional<MStr> ListBox::selectedText() const {
            const auto index = selectedIndex();
            if (!index) {
                return std::nullopt;
            }
            return itemText(*index);
        }

        std::optional<int> ListBox::selectedIndex() const {
            const auto index = ListBox_GetCurSel(hwnd());
            if (index == LB_ERR) {
                return std::nullopt;
            }
            return index;
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
