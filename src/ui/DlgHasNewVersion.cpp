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

#include "DlgHasNewVersion.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include <sstream>
#include "common/String.h"
#include "resource/resource.h"
#include "resource/ResHelper.h"

namespace ui {

/********************************************************************************************************/
/////////////////////////////////////////////* Static area *//////////////////////////////////////////////
/********************************************************************************************************/

INT_PTR CALLBACK DlgHasNewVersion::_BaseWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    DlgHasNewVersion * theDlg;
    if (message == WM_INITDIALOG) {
        theDlg = (DlgHasNewVersion*)lParam;
        theDlg->mDlgMain.setup(hWnd);
        DLSetWindowLongPtr(hWnd, lParam);
    }
    else {
        if ((theDlg = DLGetWindowLongPtr<DlgHasNewVersion *>(hWnd)) == NULL)
            return FALSE;
        if (message == WM_DESTROY)
            theDlg->mDlgMain.release();
    }

    //------------------------------------------------------

    switch (message) {
        case WM_INITDIALOG:
            theDlg->InitDlg(hWnd);
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_HAS_NEW_VERSION_CANCEL_BUTTON:
                    EndDialog(hWnd, 0);
                    break;
                case ID_HAS_NEW_VERSION_DOWNLOAD_BUTTON:
                    //std::string url = theDlg->mData->getPackageUrl();
                    //ShellExecute(NULL, _T("open"), sts::toString(url).c_str(), NULL, NULL, SW_SHOWNORMAL);
                    EndDialog(hWnd, 1);
                    break;
            }
            break;
        case WM_DESTROY:
            theDlg->DestroyDlg(hWnd);
            break;
        case WM_CLOSE:
            EndDialog(hWnd, 0);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/********************************************************************************************************/
///////////////////////////////////////* Constructors/Destructor *////////////////////////////////////////
/********************************************************************************************************/

DlgHasNewVersion::DlgHasNewVersion() {}

DlgHasNewVersion::~DlgHasNewVersion() {}

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

bool DlgHasNewVersion::show() {
    //mData = &inOutData;
    INT_PTR res = DialogBoxParam(ResHelper::hInstance,
                                 MAKEINTRESOURCE(IDD_HAS_NEW_VERSION_DIALOG),
                                 GetCOREInterface()->GetMAXHWnd(),
                                 _BaseWindowProc, (LPARAM)this);
    return (res != 0);
}

void DlgHasNewVersion::InitDlg(HWND hWnd) {
    CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
    mHWnd = hWnd;

    cLabel.setup(hWnd, IDC_HAS_NEW_VERSION_LABEL);
    cDownloadButton.setup(hWnd, ID_HAS_NEW_VERSION_DOWNLOAD_BUTTON);

    //mDlgMain.setText(sts::toString(productInfo.getProgramName()));

    _toWindow();
    mDlgMain.show();
}

void DlgHasNewVersion::DestroyDlg(HWND /*hWnd*/) {
    _toVariables();

    cLabel.release();
}

void DlgHasNewVersion::_toWindow() {
    std::stringstream labelText;
    labelText << "New Version of Plugin available on server! ( ";
    //labelText << mData->getUpdateVersion();
    labelText << " )";
    cLabel.setText(sts::toString(labelText.str()));
    cDownloadButton.setText(_T("Download"));
}

void DlgHasNewVersion::_toVariables() {
    //mData->resetCheckUpdateProcess();
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

}
