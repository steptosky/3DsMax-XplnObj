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

#include "resource/ResHelper.h"
#ifdef QT_IS_ENABLED

#include "ui-qt/toolFrame/ToolFrame.h"

#pragma warning(push, 0)
#include <GetCOREInterface.h>
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

    class XplnDockContainer : public MaxSDK::QmaxDockWidget {
    public:

        explicit XplnDockContainer(QWidget * parent, win::MainDock * mainDock)
            : QmaxDockWidget("X-Object-Options", "X-Object Options", parent),
              mWinLegacyDock(mainDock) {
            setOrientation(Qt::Vertical);
            setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

            setMaximumWidth(mWinLegacyDock->getMaxWidth());
            setMinimumWidth(mWinLegacyDock->getMaxWidth());
            setMinimumHeight(100);

            connect(this, &QDockWidget::topLevelChanged, [&](auto) {
                // when the dock becomes floating
                // it gets titleBarWidget and height are becomes less.
                // so we need make legacy dock size less too.
                auto h = height();
                const auto * tittleBar = titleBarWidget();
                if (tittleBar) {
                    h -= tittleBar->height();
                }
                mWinLegacyDock->setSize(0, h - mBottomPadding);
            });
        }

        void resizeEvent(QResizeEvent * event) override {
            QmaxDockWidget::resizeEvent(event);
            const auto * tittleBar = titleBarWidget();
            auto height = event->size().height();
            if (tittleBar) {
                height -= tittleBar->height();
            }

            mWinLegacyDock->setSize(0, height - mBottomPadding);
        }

    private:

        // the problem is the max dock widget has
        // a frame around it so content has padding
        // I don't know where I can get the correct value
        // so I used constant.
        const static int mBottomPadding = 10;
        win::MainDock * mWinLegacyDock = nullptr;

    };

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    void ToolFrame::create() {
        if (mDockContainer) {
            mDockContainer->setVisible(true);
            return;
        }

        mWinLegacyDock = new win::MainDock();
        auto * qtMaxWindow = GetCOREInterface()->GetQmaxMainWindow();
        mDockContainer = new XplnDockContainer(qtMaxWindow, mWinLegacyDock);
        auto * legacyHost = new MaxSDK::QMaxWinHost(mDockContainer);
        mDockContainer->setWidget(legacyHost);

        mWinLegacyDock->create(reinterpret_cast<HWND>(legacyHost->winId()));
        qtMaxWindow->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, mDockContainer);

        mWinLegacyDock->setActive(true);
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}

#endif
