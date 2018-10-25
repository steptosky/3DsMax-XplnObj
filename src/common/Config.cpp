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

#include "Config.h"

#pragma warning(push, 0)
#include <IPathConfigMgr.h>
#pragma warning(pop)

#include "common/Logger.h"
#include "common/String.h"

using namespace std::string_literals;

/**************************************************************************************************/
/////////////////////////////////////////* Static area *////////////////////////////////////////////
/**************************************************************************************************/

Config::Config() {
    // Path like: C:\Users\Alex\AppData\Local\Autodesk\3dsMax\9 - 64bit\enu\plugcfg
    mConfigFile = IPathConfigMgr::GetPathConfigMgr()->GetDir(APP_PLUGCFG_DIR);
    mConfigFile.Append(_T("xpln_obj_io.ini"));
    mSimDir = _T(R"(X:\X-Plane 11)");
    load();
}

Config::~Config() {
    Config::save();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void Config::setSimDir(const MaxSDK::Util::Path & dir) {
    const auto temp = mSimDir;
    mSimDir = dir;
    sigSimDirChanged(*this, temp, mSimDir);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

MaxSDK::Util::Path Config::simDir() {
    return mSimDir;
}

MaxSDK::Util::Path Config::simDatarefsFile() {
    MaxSDK::Util::Path sim = simDir();
    return sim.IsEmpty() ? sim : sim.Append(_T("Resources")).Append(_T("plugins")).Append(_T("DataRefs.txt"));
}

MaxSDK::Util::Path Config::simCommandsFile() {
    MaxSDK::Util::Path sim = simDir();
    return sim.IsEmpty() ? sim : sim.Append(_T("Resources")).Append(_T("plugins")).Append(_T("Commands.txt"));
}

MaxSDK::Util::Path Config::projectDatarefsFile() {
    MaxSDK::Util::Path sceneFolder = GetCOREInterface()->GetCurFilePath();
    return sceneFolder.IsEmpty() ? sceneFolder : sceneFolder.RemoveLeaf().Append(_T("DataRefs.txt"));
}

MaxSDK::Util::Path Config::projectCommandsFile() {
    MaxSDK::Util::Path sceneFolder = GetCOREInterface()->GetCurFilePath();
    return sceneFolder.IsEmpty() ? sceneFolder : sceneFolder.RemoveLeaf().Append(_T("Commands.txt"));
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool Config::load() {
    try {
        loadFile(sts::toMbString(mConfigFile.GetString()));
        return true;
    }
    catch (const std::exception & e) {
        LError << e.what();
        return false;
    }
}

bool Config::save() const {
    try {
        saveFile(sts::toMbString(mConfigFile.GetString()));
        return true;
    }
    catch (const std::exception & e) {
        LError << e.what();
        return false;
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
