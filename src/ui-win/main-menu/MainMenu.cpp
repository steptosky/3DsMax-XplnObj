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

#include "MainMenu.h"
#include "common/Logger.h"
#include "MainMenuActions.h"

#pragma warning(push, 0)
#include <max.h>
#include <imenuman.h>
#include "classes-desc/ClassesDescriptions.h"
#pragma warning(pop)

#define MENU_ID MenuContextId(0xba20d03)
#define MENU_NAME "X-Plane"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    MainMenu::MainMenu()
        : mIp(GetCOREInterface()) {
        create();
    }

    MainMenu::~MainMenu() {
        destroy();
        mIp = nullptr;
        mActionTable = nullptr;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    void MainMenu::create() {
        DbgAssert(ClassesDescriptions::commonClass()->NumActionTables() > 0);
        mActionTable = ClassesDescriptions::commonClass()->GetActionTable(0);

        // Set up our actions / callbacks
        auto actionManager = mIp->GetActionManager();
        if (actionManager) {
            actionManager->ActivateActionTable(this, mActionTable->GetId());
        }
        installMenu();
    }

    void MainMenu::destroy() {
        // todo the recreation mechanism and the logic when the menu should be recreated.
        // The menu is not completed yet and does not has the mechanism for recreation.
        // If we update the menu we have to recreate it or it will look like previously one.
        // So for now we just delete the menu when 3Ds Max is closed 
        // and create it again when 3Ds Max is started.
        removeMenu();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    BOOL MainMenu::ExecuteAction(const int id) {
        switch (id) {
            case MainMenuActions::ActionDonate: {
                signalDonate();
                return TRUE;
            }
            case MainMenuActions::ActionDoc: {
                signalDoc();
                return TRUE;
            }
            case MainMenuActions::ActionUpdate: {
                signalUpdate();
                return TRUE;
            }
            case MainMenuActions::ActionAbout: {
                signalAbout();
                return TRUE;
            }
            case MainMenuActions::ActionSettings: {
                signalSettings();
                return TRUE;
            }
            case MainMenuActions::ActionCppMesh: {
                signalSaveCppMesh();
                return TRUE;
            }
            default: return FALSE;
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void MainMenu::installMenu() {
        auto menuManager = mIp->GetMenuManager();
        if (menuManager->RegisterMenuBarContext(MENU_ID, _T(MENU_NAME)) || !menuManager->FindMenu(_T(MENU_NAME))) {
            //------------------------------------------------------
            // add the menu itself
            auto menu = GetIMenu();
            if (!menu) {
                XLError << "Couldn't get menu";
                return;
            }
            menu->SetTitle(_T(MENU_NAME));
            menuManager->RegisterMenu(menu, 0);
            auto menuBarContext = static_cast<IMenuBarContext*>(menuManager->GetContext(MENU_ID));
            menuBarContext->SetMenu(menu);
            //------------------------------------------------------
            DbgAssert(mActionTable);
            if (mActionTable) {
                //--------------------
                const auto createMenuItem = [&](const MainMenuActions::eAction action) {
                    auto subMenuItem = GetIMenuItem();
                    subMenuItem->SetActionItem(mActionTable->GetAction(action));
                    return subMenuItem;
                };
                //--------------------
                menu->AddItem(createMenuItem(MainMenuActions::ActionDoc));
                menu->AddItem(createMenuItem(MainMenuActions::ActionSettings));
                menu->AddItem(createMenuItem(MainMenuActions::ActionDonate));
                menu->AddItem(createMenuItem(MainMenuActions::ActionUpdate));
                menu->AddItem(createMenuItem(MainMenuActions::ActionAbout));
#ifndef NDEBUG
                menu->AddItem(createMenuItem(MainMenuActions::ActionCppMesh));
#endif
                //--------------------
            }
            else {
                XLError << "Action table is not set";
            }
            //------------------------------------------------------
            // Make a new "sub" menu item that will be installed to the menu bar
            auto mainMenuItem = GetIMenuItem();
            if (!mainMenuItem) {
                XLError << "Couldn't get menu item";
                return;
            }
            mainMenuItem->SetSubMenu(menu);
            //------------------------------------------------------
            // Add the menu and update the bar to see it.
            auto mainMenuBarContext = static_cast<IMenuBarContext*>(menuManager->GetContext(kMainMenuBar));
            auto mainMenu = mainMenuBarContext->GetMenu();
            if (!mainMenu) {
                XLError << "Main menu bar context returned nullptr for the main menu";
                return;
            }
            mainMenu->AddItem(mainMenuItem, -1);
            //------------------------------------------------------
            menuManager->UpdateMenuBar();
            //------------------------------------------------------
        }
    }

    void MainMenu::removeMenu() {
        auto menuManager = mIp->GetMenuManager();
        auto menu = menuManager->FindMenu(_T(MENU_NAME));

        if (menu) {
            while (menu->NumItems() > 0) {
                menu->RemoveItem(0);
            }
            //------------------------------------------------------
            // Remove menu from context
            auto menuContext = static_cast<IMenuBarContext*>(menuManager->GetContext(MENU_ID));
            menuContext->SetMenu(nullptr);
            menuManager->UnRegisterMenu(menu);
            //------------------------------------------------------
            // If you want the changes to be saved to the menu file
            // you can use the following. Because the menu in this example is added/removed 
            // when the context switches to the utility plug-in, this is not needed.
            menuManager->SaveMenuFile(menuManager->GetMenuFile());
            //------------------------------------------------------
            menuManager->UpdateMenuBar();
            //------------------------------------------------------
        }
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
