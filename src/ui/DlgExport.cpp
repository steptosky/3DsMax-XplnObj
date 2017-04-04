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
#include "DlgAbout.h"
#include "objects/MainObjParamsWrapper.h"
#include "converters/ConverterUtils.h"

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
					case BTN_CANCEL: {
						EndDialog(hWnd, 0);
						break;
					}
					case BTN_DONATE: {
						ShellExecute(nullptr, _T("open"), _T("http://steptosky.com/index.php/software/8-x-obj-exporter"), nullptr, nullptr, SW_SHOWNORMAL);
						break;
					}
					case BTN_CHECK_FOR_UPDATE: {
						ShellExecute(nullptr, _T("open"), _T("https://github.com/steptosky/3DsMax-XplnObj/releases"), nullptr, nullptr, SW_SHOWNORMAL);
						break;
					}
					case BTN_ABOUT: {
						DlgAbout::show();
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

		INT_PTR res = DialogBoxParam(hInstance, MAKEINTRESOURCE(DLG_EXPORT), GetCOREInterface()->GetMAXHWnd(),
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
		mBtnDonate.setup(hWnd, BTN_DONATE);
		mEdtLog.setup(hWnd, USER_OUPUT);
		mDlgMain.show();

		mBtnSaveLog.disable();

		sts::Str str(_T("Version: "));
		str.append(_T(XIO_VERSION_STRING)).append(_T("-")).append(_T(XIO_RELEASE_TYPE));
		mLblVersion.setText(str);
	}

	void DlgExport::DestroyDlg(HWND /*hWnd*/) {
		mLblVersion.release();
		mBtnCheckUpdate.release();
		mBtnSaveLog.release();
		mBtnOk.release();
		mBtnCancel.release();
		mBtnAbout.release();
		mBtnDonate.release();
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void DlgExport::saveLogRequest() {
		Interface8 * ip = GetCOREInterface8();
		MSTR fileName;
		MSTR initialDir(_T("C:\\Users\\%USERNAME%\\Documents"));
		FilterList extensionList;
		extensionList.Append(_T("Log files(*.log)"));
		extensionList.Append(_T("*.log"));
		bool res = ip->DoMaxSaveAsDialog(GetCOREInterface()->GetMAXHWnd(), _T("Log saveing"), fileName, initialDir, extensionList);
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

		std::vector<INode*> mainNodes;
		INode * nodeRoot = mIp->GetRootNode();
		int count = nodeRoot->NumberOfChildren();
		for (int i = 0; i < count; ++i) {
			collectMainNodes(nodeRoot->GetChildNode(i), mainNodes);
		}

		//----------------------------------

		if (mainNodes.empty()) {
			MessageBoxA(GetActiveWindow(), "There are no X-Obj found.", "Warning", MB_OK | MB_ICONWARNING);
			CLError << "There are no X-Obj object found.";
		}

		//----------------------------------

		std::string mainFileName(sts::toMbString(mExpFileName));
		bool produceDerivedFiles = false;
		if (mainNodes.size() > 1) {
			CLMessage << "Found " << mainNodes.size() << " main objects";
			produceDerivedFiles = true;
		}
		else {
			CLMessage << "Found " << mainNodes.size() << " main object";
		}

		for (auto currMainNode : mainNodes) {
			if (!currMainNode) {
				continue;
			}

			std::string exportFilePath(mainFileName);
			const TCHAR * nodeName = currMainNode->GetName();

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
			MainObjParamsWrapper mwrapper(currMainNode, GetCOREInterface()->GetTime(), FOREVER);
			result = mConverterer.toXpln(&mwrapper, xMain) ? TRUE : FALSE;
			if (result == FALSE) {
				CLError << "Export object: \"" << sts::toMbString(nodeName) << "\" is FAILED";
				break;
			}

			std::string signature(XIO_ORGANIZATION_NAME);
			signature.append(" ").append(XIO_PROJECT_NAME).append(":");
			signature.append(" ").append(XIO_VERSION_STRING).append("-").append(XIO_RELEASE_TYPE);
			signature.append("+[").append(XIO_COMPILE_DATE).append("]");
			xMain.pExportOptions.setSignature(signature);

			if (!xMain.exportToFile(exportFilePath)) {
				CLError << "Export object: \"" << sts::toMbString(currMainNode->GetName()) << "\" is FAILED";
			}
			else {
				CLMessage << "Export object: \"" << sts::toMbString(currMainNode->GetName()) << "\" is OK";
			}
		}

		CLMessage << "Export completed with:\r\n\t" << mErrorCount << " errors \r\n\t" << mWarningCount << " warnings";
		Logger::unregisterUserConsoleCallback(&logCallback);
		mFinished = true;
		mBtnOk.setText(_T("Close"));
		mBtnCancel.setText(_T("Close"));
		exportFinished();
		return result;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void DlgExport::collectMainNodes(INode * inRootNode, std::vector<INode*> & outMains) {
		if (MainObjParamsWrapper::isMainObj(inRootNode)) {
			outMains.push_back(inRootNode);
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
