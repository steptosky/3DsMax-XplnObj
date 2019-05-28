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

#include "DcList.h"

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <windows.h>
#include "common/Logger.h"

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        /////////////////////////////////////////* Static area *////////////////////////////////////////////
        /**************************************************************************************************/

        std::optional<INT_PTR> CALLBACK DcList::procedure(const UINT message, const WPARAM wParam, const LPARAM lParam) {
            if (message == WM_NOTIFY && LOWORD(wParam) == controlId()) {
                const auto item = reinterpret_cast<LPNMHDR>(lParam);
                if (item->code == NM_CLICK) {
                    if (onSelectionChanged) {
                        onSelectionChanged(this);
                    }
                }
                return FALSE;
            }
            return std::nullopt;
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void DcList::onParentInit(HWND owner) {
            Control::onParentInit(owner);
            mColumnsCount = 0;
            ListView_SetExtendedListViewStyle(hwnd(), LVS_EX_FULLROWSELECT);
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void DcList::setupColumnsWidth(const bool useId) {
            if (useId) {
                ListView_SetColumnWidth(hwnd(), 0, 45);
                ListView_SetColumnWidth(hwnd(), 1, 55);
                ListView_SetColumnWidth(hwnd(), 2, 400);
                ListView_SetColumnWidth(hwnd(), 3, 412);
            }
            else {
                ListView_SetColumnWidth(hwnd(), 0, 45);
                ListView_SetColumnWidth(hwnd(), 1, 400);
                ListView_SetColumnWidth(hwnd(), 2, 465);
            }
        }

        void DcList::setupColumns(const bool useId) {
            if (mColumnsCount == 0) {
                int currIndex = 0;

                LVCOLUMN column;
                column.mask = LVCF_TEXT;
                column.pszText = _T("#");
                ListView_InsertColumn(hwnd(), currIndex++, &column);
                if (useId) {
                    column.pszText = _T("id");
                    ListView_InsertColumn(hwnd(), currIndex++, &column);
                }
                column.pszText = _T("key");
                ListView_InsertColumn(hwnd(), currIndex++, &column);
                column.pszText = _T("description");
                ListView_InsertColumn(hwnd(), currIndex, &column);

                mColumnsCount = useId ? 4 : 3;
                setupColumnsWidth(useId);
            }
            else {
                if (useId && mColumnsCount != 4) {
                    LVCOLUMN column;
                    column.mask = LVCF_TEXT;
                    column.pszText = _T("id");
                    ListView_InsertColumn(hwnd(), 1, &column);
                    mColumnsCount = 4;
                    setupColumnsWidth(useId);
                    return;
                }
                if (!useId && mColumnsCount != 3) {
                    ListView_DeleteColumn(hwnd(), 1);
                    mColumnsCount = 3;
                    setupColumnsWidth(useId);
                }
            }
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        template<typename T>
        bool isFiltered(const T & data, const std::string & filter) {
            if (filter.empty()) {
                return false;
            }
            auto pos = data.mKey.find(filter);
            if (pos != std::string::npos) {
                return false;
            }

            pos = data.mDescription.find(filter);
            return pos == std::string::npos;
        }

        template<typename T>
        void setupSimListView(const HWND listHWnd, const T & data,
                              const std::string & filter) {
            ListView_SetItemCount(listHWnd, int(data.size() * 3));

            int counter = 0;

            LVITEM item;
            SecureZeroMemory(&item, sizeof(item));
            item.mask = LVIF_TEXT;
            item.iItem = 0;
            item.stateMask = 0;
            item.state = 0;
            item.iImage = 0;
            for (const auto & val : data) {
                ++counter;
                if (isFiltered(val, filter)) {
                    continue;
                }
                item.iSubItem = 0;
                auto actualString = xobj::toMStr(std::to_string(counter));
                item.pszText = const_cast<LPTSTR>(actualString.data());
                if (ListView_InsertItem(listHWnd, &item) == -1) {
                    XLError << WinCode(GetLastError());
                }

                item.iSubItem = 1;
                actualString = xobj::toMStr(val.mKey);
                item.pszText = const_cast<LPTSTR>(actualString.data());
                if (ListView_SetItem(listHWnd, &item) == -1) {
                    XLError << WinCode(GetLastError());
                }

                item.iSubItem = 2;
                actualString = xobj::toMStr(val.mDescription);
                item.pszText = const_cast<LPTSTR>(actualString.data());
                if (!ListView_SetItem(listHWnd, &item)) {
                    XLError << WinCode(GetLastError());
                }
                ++item.iItem;
            }
        }

        template<typename T>
        void setupCustomListView(const HWND listHWnd, const T & data,
                                 const std::string & filter) {
            ListView_SetItemCount(listHWnd, int(data.size() * 4));

            int counter = 0;

            LVITEM item;
            SecureZeroMemory(&item, sizeof(item));
            item.mask = LVIF_TEXT;
            item.iItem = 0;
            item.stateMask = 0;
            item.state = 0;
            item.iImage = 0;
            for (const auto & val : data) {
                ++counter;
                if (isFiltered(val, filter)) {
                    continue;
                }
                item.iSubItem = 0;
                auto actualString = xobj::toMStr(std::to_string(counter));
                item.pszText = const_cast<LPTSTR>(actualString.data());
                if (ListView_InsertItem(listHWnd, &item) == -1) {
                    XLError << WinCode(GetLastError());
                }

                item.iSubItem = 1;
                actualString = _T("none");
                if (val.mId != xobj::Dataref::invalidId()) {
                    actualString = xobj::toMStr(std::to_string(val.mId));
                }
                item.pszText = const_cast<LPTSTR>(actualString.data());
                if (ListView_SetItem(listHWnd, &item) == -1) {
                    XLError << WinCode(GetLastError());
                }

                item.iSubItem = 2;
                actualString = xobj::toMStr(val.mKey);
                item.pszText = const_cast<LPTSTR>(actualString.data());
                if (ListView_SetItem(listHWnd, &item) == -1) {
                    XLError << WinCode(GetLastError());
                }

                item.iSubItem = 3;
                actualString = xobj::toMStr(val.mDescription);
                item.pszText = const_cast<LPTSTR>(actualString.data());
                if (!ListView_SetItem(listHWnd, &item)) {
                    XLError << WinCode(GetLastError());
                }
                ++item.iItem;
            }
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void DcList::showData(const md::DatarefsFile::Ptr & file,
                              const std::optional<std::size_t> selectedItem,
                              const std::string & filter) {
            clear();
            if (!file) {
                return;
            }
            setupColumns(file->mUsesId);
            if (mColumnsCount == 4) {
                setupCustomListView(hwnd(), file->mData, filter);
            }
            else if (mColumnsCount == 3) {
                setupSimListView(hwnd(), file->mData, filter);
            }
            else {
                XLError << "incorrect columns count: " << mColumnsCount;
                return;
            }
            setSelected(selectedItem);
        }

        void DcList::showData(const md::CommandsFile::Ptr & file,
                              const std::optional<std::size_t> selectedItem,
                              const std::string & filter) {
            clear();
            if (!file) {
                return;
            }
            setupColumns(file->mUsesId);
            if (mColumnsCount == 4) {
                setupCustomListView(hwnd(), file->mData, filter);
            }
            else if (mColumnsCount == 3) {
                setupSimListView(hwnd(), file->mData, filter);
            }
            else {
                XLError << "incorrect columns count: " << mColumnsCount;
                return;
            }
            setSelected(selectedItem);
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        void DcList::setSelected(const std::optional<std::size_t> selectedItem) {
            if (selectedItem) {
                ListView_SetItemState(hwnd(), int(*selectedItem), LVIS_SELECTED | LVIS_FOCUSED, 0x000F);
                ListView_EnsureVisible(hwnd(), int(*selectedItem), true);
            }
            else {
                ListView_SetItemState(hwnd(), -1, 0, 0);
            }
        }

        void DcList::clear() {
            ListView_DeleteAllItems(hwnd());
        }

        /**************************************************************************************************/
        //////////////////////////////////////////* Functions */////////////////////////////////////////////
        /**************************************************************************************************/

        std::optional<std::size_t> DcList::selectedIndex() {
            const auto index = ListView_GetSelectionMark(hwnd());
            if (index == -1) {
                return std::nullopt;
            }

            const std::size_t textBufSize = 16;
            TCHAR textBuf[textBufSize] = {0};
            LVITEM item;
            item.mask = LVIF_TEXT;
            item.iItem = index;
            item.iSubItem = 0;
            item.pszText = textBuf;
            item.cchTextMax = textBufSize;
            ListView_GetItem(hwnd(), &item);
            try {
                auto indexFromStr = std::stoul(item.pszText);
                DbgAssert(indexFromStr != 0);
                // user friendly numbering starts from 1
                indexFromStr -= 1;
                return std::size_t(indexFromStr);
            }
            catch (const std::exception & e) {
                XLError << "Can't retrieve index from the item <" << index << ">. Reason: " << e.what();
                return std::nullopt;
            }
        }

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
