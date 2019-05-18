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

#include "DlgExport.h"

#pragma warning(push, 0)
#include <IPathConfigMgr.h>
#pragma warning(pop)

#include <xpln/obj/ObjMain.h>

#include "resource/resource.h"
#include "Info.h"
#include "common/Logger.h"
#include "objects/main/MainObjParamsWrapper.h"
#include "converters/ConverterUtils.h"
#include "gup/ObjCommon.h"
#include "models/MdLinks.h"
#include "Factory.h"
#include "resource/ResHelper.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    DlgExport * DlgExport::gExportDlg = nullptr;

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    inline sts::Str changeFileName(const TCHAR * inCurrPath, const TCHAR * inFileName) {
        assert(inCurrPath);
        assert(inFileName);
        sts::Str outString(inCurrPath);

        size_t pos = outString.find_last_of(_T("\\/"));
        if (pos != sts::Str::npos) {
            outString = outString.substr(0, pos);
        }

        outString.append(_T("\\"));
        outString.append(inFileName);
        outString.append(_T(".obj"));
        return outString;
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    void DlgExport::logCallback(sts::BaseLogger::eType type, const char * msg) {
        if (gExportDlg) {
            if (msg) {
                if (type == sts::BaseLogger::Msg) {
                    gExportDlg->mLogText.append("- - - -   ").append(msg).append("\r\n");
                }
                else {
                    if (type == sts::BaseLogger::Warning) {
                        ++gExportDlg->mWarningCount;
                    }
                    else {
                        ++gExportDlg->mErrorCount;
                    }
                    gExportDlg->mLogText.append(sts::BaseLogger::typeAsString(type))
                              .append(": ").append(msg).append("\r\n");
                }
                gExportDlg->mEdtLog.setXObjText(gExportDlg->mLogText);
            }
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    bool DlgExport::show(const TCHAR * inFileName, Interface * inIp, const bool suppressPrompts, const bool selectedOnly) {
        gExportDlg = this;
        mIp = inIp;
        mExpFileName = inFileName;
        mSuppressPrompts = suppressPrompts;
        mSelectedOnly = selectedOnly;
        mFinished = false;
        mErrorCount = 0;
        mWarningCount = 0;
        //------------------------------
        mLblVersion.setupChild(mDlgMain, LBL_VERSION);
        mBtnCheckUpdate.setupChild(mDlgMain, BTN_CHECK_FOR_UPDATE);
        mBtnSaveLog.setupChild(mDlgMain, BTN_SAVE_LOG);
        mBtnOk.setupChild(mDlgMain, BTN_OK);
        mBtnCancel.setupChild(mDlgMain, BTN_CANCEL);
        mBtnAbout.setupChild(mDlgMain, BTN_ABOUT);
        mBtnSelAll.setupChild(mDlgMain, BTN_SELECT_ALL);
        mBtnUnSelAll.setupChild(mDlgMain, BTN_UNSELECT_ALL);
        mBtnDonate.setupChild(mDlgMain, BTN_DONATE);
        mEdtLog.setupChild(mDlgMain, USER_OUPUT);
        mChkAutoExport.setupChild(mDlgMain, CHK_AUTOEXPORT);
        mLstObjects.setupChild(mDlgMain, LST_OBJECTS);
        //------------------------------
        mBtnOk.onClick = [&](auto) { mFinished ? mDlgMain.destroy(1) : startExport(); };
        mBtnSaveLog.onClick = [&](auto) { saveLogRequest(); };
        mBtnSelAll.onClick = [&](auto) { mLstObjects.checkAll(true); };
        mBtnUnSelAll.onClick = [&](auto) { mLstObjects.checkAll(false); };
        mBtnCancel.onClick = [&](auto) { mDlgMain.destroy(0); };
        mBtnCheckUpdate.onClick = [](auto) { MdLinks::openPluginBinary(); };
        mBtnAbout.onClick = [](auto) { Factory::showAboutWindow(); };
        mBtnDonate.onClick = [](auto) { MdLinks::openDonate(); };

        mLstObjects.onItemChanged = [&](auto, auto index) { slotSelObjChanged(index); };
        //------------------------------
        mDlgMain.onInit = [&](auto) { onInit(); };
        mDlgMain.onDestroy = [&](auto) { saveConfigData(); };
        //------------------------------
        const auto res = mDlgMain.create(GetCOREInterface()->GetMAXHWnd(), DLG_EXPORT);
        gExportDlg = nullptr;
        return res != 0;
    }

    void DlgExport::onInit() {
        mDlgMain.centerByParent();
        mDlgMain.setText(_T("X-Plane Obj Export"));
        mBtnSaveLog.disable();

        MStr str(_T("Version: "));
        str.Append(xobj::toMStr(Logger::versionShortString()));
        mLblVersion.setText(str);

        mBtnSaveLog.setToolTip(_T("Saves log which can help the developers to determine the problems. Use it for bug reports."));
        mBtnDonate.setToolTip(_T("If you like this plugin please, support the development."));
        mBtnCheckUpdate.setToolTip(_T("Visit the download page for seeing if a new version is available."));
        mChkAutoExport.setToolTip(_T("If it is enabled then the export will be auto-started without pressing the 'Export' button. Value is saved with the scene."));

        collectMainNodes();
        loadConfigData();

        if (mChkAutoExport.isChecked()) {
            startExport();
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgExport::saveConfigData() {
        ObjCommon * cmn = ObjCommon::instance();
        if (cmn) {
            Settings & conf = cmn->pSettings;
            conf.beginGroup(TOTEXT(DlgExport));
            conf.setValue(TOTEXT(mChkAutoExport), mChkAutoExport.isChecked());
            conf.endGroup();
        }
    }

    void DlgExport::loadConfigData() {
        ObjCommon * cmn = ObjCommon::instance();
        if (cmn) {
            Settings & conf = cmn->pSettings;
            conf.beginGroup(TOTEXT(DlgExport));
            mChkAutoExport.setChecked(conf.value(TOTEXT(mChkAutoExport), false));
            conf.endGroup();
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgExport::slotSelObjChanged(int idx) {
        for (auto & i : mMainNodesCollection) {
            if (i.second == idx) {
                MainObjParamsWrapper wrapper(i.first, mTime, FOREVER);
                wrapper.setExportEnable(mLstObjects.isChecked(idx));
                return;
            }
        }
    }

    void DlgExport::saveLogRequest() {
        Interface8 * ip = GetCOREInterface8();
        MSTR fileName(_T("3DsMax-XplnObj-Log.txt"));
        MSTR initialDir(_T("C:\\Users\\%USERNAME%\\Documents"));
        FilterList extensionList;
        extensionList.Append(_T("Log files(*.txt)"));
        extensionList.Append(_T("*.txt"));
        const bool res = ip->DoMaxSaveAsDialog(GetCOREInterface()->GetMAXHWnd(), _T("Log saving"), fileName, initialDir, extensionList);
        if (res) {
            Logger::instance()->saveLog(fileName);
        }
    }

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    void DlgExport::exportFinished() {
        mBtnSaveLog.enable();
    }

    int DlgExport::startExport() {
        Logger::registerUserConsoleCallback(&logCallback);
        int result = TRUE;

        mTime = GetCOREInterface()->GetTime();

        LMessage << "====================== Export obj ======================";
        CLMessage << "File to process: " << sts::toMbString(mExpFileName);
        LMessage << "SuppressPrompts: " << (mSuppressPrompts ? "true" : "false");
        CLMessage << "SelectedOnly: " << (mSelectedOnly ? "true" : "false");
        CLMessage << "Current time: " << mTime;

        if (mSelectedOnly) {
            MessageBoxA(GetActiveWindow(), "'Selected Only' option isn't supporting now.", "Warning", MB_OK | MB_ICONWARNING);
            CLError << "'Selected Only' option isn't supporting now.";
            exportFinished();
            return FALSE;
        }

        if (mSuppressPrompts) {
            MessageBoxA(GetActiveWindow(), "'Suppress Prompts' option isn't supporting now.", "Warning", MB_OK | MB_ICONWARNING);
            CLError << "'Suppress Prompts' option isn't supporting now.";
            exportFinished();
            return FALSE;
        }

        mDlgMain.show(true);

        //----------------------------------

        if (mMainNodesCollection.empty()) {
            MessageBoxA(GetActiveWindow(), "There are no X-Obj found.", "Warning", MB_OK | MB_ICONWARNING);
            CLError << "There are no X-Obj object found.";
        }

        //----------------------------------

        std::string mainFileName(sts::toMbString(mExpFileName));
        bool produceDerivedFiles = false;
        const auto selNodeCount = selectedNodeCount();
        if (selectedNodeCount() > 1) {
            produceDerivedFiles = true;
        }
        CLMessage << "Found " << mMainNodesCollection.size() << " main object. Selected " << selNodeCount;

        for (auto currMainNode : mMainNodesCollection) {
            if (!currMainNode.first) {
                continue;
            }
            if (!mLstObjects.isChecked(currMainNode.second)) {
                CLMessage << "Object " << sts::toMbString(currMainNode.first->GetName()) << " is skipped.";
                continue;
            }

            std::string exportFilePath(mainFileName);
            const TCHAR * nodeName = currMainNode.first->GetName();

            if (produceDerivedFiles) {
                if (_tcslen(nodeName) < 1) {
                    CLError << "A main object has incorrect name. The name is used for creating the obj file name.";
                    continue;
                }
                exportFilePath = sts::toMbString(changeFileName(mExpFileName, nodeName));
                CLMessage << "Create derived file path: " << sts::toMbString(mExpFileName) << " -> " << exportFilePath;
            }

            xobj::ObjMain xMain;
            ConverterUtils::toXTMatrix(ConverterUtils::TOOGL_MTX, xMain.mMatrix);
            MainObjParamsWrapper mwrapper(currMainNode.first, GetCOREInterface()->GetTime(), FOREVER);
            result = mConverterer.toXpln(&mwrapper, xMain) ? TRUE : FALSE;
            if (result == FALSE) {
                CLError << "Export object: \"" << sts::toMbString(nodeName) << "\" is FAILED";
                break;
            }

            sts::semver::SemVersion pluginVersion(XIO_VERSION_MAJOR, XIO_VERSION_MINOR, XIO_VERSION_PATCH,
                                                  XIO_RELEASE_TYPE, sts::MbStrUtils::joinStr("[", XIO_COMPILE_DATE, "]"));
            const std::string signature = sts::MbStrUtils::joinStr(XIO_ORGANIZATION_NAME, " ", XIO_PROJECT_NAME, ": ",
                                                                   pluginVersion.toString(true, true));

            xobj::ExportContext context;
            context.setObjFile(sts::toWString(exportFilePath));
            context.setSignature(signature);

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

            if (!xMain.exportObj(context)) {
                CLError << "Export object: \"" << sts::toMbString(currMainNode.first->GetName()) << "\" is FAILED";
            }
            else {
                CLMessage << "Export object: \"" << sts::toMbString(currMainNode.first->GetName()) << "\" is OK";
            }
        }

        CLMessage << "Export completed with:\r\n\t" << mErrorCount << " errors \r\n\t" << mWarningCount << " warnings";
        printUpdateAvailability();
        Logger::unregisterUserConsoleCallback(&logCallback);
        mFinished = true;
        mBtnOk.setText(_T("Close"));
        mBtnCancel.setText(_T("Close"));
        exportFinished();
        return result;
    }

    void DlgExport::printUpdateAvailability() {
        UpdateChecker::Update upd = ObjCommon::instance()->updateInfo();
        if (upd.valid) {
            if (!upd.error.empty()) {
                CLError << "Error during check update the plugin. The log contains more information. Please, inform the developers.";
            }
            else {
                if (upd.version > sts::semver::SemVersion(XIO_VERSION_MAJOR, XIO_VERSION_MINOR, XIO_VERSION_PATCH)) {
                    mBtnCheckUpdate.setText(_T("Get update"));
                    CLWarning << "New version '" << upd.version.toString()
                            << "' is available. Please, press the '"
                            << sts::toMbString(mBtnCheckUpdate.text())
                            << "' button to get the new version." << "\r\n"
                            << "\tSee the 'changelog.txt' file there to get the information about the changes.";
                }
            }
        }
    }

    size_t DlgExport::selectedNodeCount() {
        size_t count = 0;
        for (const auto currMainNode : mMainNodesCollection) {
            if (!currMainNode.first) {
                continue;
            }
            if (mLstObjects.isChecked(currMainNode.second)) {
                ++count;
            }
        }
        return count;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void DlgExport::collectMainNodes() {
        INode * nodeRoot = mIp->GetRootNode();
        const auto count = nodeRoot->NumberOfChildren();
        for (int i = 0; i < count; ++i) {
            collectMainNodes(nodeRoot->GetChildNode(i), mMainNodesCollection);
        }
    }

    void DlgExport::collectMainNodes(INode * inRootNode, NodeCollection & outMains) {
        if (MainObjParamsWrapper::isMainObj(inRootNode)) {
            MainObjParamsWrapper wrapper(inRootNode, mTime, FOREVER);
            int idx = mLstObjects.addItem(inRootNode->GetName());
            outMains.emplace_back(NodeCollectionStruct(inRootNode, idx));
            mLstObjects.checkItem(idx, wrapper.isExportEnable());
        }
        const auto count = inRootNode->NumberOfChildren();
        for (int i = 0; i < count; ++i) {
            collectMainNodes(inRootNode->GetChildNode(i), outMains);
        }
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
