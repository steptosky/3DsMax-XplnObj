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
#include "additional/utils/Single.h"
#include "additional/utils/BaseLogger.h"

#pragma warning(push, 0)
#include <strclass.h>
#pragma warning(pop)

#define LOG_CATEGORY_DIALOG  "Dlg"
#define LOG_CATEGORY_CONSOLE  "Csl"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

// Message to user console
#define CLMessage    CategoryMessage(LOG_CATEGORY_CONSOLE)
#define CLDebug      CategoryDebug(LOG_CATEGORY_CONSOLE)
#define CLFatal      CategoryFatal(LOG_CATEGORY_CONSOLE)
#define CLCritical   CategoryCritical(LOG_CATEGORY_CONSOLE)
#define CLError      CategoryError(LOG_CATEGORY_CONSOLE)
#define CLWarning    CategoryWarning(LOG_CATEGORY_CONSOLE)
#define CLInfo       CategoryInfo(LOG_CATEGORY_CONSOLE)

// Force push
#define CLPush LPush

// Message to dialog
#define DLMessage    CategoryMessage(LOG_CATEGORY_DIALOG)
#define DLDebug      CategoryDebug(LOG_CATEGORY_DIALOG)
#define DLFatal      CategoryFatal(LOG_CATEGORY_DIALOG)
#define DLCritical   CategoryCritical(LOG_CATEGORY_DIALOG)
#define DLError      CategoryError(LOG_CATEGORY_DIALOG)
#define DLWarning    CategoryWarning(LOG_CATEGORY_DIALOG)
#define DLInfo       CategoryInfo(LOG_CATEGORY_DIALOG)

// Force push
#define DLPush LPush

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#define LogNode(x) "Object: \"" << (x == nullptr ? "nullptr" : sts::toMbString(x->GetName())) << "\" - "
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

class LogSys;

class Logger : public sts::Single<Logger> {
    friend Single<Logger>;
    Logger();
public:

    typedef void (*UserConsoleCallBack)(sts::BaseLogger::eType type, const char * msg);

    virtual ~Logger();

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

    static void logCallBack(sts::BaseLogger::eType inType,
                            const char * inMsg,
                            const char * file,
                            int line,
                            const char * function,
                            const char * category);

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

#define log_unexpected_data(NODE, ID) LError << LogNode(NODE) << "has got unexpected data input: " << ID.toString();
#define log_unexpected_version(NODE, CURRENT) LError << LogNode(NODE) << "has got unexpected data version: " << CURRENT;
#define log_unexpected_data_length(NODE, LENGTH) LError << LogNode(NODE) << "has got unexpected data length: " << LENGTH;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
