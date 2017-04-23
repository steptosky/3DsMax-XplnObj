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

#pragma once

#include "ui/controls/Base.h"
#include "Converters/Converterer.h"
#include "ui/controls/Edit.h"
#include "common/Logger.h"
#include "update/UpdateChecker.h"

namespace ui {

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

	class DlgExport {
	public:

		DlgExport();
		~DlgExport();

		bool show(const TCHAR * inFileName, Interface * inIp, bool suppressPrompts, bool selectedOnly);

	private:

		HWND mHWnd = nullptr;;
		void InitDlg(HWND hWnd);
		void DestroyDlg(HWND hWnd);

		win::Base mLblVersion;

		win::Base mBtnCheckUpdate;
		win::Base mBtnDonate;
		win::Base mBtnSaveLog;
		win::Base mBtnOk;
		win::Base mBtnCancel;
		win::Base mBtnAbout;;
		win::Edit mEdtLog;
		win::Base mDlgMain;

		//-------------------------------------------------------------------------

		uint32_t mErrorCount = 0;
		uint32_t mWarningCount = 0;

		//-------------------------------------------------------------------------

		static INT_PTR CALLBACK callBack(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static void logCallback(sts::BaseLogger::eType type, const char * msg);

		//-------------------------------------------------------------------------

		int startExport();
		static void collectMainNodes(INode * inRootNode, std::vector<INode*> & outMains);
		void printUpdateAvailability();

		Converterer mConverterer;
		TimeValue mTime = 0;
		std::string mLogText;

		Interface * mIp = nullptr;
		const TCHAR * mExpFileName = nullptr;
		bool mSuppressPrompts = false;
		bool mSelectedOnly = false;
		bool mFinished = false;

		static DlgExport * gExportDlg;

		//-------------------------------------------------------------------------

		void saveLogRequest();
		void exportFinished();

		//-------------------------------------------------------------------------

	};

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/
}
