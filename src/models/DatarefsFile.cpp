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

#include "DatarefsFile.h"
#include <algorithm>
#include <cctype>
#include "gup/ObjCommon.h"

namespace md {

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool DatarefsFile::loadFile() {
    const auto callback = [&](const xobj::Dataref & drf) {
        mData.emplace_back(drf);
        if (drf.mId != xobj::Dataref::invalidId() && drf.mId > mGeneratorVal) {
            mGeneratorVal = drf.mId;
        }
        return true;
    };

    try {
        return xobj::DatarefsFile::loadFile(xobj::fromMPath(mFilePath.GetString()), callback);
    }
    catch (std::exception & e) {
        XLError << "Can't load file <" << xobj::fromMStr(mFilePath.GetString()) << "> reason: " << e.what();
        return false;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool DatarefsFile::loadData(const MaxSDK::Util::Path & filePath) {
    // MaxSDK::Util::Path::Exists have been added sine 3Ds Max 2013
    const auto pathConfMgr = IPathConfigMgr::GetPathConfigMgr();

    mData.clear();
    mGeneratorVal = 0;
    if (!pathConfMgr->DoesFileExist(filePath)) {
        XLError << "X-Plane datarefs file isn't found by the path: " << xobj::fromMStr(filePath.GetString());
        return false;
    }
    return loadFile();
}

bool DatarefsFile::loadSimData(const MaxSDK::Util::Path & filePath) {
    mIsEditable = false;
    mUsesId = false;
    mIsForProject = false;
    mFilePath = filePath;
    mSort = true;
    mDisplayName = _T("[X-Plane] DataRefs");
    return loadData(filePath);
}

bool DatarefsFile::loadProjectData(const MaxSDK::Util::Path & filePath) {
    auto & settings = ObjCommon::instance()->pSettings;
    mUsesId = settings.isUseDatarefsId();
    mSort = settings.sortDatarefs();

    mIsEditable = true;
    mIsForProject = true;
    mFilePath = filePath;
    mDisplayName = _T("[Project] DataRefs");
    return loadData(filePath);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool DatarefsFile::saveData(const MaxSDK::Util::Path & filePath) {
    if (mIsEditable) {
        std::size_t counter = 0;
        const auto callback = [&](xobj::Dataref & drf) {
            if (counter >= mData.size()) {
                return false;
            }
            drf = mData.at(counter);
            if (!mUsesId) {
                drf.mId = xobj::Dataref::invalidId();
            }
            ++counter;
            return true;
        };

        try {
            xobj::DatarefsFile::saveFile(xobj::fromMPath(filePath.GetString()), callback);
            return true;
        }
        catch (std::exception & e) {
            XLError << "Can't save file <" << xobj::fromMStr(filePath.GetString()) << "> reason: " << e.what();
            return false;
        }
    }
    return false;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::optional<std::size_t> DatarefsFile::indexOfKey(const std::string & key) {
    std::size_t counter = 0;
    for (const auto & v : mData) {
        if (v.mKey == key) {
            return counter;
        }
        ++counter;
    }
    return std::nullopt;
}

std::optional<std::size_t> DatarefsFile::indexOfId(const std::uint64_t id) {
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

std::uint64_t DatarefsFile::generateId() {
    ++mGeneratorVal;
    return mGeneratorVal;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void DatarefsFile::sortDataIfEnabled() {
    if (mSort) {
        const auto sortFn = [](const xobj::Dataref & v1, const xobj::Dataref & v2) {
            return std::lexicographical_compare(v1.mKey.begin(), v1.mKey.end(),
                                                v2.mKey.begin(), v2.mKey.end(),
                                                [](const char x, const char y) {
                                                    return std::tolower(static_cast<unsigned char>(x)) <
                                                           std::tolower(static_cast<unsigned char>(y));
                                                });
        };
        std::sort(mData.begin(), mData.end(), sortFn);
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

}
