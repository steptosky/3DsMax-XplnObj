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

#include "CommonClassDesc.h"
#include "resource/ResHelper.h"
#include "ui/main-menu/MainMenuActions.h"
#include "gup/ObjCommon.h"

#define COMMON_CLASS_ID	Class_ID(0xf5226b9, 0x5b131ef2)

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int CommonClassDesc::IsPublic() { return TRUE; }
void * CommonClassDesc::Create(BOOL /*loading = FALSE*/) { return reinterpret_cast<void *>(ObjCommon::instance()); }
HINSTANCE CommonClassDesc::HInstance() { return ResHelper::hInstance; }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

SClass_ID CommonClassDesc::SuperClassID() { return GUP_CLASS_ID; }
Class_ID CommonClassDesc::ClassID() { return COMMON_CLASS_ID; }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

const TCHAR * CommonClassDesc::ClassName() { return _T("X-Common"); }
const TCHAR * CommonClassDesc::Category() { return _T("X-Plane Obj common class"); }
const TCHAR * CommonClassDesc::InternalName() { return _T("xCommonObject"); }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int CommonClassDesc::NumActionTables() { return 1; }

ActionTable * CommonClassDesc::GetActionTable(int idx) {
	DbgAssert(idx == 0);
	if (mActionTable == nullptr) {
		mActionTable = new ui::MainMenuActions();
	}
	return mActionTable;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
