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

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class SemVersion {

	typedef uint32_t uint;

public:

	SemVersion();
	SemVersion(uint, uint, uint);
	SemVersion(uint, uint, uint, const char *, const char *);
	SemVersion(uint, uint, uint, const std::string &, const std::string &);

	//-------------------------------------------------------------------------

	/*
	 * \return True if the versions are equaled otherwise false.
	 */
	bool compare(const SemVersion & other, bool preRelease = false, bool build = false) const;

	/*! 
	 * \details It compares only major minor and patch.
	 * \see SemVersion::compare
	 */
	bool operator==(const SemVersion & other) const;

	/*!
	 * \details It compares only major minor and patch.
	 * \see SemVersion::compare
	 */
	bool operator!=(const SemVersion & other) const { return !this->operator==(other); }

	bool operator>(const SemVersion &) const;

	//-------------------------------------------------------------------------

	void set(uint, uint, uint);
	void set(uint, uint, uint, const char *, const char *);
	void set(uint, uint, uint, const std::string &, const std::string &);

	bool parse(const char *);
	bool parse(const std::string &);

	//-------------------------------------------------------------------------

	std::string toString() const;
	void clear();

	//-------------------------------------------------------------------------

	uint mMajor;
	uint mMinor;
	uint mPatch;
	std::string mPreRelease;
	std::string mBuild;

	//-------------------------------------------------------------------------
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
