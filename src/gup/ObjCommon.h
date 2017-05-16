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
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h> // for 3dmax 9
#include <guplib.h>
#pragma warning(pop)

#include "CloneNodeChunk.h"
#include "sts/utilities/templates/Single.h"
#include "Common/Config.h"
#include "update/UpdateChecker.h"
#include "Settings.h"

#define COMMON_CLASS_ID	Class_ID(0xf5226b9, 0x5b131ef2)

namespace ui {
	class ToolFrame;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

// TODO make the correct singlton for this class, it should return nullptr if it is deleted
class ObjCommon : public GUP, public sts_t::Single<ObjCommon> {
public:

	ObjCommon();
	~ObjCommon();

	//-------------------------------------------------------------------------

	DWORD Start() override;
	void Stop() override;

	//-------------------------------------------------------------------------

	DWORD_PTR Control(DWORD param) override;

	//-------------------------------------------------------------------------

	IOResult Save(ISave * isave) override;
	IOResult Load(ILoad * iload) override;

	//-------------------------------------------------------------------------

	UpdateChecker::Update updateInfo() const { return mUpdateChecker.updateInfo(); }

	Settings pSettings;

private:

	//-------------------------------------------------------------------------
	// Thread safe check the result of the update checking

	static void updateCheckWinCallback(HWND, UINT, UINT_PTR, DWORD);

	//-------------------------------------------------------------------------

	void DeleteThis() override;

	ui::ToolFrame * mToolFrame;
	Config * mConfig;
	CloneNodeChunk * mCloneNodeChunk;
	UpdateChecker mUpdateChecker;

	//-------------------------------------------------------------------------

	static const uint32_t mIoVersion = 1;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
