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

#include "SemVersion.h"
#include <sstream>
#include <regex>

/**************************************************************************************************/
/////////////////////////////////////////* Static area *////////////////////////////////////////////
/**************************************************************************************************/

// regex to capture semantic version
// the regex matches case insensitive
// (1) major version 0 or unlimited number
// (2) . minor version (0 or unlimited number)
// (3) . patch version (0 or unlimited number)
// (4) optional pre-release following a dash consisting of a alphanumeric letters
//     and hyphens using a non-capture subclause to exclude the dash from the 
//     pre-release string
// (5) optional build following a plus consisting of alphanumeric letters and
//     hyphens using a non-capture subclause to exclude the plus from the build string
static auto __semverRegex__ = std::regex("^(0|[1-9][0-9]*)" // (1)
										"\\.(0|[1-9][0-9]*)" // (2)
										"\\.(0|[1-9][0-9]*)" // (3)
										"(?:\\-([0-9a-z-]+[\\.0-9a-z-]*))?" // (4)
										"(?:\\+([0-9a-z-]+[\\.0-9a-z-]*))?" // (5)
										,
										std::regex_constants::ECMAScript |
										std::regex_constants::icase);

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

SemVersion::SemVersion() {
	clear();
}

SemVersion::SemVersion(uint major, uint minor, uint patch) {
	this->set(major, minor, patch);
}

SemVersion::SemVersion(uint major, uint minor, uint patch, const char * pre_release, const char * build) {
	this->set(major, minor, patch, pre_release, build);
}

SemVersion::SemVersion(uint major, uint minor, uint patch, const std::string & pre_release, const std::string & build) {
	this->set(major, minor, patch, pre_release, build);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool SemVersion::compare(const SemVersion & other, bool preRelease, bool build) const {
	if (this->operator!=(other)) {
		return false;
	}
	if (preRelease && mPreRelease != other.mPreRelease) {
		return false;
	}
	if (build && mBuild != other.mBuild) {
		return false;
	}
	return true;
}

bool SemVersion::operator==(const SemVersion & other) const {
	return mMajor == other.mMajor &&
			mMinor == other.mMinor &&
			mPatch == other.mPatch;
}

bool SemVersion::operator>(const SemVersion & other) const {
	if (this->mMajor > other.mMajor) {
		return true;
	}
	if (this->mMajor < other.mMajor) {
		return false;
	}

	if (this->mMinor > other.mMinor) {
		return true;
	}
	if (this->mMinor < other.mMinor) {
		return false;
	}

	if (this->mPatch > other.mPatch) {
		return true;
	}
	if (this->mPatch < other.mPatch) {
		return false;
	}
	return false;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void SemVersion::set(uint major, uint minor, uint patch) {
	mMajor = major;
	mMinor = minor;
	mPatch = patch;
	mPreRelease.clear();
	mBuild.clear();
}

void SemVersion::set(uint major, uint minor, uint patch, const char * preRelease, const char * build) {
	set(major, minor, patch);
	if (preRelease) {
		mPreRelease = std::string(preRelease);
	}
	else {
		mPreRelease.clear();
	}
	if (build) {
		mBuild = std::string(build);
	}
	else {
		mBuild.clear();
	}
}

void SemVersion::set(uint major, uint minor, uint patch, const std::string & preRelease, const std::string & build) {
	set(major, minor, patch);
	mPreRelease = preRelease;
	mBuild = build;
}

void SemVersion::clear() {
	mMajor = 0;
	mMinor = 0;
	mPatch = 0;
	mPreRelease.clear();
	mBuild.clear();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool SemVersion::parse(const char * version) {
	if (!version) {
		return false;
	}
	return parse(std::string(version));
}

bool SemVersion::parse(const std::string & version) {
	auto piecesMatch = std::smatch();
	if (std::regex_match(version, piecesMatch, __semverRegex__)) {
		set(std::atoi(piecesMatch[1].str().c_str()),
			std::atoi(piecesMatch[2].str().c_str()),
			std::atoi(piecesMatch[3].str().c_str()),
			piecesMatch[4],
			piecesMatch[5]);
		return true;
	}
	return false;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::string SemVersion::toString() const {
	std::ostringstream stream;
	stream << mMajor << '.' << mMinor << '.' << mPatch;
	if (!mPreRelease.empty()) {
		stream << '-' << mPreRelease;
	}
	if (!mBuild.empty()) {
		stream << '+' << mBuild;
	}
	return stream.str();
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
