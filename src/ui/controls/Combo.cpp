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

#include "Combo.h"
#include <WindowsX.h>

namespace win {

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

Combo::Combo() { }

Combo::~Combo() { }

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void Combo::addItem(const String & inItem, bool inSelected) {
    int pos = ComboBox_AddString(hwnd(), inItem.c_str());
    if (pos == CB_ERR)
        return;
    if (inSelected)
        ComboBox_SetCurSel(hwnd(), pos);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void Combo::removeItem(const String & inItem) {
    int pos = ComboBox_FindStringExact(hwnd(), 0, inItem.c_str());
    if (pos == CB_ERR)
        return;
    ComboBox_DeleteString(hwnd(), pos);
    int count = this->count();
    if (count == CB_ERR)
        return;
    setCurrSelected(count - 1);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void Combo::setCurrSelected(int inIndex) {
    int numElements = ComboBox_GetCount(hwnd());
    if (inIndex > (numElements - 1))
        inIndex = numElements;
    ComboBox_SetCurSel(hwnd(), inIndex);
}

bool Combo::setCurrSelected(const String & text) {
    return (ComboBox_SelectString(hwnd(), -1, text.c_str()) != CB_ERR);
}

int Combo::currSelected() const {
    return ComboBox_GetCurSel(hwnd());
}

Base::String Combo::currSelectedText() const {
    int count = this->count();
    if (!count) {
        return _T("");
    }
    int strLenght = ComboBox_GetLBTextLen(hwnd(), currSelected());
    if (strLenght == -1) {
        return _T("");
    }
    TCHAR * temp = new TCHAR[strLenght + 1];
    ComboBox_GetLBText(hwnd(), currSelected(), temp);
    String ret(temp);
    delete[] temp;
    return ret;
}

int Combo::count() const {
    return ComboBox_GetCount(hwnd());
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
};
