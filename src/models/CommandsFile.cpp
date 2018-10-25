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
#include <IPathConfigMgr.h>
#pragma warning(pop)

#include <algorithm>
#include <cctype>
#include "CommandsFile.h"

namespace md {

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool CommandsFile::loadFile() {
    const auto callback = [&](const xobj::Command & cmd) {
        mData.emplace_back(cmd);
        if (cmd.mId != xobj::Command::invalidId() && cmd.mId > mGeneratorVal) {
            mGeneratorVal = cmd.mId;
        }
        return true;
    };

    try {
        return xobj::CommandsFile::loadFile(xobj::fromMPath(mFilePath.GetString()), callback);
    }
    catch (std::exception & e) {
        LError << "Can't load file <" << xobj::fromMStr(mFilePath.GetString()) << "> reason: " << e.what();
        return false;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool CommandsFile::loadSimData(const MaxSDK::Util::Path & filePath) {
    // MaxSDK::Util::Path::Exists have been added sine 3Ds Max 2013
    const auto pathConfMgr = IPathConfigMgr::GetPathConfigMgr();

    mData.clear();
    mGeneratorVal = 0;
    if (!pathConfMgr->DoesFileExist(filePath)) {
        LError << "X-Plane commands file isn't found by the path: " << xobj::fromMStr(filePath.GetString());
        return false;
    }

    mIsEditable = false;
    mUsesId = false;
    mIsForProject = false;
    mFilePath = filePath;
    mDisplayName = _T("[X-Plane] Commands");
    return loadFile();
}

bool CommandsFile::loadProjectData(const MaxSDK::Util::Path & filePath) {
    // MaxSDK::Util::Path::Exists have been added sine 3Ds Max 2013
    const auto pathConfMgr = IPathConfigMgr::GetPathConfigMgr();

    mData.clear();
    mGeneratorVal = 0;
    if (!pathConfMgr->DoesFileExist(filePath)) {
        LError << "Project commands file isn't found by the path: " << xobj::fromMStr(filePath.GetString());
        return false;
    }

    mIsEditable = true;
    mUsesId = true;
    mIsForProject = true;
    mFilePath = filePath;
    mDisplayName = _T("[Project] Commands");
    return loadFile();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool CommandsFile::saveData(const MaxSDK::Util::Path & filePath) {
    if (mIsEditable) {
        std::size_t counter = 0;
        const auto callback = [&](xobj::Command & cmd) {
            if (counter >= mData.size()) {
                return false;
            }
            cmd = mData.at(counter);
            if (!mUsesId) {
                cmd.mId = xobj::Command::invalidId();
            }
            ++counter;
            return true;
        };

        try {
            xobj::CommandsFile::saveFile(xobj::fromMPath(filePath.GetString()), callback);
            return true;
        }
        catch (std::exception & e) {
            LError << "Can't save file <" << xobj::fromMStr(filePath.GetString()) << "> reason: " << e.what();
            return false;
        }
    }
    return false;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::optional<std::size_t> CommandsFile::indexOfKey(const std::string & key) {
    std::size_t counter = 0;
    for (const auto & v : mData) {
        if (v.mKey == key) {
            return counter;
        }
        ++counter;
    }
    return std::nullopt;
}

std::optional<std::size_t> CommandsFile::indexOfId(const std::uint64_t id) {
    std::size_t counter = 0;
    for (const auto & v : mData) {
        if (v.mId == id) {
            return counter;
        }
        ++counter;
    }
    return std::nullopt;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::uint64_t CommandsFile::generateId() {
    ++mGeneratorVal;
    return mGeneratorVal;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void CommandsFile::sortData() {
    const auto sortFn = [](const xobj::Command & v1, const xobj::Command & v2) {
        return std::lexicographical_compare(v1.mKey.begin(), v1.mKey.end(),
                                            v2.mKey.begin(), v2.mKey.end(),
                                            [](const char x, const char y) {
                                                return std::tolower(static_cast<unsigned char>(x)) <
                                                       std::tolower(static_cast<unsigned char>(y));
                                            });
    };
    std::sort(mData.begin(), mData.end(), sortFn);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
