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
#include <commctrl.h>

namespace win {

	/**************************************************************************************************/
	/////////////////////////////////////////* Static area *////////////////////////////////////////////
	/**************************************************************************************************/

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	ExportObjList::ExportObjList() { }

	ExportObjList::~ExportObjList() { }

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	bool ExportObjList::setup(HWND inParent, int inControlID) {
		bool res = Base::setup(inParent, inControlID);
		if (!res) {
			return res;
		}
		setup();
		return false;
	}

	void ExportObjList::setup(HWND inParent) {
		Base::setup(inParent);
		setup();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ExportObjList::checkAll(bool state) {
		assert(hwnd());
		for (int nItem = 0; nItem < ListView_GetItemCount(hwnd()); nItem++) {
			checkItem(nItem, state);
		}
	}

	void ExportObjList::checkItem(int idx, bool state) {
		assert(hwnd());
		ListView_SetCheckState(hwnd(), idx, state);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	int ExportObjList::addItem(const String & inName) {
		assert(hwnd());
		LVITEM lvi = {0};
		lvi.iItem = mLastFreeId;

		// Insert the item itself
		// Since we're always inserting item 0, new items will appear on top
		int idx = ListView_InsertItem(hwnd(), &lvi);
		if (idx == -1) {
			return idx;
		}
		++mLastFreeId;

		// Insert the subitems (columns)
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = 1;

		size_t length = inName.length() + 1;
		lvi.pszText = new TCHAR[length];
		_tcscpy_s(lvi.pszText, length, inName.c_str());

		ListView_SetItem(hwnd(), &lvi);
		return mLastFreeId - 1;
	}

	bool ExportObjList::isChecked(int idx) {
		assert(hwnd());
		if (idx == -1) {
			return false;
		}
		return ListView_GetCheckState(hwnd(), idx) != 0;
	}

	void ExportObjList::setup() {
		assert(hwnd());
		ListView_SetExtendedListViewStyle(hwnd(), LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

		LVCOLUMN lvc = {0};
		ListView_InsertColumn(hwnd(), 0, &lvc);
		lvc.mask = LVCF_TEXT;
		lvc.iSubItem++;
		lvc.pszText = _T("X-Objects");
		ListView_InsertColumn(hwnd(), 1, &lvc);

		// Set column widths
		ListView_SetColumnWidth(hwnd(), 0, LVSCW_AUTOSIZE_USEHEADER);
		ListView_SetColumnWidth(hwnd(), 1, LVSCW_AUTOSIZE_USEHEADER);
	}

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

}
