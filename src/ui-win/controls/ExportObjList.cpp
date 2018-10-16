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

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <windows.h>
#include <commctrl.h>
#include <winuser.h>

namespace ui {
namespace ctrl {

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool ExportObjList::setup(const HWND parent, const int controlId) {
        DbgAssert(parent);
        const auto res = Base::setup(parent, controlId);
        if (!res) {
            return res;
        }
        setup();
        return false;
    }

    void ExportObjList::setup(const HWND hwnd) {
        Base::setup(hwnd);
        setup();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void ExportObjList::checkAll(const bool state) {
        assert(hwnd());
        for (int nItem = 0; nItem < ListView_GetItemCount(hwnd()); nItem++) {
            checkItem(nItem, state);
        }
    }

    void ExportObjList::checkItem(const int idx, const bool state) {
        assert(hwnd());
        ListView_SetCheckState(hwnd(), idx, state);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    int ExportObjList::addItem(const String & name) {
        assert(hwnd());
        LVITEM lvi = {0};
        lvi.iItem = mLastFreeId;

        // Insert the item itself
        // Since we're always inserting item 0, new items will appear on top
        const int idx = ListView_InsertItem(hwnd(), &lvi);
        if (idx == -1) {
            return idx;
        }
        ++mLastFreeId;

        // Insert the subitems (columns)
        lvi.mask = LVIF_TEXT;
        lvi.iSubItem = 1;

        const std::size_t length = name.length() + 1;
        lvi.pszText = new TCHAR[length];
        _tcscpy_s(lvi.pszText, length, name.c_str());

        ListView_SetItem(hwnd(), &lvi);
        return mLastFreeId - 1;
    }

    bool ExportObjList::isChecked(const int idx) {
        assert(hwnd());
        if (idx == -1) {
            return false;
        }
        return ListView_GetCheckState(hwnd(), idx) != 0;
    }

    void ExportObjList::setup() {
        assert(hwnd());
        mWinColor = GetCustSysColor(COLOR_WINDOW);
        mWinTextColor = GetCustSysColor(COLOR_WINDOWTEXT);
        mWinBrush = GetCustSysColorBrush(COLOR_WINDOW);
        DbgAssert(mWinBrush);

        ListView_SetExtendedListViewStyle(hwnd(), LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

        LVCOLUMN lvc = {0};
        lvc.mask = LVCF_TEXT;
        lvc.pszText = mHeadersText[0];
        ListView_InsertColumn(hwnd(), 0, &lvc);
        lvc.mask = LVCF_TEXT;
        lvc.iSubItem++;
        lvc.pszText = mHeadersText[1];
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
        mHeader = ListView_GetHeader(hwnd());
        DbgAssert(mHeader);
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    LRESULT ExportObjList::subClassProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam,
                                        UINT_PTR /*uIdSubclass*/, DWORD_PTR /*dwRefData*/) {
        //ExportObjList * control = reinterpret_cast<ExportObjList*>(dwRefData);
        switch (uMsg) {
                // Note for future
                //			case WM_NOTIFY: {
                //				LPNMHDR param = reinterpret_cast<LPNMHDR>(lParam);
                //				if (param->code == NM_CUSTOMDRAW && param->hwndFrom == control->mHeader) {
                //					LPNMCUSTOMDRAW item = reinterpret_cast<LPNMCUSTOMDRAW>(lParam);
                //					switch (item->dwDrawStage) {
                //						case CDDS_PREPAINT: return CDRF_NOTIFYITEMDRAW;
                //						case CDDS_ITEMPREPAINT: {
                //							SetTextColor(item->hdc, control->mWinTextColor);
                //							SetBkColor(item->hdc, control->mWinColor);
                //							FillRect(item->hdc, &item->rc, control->mWinBrush);
                //							FrameRect(item->hdc, &item->rc, GetCustSysColorBrush(COLOR_BTNHIGHLIGHT));
                //							if (item->dwItemSpec == 1) {
                //								DrawText(item->hdc, control->mHeadersText[1], 4, &item->rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                //							}
                //							else {
                //								DrawText(item->hdc, control->mHeadersText[0], 1, &item->rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                //							}
                //							return CDRF_SKIPDEFAULT;
                //						}
                //						default: break;
                //					}
                //					return CDRF_DODEFAULT;
                //				}
                //				break;
                //			}
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
        LPNMCUSTOMDRAW item = reinterpret_cast<LPNMCUSTOMDRAW>(lParam);
        const auto parentWin = parent();
        DbgAssert(parentWin);

        // Background
        if (mWinBrush) {
            FillRect(item->hdc, &item->rc, mWinBrush);
        }
        if (parentWin) {
            // Items background
            SetWindowLongPtr(parentWin, DWLP_MSGRESULT,
                             static_cast<LONG>(drawItems(lParam, this)));
        }
        return TRUE;
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
