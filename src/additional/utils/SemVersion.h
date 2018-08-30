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

#include <string>
#include <cstdint>
#include <tuple>
#include "Export.h"

namespace sts {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Represents Semantic Versioning
 * \details http://semver.org/
 */
class SemVersion {
    typedef uint32_t uint;
    auto versionsTuple() const { return std::make_tuple(major, minor, patch); }
public:

    //---------------------------------------------------------------
    // @{

    uint major;
    uint minor;
    uint patch;
    std::string preRelease;
    std::string build;

    // @{
    //---------------------------------------------------------------
    // @{

    UtilsExp SemVersion();
    UtilsExp SemVersion(const uint major, const uint minor, const uint patch);
    UtilsExp SemVersion(const uint major, const uint minor, const uint patch,
                        const char * preRelease, const char * build);
    UtilsExp SemVersion(const uint major, const uint minor, const uint patch,
                        const std::string & preRelease, const std::string & build);

    // @}
    //---------------------------------------------------------------
    // @{

    /*!
     * \param [in] other
     * \param [in] preRelease compare pre-release part.
     * \param [in] build compare build part.
     * \return True if the versions are equaled otherwise false.
     */
    UtilsExp bool compare(const SemVersion & other, const bool preRelease = false, const bool build = false) const;

    /*!
     * \note It compares only major minor and patch parts.
     * \see \link SemVersion::compare \endlink 
     */
    bool operator==(const SemVersion & other) const { return versionsTuple() == other.versionsTuple(); }

    /*!
     * \note It compares only major minor and patch parts.
     * \see \link SemVersion::compare \endlink 
     */
    bool operator!=(const SemVersion & other) const { return versionsTuple() != other.versionsTuple(); }

    /*!
     * \note It compares only major minor and patch parts.
     * \see \link SemVersion::compare \endlink 
     */
    bool operator>(const SemVersion & other) const { return versionsTuple() > other.versionsTuple(); }

    /*!
     * \note It compares only major minor and patch parts.
     * \see \link SemVersion::compare \endlink 
     */
    bool operator<(const SemVersion & other) const { return versionsTuple() < other.versionsTuple(); }

    /*!
     * \note It compares only major minor and patch parts.
     * \see \link SemVersion::compare \endlink 
     */
    bool operator>=(const SemVersion & other) const { return versionsTuple() >= other.versionsTuple(); }

    /*!
     * \note It compares only major minor and patch parts.
     * \see \link SemVersion::compare \endlink 
     */
    bool operator<=(const SemVersion & other) const { return versionsTuple() <= other.versionsTuple(); }

    // @}
    //---------------------------------------------------------------
    // @{

    UtilsExp void set(const uint major, const uint minor, const uint patch);
    UtilsExp void set(const uint major, const uint minor, const uint patch,
                      const char * preRelease, const char * build);
    UtilsExp void set(const uint major, const uint minor, const uint patch,
                      const std::string & preRelease, const std::string & build);

    // @}
    //---------------------------------------------------------------
    // @{

    UtilsExp bool parse(const char *);
    UtilsExp bool parse(const std::string &);

    // @}
    //---------------------------------------------------------------
    // @{

    UtilsExp std::string toString(const bool preRelease = true, const bool build = true) const;
    UtilsExp void clear();

    // @}
    //---------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
