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

#include "AnimRotateAxisView.h"
#include "common/Logger.h"
#include "Resource/resource.h"
#include "ui/UiUtilities.h"
#include "ui/AnimCalc.h"
#include "resource/ResHelper.h"

namespace ui {

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	INT_PTR AnimRotateAxisView::panelProc(HWND /*hWnd*/, UINT msg, WPARAM wParam, LPARAM /*lParam*/) {
		switch (msg) {
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case LIST_VALUES:
						if (HIWORD(wParam) == LBN_SELCHANGE) {
							selectionChanged();
						}
						break;
					case CHK_ENABLE: setEnable();
						break;
					case CHK_REVERSE: setReverseEnable();
						break;
					case CHK_LOOP: setLoopEnable();
						break;
					case BTN_DATAREF: MessageBoxA(GetActiveWindow(), "Is not supporting right now.", "Info", 0);
						break;
					case BTN_REVERSE_VALUE: reverseValues();
						break;
					case BTN_CALC_VALUE: calculateValues();
						break;
					case BTN_UPDATE: toWindow();
						break;
					default: break;
				}
				break;
			}
			case WM_CUSTEDIT_ENTER: {
				switch (LOWORD(wParam)) {
					case EDIT_DATAREF: setDrft();
						break;
					default: break;
				}
				break;
			}
			case CC_SPINNER_CHANGE: {
				switch (LOWORD(wParam)) {
					case SPN_VAL: valueSpnChanged();
						break;
					case SPN_LOOP: loopSpnChanged();
						break;
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

	AnimRotateAxisView::AnimRotateAxisView(MdAnimRot::eAxis axis)
		: BaseProc(ResHelper::hInstance),
		mData(axis),
		mIp(GetCOREInterface()) {}

	AnimRotateAxisView::~AnimRotateAxisView() {}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimRotateAxisView::registerCallbacks() {
		RegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
		RegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
		RegisterNotification(slotAnimationModeOff, this, NOTIFY_ANIMATE_OFF);
	}

	void AnimRotateAxisView::unRegisterCallbacks() {
		UnRegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
		UnRegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
		UnRegisterNotification(slotAnimationModeOff, this, NOTIFY_ANIMATE_OFF);
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimRotateAxisView::slotObjectsDeleted(void * param, NotifyInfo * info) {
		AnimRotateAxisView * view = reinterpret_cast<AnimRotateAxisView*>(param);
		Tab<INode*> * nodeTab = reinterpret_cast<Tab<INode*>*>(info->callParam);

		if (view->mData.linkedNode()) {
			int count = nodeTab->Count();
			for (int i = 0; i < count; ++i) {
				if (view->mData.linkedNode() == *nodeTab->Addr(i)) {
					view->mData.clearLink();
				}
			}
		}
	}

	void AnimRotateAxisView::slotSelectionChange(void * param, NotifyInfo *) {
		AnimRotateAxisView * view = reinterpret_cast<AnimRotateAxisView*>(param);
		int selectedCount = view->mIp->GetSelNodeCount();
		if (selectedCount == 0) {
			view->clearValues();
			view->mData.clearLink();
		}
		else if (selectedCount == 1) {
			view->cSpnValue->SetValue(0.0f, FALSE);
			view->mData.linkNode(view->mIp->GetSelNode(0));
		}
		else {
			view->clearValues();
			view->mData.clearLink();
		}
		view->toWindow();
	}

	void AnimRotateAxisView::slotAnimationModeOff(void * param, NotifyInfo *) {
		AnimRotateAxisView * view = reinterpret_cast<AnimRotateAxisView*>(param);
		view->toWindow();
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	bool AnimRotateAxisView::create(HWND parent) {
		HWND res = CreateDialogParam(ResHelper::hInstance,
									MAKEINTRESOURCE(DLG_ANIM),
									parent,
									BaseDlgProc, reinterpret_cast<LPARAM>(this));
		if (res != nullptr) {
			registerCallbacks();
			win::Base win(res);
			win.show();
		}
		return res != nullptr;
	}

	void AnimRotateAxisView::destroy() {
		unRegisterCallbacks();
		DestroyWindow(hwnd());
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimRotateAxisView::setDataRefValueAsToolType() {
#if MAX_VERSION_MAJOR > 11
		cEditDataRef->SetTooltip(true, sts::toString(mData.mDataref).c_str());
#endif
	}

	void AnimRotateAxisView::initWindow(HWND hWnd) {
		cChkEnable.setup(hWnd, CHK_ENABLE);
		cStcName.setup(hWnd, STC_GROUP_NAME);
		cChkReverse.setup(hWnd, CHK_REVERSE);
		cChkLoop.setup(hWnd, CHK_LOOP);
		cListKeys.setup(hWnd, LIST_VALUES);
		cEditDataRef = GetICustEdit(GetDlgItem(hWnd, EDIT_DATAREF));
		cBtnDataRef.setup(hWnd, BTN_DATAREF);
		cSpnValue = SetupFloatSpinner(hWnd, SPN_VAL, SPN_VAL_EDIT, -1000000.0f, 1000000.0f, 0.0f, 0.1f);
		cSpnLoopValue = SetupFloatSpinner(hWnd, SPN_LOOP, SPN_LOOP_EDIT, -1000000.0f, 1000000.0f, 0.0f, 0.1f);
		cBtnReverseValue.setup(hWnd, BTN_REVERSE_VALUE);
		cBtnCalculateValue.setup(hWnd, BTN_CALC_VALUE);
		cBtnUpdate.setup(hWnd, BTN_UPDATE);
		cStcValue1.setup(hWnd, STC_VALUE1);
		cStcValue2.setup(hWnd, STC_VALUE2);

		cChkEnable.setToolTip(sts::toString("Enable exporting the animation."));
		cChkReverse.setToolTip(sts::toString("The keys order will be reversed while exporting."));
		cChkLoop.setToolTip(sts::toString("See obj specification."));
		cBtnDataRef.setToolTip(sts::toString("Is not supported yet."));
		cBtnReverseValue.setToolTip(sts::toString("Reverse order of keys' value."));
		cBtnCalculateValue.setToolTip(sts::toString("Auto-calculate data ref values. It takes into account the position of the frame."));
		cBtnUpdate.setToolTip(sts::toString("Update animation keys. Use it to update your animation keys when you have changed your animation without pressing auto-key button."));

		switch (mData.mAxis) {
			case MdAnimRot::X: cStcName.setText("X axis");
				break;
			case MdAnimRot::Y: cStcName.setText("Y axis");
				break;
			case MdAnimRot::Z: cStcName.setText("Z axis");
				break;
			default: ;
		}
	}

	void AnimRotateAxisView::destroyWindow(HWND /*hWnd*/) {
		cChkEnable.release();
		cChkReverse.release();
		cChkLoop.release();
		cListKeys.release();
		ReleaseICustEdit(cEditDataRef);
		cBtnDataRef.release();
		ReleaseISpinner(cSpnValue);
		ReleaseISpinner(cSpnLoopValue);
		cBtnReverseValue.release();
		cBtnCalculateValue.release();
		cBtnUpdate.release();
		cStcValue1.release();
		cStcValue2.release();
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimRotateAxisView::setWindowPos(const RECT & size) {
		win::Base(hwnd()).setRect(size);
	}

	RECT AnimRotateAxisView::clientRect() const {
		RECT rect;
		GetClientRect(hwnd(), &rect);
		return rect;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimRotateAxisView::toWindow() {
		mCurrSelected = -1;
		if (mData.hasLink()) {
			enableControls();
			cChkEnable.setState(mData.mEnable);
			cChkReverse.setState(mData.mReverse);
			cChkLoop.setState(mData.mLoopEnable);
			UiUtilities::setText(cEditDataRef, sts::toString(mData.mDataref));
			cSpnLoopValue->SetValue(mData.mLoopValue, FALSE);
			setDataRefValueAsToolType();
			makeUiList();
		}
		else {
			disableControls();
		}
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimRotateAxisView::clearValues() {
		cChkReverse.setState(false);
		cChkReverse.setState(false);
		cChkLoop.setState(false);
		cListKeys.clear();
		UiUtilities::setText(cEditDataRef, sts::toString("none"));
		cSpnValue->SetValue(0.0f, FALSE);
		cSpnLoopValue->SetValue(0.0f, FALSE);
	}

	void AnimRotateAxisView::enableControls() {
		cChkEnable.enable();
		cChkReverse.enable();
		cChkLoop.enable();
		cListKeys.enable();
		cEditDataRef->Enable();
		cBtnDataRef.enable();
		cSpnValue->Enable();
		cSpnLoopValue->Enable();
		cBtnReverseValue.enable();
		cBtnCalculateValue.enable();
		cBtnUpdate.enable();
		cStcValue1.enable();
		cStcValue2.enable();
	}

	void AnimRotateAxisView::disableControls() {
		cChkEnable.disable();
		cChkReverse.disable();
		cChkLoop.disable();
		cListKeys.disable();
		cEditDataRef->Disable();
		cBtnDataRef.disable();
		cSpnValue->Disable();
		cSpnLoopValue->Disable();
		cBtnReverseValue.disable();
		cBtnCalculateValue.disable();
		cBtnUpdate.disable();
		cStcValue1.disable();
		cStcValue2.disable();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void AnimRotateAxisView::makeUiList() {
		int sCurrSelected = cListKeys.currSelected();
		cListKeys.clear();
		MdAnimRot::KeyTimeList timeList = mData.getKeyTimeList();

		if (timeList.size() != mData.mKeyList.size()) {
			mData.mKeyList.resize(timeList.size());
		}

		int tpt = GetTicksPerFrame();
		sts::Str strTmp;
		for (size_t i = 0; i < timeList.size(); ++i) {
			strTmp.clear();
			strTmp.append(_T("#:")).append(sts::toString(i + 1)).append(_T("  "));
			strTmp.append(_T("F:")).append(sts::toString(timeList[i] / tpt)).append(_T("  "));
			strTmp.append(_T("V:")).append(sts::toString(mData.mKeyList[i]));
			cListKeys.addItem(strTmp);
		}
		cListKeys.setCurrSelected(sCurrSelected);
	}

	void AnimRotateAxisView::setEnable() {
		mData.mEnable = cChkEnable.isChecked();
		mData.saveToNode();
	}

	void AnimRotateAxisView::setReverseEnable() {
		mData.mReverse = cChkReverse.isChecked();
		mData.saveToNode();
	}

	void AnimRotateAxisView::setLoopEnable() {
		mData.mLoopEnable = cChkLoop.isChecked();
		mData.saveToNode();
	}

	void AnimRotateAxisView::setDrft() {
		mData.mDataref = sts::toMbString(UiUtilities::getText(cEditDataRef));
		setDataRefValueAsToolType();
		mData.saveToNode();
	}

	void AnimRotateAxisView::loopSpnChanged() {
		mData.mLoopValue = cSpnLoopValue->GetFVal();
		mData.saveToNode();
	}

	void AnimRotateAxisView::valueSpnChanged() {
		if (mCurrSelected == -1) {
			mCurrSelected = cListKeys.currSelected();
		}
		if (mCurrSelected == -1 || mCurrSelected >= mData.mKeyList.size()) {
			return;
		}
		mData.mKeyList[static_cast<size_t>(mCurrSelected)] = cSpnValue->GetFVal();
		mData.saveToNode();
		makeUiList();
	}

	void AnimRotateAxisView::selectionChanged() {
		mCurrSelected = cListKeys.currSelected();
		auto list1 = sts::StrUtils::split<sts::StrUtils::Vector>(sts::toString(cListKeys.currSelectedText()), _T(" "));
		if (list1.size() != 3) {
			LError << "Internal error 1.";
			return;
		}
		sts::StrUtils::trim(list1[2]);
		auto list2 = sts::StrUtils::split<sts::StrUtils::Vector>(list1[2], _T(":"));
		if (list2.size() != 2) {
			LError << "Internal error 2.";
			return;
		}
		cSpnValue->SetValue(sts::toFloat(list2[1]), FALSE);
	}

	void AnimRotateAxisView::reverseValues() {
		MdAnimRot::KeyValueList newvList;
		for (auto rIt = mData.mKeyList.rbegin(); rIt != mData.mKeyList.rend(); ++rIt) {
			newvList.push_back(*rIt);
		}
		mData.mKeyList.swap(newvList);
		mData.saveToNode();
		makeUiList();
	}

	void AnimRotateAxisView::calculateValues() {
		MdAnimRot::KeyTimeList tList = mData.getKeyTimeList();
		if (tList.size() != mData.mKeyList.size()) {
			mData.mKeyList.resize(tList.size());
		}
		AnimCalc::KeyList klist;
		size_t i = 0;
		for (auto & curr : tList) {
			klist.push_back(AnimCalc::Key());
			AnimCalc::Key & k = klist.back();
			k.keyTime = curr;
			k.datarefValue = mData.mKeyList[i];
			++i;
		}
		if (!AnimCalc().calculate(klist, cBtnCalculateValue.hwnd()))
			return;

		i = 0;
		for (auto & curr : klist) {
			mData.mKeyList[i] = curr.datarefValue;
			++i;
		}
		mData.saveToNode();
		makeUiList();
	}

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/
}
