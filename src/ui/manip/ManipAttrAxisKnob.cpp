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

#include "ManipAttrAxisKnob.h"

#pragma warning(push, 0)
#include <3dsmaxport.h>
#pragma warning(pop)

#include "ui/UiUtilities.h"
#include <xpln/enums/ECursor.h>
#include "resource/resource.h"
#include "common/Logger.h"

extern HINSTANCE hInstance;

namespace ui {

	/**************************************************************************************************/
	//////////////////////////////////////////* Static area *///////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR ManipAttrAxisKnob::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		ManipAttrAxisKnob * theDlg;
		if (msg == WM_INITDIALOG) {
			theDlg = reinterpret_cast<ManipAttrAxisKnob*>(lParam);
			DLSetWindowLongPtr(hWnd, lParam);
			theDlg->initWindow(hWnd);
		}
		else if (msg == WM_DESTROY) {
			theDlg = DLGetWindowLongPtr<ManipAttrAxisKnob*>(hWnd);
			theDlg->destroyWindow(hWnd);
		}
		else {
			theDlg = DLGetWindowLongPtr<ManipAttrAxisKnob *>(hWnd);
			if (!theDlg) {
				return FALSE;
			}
		}

		//--------------------------------------

		switch (msg) {
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case BTN_DATAREF: {
						MessageBoxA(GetActiveWindow(), "Is not supporting right now.", "Info", 0);
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
					case SPN_CLICK_DELTA: {
						theDlg->mData.setClickDelta(theDlg->mSpnClick->GetFVal());
						theDlg->save();
						break;
					}
					case SPN_HOLD_DELTA: {
						theDlg->mData.setHoldDelta(theDlg->mSpnHold->GetFVal());
						theDlg->save();
						break;
					}
					case SPN_MIN: {
						theDlg->mData.setMinimum(theDlg->mSpnMim->GetFVal());
						theDlg->save();
						break;
					}
					case SPN_MAX: {
						theDlg->mData.setMaximum(theDlg->mSpnMax->GetFVal());
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

	ManipAttrAxisKnob::ManipAttrAxisKnob(MdManip * modelData)
		: mModelData(modelData) {
		assert(mModelData);
	}

	ManipAttrAxisKnob::~ManipAttrAxisKnob() {
		ManipAttrAxisKnob::destroy();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrAxisKnob::create(HWND inParent) {
		assert(hInstance);
		assert(inParent);
		mHwnd.setup(CreateDialogParam(hInstance,
									MAKEINTRESOURCE(ROLL_MANIP_AXIS_KNOB),
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

	void ManipAttrAxisKnob::destroy() {
		if (mHwnd) {
			BOOL res = DestroyWindow(mHwnd.hwnd());
			if (!res) {
				LError << WinCode(GetLastError());
			}
			mHwnd.release();
		}
	}

	RECT ManipAttrAxisKnob::rect() const {
		return mSize;
	}

	void ManipAttrAxisKnob::move(const POINT & point) {
		if (mHwnd) {
			mHwnd.move(point);
			calculateSize();
		}
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrAxisKnob::calculateSize() {
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

	void ManipAttrAxisKnob::setManip(const xobj::AttrManipBase & manip) {
		if (manip.type() != mData.type()) {
			LError << "Incorrect manipulator: " << manip.type().toString();
			return;
		}
		mData = static_cast<const xobj::AttrManipAxisKnob &>(manip);
		mWheel.setManip(mData);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrAxisKnob::initWindow(HWND hWnd) {
		mLblName.setup(hWnd, LBL_NAME);
		mLblName.setText("Axis knob");

		std::function<void(const xobj::AttrManipWheel &)> callback = [this](const xobj::AttrManipWheel & wheel) mutable {
					assert(dynamic_cast<xobj::AttrManipWheel*>(&mData));
					static_cast<xobj::AttrManipWheel&>(mData) = wheel;
					mModelData->saveToNode(mData);
				};
		mWheel.setCallBack(callback);

		mSpnClick = SetupFloatSpinner(hWnd, SPN_CLICK_DELTA, SPN_CLICK_DELTA_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
		mSpnHold = SetupFloatSpinner(hWnd, SPN_HOLD_DELTA, SPN_HOLD_DELTA_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);

		mSpnMim = SetupFloatSpinner(hWnd, SPN_MIN, SPN_MIN_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);
		mSpnMax = SetupFloatSpinner(hWnd, SPN_MAX, SPN_MAX_EDIT, -10000.0f, 10000.0f, 0.0f, 0.1f);

		cBtnDataRef.setup(hWnd, BTN_DATAREF);
		cEdtDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_DATAREF));
		cEdtToolType = GetICustEdit(GetDlgItem(hWnd, EDIT_TOOLTIP));
		cCmbCursor.setup(hWnd, CMB_CURSOR);

		for (auto & curr : xobj::ECursor::list()) {
			cCmbCursor.addItem(sts::toString(curr.toUiString()));
		}
		cCmbCursor.setCurrSelected(0);
	}

	void ManipAttrAxisKnob::destroyWindow(HWND /*hWnd*/) {
		mLblName.release();
		ReleaseISpinner(mSpnClick);
		ReleaseISpinner(mSpnHold);
		ReleaseISpinner(mSpnMim);
		ReleaseISpinner(mSpnMax);
		cBtnDataRef.release();
		ReleaseICustEdit(cEdtDataRef);
		ReleaseICustEdit(cEdtToolType);
		cCmbCursor.release();
	}

	void ManipAttrAxisKnob::toWindow() {
		mSpnClick->SetValue(mData.clickDelta(), FALSE);
		mSpnHold->SetValue(mData.holdDelta(), FALSE);
		mSpnMim->SetValue(mData.minimum(), FALSE);
		mSpnMax->SetValue(mData.maximum(), FALSE);
		UiUtilities::setText(cEdtDataRef, sts::toString(mData.dataref()));
		UiUtilities::setText(cEdtToolType, sts::toString(mData.toolTip()));
		cCmbCursor.setCurrSelected(sts::toString(mData.cursor().toUiString()));
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}
