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

#include "AnimCalc.h"
#include "Resource/resource.h"
#pragma warning(push, 0)
#include <max.h>
#include <3dsmaxport.h>
#pragma warning(pop)

extern HINSTANCE hInstance;

namespace ui {

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR AnimCalc::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		AnimCalc * theDlg;
		if (msg == WM_INITDIALOG) {
			theDlg = reinterpret_cast<AnimCalc*>(lParam);
			DLSetWindowLongPtr(hWnd, lParam);
			CenterWindow(hWnd, theDlg->mParent);
			theDlg->mMainWin.setup(hWnd);
			theDlg->initWindow(hWnd);
		}
		else if (msg == WM_DESTROY) {
			theDlg = DLGetWindowLongPtr<AnimCalc*>(hWnd);
			theDlg->destroyWindow(hWnd);
			theDlg->mMainWin.release();
		}
		else {
			theDlg = DLGetWindowLongPtr<AnimCalc *>(hWnd);
			if (!theDlg) {
				return FALSE;
			}
		}

		//--------------------------------------

		switch (msg) {
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case IDC_BTN_OK: {
						theDlg->calculateValues();
						EndDialog(hWnd, 1);
						break;
					}
					case IDC_BTN_CANCEL: {
						EndDialog(hWnd, 0);
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

	AnimCalc::AnimCalc()
		: mParent(nullptr) {
		mData = nullptr;
	}

	AnimCalc::~AnimCalc() {
		destroy();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	bool AnimCalc::calculate(KeyList & inOutData, HWND inParent) {
		mData = &inOutData;
		mParent = inParent;
		return DialogBoxParam(
							 hInstance,
							 MAKEINTRESOURCE(IDD_TOOL_CALC_ANIM_VALUE),
							 inParent,
							 reinterpret_cast<DLGPROC>(panelProc),
							 reinterpret_cast<LPARAM>(this)
							) ? true : false;
	}

	void AnimCalc::destroy() {
		if (mMainWin) {
			EndDialog(mMainWin.hwnd(), 0);
			mMainWin.release();
		}
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimCalc::initWindow(HWND hWnd) {
		cBtnOk.setup(hWnd, IDC_BTN_OK);
		cBtnCancel.setup(hWnd, IDC_BTN_CANCEL);
		cSpnVal1 = SetupFloatSpinner(hWnd, IDC_V1_SPIN, IDC_V1_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
		cSpnVal2 = SetupFloatSpinner(hWnd, IDC_V2_SPIN, IDC_V2_EDIT, -10000.0f, 10000.0f, 1.0f, 0.1f);
		assert(cBtnOk);
		assert(cBtnCancel);
		assert(cSpnVal1);
		assert(cSpnVal2);
	}

	void AnimCalc::destroyWindow(HWND /*hWnd*/) {
		cBtnOk.release();
		cBtnCancel.release();
		ReleaseISpinner(cSpnVal1);
		ReleaseISpinner(cSpnVal2);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimCalc::calculateValues() {
		if (mData->size() < 2)
			return;
		double tStart = static_cast<double>(mData->at(0).keyTime);
		double tEnd = static_cast<double>(mData->at(mData->size() - 1).keyTime);
		double tRange = tEnd - tStart;

		double vStart = static_cast<double>(cSpnVal1->GetFVal());
		double vEnd = static_cast<double>(cSpnVal2->GetFVal());
		double vRange = vEnd - vStart;

		for (auto & curr : *mData) {
			curr.datarefValue = static_cast<float>((((curr.keyTime - tStart) / tRange) * vRange) + vStart);
		}
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/
}
