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

#ifdef QT_IS_ENABLED

#include "ui-qt/toolFrame/ToolFrame.h"

#pragma warning(push, 0)
#include "GetCOREInterface.h"
#include "MaxIcon.h"

#include <Qt/QmaxMainWindow.h>
#include <Qt/QmaxToolBar.h>
#include <Qt/QmaxDockingWinHost.h>
//#include <Qt/QmaxDockingWinHostDelegate.h>
#pragma warning(pop)

namespace ui {
namespace qt {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
#if 0
    class ToolbarDockingDelegate : public MaxSDK::QmaxProxyDockingWinHostDelegate {
    public:
        explicit ToolbarDockingDelegate(QmaxDockingWinHostDelegate * baseDelegate)
            : QmaxProxyDockingWinHostDelegate(baseDelegate) {}

        bool winEvent(MSG * msg, long * result) override;

    private:
        bool processMessage(UINT message, WPARAM wParam, LPARAM lParam) {
            switch (message) {
                case WM_COMMAND:
                    switch (LOWORD(wParam)) {
                        case ID_TB_0: // Dock or Float
                        {
                            auto tb = toolbar();
                            if (tb->isFloating()) {
                                GetCOREInterface()->GetQmaxMainWindow()->addToolBar(Qt::ToolBarArea::LeftToolBarArea, tb);
                            }
                            else {
                                MaxSDK::QmaxMainWindow::makeToolBarFloating(tb, QCursor::pos());
                            }
                            return true;
                        }
                        case ID_TB_1: // This is the check button ... ignore it.
                            return true;
                        case ID_TB_2:
                            MessageBox(NULL, _T("Pressed 2"), _T("ID_TB_2"), MB_OK);
                            return true;
                        default: // ID not recognized -- use default CUI processing
                            return false;
                    }
            }
            return false;
        }
    };
#endif
    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ToolFrame::create() {
        if (mToolBar) {
            // We have the toolbar already, just show it...(EndEditParams() hid it)
            mToolBar->setVisible(true);
        }
        else {
            // Create Qt toolbar
            const auto mainWindow = GetCOREInterface()->GetQmaxMainWindow();
            mToolBar = new MaxSDK::QmaxToolBar("CUI Test Toolbar", "CUI Test Toolbar", mainWindow);

            // Create docking win host for hosting the legacy CustToolbar
            MaxSDK::QmaxDockingWinHost * winHost = new MaxSDK::QmaxDockingWinHost(mToolBar);
            mToolBar->setWidget(winHost);

            mToolBar->setVisible(true);
#if 0
            // -- Now create the toolbar window
            const HWND hToolbar = CreateWindow(
                CUSTTOOLBARWINDOWCLASS,
                NULL,
                WS_CHILD | WS_VISIBLE,
                0, 0, 250, 100,
                reinterpret_cast<HWND>(winHost->winId()),
                NULL,
                hInstance,
                NULL);

            // -- Now link the toolbar to the CUI frame
            ICustToolbar * iToolBar = GetICustToolbar(hToolbar);

            // Get the 0th icon from the CUITest icon file.
            // Note that the index is 1-based.
            MaxBmpFileIcon * pIcon = new MaxBmpFileIcon(_T("CUITest"), 0 + 1);
            // -- Toss in a few controls of various sorts...

            // Add a push button
            // This one docks and undocks the toolbar if clicked...
            iToolBar->AddTool(TBITEM(CTB_PUSHBUTTON, pIcon, ID_TB_0));

            // Add a separator
            iToolBar->AddTool(ToolSeparatorItem(8));

            // Add a check button
            // This one doesn't do anything except change to blue when on...
            pIcon = new MaxBmpFileIcon(_T("CUITest"), 1 + 1);
            iToolBar->AddTool(TBITEM(CTB_CHECKBUTTON, pIcon, ID_TB_1));

            // Add a separator
            iToolBar->AddTool(ToolSeparatorItem(8));

            // Add a keyboard macro command.  In this case the Time Configuration dialog
            // is presented as an example.  In order to get the proper command ID to use
            // we must search through the names in the MAX UI action table and find
            // a match.  The list of names can be reviewed in the MAX UI when setting up
            // a keyboard shortcut.
            TSTR findName = _T("Time Configuration");
            ActionTable * pTable = ip->GetActionManager()->FindTable(kActionMainUI);
            assert(pTable);
            for (int i = 0; i < pTable->Count(); i++) {
                ActionItem * pAction = (*pTable)[i];
                TSTR desc;
                pAction->GetDescriptionText(desc);
                if (_tcscmp(findName.data(), desc.data()) == 0) {
                    int cmdID = pAction->GetId();
                    MacroButtonData md1(kActionMainUI, cmdID, _T("KBD Cmd"), _T("Key Macro Tooltip"));
                    iToolBar->AddTool(TBMACRO(&md1));
                    break;
                }
            }

            // Add a separator
            iToolBar->AddTool(ToolSeparatorItem(8));

            // This button is processed by our message handler (ID_TB_2)
            pIcon = new MaxBmpFileIcon(_T("CUITest"), 2 + 1);
            iToolBar->AddTool(TBITEM(CTB_PUSHBUTTON, pIcon, ID_TB_2));

            //Create a custom docking delegate which wraps the default docking delegate.
            //This custom delegate will process the controls we added.
            mDockingDelegate = new ToolbarDockingDelegate(iToolBar->GetDockingDelegate());
            winHost->setDockingDelegate(mDockingDelegate);
            winHost->setHostedWindow(hToolbar);

            // need to be added once to the mainwindow toolbar layout before we can float it
            mainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mToolBar);
            MaxSDK::QmaxMainWindow::makeToolBarFloating(mToolBar, QCursor::pos());

            // We are done, release the toolbar
            ReleaseICustToolbar(iToolBar);
#endif
        }
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}

#endif
