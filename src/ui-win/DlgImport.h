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

#include "converters/ConverterToMax.h"
#include "ui-win/controls-new/ModalDialog.h"
#include "ui-win/controls-new/EditBox.h"
#include "ui-win/controls-new/Button.h"
#include "ui-win/controls-new/Static.h"

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class DlgImport {
    public:

        //-------------------------------------------------------------------------

        DlgImport() = default;
        ~DlgImport() = default;

        //-------------------------------------------------------------------------

        bool show(const TCHAR * inFileName, Interface * inIp, bool suppressPrompts);

        //-------------------------------------------------------------------------

    private:

        wrapper::Static mLblVersion;
        wrapper::Button mBtnCheckUpdate;
        wrapper::Button mBtnDonate;
        wrapper::Button mBtnOk;
        wrapper::Button mBtnCancel;
        wrapper::Button mBtnAbout;;
        wrapper::EditBox mEdtLog;
        wrapper::ModalDialog mDlgMain;

        //-------------------------------------------------------------------------

        uint32_t mErrorCount = 0;
        uint32_t mWarningCount = 0;

        //-------------------------------------------------------------------------

        static void logCallback(std::size_t level, const char * msg);

        //-------------------------------------------------------------------------

        int startImport();
        void finish(bool falied);

        ConverterToMax mConverterer;
        TimeValue mTime = 0;
        std::string mLogText;

        Interface * mIp = nullptr;
        const TCHAR * mFileName = nullptr;
        bool mSuppressPrompts = false;
        bool mFinished = false;

        static DlgImport * gImportDlg;

        //-------------------------------------------------------------------------

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
