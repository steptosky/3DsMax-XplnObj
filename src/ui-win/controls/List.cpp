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

#include "List.h"
#include <WindowsX.h>

namespace ui {
namespace ctrl {

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    List::List() { }

    List::~List() { }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void List::addItem(const Base::String & inItem, bool inSelected) {
        int pos = ListBox_AddString(hwnd(), inItem.c_str());
        if (inSelected)
            ListBox_SetCurSel(hwnd(), pos);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    //	int List::removeItem(const std::string & inItem) {
    //		int currIndex = currSelected();
    //		if (currIndex == -1)
    //			return false;
    //		return ListBox_DeleteString(hwnd(), currIndex);
    //	}

    void List::removeItem(int inIndex) {
        ListBox_DeleteString(hwnd(), inIndex);
    }

    int List::count() const {
        return ListBox_GetCount(hwnd());
    }

    Base::String List::currSelectedText() const {
        return itemText(currSelected());
    }

    Base::String List::itemText(int index) const {
        Base::String result;
        if (index >= 0) {
            size_t length = 0;
            TCHAR * text;
            length = ListBox_GetTextLen(hwnd(), index);
            text = new TCHAR[length + 1];
            ListBox_GetText(hwnd(), index, text);
            result = text;
            delete[] text;
        }
        return result;
    }

    int List::currSelected() const {
        // Get selected index.
        int index = ListBox_GetCurSel(hwnd());
        if (index >= 0) {
            return index;
        }
        return -1;
    }

    bool List::removeCurr() {
        // Get the selected item.
        int currIndex = currSelected();
        if (currIndex == -1)
            return false;
        // Delete the selected item.
        int cStringsRemaining = ListBox_DeleteString(hwnd(), currIndex);

        // If this is not the last item, set the selection to 
        // the item immediately following the one just deleted.
        // Otherwise, set the selection to the last item.
        if (cStringsRemaining > currIndex)
            ListBox_SetCurSel(hwnd(), currIndex);
        else
            ListBox_SetCurSel(hwnd(), cStringsRemaining - 1);

        return true;
    }

    void List::clear() {
        SendMessage(hwnd(), LB_RESETCONTENT, NULL, NULL);
    }

    void List::setCurrSelected(int inIndex) {
        ListBox_SetCurSel(hwnd(), inIndex);
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
