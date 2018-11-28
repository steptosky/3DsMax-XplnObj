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

#include "UpdatedObjects.h"
#include "resource/resource.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool UpdatedObjects::show(const std::vector<INode *> * nodes, HWND parent) {
        mData = nodes;
        //-------------------------------
        cBtnOk.setupChild(mMainWin, IDC_BTN_OK);
        mEditInfo.setupChild(mMainWin, EDT_INFO);
        mLstObjects.setupChild(mMainWin, EDT_OBJECTS);
        //-------------------------------
        cBtnOk.onClick = [&](auto) { mMainWin.destroy(1); };
        //-------------------------------
        mMainWin.onInit = [&](auto win) {
            win->centerByParent();
            fillData();
        };
        //-------------------------------
        return mMainWin.create(parent, IDD_UPDATED_OBJECTS) != 0;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void UpdatedObjects::fillData() {
        if (mData->empty()) {
            mEditInfo.setText(_T("There are no affected objects."));
        }
        else {
            std::string strCount;
            if (mData->size() == 1) {
                strCount = sts::MbStrUtils::join("There is ", mData->size(), " affected object.");
            }
            else {
                strCount = sts::MbStrUtils::join("There are ", mData->size(), " affected objects.");
            }
            mEditInfo.setXObjText(sts::MbStrUtils::joinStr("The following list contains names of the affected objects.\r\n",
                                                           "You can copy/paste it somewhere and use it later to check the scene manually.\r\n\r\n",
                                                           strCount));
            std::stringstream list;
            for (const auto n : *mData) {
                list << sts::toMbString(n->GetName()) << "\r\n";
            }
            mLstObjects.setXObjText(list.str());
        }
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
