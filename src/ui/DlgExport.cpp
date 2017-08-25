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

#include "DlgExport.h "
#pragma warning(push, 0)
#include <max.h>
#include <3dsmaxport.h>
#pragma warning(pop)

#include "common/String.h"
#include "resource/resource.h"
#include "Info.h"
#include "common/Logger.h"
#include "objects/main/MainObjParamsWrapper.h"
#include "converters/ConverterUtils.h"
#include "gup/ObjCommon.h"
#include <windows.h>
#include <commctrl.h>
#include "models/MdLinks.h"
#include "Factory.h"
#include "resource/ResHelper.h"

namespace ui {

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
				gExportDlg->mEdtLog.setText(gExportDlg->mLogText);
			}
		}
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Static area *///////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR CALLBACK DlgExport::callBack(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		DlgExport * theDlg;
		if (message == WM_INITDIALOG) {
			theDlg = reinterpret_cast<DlgExport*>(lParam);
			theDlg->mDlgMain.setup(hWnd);
			DLSetWindowLongPtr(hWnd, lParam);
		}
		else {
			if ((theDlg = DLGetWindowLongPtr<DlgExport *>(hWnd)) == nullptr) {
				return FALSE;
			}
			if (message == WM_DESTROY) {
				theDlg->mDlgMain.release();
			}
		}

		//------------------------------------------------------

		switch (message) {
			case WM_INITDIALOG: {
				theDlg->InitDlg(hWnd);
				break;
			}
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case BTN_OK: {
						if (!theDlg->mFinished) {
							theDlg->startExport();
						}
						else {
							EndDialog(hWnd, 1);
						}
						break;
					}
					case BTN_SAVE_LOG: {
						theDlg->saveLogRequest();
						break;
					}
					case BTN_SELECT_ALL: {
						theDlg->mLstObjects.checkAll(true);
						break;
					}
					case BTN_UNSELECT_ALL: {
						theDlg->mLstObjects.checkAll(false);
						break;
					}
					case BTN_CANCEL: {
						EndDialog(hWnd, 0);
						break;
					}
					case BTN_DONATE: {
						MdLinks::openDonate();
						break;
					}
					case BTN_CHECK_FOR_UPDATE: {
						MdLinks::openPluginBinary();
						break;
					}
					case BTN_ABOUT: {
						Factory::showAboutWindow();
						break;
					}
					default: break;
				}
				break;
			}
			case WM_NOTIFY: {
				switch (LOWORD(wParam)) {
					case LST_OBJECTS: {
						LPNMHDR some_item = reinterpret_cast<LPNMHDR>(lParam);
						switch (some_item->code) {
							case LVN_ITEMCHANGED: {
								// Note: There is the loop call when the state is changed from the code while main nodes searching.
								LPNMLISTVIEW pnmv = reinterpret_cast<LPNMLISTVIEW>(lParam);
								if (pnmv->uChanged == LVIF_STATE) {
									theDlg->slotSelObjChanged(pnmv->iItem);
								}
								break;
							}
							case NM_CUSTOMDRAW: {
								return theDlg->mLstObjects.draw(lParam);
							}
							default: break;
						}
						break;
					}
					default: break;
				}
				break;
			}
			case WM_DESTROY: {
				theDlg->DestroyDlg(hWnd);
				break;
			}
			case WM_CLOSE: {
				EndDialog(hWnd, 0);
				break;
			}
			default: break;
		}
		return FALSE;
	}

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	DlgExport::DlgExport() {}

	DlgExport::~DlgExport() {}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	bool DlgExport::show(const TCHAR * inFileName, Interface * inIp, bool suppressPrompts, bool selectedOnly) {
		gExportDlg = this;
		mIp = inIp;
		mExpFileName = inFileName;
		mSuppressPrompts = suppressPrompts;
		mSelectedOnly = selectedOnly;
		mFinished = false;
		mErrorCount = 0;
		mWarningCount = 0;

		INT_PTR res = DialogBoxParam(ResHelper::hInstance, MAKEINTRESOURCE(DLG_EXPORT), GetCOREInterface()->GetMAXHWnd(),
									callBack, reinterpret_cast<LPARAM>(this));
		gExportDlg = nullptr;
		return res != 0;
	}

	void DlgExport::InitDlg(HWND hWnd) {
		CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
		SetWindowTextA(hWnd, "X-Plane Obj Export");
		mHWnd = hWnd;

		mLblVersion.setup(hWnd, LBL_VERSION);
		mBtnCheckUpdate.setup(hWnd, BTN_CHECK_FOR_UPDATE);
		mBtnSaveLog.setup(hWnd, BTN_SAVE_LOG);
		mBtnOk.setup(hWnd, BTN_OK);
		mBtnCancel.setup(hWnd, BTN_CANCEL);
		mBtnAbout.setup(hWnd, BTN_ABOUT);
		mBtnSelAll.setup(hWnd, BTN_SELECT_ALL);
		mBtnUnSelAll.setup(hWnd, BTN_UNSELECT_ALL);
		mBtnDonate.setup(hWnd, BTN_DONATE);
		mEdtLog.setup(hWnd, USER_OUPUT);
		mChkAutoExport.setup(hWnd, CHK_AUTOEXPORT);
		mLstObjects.setup(hWnd, LST_OBJECTS);
		mDlgMain.show();

		mBtnSaveLog.disable();

		sts::Str str(_T("Version: "));
		str.append(sts::toString(Logger::versionShortString()));
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

	void DlgExport::DestroyDlg(HWND /*hWnd*/) {
		saveConfigData();
		mLblVersion.release();
		mBtnCheckUpdate.release();
		mBtnSaveLog.release();
		mBtnOk.release();
		mBtnCancel.release();
		mBtnAbout.release();
		mBtnSelAll.release();
		mBtnUnSelAll.release();
		mBtnDonate.release();
		mChkAutoExport.release();
		mLstObjects.release();
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
			mChkAutoExport.setState(conf.value(TOTEXT(mChkAutoExport), false));
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
		MSTR fileName;
		MSTR initialDir(_T("C:\\Users\\%USERNAME%\\Documents"));
		FilterList extensionList;
		extensionList.Append(_T("Log files(*.log)"));
		extensionList.Append(_T("*.log"));
		bool res = ip->DoMaxSaveAsDialog(GetCOREInterface()->GetMAXHWnd(), _T("Log saving"), fileName, initialDir, extensionList);
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
		size_t selNodeCoun = selectedNodeCount();
		if (selectedNodeCount() > 1) {
			produceDerivedFiles = true;
		}
		CLMessage << "Found " << mMainNodesCollection.size() << " main object. Selected " << selNodeCoun;

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
			ConverterUtils::toXTMatrix(ConverterUtils::TOOGL_MTX, xMain.pMatrix);
			MainObjParamsWrapper mwrapper(currMainNode.first, GetCOREInterface()->GetTime(), FOREVER);
			result = mConverterer.toXpln(&mwrapper, xMain) ? TRUE : FALSE;
			if (result == FALSE) {
				CLError << "Export object: \"" << sts::toMbString(nodeName) << "\" is FAILED";
				break;
			}

			std::string signature = sts::MbStrUtils::joinStr(XIO_ORGANIZATION_NAME, " ", XIO_PROJECT_NAME, ": ",
															XIO_VERSION_STRING, "-", XIO_RELEASE_TYPE, "+[", XIO_COMPILE_DATE, "]");
			xMain.pExportOptions.setSignature(signature);

			if (!xMain.exportToFile(exportFilePath)) {
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
				if (upd.version > sts::SemVersion(XIO_VERSION_MAJOR, XIO_VERSION_MINOR, XIO_VERSION_PATCH)) {
					mBtnCheckUpdate.setText("Get update");
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
		for (auto currMainNode : mMainNodesCollection) {
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
		int count = nodeRoot->NumberOfChildren();
		for (int i = 0; i < count; ++i) {
			collectMainNodes(nodeRoot->GetChildNode(i), mMainNodesCollection);
		}
	}

	void DlgExport::collectMainNodes(INode * inRootNode, NodeCollection & outMains) {
		assert(mLstObjects);
		if (MainObjParamsWrapper::isMainObj(inRootNode)) {
			MainObjParamsWrapper wrapper(inRootNode, mTime, FOREVER);
			int idx = mLstObjects.addItem(inRootNode->GetName());
			outMains.emplace_back(NodeCollectionStruct(inRootNode, idx));
			mLstObjects.checkItem(idx, wrapper.isExportEnable());
		}
		int count = inRootNode->NumberOfChildren();
		for (int i = 0; i < count; ++i) {
			collectMainNodes(inRootNode->GetChildNode(i), outMains);
		}
	}

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/
}
