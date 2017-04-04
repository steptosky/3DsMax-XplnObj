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
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

namespace internal {

	inline bool _validateStringForDigit(const std::string & inString) {
		size_t length = inString.length();
		if (length > 128)
			return false;
		for (auto charcode : inString) {
			if (charcode < '0' || charcode > '9') {
				if (charcode != '.' && charcode != ',' && charcode != '-')
					return false;
			}
		}
		return true;
	}

	inline void _trimLeft(std::string & inOutStr, const std::string & inDelim = " \t\f\v\r\n") {
		inOutStr.erase(0, inOutStr.find_first_not_of(inDelim));
	}

	inline void _trimRight(std::string & inOutStr, const std::string & inDelim = " \t\f\v\r\n") {
		inOutStr.erase(inOutStr.find_last_not_of(inDelim) + 1);
	}

}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

inline const unsigned char Settings::ioVersion() const {
	return 0;
}

inline const unsigned Settings::ioIdSize() const {
	return 5;
}

inline const char * Settings::ioId() const {
	return "@STNG";
}

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Constructor init format.
 * \param [in] inFormat
 */
inline Settings::Settings(eFormats inFormat)
	: mFormat(inFormat),
	mRoot(nullptr),
	mActualValues(nullptr) {
	_init();
}

/*!
 * \details Constructor copy.
 * \param [in] inCopy
 */
inline Settings::Settings(const Settings & inCopy)
	: mRoot(nullptr),
	mActualValues(nullptr) {
	*this = inCopy;
}

/*!
 * \details Constructor move.
 * \param [in] inMove
 */
inline Settings::Settings(Settings && inMove)
	: mRoot(nullptr),
	mActualValues(nullptr) {
	*this = std::move(inMove);
}

/*!
 * \details Destructor
 */
inline Settings::~Settings() {
	delete mRoot;
}

/**************************************************************************************************/
///////////////////////////////////////////* Operators *////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Operator copy.
 * \param [in] inCopy
 */
inline Settings & Settings::operator =(const Settings & inCopy) {
	if (&inCopy == this)
		return *this;
	mFormat = inCopy.mFormat;
	mActualTreePath = inCopy.mActualTreePath;
	mRoot = new Categories(*inCopy.mRoot);
	Categories::iterator item = mRoot->find(mActualTreePath);
	if (item == mRoot->end())
		item = mRoot->insert(mRoot->begin(), Categories::value_type(mActualTreePath, Values()));
	mActualValues = &item->second;

	_arraysOperationsReset();

	return *this;
}

/*!
 * \details Operator move.
 * \param [in] inMove
 */
inline Settings & Settings::operator =(Settings && inMove) {
	if (&inMove == this)
		return *this;
	mFormat = inMove.mFormat;
	mActualTreePath = inMove.mActualTreePath;

	mRoot = inMove.mRoot;
	inMove.mRoot = nullptr;

	mActualValues = inMove.mActualValues;
	inMove.mActualValues = nullptr;

	_arraysOperationsReset();
	return *this;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

inline void Settings::_init() {
	mActualTreePath.clear();
	if (!mRoot)
		mRoot = new Categories();
	mRoot->clear();
	mActualTreePath = "DEFAULT";
	auto item = mRoot->find(mActualTreePath);
	if (item == mRoot->end())
		mRoot->insert(mRoot->begin(), Categories::value_type(mActualTreePath, Values()));
	mActualValues = &mRoot->begin()->second;
	_arraysOperationsReset();
}

inline void Settings::_arraysOperationsReset() {
	mCurrArrayIndex = 0;
	mMaxArrayIndex = -1;
	mCurrArraySize = 0;
	mCurrArrayName.clear();
	mIsReadArrayOperationInProgress = false;
	mIsWriteArrayOperationInProgress = false;
	mIsNeedWriteArraySize = false;
}

/*!
 * \details Deletes all data from the config.
 */
inline void Settings::clear() {
	_init();
}

/*!
 * \details Gets actual format for the config.
 * \return actual config format.
 */
inline Settings::eFormats Settings::format() const {
	return mFormat;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

inline const std::string & Settings::_value(const std::string & inKey) const {
	static std::string empty;
	if (mActualValues == nullptr) {
		std::cerr << __FUNCTION__ << " Internal logic error: actual values is not set " << std::endl;
		return empty;
	}

	if (mActualValues->empty())
		return empty;

	std::string key = inKey;
	// if we need write this key like key of array		
	if (mIsReadArrayOperationInProgress) {
		key = mCurrArrayName + std::string("/") + std::to_string(mCurrArrayIndex) + std::string("/") + inKey;
	}

	Values::iterator item = mActualValues->find(key);
	if (item == mActualValues->end())
		return empty;
	return item->second;
}

//----------------------------------------------------

inline std::string Settings::value(const std::string & inKey, const std::string & inDefault) const {
	const std::string & val = _value(inKey);
	return (val.empty()) ? inDefault : val;
}

inline float Settings::value(const std::string & inKey, float inDefault) const {
	const std::string & val = _value(inKey);
	switch (mFormat) {
		case bin: {
			if (val.size() != sizeof(float)) {
				return inDefault;
			}
			else {
				return *(reinterpret_cast<const float*>(val.data()));
			}
		}
		case txt: {
			if (!internal::_validateStringForDigit(val)) {
				std::cerr << __FUNCTION__ << " Can't convert the string: " << val << " to float" << std::endl;
				return inDefault;
			}
			return val.empty() ? inDefault : std::stof(val);
		}
	}

	return inDefault;
}

inline double Settings::value(const std::string & inKey, double inDefault) const {
	const std::string & val = _value(inKey);
	switch (mFormat) {
		case bin: {
			if (val.size() != sizeof(double)) {
				return inDefault;
			}
			else {
				return *(reinterpret_cast<const double*>(val.data()));
			}
		}
		case txt: {
			if (!internal::_validateStringForDigit(val)) {
				std::cerr << __FUNCTION__ << " Can't convert the string: " << val << " to double" << std::endl;
				return inDefault;
			}
			return val.empty() ? inDefault : std::stod(val);
		}
	}

	return inDefault;
}

inline int32_t Settings::value(const std::string & inKey, int32_t inDefault) const {
	const std::string & val = _value(inKey);
	switch (mFormat) {
		case bin: {
			if (val.size() != sizeof(int32_t)) {
				return inDefault;
			}
			else {
				return *(reinterpret_cast<const int32_t*>(val.data()));
			}
		}
		case txt: {
			if (!internal::_validateStringForDigit(val)) {
				std::cerr << __FUNCTION__ << " Can't convert the string: " << val << " to int32_t" << std::endl;
				return inDefault;
			}
			return val.empty() ? inDefault : std::stoi(val);
		}
	}

	return inDefault;
}

inline uint32_t Settings::value(const std::string & inKey, uint32_t inDefault) const {
	const std::string & val = _value(inKey);
	switch (mFormat) {
		case bin: {
			if (val.size() != sizeof(uint32_t)) {
				return inDefault;
			}
			else {
				return *(reinterpret_cast<const uint32_t*>(val.data()));
			}
		}
		case txt: {
			if (!internal::_validateStringForDigit(val)) {
				std::cerr << __FUNCTION__ << " Can't convert the string: " << val << " to uint32_t" << std::endl;
				return inDefault;
			}
			return val.empty() ? inDefault : std::stoul(val);
		}
	}

	return inDefault;
}

inline int64_t Settings::value(const std::string & inKey, int64_t inDefault) const {
	const std::string & val = _value(inKey);
	switch (mFormat) {
		case bin: {
			if (val.size() != sizeof(int64_t)) {
				return inDefault;
			}
			else {
				return *(reinterpret_cast<const int64_t*>(val.data()));
			}
		}
		case txt: {
			if (!internal::_validateStringForDigit(val)) {
				std::cerr << __FUNCTION__ << " Can't convert the string: " << val << " to int64_t" << std::endl;
				return inDefault;
			}
			return val.empty() ? inDefault : std::stoll(val);
		}
	}

	return inDefault;
}

inline uint64_t Settings::value(const std::string & inKey, uint64_t inDefault) const {
	const std::string & val = _value(inKey);
	switch (mFormat) {
		case bin: {
			if (val.size() != sizeof(uint64_t)) {
				return inDefault;
			}
			else {
				return *(reinterpret_cast<const uint64_t*>(val.data()));
			}
		}
		case txt: {
			if (!internal::_validateStringForDigit(val)) {
				std::cerr << __FUNCTION__ << " Can't convert the string: " << val << " to uint64_t" << std::endl;
				return inDefault;
			}
			return val.empty() ? inDefault : std::stoull(val);
		}
	}

	return inDefault;
}

//----------------------------------------------------

inline std::string Settings::value(const std::string & inKey, const char * inDefault) const {
	return value(inKey, std::string(inDefault));
}

inline bool Settings::value(const std::string & inKey, bool inDefault) const {
	return (value(inKey, static_cast<uint8_t>(inDefault)) != 0);
}

inline int8_t Settings::value(const std::string & inKey, int8_t inDefault) const {
	return static_cast<int8_t>(value(inKey, static_cast<int32_t>(inDefault)));
}

inline uint8_t Settings::value(const std::string & inKey, uint8_t inDefault) const {
	return static_cast<uint8_t>(value(inKey, static_cast<uint32_t>(inDefault)));
}

inline int16_t Settings::value(const std::string & inKey, int16_t inDefault) const {
	return static_cast<int16_t>(value(inKey, static_cast<int32_t>(inDefault)));
}

inline uint16_t Settings::value(const std::string & inKey, uint16_t inDefault) const {
	return static_cast<uint16_t>(value(inKey, static_cast<uint32_t>(inDefault)));
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

template<typename T>
inline void Settings::_setIntVlalue(const std::string & inKey, const T & inValue) {
	switch (mFormat) {
		case txt: setValue(inKey, std::to_string(inValue));
			break;
		case bin: setValue(inKey, std::string(reinterpret_cast<const char*>(&inValue), sizeof(inValue)));
			break;
	}
}

inline void Settings::setValue(const std::string & inKey, const std::string & inValue) {
	if (mActualValues == nullptr) {
		std::cerr << __FUNCTION__ << " Internal logic error: actual values is not set " << std::endl;
		return;
	}

	std::string key = inKey;
	// if we need write this key like key of array		
	if (mIsWriteArrayOperationInProgress) {
		key = mCurrArrayName + std::string("/") + std::to_string(mCurrArrayIndex) + std::string("/") + inKey;
	}
	Values::iterator item = mActualValues->find(key);
	if (item == mActualValues->end()) {
		mActualValues->insert(Values::value_type(key, inValue));
	}
	else {
		item->second = inValue;
	}
}

inline void Settings::setValue(const std::string & inKey, float inValue) {
	switch (mFormat) {
		case bin: setValue(inKey, std::string(reinterpret_cast<const char*>(&inValue), sizeof(inValue)));
			break;
		case txt: {
			std::stringstream os;
			os.setf(std::ios::fixed);
			os.precision(FLT_DIG);
			os << inValue;
			setValue(inKey, std::string(os.str()));
		}
	}
}

inline void Settings::setValue(const std::string & inKey, double inValue) {
	switch (mFormat) {
		case bin: setValue(inKey, std::string(reinterpret_cast<const char*>(&inValue), sizeof(inValue)));
			break;
		case txt: {
			std::stringstream os;
			os.setf(std::ios::fixed);
			os.precision(DBL_DIG);
			os << inValue;
			setValue(inKey, std::string(os.str()));
		}
	}
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

inline void Settings::setValue(const std::string & inKey, const char * inValue) {
	return setValue(inKey, std::string(inValue));
}

inline void Settings::setValue(const std::string & inKey, bool inValue) {
	return setValue(inKey, static_cast<uint8_t>(inValue));
}

//----------------------------------------------------

inline void Settings::setValue(const std::string & inKey, int8_t inValue) {
	_setIntVlalue<int32_t>(inKey, inValue);
}

inline void Settings::setValue(const std::string & inKey, uint8_t inValue) {
	_setIntVlalue<uint32_t>(inKey, inValue);
}

inline void Settings::setValue(const std::string & inKey, int16_t inValue) {
	_setIntVlalue<int32_t>(inKey, inValue);
}

inline void Settings::setValue(const std::string & inKey, uint16_t inValue) {
	_setIntVlalue<uint32_t>(inKey, inValue);
}

inline void Settings::setValue(const std::string & inKey, int32_t inValue) {
	_setIntVlalue<int32_t>(inKey, inValue);
}

inline void Settings::setValue(const std::string & inKey, uint32_t inValue) {
	_setIntVlalue<uint32_t>(inKey, inValue);
}

inline void Settings::setValue(const std::string & inKey, int64_t inValue) {
	_setIntVlalue<int64_t>(inKey, inValue);
}

inline void Settings::setValue(const std::string & inKey, uint64_t inValue) {
	_setIntVlalue<uint64_t>(inKey, inValue);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

inline std::string Settings::_checkLineForGroup(const std::string & inLine) const {
	size_t len = inLine.length();
	if (len < 3)
		return std::string();

	if (inLine.at(0) == '[' && inLine.at(len - 1) == ']') {
		return inLine.substr(1, len - 2);
	}
	return std::string();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Loads config data from specified file.
 * \param [in] inFileName full path with file name
 * \return True if successful otherwise false
 */
inline bool Settings::load(const std::string & inFileName) {
	switch (mFormat) {
		case txt: return _loadTxt(inFileName);
		case bin: return _loadBin(inFileName);
		default: return false;
	}
}

/*!
 * \details Saves config data to specified file.
 * \param [in] inFileName full path with file name
 * \return  True if successful otherwise false
 */
inline bool Settings::save(const std::string & inFileName) const {
	switch (mFormat) {
		case txt: return _saveTxt(inFileName);
		case bin: return _saveBin(inFileName);
		default: return false;
	}
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

inline bool Settings::_loadTxt(const std::string & inFileName) {
	if (inFileName.size() < 2) {
		std::cerr << __FUNCTION__ << " Incorrect file path." << std::endl;
		return false;
	}

	clear();

	std::ifstream file(inFileName.c_str(), std::ios_base::in);
	if (!file.is_open()) {
		std::cerr << __FUNCTION__ << " Can't open file: <" << inFileName << ">" << std::endl;
		return false;
	}

	Values * actualVals = nullptr;
	while (file.good()) {
		std::string line;
		std::getline(file, line);
		internal::_trimLeft(line);
		internal::_trimRight(line);
		// skip comments, empty lines
		if (line.length() == 0 || line.at(0) == '#' || line.at(0) == '@' || line.at(0) == '/')
			continue;

		std::string group = _checkLineForGroup(line);
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
			internal::_trimRight(key);
			if (key.empty()) {
				std::cerr << __FUNCTION__ << " Incorrect key" << std::endl;
				continue;
			}

			std::string val = line.substr(pos + 1, line.length() - (pos + 1));
			internal::_trimLeft(val);

			actualVals->insert(Values::value_type(key, val));
		}
	}

	file.close();
	return true;
}

inline bool Settings::_saveTxt(const std::string & inFileName) const {
	if (inFileName.size() < 2) {
		std::cerr << __FUNCTION__ << " Incorrect file path." << std::endl;
		return false;
	}

	std::ofstream file(inFileName.c_str());
	if (!file.is_open()) {
		std::cerr << __FUNCTION__ << " Can't open file: <" << inFileName << ">" << std::endl;
		return false;
	}

	for (auto & category : *mRoot) {
		file << '[' << category.first << ']' << std::endl;

		const Values & values = category.second;
		for (auto & val : values) {
			file << "    " << val.first << '=' << val.second << std::endl;
		}
		file << std::endl;
	}

	file.close();
	return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

inline bool Settings::_loadBin(const std::string & inFileName) {
	if (inFileName.size() < 2) {
		std::cerr << __FUNCTION__ << " Incorrect file path." << std::endl;
		return false;
	}

	clear();

	std::ifstream file(inFileName.c_str(), std::ios_base::in);
	if (!file.is_open()) {
		std::cerr << __FUNCTION__ << " Can't open file: <" << inFileName << ">" << std::endl;
		return false;
	}

	bool res = load(file);

	file.close();
	return res;
}

inline bool Settings::_saveBin(const std::string & inFileName) const {
	if (inFileName.size() < 2) {
		std::cerr << __FUNCTION__ << " Incorrect file path." << std::endl;
		return false;
	}

	std::ofstream file(inFileName.c_str());
	if (!file.is_open()) {
		std::cerr << __FUNCTION__ << " Can't open file: <" << inFileName << ">" << std::endl;
		return false;
	}

	bool res = save(file);

	file.close();
	return res;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

inline std::istream & operator >>(std::istream & inStream, Settings & outSettings) {
	if (!outSettings.load(inStream))
		throw std::runtime_error(std::string(__FUNCTION__) + " Can't load data.");
	return inStream;
}

inline std::ostream & operator <<(std::ostream & outStream, const Settings & inSettings) {
	inSettings.save(outStream);
	return outStream;
}

/*!
 * \details Loads config data from specified stream.
 * \param [in] inStream
 * \return True if successful otherwise false.
 */
inline bool Settings::load(std::istream & inStream) {
	clear();
	std::istream::streampos startPos = inStream.tellg();
	DataStreamI stream(&inStream);
	try {
		stream.readAndSetFormatVersion();
		char * id = new char[ioIdSize()];
		stream.readRawData(id, static_cast<uint64_t>(ioIdSize()));
		std::string strId(id, ioIdSize());
		delete[] id;
		if (strId != ioId()) {
			std::cerr << "Error: "
					<< __FUNCTION__ << " Incorrect data \"id\"."
					<< " Input: " << strId.c_str()
					<< " must be \"" << ioId() << "\"." << std::endl;
			inStream.seekg(startPos);
			return false;
		}

		unsigned char version = 0;
		stream >> version;
		if (version != ioVersion()) {
			std::cerr << "Error: "
					<< __FUNCTION__ << " Incorrect data \"Version\"."
					<< " Input: " << version
					<< " must be " << ioVersion() << " ." << std::endl;
			inStream.seekg(startPos);
			return false;
		}
		//-------------------------------------------------------------------------
		int32_t t = 0;
		stream >> t;
		mFormat = static_cast<eFormats>(t);
		//-----------------------
		uint32_t categorySize = 0;
		stream >> categorySize;
		//-----------------------
		while (categorySize) {
			std::string categoryName;
			uint32_t valuesSize = 0;
			stream >> categoryName;
			stream >> valuesSize;
			auto category = mRoot->insert(mRoot->begin(), Categories::value_type(categoryName, Values()));
			while (valuesSize) {
				std::string key;
				std::string val;
				stream >> key;
				stream >> val;
				category->second.insert(category->second.begin(), Values::value_type(key, val));
				--valuesSize;
			}
			--categorySize;
		}
		//-------------------------------------------------------------------------
		return true;
	}
	catch (std::exception & e) {
		std::cerr << __FUNCTION__ << " Can't load data, reason: " << e.what() << std::endl;
		inStream.seekg(startPos);
		return false;
	}
}

/*!
 * \details Saves config data to specified stream.
 * \param [in] outStream
 * \return True if successful otherwise false.
 */
inline bool Settings::save(std::ostream & outStream) const {
	assert(mRoot);
	std::ostream::streampos startPos = outStream.tellp();
	DataStreamO stream(&outStream);
	try {
		stream.writeFormatVersion();
		stream.writeRawData(ioId(), static_cast<uint64_t>(ioIdSize()));
		stream << static_cast<unsigned char>(ioVersion());
		//-------------------------------------------------------------------------
		stream << static_cast<int32_t>(mFormat);
		stream << static_cast<uint32_t>(mRoot->size());
		//-----------------------
		for (auto & category : *mRoot) {
			stream << category.first;
			stream << static_cast<uint32_t>(category.second.size());
			for (auto & val : category.second) {
				stream << val.first;
				stream << val.second;
			}
		}
		//-------------------------------------------------------------------------
		return true;
	}
	catch (std::exception & e) {
		std::cerr << __FUNCTION__ << " Can't save data, reason: " << e.what() << std::endl;
		outStream.seekp(startPos);
		return false;
	}
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Opens other group or sub-group.
 * \param [in] inGroupName
 */
inline void Settings::beginGroup(const std::string & inGroupName) {
	if (inGroupName.empty())
		return;
	if (mActualTreePath == "DEFAULT")
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

/*!
 * \details Closes actual group or sub-group.
 */
inline void Settings::endGroup() {
	endArray();

	if (mActualTreePath.empty())
		return;

	size_t pos = mActualTreePath.find_last_of('/');
	if (pos == std::string::npos) {
		Categories::iterator def = mRoot->find("DEFAULT");
		if (def == mRoot->end()) {
			std::cerr << __FUNCTION__ << " Internal error." << std::endl;
			return;
		}
		mActualTreePath = "DEFAULT";
		mActualValues = &def->second;
		return;
	}

	mActualTreePath.resize(pos);
	Categories::iterator item = mRoot->find(mActualTreePath);
	if (item == mRoot->end()) {
		std::cerr << __FUNCTION__ << " Internal error." << std::endl;
		return;
	}
	mActualValues = &item->second;
}

/*!
 * \details Gets actual croup.
 * \param [in] inFullTreePath
 * \return actual group.
 */
inline std::string Settings::actualGroup(bool inFullTreePath) const {
	if (inFullTreePath == false) {
		auto pos = mActualTreePath.find_last_of("/");
		if (pos == std::string::npos)
			return mActualTreePath;

		std::string group = mActualTreePath.substr(pos);
		if (group.length() < 2) {
			std::cerr << __FUNCTION__ << " Internal error with group's tail" << std::endl;
			return std::string();
		}
		// delete "/"
		return group.substr(1);
	}
	return mActualTreePath;
}

/*!
 * \details Checks group existence.
 * \param [in] inGroupName
 * \param [in] inFullTreePath
 * \return True if group is exists otherwise false.
 */
inline bool Settings::hasGroup(const std::string & inGroupName, bool inFullTreePath) const {
	if (inFullTreePath)
		return (mRoot->find(inGroupName) != mRoot->end());

	if (mActualTreePath == "DEFAULT")
		return false;

	return (mRoot->find(mActualTreePath + "/" + inGroupName) != mRoot->end());
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Gets the list of all keys of the actual group.
 * \return the list of all keys of the actual group.
 */
inline Settings::keyList Settings::keysList() const {
	keyList outKeys;
	if (mActualValues == nullptr) {
		std::cerr << __FUNCTION__ << " Internal logic error: actual values is not set " << std::endl;
		return outKeys;
	}

	if (mRoot->empty())
		return outKeys;

	for (auto & val : *mActualValues)
		outKeys.push_back(val.first);
	return outKeys;
}

/*!
 * \details Checks key existence.
 * \param [in] inKey
 * \return True is th key is exusts otherwise false.
 */
inline bool Settings::containsKey(const std::string & inKey) const {
	if (mActualValues == nullptr) {
		std::cerr << __FUNCTION__ << " Internal logic error: actual values is not set " << std::endl;
		return false;
	}

	if (mActualValues->empty())
		return false;

	std::string key = inKey;
	// if we need write this key like key of array		
	if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
		key = mCurrArrayName + std::string("/") + std::to_string(mCurrArrayIndex) + std::string("/") + inKey;
	}
	return (mActualValues->find(key) != mActualValues->end());
}

/*!
 * \details Removes specified key from the actual group.
 * \param [in] inKey
 * \return True id successful otherwise false.
 */
inline bool Settings::remove(const std::string & inKey) {
	if (mActualValues == nullptr) {
		std::cerr << __FUNCTION__ << " Internal logic error: actual values is not set " << std::endl;
		return false;
	}

	if (mActualValues->empty())
		return false;

	std::string key = inKey;
	// if we need write this key like key of array		
	if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
		key = mCurrArrayName + std::string("/") + std::to_string(mCurrArrayIndex) + std::string("/") + inKey;
	}
	return (mActualValues->erase(key) == 1);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Starts reading from array with inName from current group. Returns the size of the array.
 * \param [in] inName name of array
 * \return size of the array.
 */
inline uint32_t Settings::beginReadArray(const std::string & inName) {
	if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
		std::cerr << __FUNCTION__ << " You must call endArray() before." << std::endl;
		return 0;
	}
	mCurrArrayName = inName;
	uint32_t size = value(mCurrArrayName + "/size", 0);
	mIsReadArrayOperationInProgress = true;
	return size;
}

/*!
 * \details Starts writing to array of size inSize with inName in current group. If inSize is -1 (the default),
 * it is automatically determined based on the indexes of the entries written.
 * If you have many occurrences of a certain set of keys, you can use arrays to make your life easier.
 * For example, let's suppose that you want to save a variable-length list of user names and passwords.
 * \param [in] inName name of array.
 * \param [in] inSize size of array.
 */
inline void Settings::beginWriteArray(const std::string & inName, int32_t inSize) {
	if (mIsReadArrayOperationInProgress || mIsWriteArrayOperationInProgress) {
		std::cerr << __FUNCTION__ << " You must call endArray() before." << std::endl;
		return;
	}
	mCurrArrayName = inName;
	if (inSize > 0) {// if we used in parameter
		setValue(mCurrArrayName + "/size", inSize);
	}
	else {// if we auto calculating and write size later
		mIsNeedWriteArraySize = true;
	}
	mIsWriteArrayOperationInProgress = true;
}

/*!
 * \details Sets the current array index to inIndex.
 * \param [in] inIndex actual array index
 */
inline void Settings::arrayIndex(uint32_t inIndex) {
	mCurrArrayIndex = inIndex;
	// Do not delete this type reinterpret (about int32), We need compare between signed int!
	if (static_cast<int32_t>(mCurrArrayIndex) > mMaxArrayIndex)
		mMaxArrayIndex = mCurrArrayIndex;
}

/*!
 * \details Closes the array that was started using beginReadArray() or beginWriteArray().
 */
inline void Settings::endArray() {
	// calculating and writing size of wrote array
	if (mIsWriteArrayOperationInProgress && mIsNeedWriteArraySize && mMaxArrayIndex > 0) {
		mIsWriteArrayOperationInProgress = false;
		setValue(mCurrArrayName + "/size", mMaxArrayIndex + 1);
	}
	_arraysOperationsReset();
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
