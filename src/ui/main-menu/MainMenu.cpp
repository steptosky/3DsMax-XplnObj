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
#include "common/BaseLogger.h"
#include "MainMenuActions.h"

#pragma warning(push, 0)
#include <max.h>
#include <imenuman.h>
#include <iparamb2.h>
#include "classes-desc/ClassesDescriptions.h"
#pragma warning(pop)

#define MENU_ID MenuContextId(0xba20d03)
#define MENU_NAME "X-Plane"

namespace ui {

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	MainMenu::MainMenu()
		: mActionTable(nullptr),
		mIp(GetCOREInterface()) {
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
		IActionManager * actionMgr = mIp->GetActionManager();
		if (actionMgr) {
			actionMgr->ActivateActionTable(this, mActionTable->GetId());
		}
		installMenu();
	}

	void MainMenu::destroy() {
		// todo the recreation mechanism and the logic when the menu should be recreated.
		// The menu is not completed yet and does not has the mechanism for recreation.
		// If we update the menu we have to recreate it or it will look like previously one.
		// So for now we just delete the menu when 3Ds Max is closed 
		// and create it again when 3Ds Max is started.
		RemoveMenu();
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	BOOL MainMenu::ExecuteAction(int id) {
		switch (id) {
			case MENU_ACTION_DONATE: {
				signalDonate();
				return TRUE;
			}
			case MENU_ACTION_DOC: {
				signalDoc();
				return TRUE;
			}
			case MENU_ACTION_UPDATE: {
				signalUpdate();
				return TRUE;
			}
			case MENU_ACTION_ABOUT: {
				signalAbout();
				return TRUE;
			}
			case MENU_ACTION_SETTINGS: {
				signalSettings();
				return TRUE;
			}
			default: return FALSE;
		}
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void MainMenu::installMenu() {
		IMenuManager * manager = mIp->GetMenuManager();
		IMenuBarContext * menuContext = static_cast<IMenuBarContext*>(manager->GetContext(kMainMenuBar));

		if (manager->RegisterMenuBarContext(MENU_ID, _T(MENU_NAME)) || !manager->FindMenu(_T(MENU_NAME))) {
			//------------------------------------------------------
			// add the menu itself...
			IMenu * menuEx = GetIMenu();
			menuEx->SetTitle(_T(MENU_NAME));
			manager->RegisterMenu(menuEx, 0);
			IMenuBarContext * context = static_cast<IMenuBarContext*>(manager->GetContext(MENU_ID));
			context->SetMenu(menuEx);
			//------------------------------------------------------
			DbgAssert(mActionTable);
			if (mActionTable) {
				IMenuItem * itemSub = GetIMenuItem();
				itemSub->SetActionItem(mActionTable->GetAction(MENU_ACTION_DOC));
				menuEx->AddItem(itemSub);
				//------
				// todo uncomment when the settings will be made.
				//itemSub = GetIMenuItem();
				//itemSub->SetActionItem(mActionTable->GetAction(MENU_ACTION_SETTINGS));
				//menuEx->AddItem(itemSub);
				//------
				itemSub = GetIMenuItem();
				itemSub->ActAsSeparator();
				menuEx->AddItem(itemSub);
				//------
				itemSub = GetIMenuItem();
				itemSub->SetActionItem(mActionTable->GetAction(MENU_ACTION_DONATE));
				menuEx->AddItem(itemSub);
				//------
				itemSub = GetIMenuItem();
				itemSub->SetActionItem(mActionTable->GetAction(MENU_ACTION_UPDATE));
				menuEx->AddItem(itemSub);
				//------
				itemSub = GetIMenuItem();
				itemSub->SetActionItem(mActionTable->GetAction(MENU_ACTION_ABOUT));
				menuEx->AddItem(itemSub);
			}
			else {
				LError << "Action table is not set";
			}
			//------------------------------------------------------
			// Make a new "sub" menu item that will be installed to the menu bar
			IMenuItem * itemMainEx = GetIMenuItem();
			itemMainEx->SetSubMenu(menuEx);
			//------------------------------------------------------
			// Add the menu and update the bar to see it.
			IMenu * mainMenu = menuContext->GetMenu();
			DbgAssert(mainMenu);
			mainMenu->AddItem(itemMainEx, -1);
			//------------------------------------------------------
			manager->UpdateMenuBar();
			//------------------------------------------------------
		}
	}

	void MainMenu::RemoveMenu() {
		IMenuManager * manager = mIp->GetMenuManager();
		IMenu * menu = manager->FindMenu(_T(MENU_NAME));

		if (menu) {
			while (menu->NumItems() > 0) {
				menu->RemoveItem(0);
			}
			//------------------------------------------------------
			// Remove menu from context
			IMenuBarContext * context = static_cast<IMenuBarContext*>(manager->GetContext(MENU_ID));
			context->SetMenu(nullptr);
			manager->UnRegisterMenu(menu);
			//------------------------------------------------------
			// If you want the changes to be saved to the menu file
			// you can use the following. Because the menu in this example is added/removed 
			// when the context switches to the utility plug-in, this is not needed.
			manager->SaveMenuFile(manager->GetMenuFile());
			//------------------------------------------------------
			manager->UpdateMenuBar();
			//------------------------------------------------------
		}
	}

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

}
