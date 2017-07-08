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

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "Export.h"

#ifndef TOTEXT
#	define TOTEXT(X) #X
#endif

namespace sts {

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	/*!
	 * \details Implementation of the application settings, based on key/value paradigm.
	 */
	class Settings {
		typedef std::map<std::string, std::string> Values;
		typedef std::map<std::string, Values> Categories;
	public:

		typedef std::vector<std::string> keyList;
		static const char * defaultGroup;

		//-------------------------------------------------------
		// @{

		UtilsExp Settings();
		UtilsExp Settings(const Settings & copy);
		UtilsExp Settings(Settings && move) noexcept;
		UtilsExp virtual ~Settings();

		// @}
		//-------------------------------------------------------
		// @{

		UtilsExp Settings & operator =(const Settings & copy);
		UtilsExp Settings & operator =(Settings && move) noexcept;

		// @}
		//-------------------------------------------------------
		// @{

		/*!
		 * \details Checks whether the specified sub-group is exist in the current group 
		 *          or check the full group path.
		 * \param [in] groupName
		 * \param [in] fullTreePath
		 * \return True if group is exists otherwise false.
		 */
		UtilsExp bool hasGroup(const std::string & groupName, const bool fullTreePath = false) const;

		/*!
		 * \details Opens the group or sub-group.
		 * \param [in] groupName
		 */
		UtilsExp void beginGroup(const std::string & groupName);

		/*!
		 * \details Closes actual group or sub-group.
		 */
		UtilsExp void endGroup();

		/*!
		 * \details Gets actual croup name.
		 * \note you can use Settings::defaultGroup to check if there are some group opened.
		 * \code
		 * if(setting.currentGroup() == Settings::defaultGroup) {
		 *  ...
		 * }
		 * \endcode
		 * \param [in] fullTreePath
		 * \return actual group name.
		 */
		UtilsExp std::string currentGroup(const bool fullTreePath = false) const;

		// @}
		//-------------------------------------------------------
		// @{

		/*!
		 * \details Starts reading array from current group.
		 * \param [in] name name of the array
		 * \return The size of the array.
		 */
		UtilsExp uint32_t beginReadArray(const std::string & name);

		/*!
		 * \details Starts writing the array in current group. 
		 * \details If you have many occurrences of a certain set of keys, you can use arrays to make your life easier.
		 *          For example, let's suppose that you want to save a variable-length list of user names and passwords.
		 * \param [in] name name of the array.
		 * \param [in] size size of the array. -1 means the size will be auto-determined.
		 */
		UtilsExp void beginWriteArray(const std::string & name, const int32_t size = -1);

		/*!
		 * \details Sets the current array index.
		 * \param [in] index actual array index
		 */
		UtilsExp void arrayIndex(const uint32_t index);

		/*!
		 * \details Closes the array that was started using Settings::beginReadArray() or Settings::beginWriteArray().
		 */
		UtilsExp void endArray();

		// @}
		//-------------------------------------------------------
		// @{

		UtilsExp std::string value(const std::string & key, const std::string & defaultVal) const;
		UtilsExp int32_t value(const std::string & key, const int32_t defaultVal) const;
		UtilsExp int64_t value(const std::string & key, const int64_t defaultVal) const;
		UtilsExp uint32_t value(const std::string & key, const uint32_t defaultVal) const;
		UtilsExp uint64_t value(const std::string & key, const uint64_t defaultVal) const;
		UtilsExp float value(const std::string & key, const float defaultVal) const;
		UtilsExp double value(const std::string & key, const double defaultVal) const;

		std::string value(const std::string & key, const char * defaultVal) const { return value(key, std::string(defaultVal)); }
		bool value(const std::string & key, const bool defaultVal) const { return value(key, static_cast<uint8_t>(defaultVal)) != 0; }
		int8_t value(const std::string & key, const int8_t defaultVal) const { return static_cast<int8_t>(value(key, static_cast<int32_t>(defaultVal))); }
		int16_t value(const std::string & key, const int16_t defaultVal) const { return static_cast<int16_t>(value(key, static_cast<int32_t>(defaultVal))); }
		uint8_t value(const std::string & key, const uint8_t defaultVal) const { return static_cast<uint8_t>(value(key, static_cast<uint32_t>(defaultVal))); }
		uint16_t value(const std::string & key, const uint16_t defaultVal) const { return static_cast<uint16_t>(value(key, static_cast<uint32_t>(defaultVal))); }

		// @}
		//-------------------------------------------------------
		// @{

		UtilsExp void setValue(const std::string & key, const std::string & value);
		UtilsExp void setValue(const std::string & key, const float value);
		UtilsExp void setValue(const std::string & key, const double value);
		void setValue(const std::string & key, const char * value) { setValue(key, std::string(value)); }
		void setValue(const std::string & key, const bool value) { setValue(key, static_cast<uint8_t>(value)); }
		void setValue(const std::string & key, const int8_t value) { setValue(key, std::to_string(value)); }
		void setValue(const std::string & key, const int16_t value) { setValue(key, std::to_string(value)); }
		void setValue(const std::string & key, const int32_t value) { setValue(key, std::to_string(value)); }
		void setValue(const std::string & key, const int64_t value) { setValue(key, std::to_string(value)); }
		void setValue(const std::string & key, const uint8_t value) { setValue(key, std::to_string(value)); }
		void setValue(const std::string & key, const uint16_t value) { setValue(key, std::to_string(value)); }
		void setValue(const std::string & key, const uint32_t value) { setValue(key, std::to_string(value)); }
		void setValue(const std::string & key, const uint64_t value) { setValue(key, std::to_string(value)); }

		// @}
		//-------------------------------------------------------
		// @{

		/*!
		 * \return The list of all the keys of the current group.
		 */
		UtilsExp keyList keysList() const;

		/*!
		 * \details Checks whether the specified key is exist in the current croup.
		 * \param [in] key
		 * \return True if the specified key is exist otherwise false.
		 */
		UtilsExp bool containsKey(const std::string & key) const;

		/*!
		 * \details Removes specified key from the current group.
		 * \param [in] key
		 * \return True if successful otherwise false.
		 */
		UtilsExp bool remove(const std::string & key);

		// @}
		//-------------------------------------------------------
		// @{

		UtilsExp void clear();

		// @}
		//-------------------------------------------------------
		// @{

		/*!
		 * \details Loads config data from the specified file.
		 * \param [in] fileName full path with file name.
		 * \exception std::runtime_error if the file could not be loaded.
		 */
		UtilsExp void loadFile(const std::string & fileName);

		/*!
		 * \details Saves config data to the specified file.
		 * \param [in] fileName full path with file name.
		 * \exception std::runtime_error if the file could not be saved.
		 */
		UtilsExp void saveFile(const std::string & fileName) const;

		/*!
         * \details Loads config data from the specified string.
         * \param [in] string 
         * \exception std::runtime_error if the string could not be loaded.
         */
		UtilsExp void fromString(const std::string & string);

		/*!
		 * \details Saves config data to the string.
		 */
		UtilsExp std::string toString() const;

		// @}
		//-------------------------------------------------------

	private:

		void init();

		const std::string & retrieveValue(const std::string & key) const;
		static std::string checkLineForGroup(const std::string & inLine);
		void arraysOperationsReset();

		//-------------------------------------------------------

		// tree
		std::string mActualTreePath;
		Categories * mRoot;
		Values * mActualValues;

		// arrays
		uint32_t mCurrArrayIndex;
		int32_t mMaxArrayIndex;
		uint32_t mCurrArraySize;
		std::string mCurrArrayName;
		bool mIsReadArrayOperationInProgress : 1;
		bool mIsWriteArrayOperationInProgress : 1;
		bool mIsNeedWriteArraySize : 1;

		//-------------------------------------------------------

		void fromStream(std::istream & stream);
		void toStream(std::ostream & stream) const;

		//-------------------------------------------------------
	};

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/
}
