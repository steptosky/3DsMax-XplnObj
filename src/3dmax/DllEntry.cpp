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

#include <array>

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include "Info.h"
#include "common/Logger.h"
#include "resource/ResHelper.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#define FnExport __declspec(dllexport)

#ifndef NDEBUG
#	define CONSOLE_ENABLED
#endif

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

typedef ClassDesc2 *(*ClassDesc2Creators)();
static std::array<ClassDesc2Creators, 6> gClassDesc2Array = {
    &ClassesDescriptions::commonClass,
    &ClassesDescriptions::mainObj,
    &ClassesDescriptions::lodObj,
    &ClassesDescriptions::smokeObj,
    &ClassesDescriptions::exporter,
    &ClassesDescriptions::importer,
};

extern "C" {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

// This function is called by Windows when the DLL is loaded.  This 
// function may also be called many times during time critical operations
// like rendering.  Therefore developers need to be careful what they
// do inside this function.  In the code below, note how after the DLL is
// loaded the first time only a few statements are executed.
BOOL WINAPI DllMain(const HINSTANCE hInstDll, const ULONG fdwReason, const LPVOID /*lpvReserved*/) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        ResHelper::setHInstance(hInstDll);
        DisableThreadLibraryCalls(hInstDll);
    }
    return TRUE;
}

//-------------------------------------------------------------------------

// This function returns the number of plug-in classes this DLL
FnExport int LibNumberClasses() {
    return static_cast<int>(gClassDesc2Array.size());
}

// This function returns the number of plug-in classes this DLL
FnExport ClassDesc * LibClassDesc(const int i) {
    if (i < 0 || std::size_t(i) >= gClassDesc2Array.size()) {
        return nullptr;
    }
    return gClassDesc2Array[std::size_t(i)]();
}

//-------------------------------------------------------------------------

// This function returns a pre-defined constant indicating the version of 
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
FnExport ULONG LibVersion() {
    return VERSION_3DSMAX;
}

// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
FnExport const TCHAR * LibDescription() {
    return _T(XIO_PROJECT_NAME);
}

// 3ds Max's defer loading mechanism
FnExport ULONG CanAutoDefer() {
    return FALSE;
}

//-------------------------------------------------------------------------

// This function is called once, right after your plugin has been loaded by 3ds Max. 
// Perform one-time plugin initialization in this method.
// Return TRUE if you deem your plugin successfully loaded, or FALSE otherwise. If 
// the function returns FALSE, the system will NOT load the plugin, it will then call FreeLibrary
// on your DLL, and send you a message.
FnExport int LibInitialize(void) {
#ifdef CONSOLE_ENABLED
    // console in debug mode
    AllocConsole();
    freopen("CONOUT$", "wb", stdout);
    freopen("CONOUT$", "wb", stderr);
#endif
    Logger::instance();
    return TRUE;
}

// This function is called once, just before the plugin is unloaded. 
// Perform one-time plugin un-initialization in this method."
// The system doesn't pay attention to a return value.
FnExport int LibShutdown(void) {
#ifdef CONSOLE_ENABLED
    // console in debug mode
    fclose(stdout);
    fclose(stderr);
    FreeConsole();
#endif
    Logger::free();
    return TRUE;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
