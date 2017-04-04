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

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <Windows.h>
#include "sts/utilities/templates/Single.h"

extern HINSTANCE hInstance;

namespace ui {

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	class MainDock;

	class ToolFrame : public CUIFrameMsgHandler, public CUIPosData, public sts_t::Single<ToolFrame> {

		ToolFrame(const ToolFrame &) {}

		ToolFrame & operator =(const ToolFrame &) {
			return *this;
		}

	public:

		ToolFrame();
		virtual ~ToolFrame();

		void create();

	private:

		int GetWidth(int sizeType, int orient) override;
		int GetHeight(int sizeType, int orient) override;

		int ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

		//--------------------------------------

		CUIPosData * getSize();

		//--------------------------------------

		void saveConfig();
		void loadConfig();

		//--------------------------------------

		HWND mFrameHandle = nullptr;
		Interface * mIp = nullptr;
		MainDock * mMainDockUI = nullptr;
		bool mResizing;
		int mBorderPx;
		int mClientWidth;
		int mClientHeight;

		DWORD mCurrFramePos;
		bool mIsHidden;

	};

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/
}
