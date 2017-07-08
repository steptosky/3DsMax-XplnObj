/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

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

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#include "additional/utils/SemVersion.h"
#include <sstream>
#include <regex>

namespace sts {

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

	SemVersion::SemVersion(const uint major, const uint minor, const uint patch) {
		set(major, minor, patch);
	}

	SemVersion::SemVersion(const uint major, const uint minor, const uint patch,
							const char * preRelease, const char * build) {
		set(major, minor, patch, preRelease, build);
	}

	SemVersion::SemVersion(const uint major, const uint minor, const uint patch,
							const std::string & preRelease, const std::string & build) {
		set(major, minor, patch, preRelease, build);
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	bool SemVersion::compare(const SemVersion & inOther, const bool inPreRelease, const bool inBuild) const {
		if (this->operator!=(inOther)) {
			return false;
		}
		if (inPreRelease && preRelease != inOther.preRelease) {
			return false;
		}
		if (inBuild && build != inOther.build) {
			return false;
		}
		return true;
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void SemVersion::set(const uint inMajor, const uint inMinor, const uint inPatch) {
		major = inMajor;
		minor = inMinor;
		patch = inPatch;
		preRelease.clear();
		build.clear();
	}

	void SemVersion::set(const uint inMajor, const uint inMinor, const uint inPatch,
						const char * inPreRelease, const char * inBuild) {
		set(inMajor, inMinor, inPatch);
		if (inPreRelease) {
			preRelease.append(inPreRelease);
		}
		if (inBuild) {
			build.append(inBuild);
		}
	}

	void SemVersion::set(const uint inMajor, const uint inMinor, const uint inPatch,
						const std::string & inPreRelease, const std::string & inBuild) {
		set(inMajor, inMinor, inPatch);
		preRelease = inPreRelease;
		build = inBuild;
	}

	void SemVersion::clear() {
		major = 0;
		minor = 0;
		patch = 0;
		preRelease.clear();
		build.clear();
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	bool SemVersion::parse(const char * version) {
		if (!version) {
			clear();
			return false;
		}
		return parse(std::string(version));
	}

	bool SemVersion::parse(const std::string & version) {
		auto piecesMatch = std::smatch();
		if (regex_match(version, piecesMatch, __semverRegex__)) {
			set(atoi(piecesMatch[1].str().c_str()),
				atoi(piecesMatch[2].str().c_str()),
				atoi(piecesMatch[3].str().c_str()),
				piecesMatch[4],
				piecesMatch[5]);
			return true;
		}
		clear();
		return false;
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	std::string SemVersion::toString(const bool inPreRelease, const bool inBuild) const {
		std::ostringstream stream;
		stream << major << '.' << minor << '.' << patch;
		if (inPreRelease && !preRelease.empty()) {
			stream << '-' << preRelease;
		}
		if (inBuild && !build.empty()) {
			stream << '+' << build;
		}
		return stream.str();
	}

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/
}
