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
#include <IPathConfigMgr.h>

#include <Qt/QmaxMainWindow.h>
#include <Qt/QMaxWinHost.h>
#include <Qt/QmaxDockWidget.h>

#include <QResizeEvent>
#pragma warning(pop)

#include "ui-win/MainDock.h"

struct NotifyInfo;

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
            connect(mWinLegacyDock, &QObject::destroyed, [&]() { mWinLegacyDock = nullptr; });
        }

        virtual ~XplnDockContainer() = default;

        //-------------------------------------------------------------------------

        void createLegacyWin() const {
            if (mWinLegacyDock) {
                mWinLegacyDock->createLegacyWin();
            }
        }

        void destroyLegacyWin() const {
            if (mWinLegacyDock) {
                mWinLegacyDock->destroyLegacyWin();
            }
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
    // Actual for 2019 max only and maybe for next version but on 08.09.18 [2019] is the latest one.
    // The problem that the 3Ds Max 2019 loads layout file before we create our dock widget, 
    // so the dock windows can't be placed to the correct (where it was last time) place with 3Ds Max. 
    // We save our own layout file before 3Ds Max shutdown and loads it after our widget is created.
    // It isn't a good solution but I haven't found better one.
    // BTW I have also tried to create the widget in GUP constructor - result the same.
    /**************************************************************************************************/

#if MAX_VERSION_MAJOR == 21 // 2019
    inline QString layoutFile() {
        const auto layoutFile = IPathConfigMgr::GetPathConfigMgr()->GetDir(APP_PLUGCFG_DIR);
        return QString::fromWCharArray(layoutFile).append("\\fix-2019.layout");
    }

    static void shutdown(void * param, NotifyInfo *) {
        const auto filePath = layoutFile();
        auto * qtMaxWindow = GetCOREInterface()->GetQmaxMainWindow();

        QFile file(filePath);
        file.open(QIODevice::WriteOnly);
        file.close();
        qtMaxWindow->saveLayout(filePath);
        UnRegisterNotification(shutdown, param, NOTIFY_SYSTEM_SHUTDOWN);
    }
#endif

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
        QObject::connect(mDockContainer, &QObject::destroyed, [&]() { mDockContainer = nullptr; });

#if MAX_VERSION_MAJOR == 21 // 2019 // see information above
        const auto filePath = layoutFile();
        if (QFile::exists(filePath)) {
            qtMaxWindow->loadLayout(filePath);
        }
        RegisterNotification(shutdown, this, NOTIFY_SYSTEM_SHUTDOWN);
#endif
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
