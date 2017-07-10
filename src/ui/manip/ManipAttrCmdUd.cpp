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

#include "ManipAttrCmdUd.h"

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

	INT_PTR ManipAttrCmdUd::panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		ManipAttrCmdUd * theDlg;
		if (msg == WM_INITDIALOG) {
			theDlg = reinterpret_cast<ManipAttrCmdUd*>(lParam);
			DLSetWindowLongPtr(hWnd, lParam);
			theDlg->initWindow(hWnd);
		}
		else if (msg == WM_DESTROY) {
			theDlg = DLGetWindowLongPtr<ManipAttrCmdUd*>(hWnd);
			theDlg->destroyWindow(hWnd);
		}
		else {
			theDlg = DLGetWindowLongPtr<ManipAttrCmdUd *>(hWnd);
			if (!theDlg) {
				return FALSE;
			}
		}

		//--------------------------------------

		switch (msg) {
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case BTN_COMMAND2: {
						Factory::showNotImplemented();
						break;
					}
					case BTN_COMMAND: {
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
					case EDIT_COMMAND: {
						theDlg->mData.setCmdPositive(sts::toMbString(UiUtilities::getText(theDlg->cEdtCmdPos)));
						theDlg->save();
						break;
					}
					case EDIT_COMMAND2: {
						theDlg->mData.setCmdNegative(sts::toMbString(UiUtilities::getText(theDlg->cEdtCmdNeg)));
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
			default: break;
		}
		return 0;
	}

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	ManipAttrCmdUd::ManipAttrCmdUd(MdManip * modelData)
		: mModelData(modelData) {
		assert(mModelData);
	}

	ManipAttrCmdUd::~ManipAttrCmdUd() {
		ManipAttrCmdUd::destroy();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrCmdUd::create(HWND inParent) {
		assert(inParent);
		mHwnd.setup(CreateDialogParam(ResHelper::hInstance,
									MAKEINTRESOURCE(ROLL_MANIP_COMMAND_KNOB), /* the same template */
									inParent,
									reinterpret_cast<DLGPROC>(panelProc),
									reinterpret_cast<LPARAM>(this)));
		assert(mHwnd);
		if (mHwnd) {
			toWindow();
			mHwnd.show(true);
		}
		else {
			LError << WinCode(GetLastError());
		}
	}

	void ManipAttrCmdUd::destroy() {
		if (mHwnd) {
			BOOL res = DestroyWindow(mHwnd.hwnd());
			if (!res) {
				LError << WinCode(GetLastError());
			}
			mHwnd.release();
		}
	}

	RECT ManipAttrCmdUd::rect() const {
		RECT r{0,0,0,0};
		if (mHwnd) {
			r = mHwnd.rect();
		}
		return r;
	}

	void ManipAttrCmdUd::move(const POINT & point) {
		if (mHwnd) {
			mHwnd.move(point);
		}
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrCmdUd::setManip(const xobj::AttrManipBase & manip) {
		if (manip.type() != mData.type()) {
			LError << "Incorrect manipulator: " << manip.type().toString();
			return;
		}
		mData = static_cast<const xobj::AttrManipCmdSwitchUpDown &>(manip);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ManipAttrCmdUd::initWindow(HWND hWnd) {
		mLblName.setup(hWnd, LBL_NAME);
		mLblName.setText("Command switch up-down");
		cBtnCmdPos.setup(hWnd, BTN_COMMAND);
		cBtnCmdNeg.setup(hWnd, BTN_COMMAND2);
		cEdtCmdPos = GetICustEdit(GetDlgItem(hWnd, EDIT_COMMAND));
		cEdtCmdNeg = GetICustEdit(GetDlgItem(hWnd, EDIT_COMMAND2));

		cEdtToolType = GetICustEdit(GetDlgItem(hWnd, EDIT_TOOLTIP));
		cCmbCursor.setup(hWnd, CMB_CURSOR);

		for (auto & curr : xobj::ECursor::list()) {
			cCmbCursor.addItem(sts::toString(curr.toUiString()));
		}
		cCmbCursor.setCurrSelected(0);
	}

	void ManipAttrCmdUd::destroyWindow(HWND /*hWnd*/) {
		mLblName.release();
		cBtnCmdNeg.release();
		ReleaseICustEdit(cEdtCmdNeg);
		cBtnCmdPos.release();
		ReleaseICustEdit(cEdtCmdPos);
		ReleaseICustEdit(cEdtToolType);
		cCmbCursor.release();
	}

	void ManipAttrCmdUd::toWindow() {
		UiUtilities::setText(cEdtCmdNeg, sts::toString(mData.cmdNegative()));
		UiUtilities::setText(cEdtCmdPos, sts::toString(mData.cmdPositive()));
		UiUtilities::setText(cEdtToolType, sts::toString(mData.toolTip()));
		cCmbCursor.setCurrSelected(sts::toString(mData.cursor().toUiString()));
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}
