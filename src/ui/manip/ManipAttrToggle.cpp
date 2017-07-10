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

#include "ManipAttrToggle.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include "ui/UiUtilities.h"
#include <xpln/enums/ECursor.h>
#include "resource/resource.h"
#include "common/Logger.h"
#include "resource/ResHelper.h"
#include "ui/Factory.h"

namespace ui {

	/**************************************************************************************************/
	//////////////////////////////////////////* Static area *///////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR ManipAttrToggle::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		ManipAttrToggle * theDlg;
		if (msg == WM_INITDIALOG) {
			theDlg = reinterpret_cast<ManipAttrToggle*>(lParam);
			DLSetWindowLongPtr(hWnd, lParam);
			theDlg->initWindow(hWnd);
		}
		else if (msg == WM_DESTROY) {
			theDlg = DLGetWindowLongPtr<ManipAttrToggle*>(hWnd);
			theDlg->destroyWindow(hWnd);
		}
		else {
			theDlg = DLGetWindowLongPtr<ManipAttrToggle *>(hWnd);
			if (!theDlg) {
				return FALSE;
			}
		}

		//--------------------------------------

		switch (msg) {
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case BTN_DATAREF: {
						Factory::showNotImplemented();
						break;
					}
					case CMB_CURSOR: {
						if (HIWORD(wParam) == CBN_SELCHANGE) {
							theDlg->mData.setCursor(xobj::ECursor::fromUiString(sts::toMbString(theDlg->cCmbCursor.currSelectedText()).c_str()));
							theDlg->save();
						}
						break;
					}
					default: break;
				}
				break;
			}
			case WM_CUSTEDIT_ENTER: {
				switch (LOWORD(wParam)) {
					case EDIT_DATAREF: {
						theDlg->mData.setDataref(sts::toMbString(UiUtilities::getText(theDlg->cEdtDataRef)));
						theDlg->save();
						break;
					}
					case EDIT_TOOLTIP: {
						theDlg->mData.setToolTip(sts::toMbString(UiUtilities::getText(theDlg->cEdtToolType)));
						theDlg->save();
						break;
					}
					default: break;
				}
				break;
			}
			case CC_SPINNER_CHANGE: {
				switch (LOWORD(wParam)) {
					case SPN_ON: {
						theDlg->mData.setOn(theDlg->mSpnOn->GetFVal());
						theDlg->save();
						break;
					}
					case SPN_OFF: {
						theDlg->mData.setOff(theDlg->mSpnOff->GetFVal());
						theDlg->save();
						break;
					}
					default: break;
				}
				break;
			}
			default: break;
		}
		return 0;
	}

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	ManipAttrToggle::ManipAttrToggle(MdManip * modelData)
		: mModelData(modelData) {
		assert(mModelData);
	}

	ManipAttrToggle::~ManipAttrToggle() {
		ManipAttrToggle::destroy();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrToggle::create(HWND inParent) {
		assert(inParent);
		mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
									MAKEINTRESOURCE(ROLL_MANIP_TOGGLE),
									inParent,
									reinterpret_cast<DLGPROC>(panelProc),
									reinterpret_cast<LPARAM>(this)));
		assert(mHwnd);
		if (mHwnd) {
			toWindow();
			mWheel.create(inParent);
			calculateSize();
			mHwnd.show(true);
		}
		else {
			LError << WinCode(GetLastError());
		}
	}

	void ManipAttrToggle::destroy() {
		if (mHwnd) {
			BOOL res = DestroyWindow(mHwnd.hwnd());
			if (!res) {
				LError << WinCode(GetLastError());
			}
			mHwnd.release();
		}
	}

	RECT ManipAttrToggle::rect() const {
		return mSize;
	}

	void ManipAttrToggle::move(const POINT & point) {
		if (mHwnd) {
			mHwnd.move(point);
			calculateSize();
		}
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrToggle::calculateSize() {
		if (mHwnd) {
			mSize = mHwnd.rect();
			mWheel.move(POINT{0, mSize.bottom});
			RECT wheelRect = mWheel.rect();
			mSize.bottom += (wheelRect.bottom - wheelRect.top);
		}
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrToggle::setManip(const xobj::AttrManipBase & manip) {
		if (manip.type() != mData.type()) {
			LError << "Incorrect manipulator: " << manip.type().toString();
			return;
		}
		mData = static_cast<const xobj::AttrManipToggle &>(manip);
		mWheel.setManip(mData);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrToggle::initWindow(HWND hWnd) {
		std::function<void(const xobj::AttrManipWheel &)> callback = [this](const xobj::AttrManipWheel & wheel) mutable {
					assert(dynamic_cast<xobj::AttrManipWheel*>(&mData));
					static_cast<xobj::AttrManipWheel&>(mData) = wheel;
					mModelData->saveToNode(mData);
				};
		mWheel.setCallBack(callback);

		mSpnOn = SetupFloatSpinner(hWnd, SPN_ON, SPN_ON_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
		mSpnOff = SetupFloatSpinner(hWnd, SPN_OFF, SPN_OFF_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);

		cBtnDataRef.setup(hWnd, BTN_DATAREF);
		cEdtDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_DATAREF));
		cEdtToolType = GetICustEdit(GetDlgItem(hWnd, EDIT_TOOLTIP));
		cCmbCursor.setup(hWnd, CMB_CURSOR);

		for (auto & curr : xobj::ECursor::list()) {
			cCmbCursor.addItem(sts::toString(curr.toUiString()));
		}
		cCmbCursor.setCurrSelected(0);
	}

	void ManipAttrToggle::destroyWindow(HWND /*hWnd*/) {
		ReleaseISpinner(mSpnOn);
		ReleaseISpinner(mSpnOff);

		cBtnDataRef.release();
		ReleaseICustEdit(cEdtDataRef);
		ReleaseICustEdit(cEdtToolType);
		cCmbCursor.release();
	}

	void ManipAttrToggle::toWindow() {
		mSpnOn->SetValue(mData.on(), FALSE);
		mSpnOff->SetValue(mData.off(), FALSE);
		UiUtilities::setText(cEdtDataRef, sts::toString(mData.dataref()));
		UiUtilities::setText(cEdtToolType, sts::toString(mData.toolTip()));
		cCmbCursor.setCurrSelected(sts::toString(mData.cursor().toUiString()));
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}
