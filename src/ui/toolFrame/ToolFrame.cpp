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

#include "ToolFrame.h"
#include "ui/MainDock.h"
#include "common/Config.h"

namespace ui {

	/**************************************************************************************************/
	//////////////////////////////////////////* Static area *///////////////////////////////////////////
	/**************************************************************************************************/

	int ToolFrame::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_DESTROY: {
				saveConfig();
				if (mMainDockUI) {
					delete mMainDockUI;
					mMainDockUI = nullptr;
				}
				break;
			}
			case WM_SIZE: {
				if (mResizing) {
					mResizing = false;
					RECT frameSize;
					RECT frameClSize;
					GetWindowRect(mFrameHandle, &frameSize);
					GetClientRect(mFrameHandle, &frameClSize);
					MoveWindow(mFrameHandle, frameSize.left, frameSize.top, frameSize.right - frameSize.left,
								HIWORD(lParam) + (frameSize.bottom - frameSize.top - frameClSize.bottom),
								TRUE);
					mMainDockUI->setSize(0, (HIWORD(lParam)));
					return TRUE;
				}
				break;
			}
			case WM_SIZING: {
				RECT * rect = reinterpret_cast<RECT*>(lParam);
				RECT dockSize = mMainDockUI->getSize(true);
				rect->right = rect->left + mClientWidth + mBorderPx * 2;
				mResizing = true;
				return TRUE;
			}
			case CUI_POSDATA_MSG: {
				CUIPosData ** cpd = (CUIPosData **)lParam;
				cpd[0] = getSize();
				return TRUE;
			}
			case WM_SHOWWINDOW:
				if (wParam == FALSE) {
					this->mMainDockUI->becomeHidden();
					mIsHidden = true;
				}
				else {
					this->mMainDockUI->becomeShown();
					mIsHidden = false;
				}
				break;
		}
		return FALSE;
	}

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	ToolFrame::ToolFrame() {
		mIp = GetCOREInterface();

		mMainDockUI = nullptr;
		mResizing = false;
		mBorderPx = GetSystemMetrics(SM_CXSIZEFRAME);
		mClientWidth = 180;
		mClientHeight = 400;
		mCurrFramePos = CUI_FLOATING;
		mIsHidden = false;
		loadConfig();
	}

	ToolFrame::~ToolFrame() {}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ToolFrame::create() {
		mFrameHandle = CreateCUIFrameWindow(mIp->GetMAXHWnd(), _T("X-Object options"), 0, 0, 100, 200);
		ICUIFrame * frame = ::GetICUIFrame(mFrameHandle);

		frame->Hide(mIsHidden ? TRUE : FALSE);
		frame->SetContentType(CUI_HWND);
		frame->SetPosType(CUI_VERT_DOCK | CUI_FLOAT | CUI_DONT_SAVE | CUI_MAX_SIZED);
		frame->SetName(_T("X-Object options"));
		frame->InstallMsgHandler(static_cast<CUIFrameMsgHandler*>(this));
		frame->SetMenuDisplay(CUI_MENU_SHOW_ENABLED);
		frame->SetSystemWindow(FALSE);

		mMainDockUI = new MainDock();
		mMainDockUI->create(mFrameHandle);
		RECT dockSize = mMainDockUI->getSize(true);
		mClientHeight = dockSize.bottom;
		mClientWidth = dockSize.right;
		//frame->SetContentHandle(mMainDockUI->hwnd());

		if (mCurrFramePos == CUI_FLOATING) {
			dockSize.right = dockSize.right + mBorderPx * 2;
			dockSize.left = dockSize.left + 50;
			dockSize.right = dockSize.right + 50;
			dockSize.top = dockSize.top + 100;
			dockSize.bottom = dockSize.bottom + 100;
			GetCUIFrameMgr()->FloatCUIWindow(mFrameHandle, &dockSize);
		}
		else {
			GetCUIFrameMgr()->DockCUIWindow(mFrameHandle, mCurrFramePos);
		}

		ReleaseICUIFrame(frame);
		GetCUIFrameMgr()->RecalcLayout(TRUE);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	CUIPosData * ToolFrame::getSize() {
		return static_cast<CUIPosData*>(this);
	}

	int ToolFrame::GetWidth(int /*sizeType*/, int /*orient*/) {
		return mClientWidth;
	}

	int ToolFrame::GetHeight(int /*sizeType*/, int /*orient*/) {
#if MAX_VERSION_MAJOR > 15
		RECT rect;
		GetClientRect(GetCOREInterface()->GetMAXHWnd(), &rect);
		RECT sizeFrame;
		GetClientRect(mFrameHandle, &sizeFrame);
		mClientHeight = sizeFrame.bottom;
		mMainDockUI->setSize(0, mClientHeight);
		return rect.bottom;
#else
		// client size
		RECT size;
		GetClientRect(GetCOREInterface()->GetMAXHWnd(), &size);
		//size.right = size.right - size.left;
		mClientHeight = size.bottom - size.top;

		// minus tool bars
		CUIFrameMgr * fm = GetCUIFrameMgr();
		ICUIFrame * f = nullptr;
		int count = fm->GetCount();
		int currTopRang = 0;
		int currBottomRang = 0;
		int currShift = 0;
		RECT tmpRect;
		for (int i = 0; i < count; ++i) {
			f = fm->GetICUIFrame(i);
			if (!f->IsFloating() && !f->IsHidden() && f->IsEnabled()) {
				if (f->GetCurPosition() == CUI_TOP_DOCK) {
					if (currTopRang < f->GetPosRank()) {
						currTopRang = f->GetPosRank();
						GetWindowRect(f->GetHwnd(), &tmpRect);
						currShift = currShift + (tmpRect.bottom - tmpRect.top);
					}
				}
				if (f->GetCurPosition() == CUI_BOTTOM_DOCK) {
					if (currBottomRang < f->GetPosRank()) {
						currBottomRang = f->GetPosRank();
						GetWindowRect(f->GetHwnd(), &tmpRect);
						currShift = currShift + (tmpRect.bottom - tmpRect.top);
					}
				}
			}

		//         win::Base base;
		//         base.setup(f->GetHwnd());
		//         std::cout << f->GetPosRank() << " " << f->GetPosSubrank() << " Name: " << sts::toMbString(base.text()).c_str();
		//         RECT s = base.size();
		//         std::cout << " size: " << s.right - s.left << " " << s.bottom - s.top << std::endl;
		}

		mClientHeight = mClientHeight - currShift - 60/* buttom line 50*/;

		//    std::cout << mClientHeight << std::endl;

		mMainDockUI->setSize(0, mClientHeight);
		return mClientHeight;
#endif
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void ToolFrame::saveConfig() {
		Config * conf = Config::instance();
		conf->beginGroup(TOTEXT(ToolFrame));

		ICUIFrame * frame = ::GetICUIFrame(mFrameHandle);

		conf->setValue(TOTEXT(mCurrFramePos), static_cast<int>(frame->GetCurPosition()));
		conf->setValue(TOTEXT(mIsHidden), mIsHidden);

		ReleaseICUIFrame(frame);

		conf->endGroup();
		conf->safeFree();
	}

	void ToolFrame::loadConfig() {
		Config * conf = Config::instance();
		conf->beginGroup(TOTEXT(ToolFrame));

		mCurrFramePos = conf->value(TOTEXT(mCurrFramePos), static_cast<int>(CUI_FLOATING));
		mIsHidden = conf->value(TOTEXT(mIsHidden), true);

		conf->endGroup();
		conf->safeFree();
	}

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/
}
