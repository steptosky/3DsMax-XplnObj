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

#include "LightSpillCust.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "ui/UiUtilities.h"
#include "resource/ResHelper.h"

namespace ui {

	/**************************************************************************************************/
	//////////////////////////////////////////* Static area *///////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR LightSpillCust::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		LightSpillCust * theDlg;
		if (msg == WM_INITDIALOG) {
			theDlg = reinterpret_cast<LightSpillCust*>(lParam);
			DLSetWindowLongPtr(hWnd, lParam);
			theDlg->initWindow(hWnd);
		}
		else if (msg == WM_DESTROY) {
			theDlg = DLGetWindowLongPtr<LightSpillCust*>(hWnd);
			theDlg->destroyWindow(hWnd);
		}
		else {
			theDlg = DLGetWindowLongPtr<LightSpillCust *>(hWnd);
			if (!theDlg) {
				return FALSE;
			}
		}

		//--------------------------------------

		switch (msg) {
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case IDC_BTN_DATAREF: {
						MessageBoxA(GetActiveWindow(), "Is not supporting right now.", "Info", 0);
						break;
					}
					default: break;
				}
				break;
			}
			case WM_CUSTEDIT_ENTER: {
				switch (LOWORD(wParam)) {
					case IDC_EDIT_DATAREF: {
						theDlg->mData->setDataRef(sts::toMbString(UiUtilities::getText(theDlg->cEdtDataRef)));
						theDlg->eventParamChanged(true);
						break;
					}
					default: break;
				}
				break;
			}
			case CC_SPINNER_CHANGE: {
				switch (LOWORD(wParam)) {
					case IDC_R_SPIN: {
						xobj::Color color = theDlg->mData->color();
						color.setRed(theDlg->mSpnR->GetFVal());
						theDlg->mData->setColor(color);
						theDlg->eventParamChanged(true);
						break;
					}
					case IDC_G_SPIN: {
						xobj::Color color = theDlg->mData->color();
						color.setGreen(theDlg->mSpnG->GetFVal());
						theDlg->mData->setColor(color);
						theDlg->eventParamChanged(true);
						break;
					}
					case IDC_B_SPIN: {
						xobj::Color color = theDlg->mData->color();
						color.setBlue(theDlg->mSpnB->GetFVal());
						theDlg->mData->setColor(color);
						theDlg->eventParamChanged(true);
						break;
					}
					case IDC_A_SPIN: {
						xobj::Color color = theDlg->mData->color();
						color.setAlpha(theDlg->mSpnA->GetFVal());
						theDlg->mData->setColor(color);
						theDlg->eventParamChanged(true);
						break;
					}
					case IDC_SIZE_SPIN: {
						theDlg->mData->setSize(theDlg->mSpnSize->GetFVal());
						theDlg->eventParamChanged(true);
						break;
					}
					default: break;
				}
				break;
			}
			default: break;
		}
		return FALSE;
	}

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	LightSpillCust::LightSpillCust() {
		mData = nullptr;
	}

	LightSpillCust::~LightSpillCust() {
		LightSpillCust::destroy();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void LightSpillCust::show(xobj::ObjLightSpillCust * inData) {
		mData = inData;
		toWindow();
		mHwnd.show();
	}

	void LightSpillCust::hide() {
		mHwnd.hide();
	}

	void LightSpillCust::create(HWND inParent) {
		assert(inParent);
		mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
									MAKEINTRESOURCE(IDD_ROLL_LIGHT_SPILLCUST_OBJ),
									inParent,
									reinterpret_cast<DLGPROC>(panelProc),
									reinterpret_cast<LPARAM>(this)));
		assert(mHwnd);
	}

	void LightSpillCust::destroy() {
		assert(mHwnd);
		DestroyWindow(mHwnd.hwnd());
		mHwnd.release();
		mData = nullptr;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void LightSpillCust::initWindow(HWND hWnd) {
		mSpnR = SetupFloatSpinner(hWnd, IDC_R_SPIN, IDC_R_EDIT, 0.0f, 1.0f, 0.5f, 0.1f);
		mSpnG = SetupFloatSpinner(hWnd, IDC_G_SPIN, IDC_G_EDIT, 0.0f, 1.0f, 0.5f, 0.1f);
		mSpnB = SetupFloatSpinner(hWnd, IDC_B_SPIN, IDC_B_EDIT, 0.0f, 1.0f, 0.5f, 0.1f);
		mSpnA = SetupFloatSpinner(hWnd, IDC_A_SPIN, IDC_A_EDIT, 0.0f, 1.0f, 0.5f, 0.1f);

		mSpnSize = SetupFloatSpinner(hWnd, IDC_SIZE_SPIN, IDC_SIZE_EDIT, 0.0f, 100.0f, 1.0f, 0.1f);

		cBtnDataRef.setup(hWnd, IDC_BTN_DATAREF);
		cEdtDataRef = GetICustEdit(GetDlgItem(hWnd, IDC_EDIT_DATAREF));

		assert(mSpnR);
		assert(mSpnG);
		assert(mSpnB);
		assert(mSpnA);
		assert(mSpnSize);
		assert(cEdtDataRef);
		assert(cBtnDataRef);
	}

	void LightSpillCust::destroyWindow(HWND /*hWnd*/) {
		ReleaseISpinner(mSpnR);
		ReleaseISpinner(mSpnG);
		ReleaseISpinner(mSpnB);
		ReleaseISpinner(mSpnA);
		ReleaseISpinner(mSpnSize);

		cBtnDataRef.release();
		ReleaseICustEdit(cEdtDataRef);
	}

	void LightSpillCust::toWindow() {
		if (mData) {
			enableControls();
			const xobj::Color & color = mData->color();
			mSpnR->SetValue(color.red(), FALSE);
			mSpnG->SetValue(color.green(), FALSE);
			mSpnB->SetValue(color.blue(), FALSE);
			mSpnA->SetValue(color.alpha(), FALSE);
			mSpnSize->SetValue(mData->size(), FALSE);
			UiUtilities::setText(cEdtDataRef, sts::toString(mData->dataRef()));
		}
		else {
			disableControls();
		}
	}

	void LightSpillCust::toData() {
		xobj::Color color(mSpnR->GetFVal(), mSpnG->GetFVal(), mSpnB->GetFVal(), mSpnA->GetFVal());
		mData->setColor(color);
		mData->setSize(mSpnSize->GetFVal());
		mData->setDataRef(sts::toMbString(UiUtilities::getText(cEdtDataRef)));
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void LightSpillCust::enableControls() {
		mSpnR->Enable();
		mSpnG->Enable();
		mSpnB->Enable();
		mSpnA->Enable();
		mSpnSize->Enable();

		cBtnDataRef.enable();
		cEdtDataRef->Enable();
	}

	void LightSpillCust::disableControls() {
		mSpnR->Disable();
		mSpnG->Disable();
		mSpnB->Disable();
		mSpnA->Disable();
		mSpnSize->Disable();

		cBtnDataRef.disable();
		cEdtDataRef->Disable();
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/
}
