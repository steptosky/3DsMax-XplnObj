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

#include "ObjCommon.h"
#include "ui/toolFrame/ToolFrame.h"
#include "common/Logger.h"
#include "ui/Factory.h"
#include "ui/main-menu/MainMenuActions.h"
#include "resource/ResHelper.h"
#include "common/NodeVisitor.h"

#ifndef IO_SAVE_CAST
#	if MAX_VERSION_MAJOR > 14
#		define IO_SAVE_CAST const BYTE
#	else
#		define IO_SAVE_CAST const void
#	endif
#endif

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

ObjCommon::ObjCommon() {
    mToolFrame = nullptr;
    mConfig = Config::instance();
    mCloneNodeChunk = nullptr;
}

ObjCommon::~ObjCommon() {
    mConfig->free();
}

void ObjCommon::DeleteThis() {
    this->free();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

//
// Another example but it does not work in 3d max 9
//
// #define WM_TRIGGER_CALLBACK WM_USER+4764
//
// void ObjCommon::setUpdateCallBack() {
//     PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_TRIGGER_CALLBACK, (UINT_PTR)&ObjCommon::updateWinCallback, (UINT_PTR)this);
// }
//
// void ObjCommon::updateWinCallback(UINT_PTR ptr) {}
//

#define WM_MY_TIMER_ID WM_USER+4764
#define WM_MY_TIMER_TIME 5000

void ObjCommon::updateCheckWinCallback(HWND hwnd, UINT /*uMsg*/, UINT_PTR idEvent, DWORD /*dwTime*/) {
    HWND maxHWND = GetCOREInterface()->GetMAXHWnd();
    if (maxHWND == hwnd && idEvent == WM_MY_TIMER_ID) {
        UpdateChecker::Update upd = ObjCommon::instance()->updateInfo();
        if (upd.valid) {
            KillTimer(maxHWND, WM_MY_TIMER_ID);
            if (!upd.error.empty()) {
                for (auto & e : upd.error) {
                    LError << "[version] " << e;
                }
            }
            else {
                LMessage << "[version] Got from the remote: " << upd.version.toString();
            }
        }
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

DWORD ObjCommon::Start() {
    //-- Mein Menu ---------------------------
    mMainMenuView.reset(ui::Factory::createMainMenuView());
    mMainMenuPresenter = std::make_unique<presenters::MainMenu>(mMainMenuView.get());
    //----------------------------------------

    mCloneNodeChunk = new CloneNodeChunk();
    mToolFrame = ui::ToolFrame::instance();
    mToolFrame->create();

    mUpdateChecker.checkForUpdate();
    SetTimer(GetCOREInterface()->GetMAXHWnd(), UINT_PTR(WM_MY_TIMER_ID),
             UINT(WM_MY_TIMER_TIME), reinterpret_cast<TIMERPROC>(&ObjCommon::updateCheckWinCallback));

    RegisterNotification(slotFileOpened, this, NOTIFY_FILE_POST_OPEN);

    return GUPRESULT_KEEP;
}

void ObjCommon::Stop() {
    //-- Mein Menu ---------------------------
    mMainMenuPresenter.reset();
    mMainMenuView.reset();
    //----------------------------------------
    UnRegisterNotification(slotFileOpened, this, NOTIFY_FILE_POST_OPEN);
    mUpdateChecker.freeResources();
    mToolFrame->free();
    delete mCloneNodeChunk;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

DWORD_PTR ObjCommon::Control(DWORD /*param*/) {
    return 0;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

// It is necessary for compiling for old version of 3d max.
#if MAX_VERSION_MAJOR > 14
#	define myByte BYTE
#else
#	define myByte void
#endif

// The case ID is not started from 0 because some previous versions of the plugin are used 0 and 1
// but that data is not necessary anymore.
IOResult ObjCommon::Save(ISave * isave) {
    try {
        ULONG temp = 0;
        pSettings.prepareDataForSave();
        std::string settings = pSettings.toString();
        //------------------------------------
        isave->BeginChunk(2);
        isave->Write(reinterpret_cast<const myByte *>(&mIoVersion), sizeof(mIoVersion), &temp);
        isave->EndChunk();
        DbgAssert(sizeof(mIoVersion) == temp);
        //------------------------------------
        uint32_t strLength = uint32_t(settings.length());
        isave->BeginChunk(3);
        isave->Write(reinterpret_cast<const myByte *>(&strLength), sizeof(strLength), &temp);
        isave->EndChunk();
        DbgAssert(sizeof(strLength) == temp);
        //------------------------------------
        isave->BeginChunk(4);
        isave->Write(reinterpret_cast<const myByte *>(settings.c_str()), strLength, &temp);
        isave->EndChunk();
        DbgAssert(strLength == temp);
        //------------------------------------
    }
    catch (std::exception & e) {
        LError << "Can't save " << TOTEXT(ObjCommon) << " Reason: " << e.what();
        return IO_ERROR;
    }
    return IO_OK;
}

IOResult ObjCommon::Load(ILoad * iload) {
    try {
        ULONG temp;
        uint32_t version = 0;
        uint32_t strLength = 0;
        IOResult res;
        while ((res = iload->OpenChunk()) == IO_OK) {
            switch (iload->CurChunkID()) {
                case 2: {
                    iload->Read(reinterpret_cast<myByte *>(&version), sizeof(version), &temp);
                    DbgAssert(sizeof(mIoVersion) == temp);
                    if (version != mIoVersion) {
                        LError << "Got incorrect version " << version << "/" << mIoVersion;
                        return IO_ERROR;
                    }
                    break;
                }
                case 3: {
                    iload->Read(reinterpret_cast<myByte *>(&strLength), sizeof(strLength), &temp);
                    DbgAssert(sizeof(strLength) == temp);
                    break;
                }
                case 4: {
                    char * str = new char[strLength];
                    iload->Read(reinterpret_cast<myByte *>(str), strLength, &temp);
                    DbgAssert(strLength == temp);
                    std::string stdstr(str, size_t(strLength));
                    delete[] str;
                    pSettings.fromString(stdstr);
                    if (pSettings.isSavedAsXplnScene() && pSettings.pluginVersion() < pSettings.sceneVersion()) {
                        ui::Factory::showVersionIncompatible();
                        return IO_ERROR;
                    }
                    break;
                }
                default: break;
            }
            iload->CloseChunk();
            if (res != IO_OK) {
                return res;
            }
        }
        //------------------------------------
    }
    catch (std::exception & e) {
        LError << "Can't load " << TOTEXT(ObjCommon) << " Reason: " << e.what();
        return IO_ERROR;
    }
    return IO_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ObjCommon::slotFileOpened(void * param, NotifyInfo *) {
    ObjCommon * d = static_cast<ObjCommon*>(param);
    // NodeVisitor::sceneContainsMainObj() - this is needed for the old scenes which 
    // have not set the flag d->pSettings.isSavedAsXplnScene()
    // This situation slows down loading non-x-plane scenes because it needs
    // to check whether the scene contains main x-plane object.
    if (d->pSettings.isSavedAsXplnScene() || NodeVisitor::sceneContainsMainObj()) {
        d->mSceneUpdater.update(d->pSettings.sceneVersion(), d->pSettings.pluginVersion());
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
