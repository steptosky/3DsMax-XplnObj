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

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <Windows.h>
#include <Winhttp.h>
#include "UpdateChecker.h"

#include "common/Logger.h"
#include "common/String.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

UpdateChecker::~UpdateChecker() {
    freeResources();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void UpdateChecker::freeResources() {
    if (mThread) {
        mThread->join();
        delete mThread;
        mThread = nullptr;
    }
    mUpdateInfo = Update();
}

void UpdateChecker::checkForUpdate() {
    if (mThread) {
        LError << TOTEXT(UpdateChecker) << " - already has the thread";
        DbgAssert(false);
        return;
    }
    mThread = new std::thread(checkUpdateTask, this);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

UpdateChecker::Update UpdateChecker::updateInfo() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mUpdateInfo;
}

void UpdateChecker::setUpdateInfo(Update ipdate) {
    std::lock_guard<std::mutex> lock(mMutex);
    mUpdateInfo = ipdate;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void UpdateChecker::checkUpdateTask(void * inUserData) {
    UpdateChecker * updateChecker = reinterpret_cast<UpdateChecker*>(inUserData);
    Update update{};
    update.valid = true;
    Response response = latestReleaseTag();
    if (!response.error.empty()) {
        update.error = response.error;
        update.error.emplace_back("Can't get information about the latest release. See log for more information.");
    }
    else {
        try {
            std::string ver = UpdateChecker::extractVersion(response.body);
            update.version = sts::semver::SemVersion::parse(ver);
            if (!update.version) {
                update.error.emplace_back("Can't parse the version: ");
                update.error.back().append(ver);
            }
        }
        catch (std::exception & e) {
            update.error.emplace_back("Can't parse version: ");
            update.error.back().append(e.what());
        }
    }
    updateChecker->setUpdateInfo(update);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

/*
 * https://msdn.microsoft.com/en-us/library/aa384270(v=vs.85).aspx
 */
UpdateChecker::Response UpdateChecker::latestReleaseTag() {
    Response response{};
    const LPWSTR url = L"api.github.com";
    const LPWSTR urlDoc = L"/repos/steptosky/3DsMax-XplnObj/releases/latest";

    // Use WinHttpOpen to obtain a session handle.
    HINTERNET hSession = WinHttpOpen(L"WinHTTP",
                                     WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);

    HINTERNET hConnect = nullptr;
    // Specify an HTTP server.
    if (hSession) {
        hConnect = WinHttpConnect(hSession, url, INTERNET_DEFAULT_HTTPS_PORT, 0);
    }
    else {
        response.error.emplace_back();
        response.error.back()
                .append(__STS_FUNC_NAME__)
                .append(" wincode: ")
                .append(sts::toMbString(uint64_t(GetLastError())));
    }

    HINTERNET hRequest = nullptr;
    // Create an HTTP request handle.
    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect, L"GET", urlDoc,
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      WINHTTP_FLAG_SECURE);
    }
    else {
        response.error.emplace_back();
        response.error.back()
                .append(__STS_FUNC_NAME__)
                .append(" wincode: ")
                .append(sts::toMbString(uint64_t(GetLastError())));
    }

    BOOL bResults = FALSE;
    // Send a request.
    if (hRequest) {
        bResults = WinHttpSendRequest(hRequest,
                                      WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                      WINHTTP_NO_REQUEST_DATA, 0,
                                      0, 0);
    }
    else {
        response.error.emplace_back();
        response.error.back()
                .append(__STS_FUNC_NAME__)
                .append(" wincode: ")
                .append(sts::toMbString(uint64_t(GetLastError())));
    }

    // End the request.
    if (bResults) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }
    else {
        response.error.emplace_back();
        response.error.back()
                .append(__STS_FUNC_NAME__)
                .append(" wincode: ")
                .append(sts::toMbString(uint64_t(GetLastError())));
    }

    // Keep checking for data until there is nothing left.
    LPSTR pszOutBuffer;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    if (bResults) {
        do {
            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                response.error.emplace_back();
                response.error.back()
                        .append(__STS_FUNC_NAME__)
                        .append(" wincode: ")
                        .append(sts::toMbString(uint64_t(GetLastError())));
            }

            // Allocate space for the buffer.
            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer) {
                response.error.emplace_back();
                response.error.back()
                        .append(__STS_FUNC_NAME__)
                        .append(" Out of memory");
                dwSize = 0;
            }
            else {
                // Read the data.
                ZeroMemory(pszOutBuffer, dwSize + 1);
                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                    response.error.emplace_back();
                    response.error.back()
                            .append(__STS_FUNC_NAME__)
                            .append(" wincode: ")
                            .append(sts::toMbString(uint64_t(GetLastError())));
                }
                else {
                    response.body.append(pszOutBuffer);
                }

                // Free the memory allocated to the buffer.
                delete[] pszOutBuffer;
            }
        } while (dwSize > 0);
    }

    // Report any errors.
    if (!bResults) {
        response.error.emplace_back();
        response.error.back()
                .append(__STS_FUNC_NAME__)
                .append(" wincode: ")
                .append(sts::toMbString(uint64_t(GetLastError())));
    }

    // Close any open handles.
    if (hRequest)
        WinHttpCloseHandle(hRequest);
    if (hConnect)
        WinHttpCloseHandle(hConnect);
    if (hSession)
        WinHttpCloseHandle(hSession);

    return response;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
