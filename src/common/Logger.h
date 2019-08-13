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

#include <vector>
#include <xpln/common/Logger.h>
#include "additional/utils/Single.h"

#pragma warning(push, 0)
#include <strclass.h>
#pragma warning(pop)

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class LogSys;

class Logger final : public sts::Single<Logger> {
    friend Single<Logger>;
    Logger();
public:

    typedef void (*UserConsoleCallBack)(std::size_t level, const char * msg);

    ~Logger() = default;

    //-------------------------------------------------------------------------

    /*!
     * \details The callback will be called only for messages 
     *          that must be displayed in the user console.
     * \param callback [in] 
     */
    static void registerUserConsoleCallback(UserConsoleCallBack callback);

    /*!
     * \see \link Logger::registerUserConsoleCallback \endlink
     * \param callback [in] 
     */
    static void unregisterUserConsoleCallback(UserConsoleCallBack callback);

    //-------------------------------------------------------------------------

    /*!
     * \details Saves current log to the given file path.
     * \param where 
     */
    void saveLog(const MSTR & where) const;

    //-------------------------------------------------------------------------

    static std::string aboutXLibInfo(bool inUseWinEol);
    static std::string shortAboutXLibInfo(bool inUseWinEol);

    static std::string aboutInfo(bool inUseWinEol);
    static std::string shortAboutInfo(bool inUseWinEol);

    static const std::string & versionShortString() { return mVersionShortString; }
    static const std::string & versionString() { return mVersionString; }

    //-------------------------------------------------------------------------

    static const char * levelAsString(std::size_t level);

    //-------------------------------------------------------------------------

private:

    //-------------------------------------------------------------------------

    static void printInformation();
    static void createVersionStrings();

    static std::vector<UserConsoleCallBack> mCallbacks;
    static LogSys * mMaxLog;
    static std::string mVersionShortString;
    static std::string mVersionString;

    MSTR mLogFile;

    //-------------------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#define LOG_CATEGORY_DIALOG  "Dlg"
#define LOG_CATEGORY_CONSOLE  "Csl"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

// Message to user console
#define CLMessage    LcMessage(xobj::Logger::mInstance,LOG_CATEGORY_CONSOLE)
#define CLDebug      LcDebug(xobj::Logger::mInstance,LOG_CATEGORY_CONSOLE)
#define CLFatal      LcCritical(xobj::Logger::mInstance,LOG_CATEGORY_CONSOLE)
#define CLCritical   LcCritical(xobj::Logger::mInstance,LOG_CATEGORY_CONSOLE)
#define CLError      LcError(xobj::Logger::mInstance,LOG_CATEGORY_CONSOLE)
#define CLWarning    LcWarning(xobj::Logger::mInstance,LOG_CATEGORY_CONSOLE)
#define CLInfo       LcInfo(xobj::Logger::mInstance,LOG_CATEGORY_CONSOLE)

// Force push
#define CLPush LPush

// Message to dialog
#define DLMessage    LcMessage(xobj::Logger::mInstance,LOG_CATEGORY_DIALOG)
#define DLDebug      LcDebug(xobj::Logger::mInstance,LOG_CATEGORY_DIALOG)
#define DLFatal      LcCritical(xobj::Logger::mInstance,LOG_CATEGORY_DIALOG)
#define DLCritical   LcCritical(xobj::Logger::mInstance,LOG_CATEGORY_DIALOG)
#define DLError      LcError(xobj::Logger::mInstance,LOG_CATEGORY_DIALOG)
#define DLWarning    LcWarning(xobj::Logger::mInstance,LOG_CATEGORY_DIALOG)
#define DLInfo       LcInfo(xobj::Logger::mInstance,LOG_CATEGORY_DIALOG)

// Force push
#define DLPush LPush

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#define LogNode(x) "Object: \"" << (x == nullptr ? "nullptr" : sts::toMbString(x->GetName())) << "\" - "
#define LogNodeRef(x) "Object: \"" << sts::toMbString(x.GetName()) << "\" - "
#define WinCode(x) "win code:" << x

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#ifndef Debug
#	ifndef NDEBUG
#		define Debug(x) x
#	else
#		define Debug(x)
#	endif
#endif

#ifndef TOTEXT
#	define TOTEXT(x) #x
#endif

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#define log_unexpected_data(NODE, ID) XLError << LogNode(NODE) << "has got unexpected data input: " << ID.toString();
#define log_unexpected_version(NODE, CURRENT) XLError << LogNode(NODE) << "has got unexpected data version: " << CURRENT;
#define log_unexpected_data_length(NODE, LENGTH) XLError << LogNode(NODE) << "has got unexpected data length: " << LENGTH;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
