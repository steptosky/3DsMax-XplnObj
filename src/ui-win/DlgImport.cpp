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

#include "DlgImport.h "
#pragma warning(push, 0)
#include <max.h>
#include <3dsmaxport.h>
#pragma warning(pop)

#include <xpln/obj/ObjMain.h>
#include "common/String.h"
#include "resource/resource.h"
#include "Info.h"
#include "common/Logger.h"
#include "DlgAbout.h"
#include "converters/ConverterMain.h"
#include "converters/ConverterUtils.h"
#include "resource/ResHelper.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    DlgImport * DlgImport::gImportDlg = nullptr;

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    void DlgImport::logCallback(sts::BaseLogger::eType type, const char * msg) {
        if (gImportDlg) {
            if (msg) {
                if (type == sts::BaseLogger::Msg) {
                    gImportDlg->mLogText.append("- - - -   ").append(msg).append("\r\n");
                }
                else {
                    if (type == sts::BaseLogger::Warning) {
                        ++gImportDlg->mWarningCount;
                    }
                    else {
                        ++gImportDlg->mErrorCount;
                    }
                    gImportDlg->mLogText.append(sts::BaseLogger::typeAsString(type))
                              .append(": ").append(msg).append("\r\n");
                }
                gImportDlg->mEdtLog.setText(gImportDlg->mLogText);
            }
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    INT_PTR CALLBACK DlgImport::callBack(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        DlgImport * theDlg;
        if (message == WM_INITDIALOG) {
            theDlg = reinterpret_cast<DlgImport*>(lParam);
            theDlg->mDlgMain.setup(hWnd);
            DLSetWindowLongPtr(hWnd, lParam);
        }
        else {
            if ((theDlg = DLGetWindowLongPtr<DlgImport *>(hWnd)) == nullptr) {
                return FALSE;
            }
            if (message == WM_DESTROY) {
                theDlg->mDlgMain.release();
            }
        }

        //------------------------------------------------------

        switch (message) {
            case WM_INITDIALOG:
                theDlg->InitDlg(hWnd);
                break;
            case WM_COMMAND:
                switch (LOWORD(wParam)) {
                    case BTN_OK:
                        if (!theDlg->mFinished) {
                            theDlg->startImport();
                        }
                        else {
                            EndDialog(hWnd, 1);
                        }
                        break;
                    case BTN_CANCEL:
                        EndDialog(hWnd, 0);
                        break;
                    case BTN_DONATE:
                        ShellExecute(nullptr, _T("open"), _T("http://steptosky.com/index.php/software/8-x-obj-exporter"), nullptr, nullptr, SW_SHOWNORMAL);
                        break;
                    case BTN_CHECK_FOR_UPDATE:
                        ShellExecute(nullptr, _T("open"), _T("http://steptosky.com/index.php/software/8-x-obj-exporter"), nullptr, nullptr, SW_SHOWNORMAL);
                        break;
                    case BTN_ABOUT:
                        DlgAbout::show();
                        break;
                    default: break;
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

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    DlgImport::DlgImport() {}

    DlgImport::~DlgImport() {}

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool DlgImport::show(const TCHAR * inFileName, Interface * inIp, bool suppressPrompts) {
        gImportDlg = this;
        mIp = inIp;
        mFileName = inFileName;
        mSuppressPrompts = suppressPrompts;
        mFinished = false;
        mErrorCount = 0;
        mWarningCount = 0;

        INT_PTR res = DialogBoxParam(ResHelper::hInstance, MAKEINTRESOURCE(DLG_EXPORT), GetCOREInterface()->GetMAXHWnd(),
                                     callBack, reinterpret_cast<LPARAM>(this));
        gImportDlg = nullptr;
        return res != 0;
    }

    void DlgImport::InitDlg(HWND hWnd) {
        CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
        SetWindowTextA(hWnd, "X-Plane Obj Import");
        mHWnd = hWnd;

        mLblVersion.setup(hWnd, LBL_VERSION);
        mBtnCheckUpdate.setup(hWnd, BTN_CHECK_FOR_UPDATE);
        mBtnOk.setup(hWnd, BTN_OK);
        mBtnCancel.setup(hWnd, BTN_CANCEL);
        mBtnAbout.setup(hWnd, BTN_ABOUT);
        mBtnDonate.setup(hWnd, BTN_DONATE);
        mEdtLog.setup(hWnd, USER_OUPUT);
        mDlgMain.show();
        mLblVersion.setText(sts::StrUtils::joinStr(_T(XIO_VERSION_STRING), _T("-"), _T(XIO_RELEASE_TYPE)));
    }

    void DlgImport::DestroyDlg(HWND /*hWnd*/) {
        mLblVersion.release();
        mBtnCheckUpdate.release();
        mBtnOk.release();
        mBtnCancel.release();
        mBtnAbout.release();
        mBtnDonate.release();
    }

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    int DlgImport::startImport() {
        Logger::registerUserConsoleCallback(&logCallback);
        int result = TRUE;

        mTime = GetCOREInterface()->GetTime();
        std::string mainFileName(sts::toMbString(mFileName));

        LMessage << "====================== Import obj ======================";
        CLMessage << "File to process: " << mainFileName;

        if (mSuppressPrompts) {
            MessageBoxA(GetActiveWindow(), "'Suppress Prompts' option isn't supporting now.", "Warning", MB_OK | MB_ICONWARNING);
            CLError << "'Suppress Prompts' option isn't supporting now.";
            return FALSE;
        }

        mDlgMain.show();

        //----------------------------------

        if (mainFileName.empty()) {
            CLError << "Filename is empty";
        }

        xobj::ObjMain xMain;
        ConverterUtils::toXTMatrix(ConverterUtils::FROMOGL_MTX, xMain.pMatrix);
        if (!xMain.importFromFile(mainFileName)) {
            finish(true);
            return false;
        }

        if (!ConverterToMax::toMax(xMain)) {
            finish(true);
            return false;
        }

        //----------------------------------

        finish(false);
        return result;
    }

    void DlgImport::finish(bool falied) {
        if (falied) {
            CLError << "Import is FAILED";
        }
        CLMessage << "Import completed with:\r\n\t" << mErrorCount << " errors \r\n\t" << mWarningCount << " warnings";
        Logger::unregisterUserConsoleCallback(&logCallback);
        mFinished = true;
        mBtnOk.setText(_T("Close"));
        mBtnCancel.setText(_T("Close"));
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
