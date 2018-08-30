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

#include <iostream>
#include <sstream>
#include <mutex>

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#ifndef __STS_FUNC_NAME__
#	ifdef _MSC_VER
#		define __STS_FUNC_NAME__ __FUNCTION__
#	else
#		define __STS_FUNC_NAME__ __PRETTY_FUNCTION__
#	endif
#endif

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

namespace sts {

/*! 
 * \details This is a base logger interface. By default it prints all messages to std::cout.
 * \pre Before you will be able to use this logger you must create its variable somewhere,
 *      this logger implements the pattern singleton.
 * \code sts::BaseLogger * sts::BaseLogger::mInstance = nullptr; \endcode
 * \details Default log level is \"Debug\".
 * \details Default thread safe is \"false\".
 * \details The logger supports categories.
 * \code CategoryMessage("my category") << "my message"; \endcode
 * \note You can define log printing for your own types. \see \link LogMessage \endlink
 */
class BaseLogger {
public:

    enum eType {
        Fatal = 0,
        Critical = 1,
        Error = 2,
        Warning = 3,
        Info = 4,
        Msg = 5,
        Debug = 6,
    };

    /*!
     * \warning Don't forget to check the parameters for nullptr.
     */
    typedef void (*CallBack)(eType type, const char * msg,
                             const char * file, int line, const char * function,
                             const char * category);

    //---------------------------------------------------------------
    // @{

    static BaseLogger & instance() {
        if (mInstance == nullptr) {
            mInstance = new BaseLogger();
        }
        return *mInstance;
    }

    // @}
    //---------------------------------------------------------------
    // @{

    void log(eType inType, const char * inMsg,
             const char * inFile, int inLine, const char * inFunction,
             const char * inCategory) const {
        if (mThreadSafe) {
            std::lock_guard<std::mutex> lock(mMutex);
        }
        if (inType <= mLevel) {
            mCallBack(inType, inMsg, inFile, inLine, inFunction, inCategory);
        }
    }

    // @}
    //---------------------------------------------------------------
    // @{

    /*! 
     * \details Sets the thread safe logging. Default is false.
     * \warning It is strongly recommended to set this parameter once when your program is run
     *          otherwise in some cases it can lead to undefined behavior.
     */
    void setThreadSafe(bool state) {
        mThreadSafe = state;
    }

    bool isThreadSafe() const {
        return mThreadSafe;
    }

    // @}
    //---------------------------------------------------------------
    // @{

    void setLevel(eType inLevel) {
        mLevel = inLevel;
    }

    eType level() const {
        return mLevel;
    }

    // @}
    //---------------------------------------------------------------
    // @{

    void setCallBack(CallBack inCallBack) {
        mCallBack = inCallBack;
    }

    void removeCallBack() {
        mCallBack = defaultCallBack;
    }

    // @}
    //---------------------------------------------------------------
    // @{

    static const char * typeAsString(eType inType) {
        switch (inType) {
            case Msg: return "";
            case Info: return "INF";
            case Warning: return "WRN";
            case Error: return "ERR";
            case Critical: return "CRL";
            case Fatal: return "FTL";
            case Debug: return "DBG";
            default: return "";
        }
    }

    // @}
    //---------------------------------------------------------------

private:

    static void defaultCallBack(eType inType,
                                const char * inMsg,
                                const char * inFile,
                                int inLine,
                                const char * inFunction,
                                const char * inCategory) {

        if (inType == Msg || inType == Info) {
            std::cout << typeAsString(inType) << ": "
                    << (inCategory ? inCategory : "") << (inCategory ? " " : "")
                    << (inMsg ? inMsg : "") << std::endl;
        }
        else {
            std::cout << typeAsString(inType) << ": "
                    << (inCategory ? inCategory : "") << (inCategory ? " " : "") << (inMsg ? inMsg : "") << "\n\t<"
                    << (inFunction ? inFunction : "") << " -> " << (inFile ? inFile : "") << "(" << inLine << ")>"
                    << std::endl;
        }
    }

    BaseLogger() = default;
    ~BaseLogger() = default;
    BaseLogger(const BaseLogger &) = delete;
    BaseLogger & operator=(const BaseLogger &) = delete;

    eType mLevel = Debug;
    CallBack mCallBack = defaultCallBack;
    bool mThreadSafe = false;
    mutable std::mutex mMutex;
    static BaseLogger * mInstance;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Represents one log message.
 * \note In normal way you should not use this class directly use the macros instead.
 * \code LWarning << "My warning"; \endcode
 * \note The message will be printed when destructor is called 
 *       or if you manually force printing with the method LogMessage::push() 
 *       or operator << with LogMessage::CmdPush param. 
 *       Also you can use the macro LPush.
 * \code LWarning << "My warning" << LPush; \endcode
 *       It can be needed when you process the exceptions.
 * \note As the class has template operator operator<< you can define log printing for your own types. 
 * \code 
 * // define somewhere
 * template<>
 * inline sts::LogMessage & sts::LogMessage::operator<<<MyType>(const MyType & msg) {
 *     this->operator<< msg; // fix it for your type
 *     return *this;
 * }
 * \endcode
 */
class LogMessage {
public:

    struct CmdPush {};

    // @}
    //---------------------------------------------------------------
    // @{

    LogMessage()
        : mLog(&BaseLogger::instance()),
          mPushed(false) {}

    explicit LogMessage(const char * category)
        : LogMessage(nullptr, 0, nullptr, category) {}

    LogMessage(const char * file, int line, const char * function)
        : LogMessage(file, line, function, nullptr) {}

    LogMessage(const char * file, int line, const char * function, const char * category)
        : mLog(&BaseLogger::instance()),
          mFile(file),
          mFunction(function),
          mCategory(category),
          mLine(line),
          mPushed(false) {}

    ~LogMessage() {
        push();
    }

    // @}
    //---------------------------------------------------------------
    // @{

    template<class T>
    LogMessage & operator<<(const T & msg) {
        if (mType <= mLog->level()) {
            mStream << msg;
        }
        return *this;
    }

    LogMessage & operator<<(const CmdPush &) {
        push();
        return *this;
    }

    // @}
    //---------------------------------------------------------------
    // @{

    LogMessage & debug() {
        mType = BaseLogger::Debug;
        return *this;
    }

    LogMessage & warning() {
        mType = BaseLogger::Warning;
        return *this;
    }

    LogMessage & error() {
        mType = BaseLogger::Error;
        return *this;
    }

    LogMessage & critical() {
        mType = BaseLogger::Critical;
        return *this;
    }

    LogMessage & fatal() {
        mType = BaseLogger::Fatal;
        return *this;
    }

    LogMessage & info() {
        mType = BaseLogger::Info;
        return *this;
    }

    LogMessage & message() {
        mType = BaseLogger::Msg;
        return *this;
    }

    // @}
    //---------------------------------------------------------------
    // @{

    void push() {
        if (!mPushed) {
            mLog->log(mType, mStream.str().c_str(), mFile, mLine, mFunction, mCategory);
            mPushed = true;
        }
    }

    // @}
    //---------------------------------------------------------------

private:

    BaseLogger::eType mType = BaseLogger::Msg;
    std::stringstream mStream;
    BaseLogger * mLog = nullptr;
    const char * mFile = nullptr;
    const char * mFunction = nullptr;
    const char * mCategory = nullptr;
    int mLine = 0;
    bool mPushed:1;

};
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

// Log messages
#define LMessage    sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__).message()
#define LInfo       sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__).info()
#define LDebug      sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__).debug()
#define LFatal      sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__).fatal()
#define LCritical   sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__).critical()
#define LError      sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__).error()
#define LWarning    sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__).warning()

// Category messages
#define CategoryMessage(X)    sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__, X).message()
#define CategoryInfo(X)       sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__, X).info()
#define CategoryDebug(X)      sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__, X).debug()
#define CategoryFatal(X)      sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__, X).fatal()
#define CategoryCritical(X)   sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__, X).critical()
#define CategoryError(X)      sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__, X).error()
#define CategoryWarning(X)    sts::LogMessage(__FILE__, __LINE__, __STS_FUNC_NAME__, X).warning()

// Force push
#define LPush sts::LogMessage::CmdPush()
#define LEOL "\n"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
