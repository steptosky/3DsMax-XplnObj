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

#pragma warning(push, 0)
#include <Path.h>
#pragma warning(pop)

#include <memory>
#include <xpln/utils/CommandsFile.h>

namespace md {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class CommandsFile {
public:

    //-------------------------------------------------------------------------

    typedef std::shared_ptr<CommandsFile> Ptr;
    typedef xobj::Command data_type;

    //-------------------------------------------------------------------------

    CommandsFile() = default;
    CommandsFile(const CommandsFile &) = default;
    CommandsFile(CommandsFile &&) = default;

    virtual ~CommandsFile() = default;

    CommandsFile & operator=(const CommandsFile &) = default;
    CommandsFile & operator=(CommandsFile &&) = default;

    //-------------------------------------------------------------------------

    MaxSDK::Util::Path mFilePath;
    MStr mDisplayName;
    bool mIsEditable = false;
    bool mUsesId = false;
    bool mIsForProject = false;

    std::vector<xobj::Command> mData;

    //-------------------------------------------------------------------------

    std::optional<std::size_t> indexOfKey(const std::string & key);
    std::optional<std::size_t> indexOfId(std::uint64_t id);

    std::uint64_t generateId();

    //-------------------------------------------------------------------------

    bool loadData(const MaxSDK::Util::Path & filePath);
    bool loadSimData(const MaxSDK::Util::Path & filePath);
    bool loadProjectData(const MaxSDK::Util::Path & filePath);

    bool saveData(const MaxSDK::Util::Path & filePath);

    //-------------------------------------------------------------------------

    void sortData();

    //-------------------------------------------------------------------------

private:

    bool loadFile();
    std::uint64_t mGeneratorVal = 0;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
