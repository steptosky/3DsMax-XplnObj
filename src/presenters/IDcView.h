#pragma once

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

#include <functional>
#include "common/String.h"

namespace presenters {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

template<typename T>
class IDcView {
public:

    //-----------------------------------------

    typedef T File;
    typedef typename T::Ptr FilePtr;
    typedef typename T::data_type data_type;
    typedef std::vector<FilePtr> Files;

    //-----------------------------------------

    IDcView() = default;
    virtual ~IDcView() = default;

    //-----------------------------------------

    virtual void open() = 0;
    virtual void setAvailableFiles(const Files * files) = 0;
    virtual void setCurrKey(const MStr & key) = 0;
    virtual void setSearchKey(const MStr & key) = 0;
    virtual void setCurrFile(const FilePtr & file, std::optional<std::size_t> selectedItem) = 0;

    //-----------------------------------------

    std::function<void()> sigReady;
    std::function<void(const FilePtr &, const MStr & key)> sigKeyChanged;
    std::function<void(const MStr &)> sigSearchKeyChanged;
    std::function<void(const MStr &)> sigCurrFileChanged;

    //-----------------------------------------
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
