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

#include "ui-win/controls/Base.h"
#include "common/String.h"
#include "controls/Combo.h"
#include "controls/DcList.h"

namespace ui {
namespace win {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    /*!
     * \details View for datarefs and commands.
     */
    template<typename T>
    class DlgDcView : public T {
    public:

        DlgDcView() = default;
        virtual ~DlgDcView() = default;

        //-------------------------------------------------------------------------

        void open() override;
        void setAvailableFiles(const typename T::Files * files) override;
        void setCurrKey(const MStr & key) override;
        void setSearchKey(const MStr & key) override;
        void setCurrFile(const typename T::FilePtr & file, std::optional<std::size_t> selectedItem) override;

        //-------------------------------------------------------------------------

    private:

        void initDlg(HWND hWnd);
        void destroyDlg(HWND hWnd);

        void fileChanged();
        void keyEntered();
        void searchKey();
        void clearSearchKey();
        void keySelected();
        void newData();

        void setDataEditable(bool state);

        static const MCHAR * tittle();

        static INT_PTR callBack(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        ctrl::Base mDlgMain;
        ctrl::Base mCtrlLblCurrPath;
        ctrl::Base mCtrlLblCurrPathType;
        ctrl::Base mCtrlBtnNewData;
        ctrl::Combo mCtrlComboFile;
        ctrl::DcList mCtrlList;
        ICustEdit * mCtrlKey = nullptr;
        ICustEdit * mCtrlSearchKey = nullptr;

        MStr mCurrKey;
        typename T::FilePtr mCurrFile;

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}

#include "DlgDcView.inl.h"
