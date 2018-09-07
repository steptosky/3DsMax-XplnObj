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
#include <memory>

#pragma warning(push, 0)
#include <maxicon.h>

#include <Qt/QmaxMainWindow.h>
#include <Qt/QMaxWinHost.h>
#include <Qt/QmaxDockWidget.h>

#include <QResizeEvent>
#pragma warning(pop)

#include "ui-win/MainDock.h"

namespace ui {
namespace qt {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    class XplnLegacyDock : public MaxSDK::QMaxWinHost {
    public:

        explicit XplnLegacyDock(QWidget * parent)
            : QMaxWinHost(parent) {
            createLegacyWin();
        }

        virtual ~XplnLegacyDock() {
            destroyLegacyWin();
        }

        //-------------------------------------------------------------------------

        void showEvent(QShowEvent * event) override {
            if (mWinLegacyDock) {
                mWinLegacyDock->setActive(true);
            }
            QMaxWinHost::showEvent(event);
        }

        void hideEvent(QHideEvent * event) override {
            if (mWinLegacyDock) {
                mWinLegacyDock->setActive(false);
            }
            QMaxWinHost::hideEvent(event);
        }

        //-------------------------------------------------------------------------

        void resizeEvent(QResizeEvent * event) override {
            if (mWinLegacyDock) {
                mWinLegacyDock->setSize(0, event->size().height());
            }
            QMaxWinHost::resizeEvent(event);
        }

        //-------------------------------------------------------------------------

        void createLegacyWin() {
            if (!mWinLegacyDock) {
                mWinLegacyDock = std::make_unique<win::MainDock>();
                mWinLegacyDock->create(reinterpret_cast<HWND>(winId()));
                setHostedWindow(mWinLegacyDock->getHwnd());
                setMaximumWidth(mWinLegacyDock->getMaxWidth());
                setMinimumWidth(mWinLegacyDock->getMaxWidth());
                setMinimumHeight(300);
            }
        }

        void destroyLegacyWin() {
            setHostedWindow(nullptr);
            mWinLegacyDock.reset();
        }

        //-------------------------------------------------------------------------

    private:

        std::unique_ptr<win::MainDock> mWinLegacyDock;

    };

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    class XplnDockContainer : public MaxSDK::QmaxDockWidget {
    public:

        explicit XplnDockContainer(QWidget * parent)
            : QmaxDockWidget("XObjectOptions", "X-Object Options", parent) {
            setOrientation(Qt::Vertical);
            setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
            mWinLegacyDock = new XplnLegacyDock(this);
            setWidget(mWinLegacyDock);
        }

        virtual ~XplnDockContainer() = default;

        //-------------------------------------------------------------------------

        void createLegacyWin() const {
            mWinLegacyDock->createLegacyWin();
        }

        void destroyLegacyWin() const {
            mWinLegacyDock->destroyLegacyWin();
        }

        //-------------------------------------------------------------------------

    private:

        // QT will delete it for us while parent is being deleted.
        XplnLegacyDock * mWinLegacyDock = nullptr;

    };

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    ToolFrame::~ToolFrame() {
        destroy();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ToolFrame::create() {
        if (mDockContainer) {
            mDockContainer->createLegacyWin();
            mDockContainer->setVisible(true);
            return;
        }

        auto * qtMaxWindow = GetCOREInterface()->GetQmaxMainWindow();
        mDockContainer = new XplnDockContainer(qtMaxWindow);
        qtMaxWindow->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, mDockContainer);
    }

    void ToolFrame::destroy() {
        if (mDockContainer) {
            // mDockContainer will delete by QT
            // we just delete our legacy window
            mDockContainer->destroyLegacyWin();
        }
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}

#endif
