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

#include <vector>
#include <utility>

#include "converters/ConverterToXpln.h"
#include "common/Logger.h"
#include "ui-win/controls-new/ModalDialog.h"
#include "ui-win/controls-new/ExportObjList.h"
#include "ui-win/controls-new/Button.h"
#include "ui-win/controls-new/EditBox.h"
#include "ui-win/controls-new/CheckBox.h"
#include "ui-win/controls-new/Static.h"

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class DlgExport {
    public:

        //-------------------------------------------------------------------------

        DlgExport() = default;
        ~DlgExport() = default;

        //-------------------------------------------------------------------------

        bool show(const TCHAR * inFileName, Interface * inIp, bool suppressPrompts, bool selectedOnly);

        //-------------------------------------------------------------------------

    private:

        typedef std::pair<INode *, int> NodeCollectionStruct;
        typedef std::vector<NodeCollectionStruct> NodeCollection;

        void onInit();

        wrapper::ModalDialog mDlgMain;
        wrapper::Static mLblVersion;
        wrapper::Button mBtnCheckUpdate;
        wrapper::Button mBtnDonate;
        wrapper::Button mBtnSaveLog;
        wrapper::Button mBtnOk;
        wrapper::Button mBtnCancel;
        wrapper::Button mBtnAbout;;
        wrapper::Button mBtnSelAll;
        wrapper::Button mBtnUnSelAll;
        wrapper::EditBox mEdtLog;
        wrapper::CheckBox mChkAutoExport;
        wrapper::ExportObjList mLstObjects;

        //-------------------------------------------------------------------------

        uint32_t mErrorCount = 0;
        uint32_t mWarningCount = 0;

        //-------------------------------------------------------------------------

        static void logCallback(sts::BaseLogger::eType type, const char * msg);

        //-------------------------------------------------------------------------

        int startExport();
        void collectMainNodes();
        void collectMainNodes(INode * inRootNode, NodeCollection & outMains);
        void printUpdateAvailability();
        size_t selectedNodeCount();
        void slotSelObjChanged(int idx);

        ConverterToXpln mConverterer;
        TimeValue mTime = 0;
        std::string mLogText;

        Interface * mIp = nullptr;
        const TCHAR * mExpFileName = nullptr;
        bool mSuppressPrompts = false;
        bool mSelectedOnly = false;
        bool mFinished = false;

        static DlgExport * gExportDlg;

        NodeCollection mMainNodesCollection;

        //-------------------------------------------------------------------------

        void saveConfigData();
        void loadConfigData();

        void saveLogRequest();
        void exportFinished();

        //-------------------------------------------------------------------------

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
