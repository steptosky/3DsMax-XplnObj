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

#include <optional>
#include <functional>
#include "common/String.h"
#include "IDcView.h"
#include "common/Config.h"
#include "gup/Settings.h"

struct NotifyInfo;

namespace presenters {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details
 */
template<typename T>
class Dc : public sts::Single<Dc<T>>, public sts::signals::AutoDisconnect {
public:
    typedef IDcView<T> IView;
private:
    friend sts::Single<Dc<T>>;
    explicit Dc(IView * view);
    virtual ~Dc();
public:

    //-------------------------------------------------------------------------

    static MStr selectData(const MStr & current);

    //-------------------------------------------------------------------------

private:

    void onSettingsChanged(Settings *);

    void loadSimDatarefs();
    void loadProjectDatarefs();

    void unloadIf(const std::function<bool(const typename IView::Files::value_type & v)> & fn);

    void onSimDirChanged(Config &, const MaxSDK::Util::Path &, const MaxSDK::Util::Path &);

    void onViewReady();
    void onKeyChanged(const typename IView::FilePtr & file, const MStr & key);
    void onSearchKeyChanged(const MStr & data);
    void onCurrFileChanged(const MStr & name);
    static void onFileOpened(void * param, NotifyInfo *);
    static void onSystemReset(void * param, NotifyInfo *);
    static void onSystemNew(void * param, NotifyInfo *);

    static MaxSDK::Util::Path simDataFile(Config & config);
    static MaxSDK::Util::Path projectDataFile(Config & config);

    static std::optional<std::size_t> indexOfPath(const typename IView::Files & data, const MaxSDK::Util::Path & path);
    static std::optional<std::size_t> indexOfDisplayName(const typename IView::Files & data, const MStr & displayName);

    MStr mCurrKey;
    MStr mCurrSearchKey;
    IView * mView;
    typename IView::Files mDatarefs;
    MaxSDK::Util::Path mSelectedFile;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}

#include "Dc.inl.h"
