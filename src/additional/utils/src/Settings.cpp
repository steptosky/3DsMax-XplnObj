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

#include "additional/utils/Settings.h"
#include <cassert>
#include <sstream>
#include <limits>
#include <fstream>

#ifndef __STS_FUNC_NAME__
#	ifdef _MSC_VER
#		define __STS_FUNC_NAME__ __FUNCTION__
#	else
#		define __STS_FUNC_NAME__ __PRETTY_FUNCTION__
#	endif
#endif

#ifndef Debug
#	ifndef NDEBUG
#		define Debug(x) x
#	else
#		define Debug(x)
#	endif
#endif

#define noexcept _NOEXCEPT

#ifndef NDEBUG
#   define EXC_PRINT_FILE
#endif

#ifdef EXC_PRINT_FILE
#   define exctxt1(X) std::string(__STS_FUNC_NAME__).append(" : ").append(__FILE__).append(" : ").append(std::to_string(__LINE__)).append(" : ").append(X).c_str()
#   define exctxt2(X, Y) std::string(__STS_FUNC_NAME__).append(" : ").append(__FILE__).append(" : ").append(std::to_string(__LINE__)).append(" : ").append(X).append(Y).c_str()
#   define exctxt3(X, Y, Z) std::string(__STS_FUNC_NAME__).append(" : ").append(__FILE__).append(" : ").append(std::to_string(__LINE__)).append(" : ").append(X).append(Y).append(Z).c_str()
#else
#   define exctxt1(X) std::string(__STS_FUNC_NAME__).append(" ").append(X).c_str()
#   define exctxt2(X, Y) std::string(__STS_FUNC_NAME__).append(" ").append(X).append(Y).c_str()
#   define exctxt3(X, Y, Z) std::string(__STS_FUNC_NAME__).append(" ").append(X).append(Y).append(Z).c_str()
#endif

namespace sts {

	/**************************************************************************************************/
	/////////////////////////////////////////* Static area *////////////////////////////////////////////
	/**************************************************************************************************/

	inline bool validateStringForDigit(const std::string & inString) {
		size_t length = inString.length();
		if (length > 64)
			return false;
		for (auto charcode : inString) {
			if (charcode < '0' || charcode > '9') {
				if (charcode != '.' && charcode != ',' && charcode != '-')
					return false;
			}
		}
		return true;
	}

	inline void trimLeft(std::string & inOutStr, const std::string & inDelim = " \t\f\v\r\n") {
		inOutStr.erase(0, inOutStr.find_first_not_of(inDelim));
	}

	inline void trimRight(std::string & inOutStr, const std::string & inDelim = " \t\f\v\r\n") {
		inOutStr.erase(inOutStr.find_last_not_of(inDelim) + 1);
	}

	const char * Settings::defaultGroup = "DEFAULT";

	/**************************************************************************************************/
	////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
	/**************************************************************************************************/

	Settings::Settings()
		: mRoot(nullptr),
		mActualValues(nullptr) {
		init();
	}

	Settings::Settings(const Settings & copy)
		: mRoot(nullptr),
		mActualValues(nullptr) {
		*this = copy;
	}

	Settings::Settings(Settings && move) noexcept
		: mRoot(nullptr),
		mActualValues(nullptr) {
		*this = std::move(move);
	}

	Settings::~Settings() {
		delete mRoot;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Operators *////////////////////////////////////////////
	/**************************************************************************************************/

	Settings & Settings::operator =(const Settings & copy) {
		if (&copy == this)
			return *this;
		mActualTreePath = copy.mActualTreePath;
		mRoot = new Categories(*copy.mRoot);
		Categories::iterator item = mRoot->find(mActualTreePath);
		if (item == mRoot->end())
			item = mRoot->insert(mRoot->begin(), Categories::value_type(mActualTreePath, Values()));
		mActualValues = &item->second;
		arraysOperationsReset();
		return *this;
	}

	Settings & Settings::operator =(Settings && move) noexcept {
		if (&move == this)
			return *this;
		mActualTreePath = move.mActualTreePath;
		mRoot = move.mRoot;
		move.mRoot = nullptr;
		mActualValues = move.mActualValues;
		move.mActualValues = nullptr;
		arraysOperationsReset();
		return *this;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void Settings::init() {
		mActualTreePath.clear();
		if (!mRoot)
			mRoot = new Categories();
		mRoot->clear();
		mActualTreePath = defaultGroup;
		auto item = mRoot->find(mActualTreePath);
		if (item == mRoot->end())
			mRoot->insert(mRoot->begin(), Categories::value_type(mActualTreePath, Values()));
		mActualValues = &mRoot->begin()->second;
		arraysOperationsReset();
	}

	void Settings::clear() {
		init();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	const std::string & Settings::retrieveValue(const std::string & key) const {
		static std::string empty;
		if (mActualValues == nullptr) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Internal logic error: actual values is not set " << std::endl;)
			return empty;
		}

		if (mActualValues->empty())
			return empty;

		std::string currKey = key;
		// if we need write this key like key of array		
		if (mIsReadArrayOperationInProgress) {
			currKey = mCurrArrayName + std::string("/").append(std::to_string(mCurrArrayIndex)).append(std::string("/")).append(key);
		}

		Values::iterator item = mActualValues->find(currKey);
		if (item == mActualValues->end())
			return empty;
		return item->second;
	}

	std::string Settings::checkLineForGroup(const std::string & inLine) {
		size_t len = inLine.length();
		if (len < 3)
			return std::string();

		if (inLine.at(0) == '[' && inLine.at(len - 1) == ']') {
			return inLine.substr(1, len - 2);
		}
		return std::string();
	}

	void Settings::arraysOperationsReset() {
		mCurrArrayIndex = 0;
		mMaxArrayIndex = -1;
		mCurrArraySize = 0;
		mCurrArrayName.clear();
		mIsReadArrayOperationInProgress = false;
		mIsWriteArrayOperationInProgress = false;
		mIsNeedWriteArraySize = false;
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	std::string Settings::value(const std::string & key, const std::string & defaultVal) const {
		const std::string & val = retrieveValue(key);
		return val.empty() ? defaultVal : val;
	}

	float Settings::value(const std::string & key, const float defaultVal) const {
		const std::string & val = retrieveValue(key);
		if (!validateStringForDigit(val)) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Can't convert the string: " << val << " to float" << std::endl;)
			return defaultVal;
		}
		return val.empty() ? defaultVal : stof(val);
	}

	double Settings::value(const std::string & key, const double defaultVal) const {
		const std::string & val = retrieveValue(key);
		if (!validateStringForDigit(val)) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Can't convert the string: " << val << " to double" << std::endl;)
			return defaultVal;
		}
		return val.empty() ? defaultVal : stod(val);
	}

	int32_t Settings::value(const std::string & key, const int32_t defaultVal) const {
		const std::string & val = retrieveValue(key);
		if (!validateStringForDigit(val)) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Can't convert the string: " << val << " to int32_t" << std::endl;)
			return defaultVal;
		}
		return val.empty() ? defaultVal : stoi(val);
	}

	uint32_t Settings::value(const std::string & key, const uint32_t defaultVal) const {
		const std::string & val = retrieveValue(key);
		if (!validateStringForDigit(val)) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Can't convert the string: " << val << " to uint32_t" << std::endl;)
			return defaultVal;
		}
		return val.empty() ? defaultVal : stoul(val);
	}

	int64_t Settings::value(const std::string & key, const int64_t defaultVal) const {
		const std::string & val = retrieveValue(key);
		if (!validateStringForDigit(val)) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Can't convert the string: " << val << " to int64_t" << std::endl;)
			return defaultVal;
		}
		return val.empty() ? defaultVal : stoll(val);
	}

	uint64_t Settings::value(const std::string & key, const uint64_t defaultVal) const {
		const std::string & val = retrieveValue(key);
		if (!validateStringForDigit(val)) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Can't convert the string: " << val << " to uint64_t" << std::endl;)
			return defaultVal;
		}
		return val.empty() ? defaultVal : stoull(val);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void Settings::setValue(const std::string & key, const std::string & value) {
		if (mActualValues == nullptr) {
			Debug(std::cerr << __STS_FUNC_NAME__ << " Internal logic error: actual values is not set " << std::endl;)
			return;
		}

		std::string currKey = key;
		// if we need write this key like key of array		
		if (mIsWriteArrayOperationInProgress) {
			currKey = mCurrArrayName + std::string("/") + std::to_string(mCurrArrayIndex) + std::string("/") + key;
		}
		Values::iterator item = mActualValues->find(currKey);
		if (item == mActualValues->end()) {
			mActualValues->insert(Values::value_type(currKey, value));
		}
		else {
			item->second = value;
		}
	}

	void Settings::setValue(const std::string & key, const float value) {
		std::stringstream os;
		os.setf(std::ios::fixed);
		os.precision(std::numeric_limits<float>::digits);
		os << value;
		setValue(key, os.str());
	}

	void Settings::setValue(const std::string & key, const double value) {
		std::stringstream sream;
		sream.setf(std::ios::fixed);
		sream.precision(std::numeric_limits<double>::digits);
		sream << value;
		setValue(key, sream.str());
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	Settings::keyList Settings::keysList() const {
		keyList outKeys;
		if (mActualValues == nullptr) {
			std::cerr << __STS_FUNC_NAME__ << " Internal logic error: actual values is not set " << std::endl;
			return outKeys;
		}

		if (mRoot->empty())
			return outKeys;

		for (auto & val : *mActualValues)
			outKeys.push_back(val.first);
		return outKeys;
	}

	bool Settings::containsKey(const std::string & key) const {
		if (mActualValues == nullptr) {
			std::cerr << __STS_FUNC_NAME__ << " Internal logic error: actual values is not set " << std::endl;
			return false;
		}

		if (mActualValues->empty())
			return false;

		std::string currKey = key;
		// if we need write this key like key of array		
		if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
			currKey = mCurrArrayName + std::string("/") + std::to_string(mCurrArrayIndex) + std::string("/") + key;
		}
		return (mActualValues->find(currKey) != mActualValues->end());
	}

	bool Settings::remove(const std::string & key) {
		if (mActualValues == nullptr) {
			std::cerr << __STS_FUNC_NAME__ << " Internal logic error: actual values is not set " << std::endl;
			return false;
		}

		if (mActualValues->empty())
			return false;

		std::string currKey = key;
		// if we need write this key like key of array		
		if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
			currKey = mCurrArrayName + std::string("/") + std::to_string(mCurrArrayIndex) + std::string("/") + key;
		}
		return (mActualValues->erase(currKey) == 1);
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void Settings::beginGroup(const std::string & inGroupName) {
		if (inGroupName.empty())
			return;
		if (mActualTreePath == defaultGroup)
			mActualTreePath.clear();
		if (!mActualTreePath.empty())
			mActualTreePath.append("/");
		mActualTreePath.append(inGroupName);

		Categories::iterator item = mRoot->find(mActualTreePath);
		if (item == mRoot->end()) {
			item = mRoot->insert(mRoot->begin(), Categories::value_type(mActualTreePath, Values()));
		}
		mActualValues = &item->second;
	}

	void Settings::endGroup() {
		endArray();

		if (mActualTreePath.empty())
			return;

		size_t pos = mActualTreePath.find_last_of('/');
		if (pos == std::string::npos) {
			Categories::iterator def = mRoot->find(defaultGroup);
			if (def == mRoot->end()) {
				std::cerr << __STS_FUNC_NAME__ << " Internal error." << std::endl;
				return;
			}
			mActualTreePath = defaultGroup;
			mActualValues = &def->second;
			return;
		}

		mActualTreePath.resize(pos);
		Categories::iterator item = mRoot->find(mActualTreePath);
		if (item == mRoot->end()) {
			std::cerr << __STS_FUNC_NAME__ << " Internal error." << std::endl;
			return;
		}
		mActualValues = &item->second;
	}

	std::string Settings::currentGroup(const bool fullTreePath) const {
		if (fullTreePath == false) {
			auto pos = mActualTreePath.find_last_of("/");
			if (pos == std::string::npos)
				return mActualTreePath;

			std::string group = mActualTreePath.substr(pos);
			if (group.length() < 2) {
				std::cerr << __STS_FUNC_NAME__ << " Internal error with group's tail" << std::endl;
				return std::string();
			}
			// delete "/"
			return group.substr(1);
		}
		return mActualTreePath;
	}

	bool Settings::hasGroup(const std::string & groupName, const bool fullTreePath) const {
		if (fullTreePath)
			return (mRoot->find(groupName) != mRoot->end());

		if (mActualTreePath == defaultGroup)
			return false;

		return (mRoot->find(mActualTreePath + "/" + groupName) != mRoot->end());
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	uint32_t Settings::beginReadArray(const std::string & name) {
		if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
			std::cerr << __STS_FUNC_NAME__ << " You must call endArray() before." << std::endl;
			return 0;
		}
		mCurrArrayName = name;
		uint32_t size = value(mCurrArrayName + "/size", 0);
		mIsReadArrayOperationInProgress = true;
		return size;
	}

	void Settings::beginWriteArray(const std::string & name, int32_t size) {
		if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
			std::cerr << __STS_FUNC_NAME__ << " You must call endArray() before." << std::endl;
			return;
		}
		mCurrArrayName = name;
		if (size > 0) {// if we used in parameter
			setValue(mCurrArrayName + "/size", size);
		}
		else {// if we auto calculating and write size later
			mIsNeedWriteArraySize = true;
		}
		mIsWriteArrayOperationInProgress = true;
	}

	void Settings::arrayIndex(const uint32_t index) {
		mCurrArrayIndex = index;
		// Do not delete this type reinterpret (about int32), We need compare between signed int!
		if (static_cast<int32_t>(mCurrArrayIndex) > mMaxArrayIndex)
			mMaxArrayIndex = mCurrArrayIndex;
	}

	void Settings::endArray() {
		// calculating and writing size of wrote array
		if (mIsWriteArrayOperationInProgress && mIsNeedWriteArraySize && mMaxArrayIndex > 0) {
			mIsWriteArrayOperationInProgress = false;
			setValue(mCurrArrayName + "/size", mMaxArrayIndex + 1);
		}
		arraysOperationsReset();
	}

	/**************************************************************************************************/
	///////////////////////////////////////////* Functions *////////////////////////////////////////////
	/**************************************************************************************************/

	void Settings::loadFile(const std::string & fileName) {
		if (fileName.size() < 2) {
			throw std::runtime_error(exctxt1("Incorrect file path"));
		}

		clear();

		std::ifstream file(fileName.c_str(), std::ios_base::in);
		if (!file.is_open()) {
			throw std::runtime_error(exctxt2("Can't open file: ", fileName));
		}

		try {
			fromStream(file);
		}
		catch (const std::exception &) {
			file.close();
			throw;
		}
		file.close();
	}

	void Settings::saveFile(const std::string & fileName) const {
		if (fileName.size() < 2) {
			throw std::runtime_error(exctxt1("Incorrect file path"));
		}

		std::ofstream file(fileName.c_str());
		if (!file.is_open()) {
			throw std::runtime_error(exctxt2("Can't open file: ", fileName));
		}

		toStream(file);
		file.close();
	}

	void Settings::fromString(const std::string & string) {
		std::stringstream stream(string);
		fromStream(stream);
	}

	std::string Settings::toString() const {
		std::stringstream stream;
		toStream(stream);
		return stream.str();
	}

	/**************************************************************************************************/
	//////////////////////////////////////////* Functions */////////////////////////////////////////////
	/**************************************************************************************************/

	void Settings::fromStream(std::istream & stream) {
		Values * actualVals = nullptr;
		while (stream.good()) {
			std::string line;
			getline(stream, line);
			trimLeft(line);
			trimRight(line);
			// skip comments, empty lines
			if (line.length() == 0 || line.at(0) == '#' || line.at(0) == '@' || line.at(0) == '/')
				continue;

			std::string group = checkLineForGroup(line);
			if (!group.empty()) { // if it is group?
				Categories::iterator item = mRoot->find(group);
				if (item == mRoot->end())
					item = mRoot->insert(mRoot->begin(), Categories::value_type(group, Values()));
				actualVals = &item->second;
				continue;
			}
			// else it parameter line
			size_t pos = line.find_first_of("=");
			if (pos != std::string::npos) {

				std::string key = line.substr(0, pos);
				trimRight(key);
				if (key.empty()) {
					Debug(std::cerr << __STS_FUNC_NAME__ << " Incorrect key" << std::endl;)
					continue;
				}

				std::string val = line.substr(pos + 1, line.length() - (pos + 1));
				trimLeft(val);

				actualVals->insert(Values::value_type(key, val));
			}
		}
	}

	void Settings::toStream(std::ostream & stream) const {
		for (auto & category : *mRoot) {
			stream << '[' << category.first << ']' << std::endl;
			const Values & values = category.second;
			for (auto & val : values) {
				stream << "    " << val.first << '=' << val.second << std::endl;
			}
			stream << std::endl;
		}
	}

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/
}
