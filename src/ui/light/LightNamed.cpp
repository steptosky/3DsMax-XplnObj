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

#include "LightNamed.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "ui/UiUtilities.h"

extern HINSTANCE hInstance;

namespace ui {

	/**************************************************************************************************/
	//////////////////////////////////////////* Static area *///////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR LightNamed::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		LightNamed * theDlg;
		if (msg == WM_INITDIALOG) {
			theDlg = reinterpret_cast<LightNamed*>(lParam);
			DLSetWindowLongPtr(hWnd, lParam);
			theDlg->initWindow(hWnd);
		}
		else if (msg == WM_DESTROY) {
			theDlg = DLGetWindowLongPtr<LightNamed*>(hWnd);
			theDlg->destroyWindow(hWnd);
		}
		else {
			theDlg = DLGetWindowLongPtr<LightNamed *>(hWnd);
			if (!theDlg) {
				return FALSE;
			}
		}

		//--------------------------------------

		switch (msg) {
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case IDC_CMB_LIGHTNAMED: {
						if (HIWORD(wParam) == CBN_SELCHANGE) {
							theDlg->mData->setLightId(xobj::ELightNamed::fromUiString(sts::toMbString(theDlg->cCmbName.currSelectedText()).c_str()));
							theDlg->eventParamChanged(true);
						}
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

	LightNamed::LightNamed() {
		mData = nullptr;
	}

	LightNamed::~LightNamed() {
		LightNamed::destroy();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void LightNamed::show(xobj::ObjLightNamed * inData) {
		mData = inData;
		toWindow();
		mHwnd.show();
	}

	void LightNamed::hide() {
		mHwnd.hide();
	}

	void LightNamed::create(HWND inParent) {
		assert(hInstance);
		assert(inParent);
		mHwnd.setup(CreateDialogParam(hInstance,
									MAKEINTRESOURCE(IDD_ROLL_LIGHT_NAMED_OBJ),
									inParent,
									reinterpret_cast<DLGPROC>(panelProc),
									reinterpret_cast<LPARAM>(this)));
		assert(mHwnd);
	}

	void LightNamed::destroy() {
		assert(mHwnd);
		DestroyWindow(mHwnd.hwnd());
		mHwnd.release();
		mData = nullptr;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void LightNamed::initWindow(HWND hWnd) {
		cCmbName.setup(hWnd, IDC_CMB_LIGHTNAMED);
		assert(cCmbName);
		for (auto & curr : xobj::ELightNamed::list()) {
			cCmbName.addItem(sts::toString(curr.toUiString()));
		}
		cCmbName.setCurrSelected(0);
	}

	void LightNamed::destroyWindow(HWND /*hWnd*/) {
		cCmbName.release();
	}

	void LightNamed::toWindow() {
		if (mData) {
			enableControls();
			cCmbName.setCurrSelected(sts::toString(mData->lightId().toUiString()));
		}
		else {
			disableControls();
		}
	}

	void LightNamed::toData() {
		mData->setLightId(xobj::ELightNamed::fromUiString(sts::toMbString(cCmbName.currSelectedText()).c_str()));
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void LightNamed::enableControls() {
		cCmbName.enable();
	}

	void LightNamed::disableControls() {
		cCmbName.disable();
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/
}
