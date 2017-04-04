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

#include "DlgAbout.h "

#pragma warning(push, 0)
#include <max.h>
#include <3dsmaxport.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "common/Logger.h"

namespace ui {

	/**************************************************************************************************/
	//////////////////////////////////////////* Static area *///////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR CALLBACK DlgAbout::callBack(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		DlgAbout * theDlg;
		if (message == WM_INITDIALOG) {
			theDlg = reinterpret_cast<DlgAbout*>(lParam);
			theDlg->mDlgMain.setup(hWnd);
			DLSetWindowLongPtr(hWnd, lParam);
		}
		else {
			if ((theDlg = DLGetWindowLongPtr<DlgAbout *>(hWnd)) == nullptr) {
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
					case BTN_OK:
						EndDialog(hWnd, 1);
						break;
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

	DlgAbout::DlgAbout() {}

	DlgAbout::~DlgAbout() {}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void DlgAbout::open() {
		DialogBoxParam(hInstance, MAKEINTRESOURCE(DLG_ABOUT), GetCOREInterface()->GetMAXHWnd(),
						callBack, reinterpret_cast<LPARAM>(this));
	}

	void DlgAbout::show() {
		DlgAbout dlg;
		dlg.open();
	}

	void DlgAbout::InitDlg(HWND hWnd) {
		CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
		mHWnd = hWnd;

		mLblThisAbout.setup(hWnd, LBL_TEXT);
		mLblLibAbout.setup(hWnd, LBL_TEXT2);
		mBtnOk.setup(hWnd, BTN_OK);
		mLblThisAbout.setText(Logger::aboutInfo(true));
		mLblLibAbout.setText(Logger::aboutXLibInfo(true));
		mDlgMain.show();
	}

	void DlgAbout::DestroyDlg(HWND /*hWnd*/) {
		mLblThisAbout.release();
		mLblLibAbout.release();
		mBtnOk.release();
	}

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/
}
