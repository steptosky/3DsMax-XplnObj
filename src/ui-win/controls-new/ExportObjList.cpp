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
#include <minwinbase.h>

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
                    case NM_CUSTOMDRAW: {
                        return draw(lParam);
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
            if (idx < 0) {
                return false;
            }
            return ListView_GetCheckState(hwnd(), idx) != 0;
        }

        void ExportObjList::setup() {
            mWinColor = GetCustSysColor(COLOR_WINDOW);
            mWinTextColor = GetCustSysColor(COLOR_WINDOWTEXT);
            mWinBrush = GetCustSysColorBrush(COLOR_WINDOW);
            DbgAssert(mWinBrush);

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

            // Set column widths
            ListView_SetColumnWidth(hwnd(), 0, LVSCW_AUTOSIZE_USEHEADER);
            ListView_SetColumnWidth(hwnd(), 1, LVSCW_AUTOSIZE_USEHEADER);

            if (mWinBrush) {
                if (!SetWindowSubclass(hwnd(), &ExportObjList::subClassProc, 1, reinterpret_cast<DWORD_PTR>(this))) {
                    LError << "Can't subclass " << TOTEXT(ExportObjList);
                }
            }
            else {
                LError << "Can't get window brush " << TOTEXT(ExportObjList);
            }
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        LRESULT ExportObjList::subClassProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam,
                                            UINT_PTR /*uIdSubclass*/, DWORD_PTR /*dwRefData*/) {
            switch (uMsg) {
                case WM_NCDESTROY: {
                    // You must remove your window subclass before the window being 
                    // subclassed is destroyed. This is typically done either by removing 
                    // the subclass once your temporary need has passed, or if you are 
                    // installing a permanent subclass, by inserting a call to 
                    // RemoveWindowSubclass inside the subclass procedure itself:
                    if (!RemoveWindowSubclass(hWnd, &ExportObjList::subClassProc, 1)) {
                        LError << "Can't remove subclass " << TOTEXT(ExportObjList);
                    }
                    break;
                }
                default: break;
            }
            return DefSubclassProc(hWnd, uMsg, wParam, lParam);
        }

        // Items background
        LRESULT ExportObjList::drawItems(LPARAM lParam, ExportObjList * data) {
            const auto lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
            switch (lplvcd->nmcd.dwDrawStage) {
                case CDDS_PREPAINT: return CDRF_NOTIFYITEMDRAW;
                case CDDS_ITEMPREPAINT: return CDRF_NOTIFYSUBITEMDRAW;
                case CDDS_SUBITEM | CDDS_ITEMPREPAINT: {
                    //Before a subitem is drawn
                    lplvcd->clrText = data->mWinTextColor;
                    lplvcd->clrTextBk = data->mWinColor;
                    lplvcd->clrFace = data->mWinColor;
                    return CDRF_NEWFONT;
                }
                default: break;
            }
            return CDRF_DODEFAULT;
        }

        LRESULT ExportObjList::draw(LPARAM lParam) {
            const auto item = reinterpret_cast<LPNMCUSTOMDRAW>(lParam);
            const auto parentWin = parent();
            DbgAssert(parentWin);

            // Background
            if (mWinBrush) {
                FillRect(item->hdc, &item->rc, mWinBrush);
            }
            if (parentWin) {
                // Items background
                SetWindowLongPtr(parentWin, DWLP_MSGRESULT, static_cast<LONG>(drawItems(lParam, this)));
            }
            return TRUE;
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
