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

#include "models/DatarefsFile.h"
#include "models/CommandsFile.h"
#include "ui-win/controls-new/Control.h"

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/

        /*!
         * \details List control for datarefs and commands.
         */
        class DcList : public Control {
        public:

            DcList() = default;
            DcList(const DcList &) = delete;
            DcList(DcList &&) = delete;

            virtual ~DcList() = default;

            DcList & operator=(const DcList &) = delete;
            DcList & operator=(DcList &&) = delete;

            //-------------------------------------------------------------------------

            std::function<void(DcList *)> onSelectionChanged;

            //-------------------------------------------------------------------------

            void showData(const md::DatarefsFile::Ptr & file,
                          std::optional<std::size_t> selectedItem,
                          const std::string & filter = std::string());

            void showData(const md::CommandsFile::Ptr & file,
                          std::optional<std::size_t> selectedItem,
                          const std::string & filter = std::string());

            void clear();

            //-------------------------------------------------------------------------

            std::optional<std::size_t> selectedIndex();

            //-------------------------------------------------------------------------

        private:

            std::optional<INT_PTR> procedure(UINT message, WPARAM wParam, LPARAM lParam) override;
            void onParentInit(HWND owner) override;

            void setupColumns(bool useId);
            void setupColumnsWidth(bool useId);
            void setSelected(std::optional<std::size_t> selectedItem);
            int mColumnsCount = 0;

        };

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/
    }
}
}
