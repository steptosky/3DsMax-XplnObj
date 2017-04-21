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
#include <thread>
#include "common/BaseLogger.h"
#include "common/Logger.h"

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
	sts::BaseLogger::instance().setThreadSafe(true);
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
	sts::BaseLogger::instance().setThreadSafe(false);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void UpdateChecker::checkUpdateTask(void * inUserData) {
	LInfo << "[version] Start checking for new version";
	UpdateChecker * updateChecker = reinterpret_cast<UpdateChecker*>(inUserData);
	Update update{};
	Response response = latestReleaseTag();
	if (response.error) {
		update.valid = true;
		update.error = "Can't get information about the latest release. See log for more information.";
		LError << "[version] " << update.error;
	}
	else {
		update.valid = true;
		std::string ver = UpdateChecker::extractVersion(response.body);
		LInfo << "[version] Got: " << ver;
		if (!update.version.parse(ver)) {
			update.error = "Can't parse the version: ";
			update.error.append(ver);
			LError << "[version] " << update.error;
		}
	}
	updateChecker->setUpdateInfo(update);
	LInfo << "[version] Finish checking for new version";
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
		LError << WinCode(GetLastError());
		response.error = true;
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
		LError << WinCode(GetLastError());
		response.error = true;
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
		LError << WinCode(GetLastError());
		response.error = true;
	}

	// End the request.
	if (bResults) {
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	}
	else {
		LError << WinCode(GetLastError());
		response.error = true;
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
				LError << WinCode(GetLastError());
				response.error = true;
			}

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer) {
				LError << "Out of memory";
				response.error = true;
				dwSize = 0;
			}
			else {
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);
				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
					LError << WinCode(GetLastError());
					response.error = true;
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
		LError << WinCode(GetLastError());
		response.error = true;
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
