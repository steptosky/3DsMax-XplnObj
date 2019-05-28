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

#include "Logger.h"
#include "Info.h"

#pragma warning(push, 0)
#include <max.h>
#include <log.h>
#include <IPathConfigMgr.h>
#pragma warning(pop)

#include "common/String.h"
#include <xpln/Info.h>

#ifndef NDEBUG
#	define LOG_LEVEL stsff::logging::BaseLogger::eLevel::LvlDebug
#else
#	define LOG_LEVEL stsff::logging::BaseLogger::eLevel::LvlMsg
#endif

#define LOG_PREFIX "[X-Obj]"

using namespace stsff::logging;

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

//sts::BaseLogger * sts::BaseLogger::mInstance = nullptr;
std::vector<Logger::UserConsoleCallBack> Logger::mCallbacks;
LogSys * Logger::mMaxLog = nullptr;
std::string Logger::mVersionString;
std::string Logger::mVersionShortString;

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void Logger::saveLog(const MSTR & where) const {
    IPathConfigMgr * paths = IPathConfigMgr::GetPathConfigMgr();
    const MaxSDK::Util::Path p(mLogFile);
    if (!paths->DoesFileExist(p)) {
        MessageBoxA(GetActiveWindow(),
                    "The log file does not exist. For some reason the 3DsMax or the plug-in didn't provide this file.\r\nCheck 3DsMax log settings.",
                    "Error", MB_ICONERROR);
    }
    else {
        if (!CopyFile(p.GetCStr(), where.data(),FALSE)) {
            const auto error = GetLastError();
            MessageBoxA(GetActiveWindow(), sts::MbStrUtils::join("Can't save log file. code: ", error).c_str(),
                        "Error", MB_ICONERROR);
        }
    }
}

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

Logger::Logger() {
    if (!mMaxLog) {
        mMaxLog = GetCOREInterface()->Log();
    }
    //--------------------------------
    createVersionStrings();
    //--------------------------------
    IPathConfigMgr * paths = IPathConfigMgr::GetPathConfigMgr();
    MaxSDK::Util::Path p(paths->GetDir(APP_MAXDATA_DIR));
    p.AddTrailingBackslash();
    p.Append(_T("Network"));
    // 3d max 9 and maybe some later versions can't create 
    // Max.log file because the folder "Network" does not exist.
    // So I fixed it. (hello Autodesk :D)
    if (!paths->DoesFileExist(p)) {
        if (!paths->CreateDirectoryHierarchy(p)) {
            MessageBox(GetActiveWindow(),
                       sts::StrUtils::joinStr(_T(" Can't create dir for the log file: "), p.GetCStr()).c_str(),
                       _T("Error"), MB_ICONERROR);
        }
    }
    p.AddTrailingBackslash();
    p.Append(_T("Max.log"));
    mLogFile = p.GetString();
    //--------------------------------
    printInformation();
    xobj::Logger::mInstance.setLevel(LOG_LEVEL);
    //--------------------------------
    const auto printer = [](const BaseLogger &, const BaseLogger::LogMsg & m, const DWORD msgType, const bool printLocation) {
        BOOL dialog = NO_DIALOG;
        if (!m.mCategory.empty()) {
            if (strcmp(m.mCategory.data(), LOG_CATEGORY_DIALOG) == 0 || m.mLevel == BaseLogger::LvlCritical) {
                dialog = DISPLAY_DIALOG;
            }
            else if (strcmp(m.mCategory.data(), LOG_CATEGORY_CONSOLE) == 0 || strcmp(m.mCategory.data(), LOG_CATEGORY_FOR_USER) == 0) {
                for (auto & c : mCallbacks) {
                    c(m.mLevel, m.mMsg.data());
                }
            }
        }

        mMaxLog->LogEntry(msgType, dialog, _M(XIO_PROJECT_NAME),
                          m.mCategory.empty() ? _M("%s%s %s") : _M("%s %s %s"),
                          _M(LOG_PREFIX),
                          m.mCategory.empty() ? _M("") : sts::toString(m.mCategory.data()).c_str(),
                          sts::toString(m.mMsg.data()).c_str());
        if (printLocation) {
            mMaxLog->LogEntry(msgType, NO_DIALOG, _M(XIO_PROJECT_NAME),
                              _M("%s\t\t<%s -> %s(%d)>"),
                              _M(LOG_PREFIX),
                              m.mFunction.empty() ? _M("") : sts::toString(m.mFunction.data()).c_str(),
                              m.mFile.empty() ? _M("") : sts::toString(sourceFileName(m.mFile.data())).c_str(),
                              m.mLine);
        }
    };
    //--------------------------------
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlDebug, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::clog, "DBG: %LN %MC %MS", colorize::magenta);
        Debug(printer(l, m, SYSLOG_DEBUG, false));
    });
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlMsg, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::clog, "%LN %MC %MS", nullptr);
        Debug(printer(l, m, SYSLOG_INFO, false));
    });
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlInfo, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::clog, "INF: %LN %MC %MS", colorize::cyan);
        Debug(printer(l, m, SYSLOG_INFO, false));
    });
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlSuccess, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::clog, "INF: %LN %MC %MS | OK", colorize::green);
        Debug(printer(l, m, SYSLOG_INFO, false));
    });
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlWarning, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::clog, "WRN: %LN %MC %MS", colorize::yellow);
        Debug(printer(l, m, SYSLOG_WARN, false));
    });
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlFail, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::cerr, "ERR: %LN %MC %MS | FAIL\n\t[%TM(%Y-%m-%d] [%T)] [%FN -> %FI(%LI)]", colorize::red);
        Debug(printer(l, m, SYSLOG_ERROR, false));
    });
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlError, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::cerr, "ERR: %LN %MC %MS \n\t[%TM(%Y-%m-%d] [%T)] [%FN -> %FI(%LI)]", colorize::red);
        Debug(printer(l, m, SYSLOG_ERROR, true));
    });
    xobj::Logger::mInstance.setHandler(BaseLogger::LvlCritical, [&printer](const BaseLogger & l, const BaseLogger::LogMsg & m) {
        BaseLogger::defaultHandler(l, m, std::cerr, "ERR: %LN %MC %MS \n\t[%TM(%Y-%m-%d] [%T)] [%FN -> %FI(%LI)]", colorize::red);
        Debug(printer(l, m, SYSLOG_ERROR, true));
    });
    //--------------------------------
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void Logger::createVersionStrings() {
    std::stringstream stream;
    stream << XIO_VERSION_STRING;
    if (strlen(XIO_RELEASE_TYPE) > 0) {
        stream << "-" << XIO_RELEASE_TYPE;
    }
    mVersionShortString = stream.str();

    stream << "+" << XIO_VCS_REVISION << " (" << XIO_VCS_BRANCH << ") " << XIO_COMPILE_DATE;
#ifndef NDEBUG
    stream << " (" << XIO_COMPILE_TIME << ") " << "DEBUG";
#endif

    mVersionString = stream.str();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

const char * Logger::levelAsString(const std::size_t level) {
    switch (level) {
        case BaseLogger::LvlDebug: return "DBG";
        case BaseLogger::LvlMsg: return "";
        case BaseLogger::LvlInfo: return "INF";
        case BaseLogger::LvlSuccess: return "INF";
        case BaseLogger::LvlWarning: return "WRN";
        case BaseLogger::LvlFail: return "ERR";
        case BaseLogger::LvlError: return "ERR";
        case BaseLogger::LvlCritical: return "ERR";
        default: return "";
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::string Logger::aboutXLibInfo(const bool winEol) {
    std::stringstream stream;
    const char * eol = winEol ? "\r\n" : "\n";
    //-------------------------------------------------------------------------

    stream << "Project: " << XOBJ_PROJECT_NAME << eol;
    stream << "Organization: " << XOBJ_ORGANIZATION_NAME << " (" << XOBJ_ORGANIZATION_WEBLINK << ")" << eol;
    stream << "Desc: " << XOBJ_PROJECT_DESCRIPTION << eol;
    stream << "Link: " << XOBJ_PROJECT_WEBLINK << eol;

    stream << "Version: " XOBJ_VERSION_STRING << "-" << XOBJ_RELEASE_TYPE << "+" << XOBJ_VCS_REVISION << " (" << XOBJ_VCS_BRANCH << ") "
            << XOBJ_COMPILE_DATE << Debug(" (" << XOBJ_COMPILE_TIME << ") " << "DEBUG" <<) eol;

    stream << "Compiler: " << XOBJ_COMPILER_NAME << " " << XOBJ_COMPILER_VERSION << eol;
    stream << XOBJ_COPYRIGHT << eol;
    stream << "Contacts: " << XOBJ_ORGANIZATION_WEBLINK << eol;
    stream << "License: " << XOBJ_LICENSE_TYPE << eol;
    stream << "Sources: " << XOBJ_PROJECT_SOURCES_WEBLINK << eol;

    stream << "Contributors: " << eol;
    for (size_t i = 0; i < XOBJ_ARRAY_LENGTH(XOBJ_CONTRIBUTORS); ++i) {
        stream << "    " << XOBJ_CONTRIBUTORS[i] << eol;
    }

    //-------------------------------------------------------------------------
    return stream.str();
}

std::string Logger::shortAboutXLibInfo(const bool winEol) {
    std::stringstream stream;
    const char * eol = winEol ? "\r\n" : "\n";
    //-------------------------------------------------------------------------

    stream << "Project: " << XOBJ_PROJECT_NAME << eol;
    stream << "Version: " XOBJ_VERSION_STRING << "-" << XOBJ_RELEASE_TYPE << "+" << XOBJ_VCS_REVISION << " (" << XOBJ_VCS_BRANCH << ") "
            << XOBJ_COMPILE_DATE << Debug(" (" << XOBJ_COMPILE_TIME << ") " << "DEBUG" <<) eol;
    stream << "Compiler: " << XOBJ_COMPILER_NAME << " " << XOBJ_COMPILER_VERSION << eol;

    //-------------------------------------------------------------------------
    return stream.str();
}

std::string Logger::aboutInfo(bool inUseWinEol) {
    std::stringstream stream;
    const char * eol = inUseWinEol ? "\r\n" : "\n";
    //-------------------------------------------------------------------------

#ifndef MAX_PRODUCT_YEAR_NUMBER
    stream << "Project: " XIO_PROJECT_NAME << " " << MAX_VERSION_MAJOR << eol;
#else
    stream << "Project: " XIO_PROJECT_NAME << " " << MAX_PRODUCT_YEAR_NUMBER << " (" << MAX_VERSION_MAJOR << ")" << eol;
#endif

    stream << "Organization: " << XIO_ORGANIZATION_NAME << " (" << XIO_ORGANIZATION_WEBLINK << ")" << eol;
    stream << "Desc: " << XIO_PROJECT_DESCRIPTION << eol;
    stream << "Link: " << XIO_PROJECT_WEBLINK << eol;

    stream << "Version: " << versionString() << eol;

    stream << "Compiler: " << XIO_COMPILER_NAME << " " << XIO_COMPILER_VERSION << eol;
    stream << XIO_COPYRIGHT << eol;
    stream << "Contacts: " << XIO_ORGANIZATION_WEBLINK << eol;
    stream << "License: " << XIO_LICENSE_TYPE << eol;
    stream << "Sources: " << XIO_PROJECT_SOURCES_WEBLINK << eol;

    stream << "Contributors: " << eol;
    for (size_t i = 0; i < XIO_ARRAY_LENGTH(XIO_CONTRIBUTORS); ++i) {
        stream << "    " << XIO_CONTRIBUTORS[i] << eol;
    }

    stream << eol << "Open source libraries: " << eol;
    for (size_t i = 0; i < XIO_ARRAY_LENGTH(XIO_LIBRARIES); ++i) {
        if (XIO_LIBRARIES[i].libName) {
            stream << "    " << XIO_LIBRARIES[i].libName;
        }
        if (XIO_LIBRARIES[i].license) {
            stream << " (" << XIO_LIBRARIES[i].license << ") ";
        }
        if (XIO_LIBRARIES[i].copyright) {
            stream << " " << XIO_LIBRARIES[i].copyright;
        }
        stream << eol;
    }

    //-------------------------------------------------------------------------

    return stream.str();
}

std::string Logger::shortAboutInfo(bool inUseWinEol) {
    std::stringstream stream;
    const char * eol = inUseWinEol ? "\r\n" : "\n";
    //-------------------------------------------------------------------------

#ifndef MAX_PRODUCT_YEAR_NUMBER
    stream << "Project: " XIO_PROJECT_NAME << " " << MAX_VERSION_MAJOR << eol;
#else
    stream << "Project: " XIO_PROJECT_NAME << " " << MAX_PRODUCT_YEAR_NUMBER << " (" << MAX_VERSION_MAJOR << ")" << eol;
#endif
    stream << "Version: " << versionString() << eol;

    stream << "Compiler: " << XIO_COMPILER_NAME << " " << XIO_COMPILER_VERSION << eol;

    //-------------------------------------------------------------------------

    return stream.str();
}

void Logger::printInformation() {
    auto msg1 = sts::MbStrUtils::splitToList(shortAboutInfo(false), "\n");
    auto msg2 = sts::MbStrUtils::splitToList(shortAboutXLibInfo(false), "\n");
    XLMessage << "************************************************************";
    for (const auto & s : msg1) {
        XLMessage << s;
    }
    XLMessage << "************************************************************";
    for (const auto & s : msg2) {
        XLMessage << s;
    }
    XLMessage << "************************************************************";
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void Logger::registerUserConsoleCallback(UserConsoleCallBack callback) {
    mCallbacks.emplace_back(callback);
}

void Logger::unregisterUserConsoleCallback(UserConsoleCallBack callback) {
    for (auto it = mCallbacks.begin(); it != mCallbacks.end(); ++it) {
        if (*it == callback) {
            mCallbacks.erase(it);
            return;
        }
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
