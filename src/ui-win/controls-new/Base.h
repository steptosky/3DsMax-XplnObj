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

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <cstdint>
#include <optional>
#include "common/String.h"

namespace ui {
namespace win {
    namespace wrapper {

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/

        class Base {
        public:

            //-------------------------------------------------------------------------

            Base() = default;
            Base(const Base &) = delete;
            Base(Base &&) = delete;

            virtual ~Base() = default;

            Base & operator=(const Base &) = delete;
            Base & operator=(Base &&) = delete;

            //-------------------------------------------------------------------------

            HWND parent();
            HWND hwnd() { return mHwnd; }
            HWND hwnd() const { return mHwnd; }
            int controlId() const { return mControlId; }

            //-------------------------------------------------------------------------

            void setRect(const RECT & rect, bool repaint = false);
            RECT winRect() const;
            RECT clientRect() const;
            void move(const POINT & point, bool repaint = false);

            void setSize(const SIZE & size);
            SIZE size() const;

            //-------------------------------------------------------------------------

            void enable(bool state);
            void enable() { enable(true); }
            void disable() { enable(false); }

            void show(bool state = true);
            void hide() { show(false); }

            virtual void setText(const MStr & text);
            virtual void setXObjText(const std::string & text);

            virtual MStr text() const;
            virtual std::string textXObj() const;

            //-------------------------------------------------------------------------

            virtual void setToolTip(const MStr & text, std::uint32_t showTimeMilliseconds = 10000);

            //-------------------------------------------------------------------------

        protected:

            static INT_PTR procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
            virtual std::optional<INT_PTR> procedure(UINT message, WPARAM wParam, LPARAM lParam) = 0;

            void setHwnd(HWND hwnd) { mHwnd = hwnd; }
            void setId(const int id) { mControlId = id; }

        private:

            HWND mHwnd = nullptr;
            int mControlId = -1;

        };

        /**************************************************************************************************/
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**************************************************************************************************/

    }
}
}
