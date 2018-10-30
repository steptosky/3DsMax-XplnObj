#pragma once

/*
**  Copyright(C) 2018, StepToSky
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

#include "Control.h"
#include <functional>
#include <string>

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/

        class MaxEdit : public Control {
        public:

            //-------------------------------------------------------------------------

            MaxEdit() = default;
            MaxEdit(const MaxEdit &) = delete;
            MaxEdit(MaxEdit &&) = delete;

            virtual ~MaxEdit() = default;

            MaxEdit & operator=(const MaxEdit &) = delete;
            MaxEdit & operator=(MaxEdit &&) = delete;

            //-------------------------------------------------------------------------

            std::function<void(MaxEdit *)> onLostFocus;
            std::function<void(MaxEdit *)> onEnter;

            //-------------------------------------------------------------------------

            void setToolTip(const MStr & text, std::uint32_t showTimeMilliseconds = 10000) override;

            //-------------------------------------------------------------------------

            void setText(const MStr & str) override;
            void setXObjText(const std::string & str) override;
            MStr text() const override;
            std::string textXObj() const override;

            //-------------------------------------------------------------------------

        protected:

            std::optional<INT_PTR> procedure(UINT message, WPARAM wParam, LPARAM lParam) override;
            void onParentInit(HWND owner) override;
            void onParentDestroy(HWND owner) override;

        private:

            ICustEdit * mControl = nullptr;

        };

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/

    }
}
}
