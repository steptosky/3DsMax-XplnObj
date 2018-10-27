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

#pragma warning(push, 0)
#include <Path.h>
#pragma warning(pop)

#include <sts/signals/Signal.h>
#include "additional/utils/Single.h"
#include "additional/utils/Settings.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Access to the external config (ini) file
 */
class Config : public sts::Settings, public sts::Single<Config> {
public:

    Config();
    virtual ~Config();

    //-------------------------------------------------------------------------

    /*!
     * \details Is emit when the sim folder is changed.
     * \details [this, old, new]
     */
    sts::signals::Signal<Config&, const MaxSDK::Util::Path&, const MaxSDK::Util::Path&> sigSimDirChanged;

    //-------------------------------------------------------------------------

    void setSimDir(const MaxSDK::Util::Path & dir);

    MaxSDK::Util::Path simDir();
    MaxSDK::Util::Path simDatarefsFile();
    MaxSDK::Util::Path simCommandsFile();

    MaxSDK::Util::Path projectDatarefsFile();
    MaxSDK::Util::Path projectCommandsFile();

    //-------------------------------------------------------------------------

    bool load();
    bool save() const;

    //-------------------------------------------------------------------------

private:

    MaxSDK::Util::Path mSimDir;
    MaxSDK::Util::Path mConfigFile;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
