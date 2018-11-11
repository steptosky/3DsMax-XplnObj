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

#include "ExportObjList.h"
#include <windows.h>

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        /////////////////////////////////////////* Static area *////////////////////////////////////////////
        /**************************************************************************************************/

        std::optional<INT_PTR> CALLBACK ExportObjList::procedure(const UINT message, const WPARAM wParam, const LPARAM lParam) {
            if (message == WM_NOTIFY && LOWORD(wParam) == controlId()) {
                const auto someItem = reinterpret_cast<LPNMHDR>(lParam);
                switch (someItem->code) {
                    case LVN_ITEMCHANGED: {
                        const auto lpItem = reinterpret_cast<LPNMLISTVIEW>(lParam);
                        if (lpItem->uChanged == LVIF_STATE) {
                            if (onItemChanged) {
                                onItemChanged(this, lpItem->iItem);
                            }
                        }
                        break;
                    }
                    default: break;
                }
                return FALSE;
            }
            return std::nullopt;
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void ExportObjList::onParentInit(HWND owner) {
            Control::onParentInit(owner);
            reset();
            setup();
        }

        void ExportObjList::reset() {
            mLastFreeId = 0;
            mWinColor = 0;
            mWinTextColor = 0;
            mWinBrush = nullptr;
        }

        /**************************************************************************************************/
        ///////////////////////////////////////////* Functions *////////////////////////////////////////////
        /**************************************************************************************************/

        void ExportObjList::checkAll(const bool state) {
            const auto count = ListView_GetItemCount(hwnd());
            for (int nItem = 0; nItem < count; ++nItem) {
                checkItem(nItem, state);
            }
        }

        void ExportObjList::checkItem(const int idx, const bool state) {
            ListView_SetCheckState(hwnd(), idx, state);
        }

        /**************************************************************************************************/
        ///////////////////////////////////////////* Functions *////////////////////////////////////////////
        /**************************************************************************************************/

        int ExportObjList::addItem(const MStr & name) {
            LVITEM item;
            SecureZeroMemory(&item, sizeof(item));
            item.iItem = mLastFreeId;

            if (ListView_InsertItem(hwnd(), &item) == -1) {
                LError << WinCode(GetLastError());
                return -1;
            }
            ++mLastFreeId;

            item.mask = LVIF_TEXT;
            item.iSubItem = 1;
            item.pszText = const_cast<LPTSTR>(name.data());

            if (ListView_SetItem(hwnd(), &item) == -1) {
                LError << WinCode(GetLastError());
            }
            return mLastFreeId - 1;
        }

        bool ExportObjList::isChecked(const int idx) {
            return idx < 0 ? false : ListView_GetCheckState(hwnd(), idx) != 0;
        }

        void ExportObjList::setup() {
            ListView_SetBkColor(hwnd(), GetCustSysColor(COLOR_WINDOW));
            ListView_SetTextColor(hwnd(), GetCustSysColor(COLOR_WINDOWTEXT));
            ListView_SetTextBkColor(hwnd(), GetCustSysColor(COLOR_WINDOW));

            ListView_SetExtendedListViewStyle(hwnd(), LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

            LVCOLUMN lvc;
            SecureZeroMemory(&lvc, sizeof(lvc));
            lvc.mask = LVCF_TEXT;
            lvc.pszText = _T("V");
            ListView_InsertColumn(hwnd(), 0, &lvc);
            lvc.mask = LVCF_TEXT;
            lvc.iSubItem++;
            lvc.pszText = _T("X-Objects");
            ListView_InsertColumn(hwnd(), 1, &lvc);

            ListView_SetColumnWidth(hwnd(), 0, LVSCW_AUTOSIZE_USEHEADER);
            ListView_SetColumnWidth(hwnd(), 1, LVSCW_AUTOSIZE_USEHEADER);
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
