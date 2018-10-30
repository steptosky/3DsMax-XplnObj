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

#include "DlgImport.h"

#pragma warning(push, 0)
#include <IPathConfigMgr.h>
#pragma warning(pop)

#include <xpln/obj/ObjMain.h>
#include "resource/resource.h"
#include "Info.h"
#include "common/Logger.h"
#include "converters/ConverterUtils.h"
#include "resource/ResHelper.h"
#include "common/Config.h"
#include "models/MdLinks.h"
#include "Factory.h"

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
                gImportDlg->mEdtLog.setXObjText(gImportDlg->mLogText);
            }
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool DlgImport::show(const TCHAR * inFileName, Interface * inIp, const bool suppressPrompts) {
        gImportDlg = this;
        mIp = inIp;
        mFileName = inFileName;
        mSuppressPrompts = suppressPrompts;
        mFinished = false;
        mErrorCount = 0;
        mWarningCount = 0;
        //------------------------------
        mLblVersion.setupChild(mDlgMain, LBL_VERSION);
        mBtnCheckUpdate.setupChild(mDlgMain, BTN_CHECK_FOR_UPDATE);
        mBtnOk.setupChild(mDlgMain, BTN_OK);
        mBtnCancel.setupChild(mDlgMain, BTN_CANCEL);
        mBtnAbout.setupChild(mDlgMain, BTN_ABOUT);
        mBtnDonate.setupChild(mDlgMain, BTN_DONATE);
        mEdtLog.setupChild(mDlgMain, USER_OUPUT);
        //------------------------------
        mBtnOk.onClick = [&](auto) { mFinished ? mDlgMain.destroy(1) : startImport(); };
        mBtnCancel.onClick = [&](auto) { mDlgMain.destroy(0); };
        mBtnCheckUpdate.onClick = [](auto) { MdLinks::openPluginBinary(); };
        mBtnAbout.onClick = [](auto) { Factory::showAboutWindow(); };
        mBtnDonate.onClick = [](auto) { MdLinks::openDonate(); };
        //------------------------------
        mDlgMain.onInit = [&](auto win) {
            win->centerByParent();
            win->setText(_T("X-Plane Obj Import"));
            mLblVersion.setText(xobj::toMStr(sts::StrUtils::joinStr(_T(XIO_VERSION_STRING), _T("-"), _T(XIO_RELEASE_TYPE))));
        };
        //------------------------------
        const auto res = mDlgMain.create(GetCOREInterface()->GetMAXHWnd(), DLG_EXPORT);
        gImportDlg = nullptr;
        return res != 0;
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
        xobj::ImportContext context;
        context.setObjFile(sts::toWString(mainFileName));

        // MaxSDK::Util::Path::Exists have been added sine 3Ds Max 2013
        const auto pathConfMgr = IPathConfigMgr::GetPathConfigMgr();
        auto config = Config::instance();
        const auto projectDatarefs = config->projectDatarefsFile();
        const auto projectCommands = config->projectCommandsFile();

        if (pathConfMgr->DoesFileExist(projectDatarefs)) {
            context.setDatarefsFile(xobj::fromMPath(projectDatarefs));
        }
        if (pathConfMgr->DoesFileExist(projectCommands)) {
            context.setCommandsFile(xobj::fromMPath(projectCommands));
        }

        ConverterUtils::toXTMatrix(ConverterUtils::FROMOGL_MTX, xMain.pMatrix);
        if (!xMain.importObj(context)) {
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
