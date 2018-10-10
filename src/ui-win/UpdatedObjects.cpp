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

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include "UpdatedObjects.h"
#include "Resource/resource.h"
#include "resource/ResHelper.h"
#include "common/String.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK UpdatedObjects::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        UpdatedObjects * theDlg;
        if (msg == WM_INITDIALOG) {
            theDlg = reinterpret_cast<UpdatedObjects*>(lParam);
            DLSetWindowLongPtr(hWnd, lParam);
            CenterWindow(hWnd, theDlg->mParent);
            theDlg->mMainWin.setup(hWnd);
            theDlg->initWindow(hWnd);
        }
        else if (msg == WM_DESTROY) {
            theDlg = DLGetWindowLongPtr<UpdatedObjects*>(hWnd);
            theDlg->destroyWindow(hWnd);
            theDlg->mMainWin.release();
        }
        else {
            theDlg = DLGetWindowLongPtr<UpdatedObjects *>(hWnd);
            if (!theDlg) {
                return FALSE;
            }
        }

        //--------------------------------------

        switch (msg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case IDC_BTN_OK: {
                        EndDialog(hWnd, 1);
                        break;
                    }
                    default: break;
                }
                break;
            }
            case WM_CLOSE:
                EndDialog(hWnd, 0);
            default:
                return 0;
        }
        return 0;
    }

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    UpdatedObjects::UpdatedObjects()
        : mParent(nullptr) {
        mData = nullptr;
    }

    UpdatedObjects::~UpdatedObjects() {
        destroy();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool UpdatedObjects::show(const std::vector<INode *> * nodes, HWND parent) {
        DbgAssert(nodes);
        DbgAssert(parent);
        mData = nodes;
        mParent = parent;
        return DialogBoxParam(ResHelper::hInstance,
                              MAKEINTRESOURCE(IDD_UPDATED_OBJECTS),
                              parent, panelProc,
                              reinterpret_cast<LPARAM>(this)) ? true : false;
    }

    void UpdatedObjects::destroy() {
        if (mMainWin) {
            EndDialog(mMainWin.hwnd(), 0);
            mMainWin.release();
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void UpdatedObjects::initWindow(HWND hWnd) {
        cBtnOk.setup(hWnd, IDC_BTN_OK);
        mEditInfo.setup(hWnd, EDT_INFO);
        mLstObjects.setup(hWnd, EDT_OBJECTS);
        showData();
    }

    void UpdatedObjects::destroyWindow(HWND /*hWnd*/) {
        cBtnOk.release();
        mEditInfo.release();
        mLstObjects.release();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void UpdatedObjects::showData() {
        if (mData->empty()) {
            mEditInfo.setText(_T("There are no affected objects."));
        }
        else {
            sts::Str strCount;
            if (mData->size() == 1) {
                strCount = sts::StrUtils::join(_T("There is "), mData->size(), _T(" affected object."));
            }
            else {
                strCount = sts::StrUtils::join(_T("There are "), mData->size(), _T(" affected objects."));
            }
            mEditInfo.setText(sts::StrUtils::joinStr(_T("The following list contains names of the affected objects.\r\n"),
                                                     _T("You can copy/paste it somewhere and use it later to check the scene manually.\r\n\r\n"),
                                                     strCount));
            std::stringstream list;
            for (const auto n : *mData) {
                list << sts::toMbString(n->GetName()) << "\r\n";
            }
            mLstObjects.setText(list.str());
        }
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
