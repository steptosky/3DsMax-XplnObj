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

#include "MainMenuActions.h"
#include "resource/resource.h"
#include "resource/ResHelper.h"

#define NumElements(array) (sizeof(array) / sizeof(array[0]))

namespace ui {
namespace win {

    /**************************************************************************************************/
    /////////////////////////////////////////* Static area *////////////////////////////////////////////
    /**************************************************************************************************/

    static ActionDescription gActions[] = {
            {MainMenuActions::ActionDoc, IDS_MAIN_MENU_DOC, IDS_MAIN_MENU_DOC, IDS_MAIN_MENU_ACTION_TABLE_CATEGORY},
            {MainMenuActions::ActionUpdate, IDS_MAIN_MENU_UPDATE,IDS_MAIN_MENU_UPDATE, IDS_MAIN_MENU_ACTION_TABLE_CATEGORY},
            {MainMenuActions::ActionDonate, IDS_MAIN_MENU_DONATE, IDS_MAIN_MENU_DONATE, IDS_MAIN_MENU_ACTION_TABLE_CATEGORY},
            {MainMenuActions::ActionAbout, IDS_MAIN_MENU_ABOUT, IDS_MAIN_MENU_ABOUT, IDS_MAIN_MENU_ACTION_TABLE_CATEGORY},
            {MainMenuActions::ActionSettings, IDS_MAIN_MENU_SETTINGS, IDS_MAIN_MENU_SETTINGS, IDS_MAIN_MENU_ACTION_TABLE_CATEGORY},
#ifndef NDEBUG
            {MainMenuActions::ActionCppMesh, IDS_MAIN_MENU_CPP_MESH, IDS_MAIN_MENU_CPP_MESH, IDS_MAIN_MENU_ACTION_TABLE_CATEGORY},
#endif
    };

    const TSTR & MainMenuActions::name() {
        return nameNotConst();
    }

    TSTR & MainMenuActions::nameNotConst() {
        static TSTR name(ResHelper::string256(IDS_MAIN_MENU_ACTION_TABLE_CATEGORY));
        return name;
    }

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    MainMenuActions::MainMenuActions()
        : ActionTable(mTableId, mTableContextId, nameNotConst()) {

        BuildActionTable(nullptr, NumElements(gActions), gActions, ResHelper::hInstance);
        DbgAssert(MainMenuActions::Count() == NumElements(gActions));
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
