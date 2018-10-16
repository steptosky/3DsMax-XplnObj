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

#pragma warning(push, 0)
#include <max.h>
#include <3dsmaxport.h>
#pragma warning(pop)

namespace max {
namespace ctrl {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    class RollupBase {
    public:

        explicit RollupBase(HINSTANCE hinstance)
            : Id(0),
              mMainWnd(nullptr),
              mHInstance(hinstance) {
            assert(mHInstance);
        }

        virtual ~RollupBase() {
            if (mMainWnd) {
                deleteRollup();
            }
            mMainWnd = nullptr;
        }

        //-------------------------------------------------------------------------

        HWND hwnd() { return mMainWnd; }
        virtual void create() = 0;
        virtual void destroy() = 0;
        //-------------------------------------------------------------------------

        bool isOpen() {
            IRollupWindow * Ip = getInterface();
            return (Ip->IsPanelOpen(Id) == TRUE) ? true : false;
        }

        void setOpen(bool open) {
            IRollupWindow * Ip = getInterface();
            return Ip->SetPanelOpen(Id, (open) ? TRUE : FALSE);
        }

    protected:

        void createRollup(int panel, TSTR name, void * param) {
            IRollupWindow * Ip = getInterface();
            if (Ip) {
                if (mMainWnd) {
                    Id = Ip->ReplaceRollup(Id, mHInstance, MAKEINTRESOURCE(panel),
                                           panelDlgProc, name, (LPARAM)param, NULL);
                    mMainWnd = Ip->GetPanelDlg(Id);
                }
                else {
                    Id = Ip->AppendRollup(mHInstance, MAKEINTRESOURCE(panel),
                                          panelDlgProc, name, (LPARAM)param, NULL);
                    mMainWnd = Ip->GetPanelDlg(Id);
                }
            }
        }

        void deleteRollup() {
            IRollupWindow * Ip = getInterface();
            if (Ip) {
                if (mMainWnd)
                    Ip->DeleteRollup(Id, 1);
                mMainWnd = nullptr;
            }
        }

    private:

        virtual void initWindow(HWND hWnd) = 0;
        virtual void destroyWindow(HWND hWnd) = 0;
        virtual IRollupWindow * getInterface() = 0;
        virtual INT_PTR panelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        //-------------------------------------------------------------------------

        static INT_PTR CALLBACK panelDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            RollupBase * theDlg = nullptr;
            if (msg == WM_INITDIALOG) {
                theDlg = reinterpret_cast<RollupBase *>(lParam);
                theDlg->mMainWnd = hWnd;
                DLSetWindowLongPtr(hWnd, lParam);
                theDlg->initWindow(hWnd);
            }
            else if (msg == WM_DESTROY) {
                theDlg = DLGetWindowLongPtr<RollupBase *>(hWnd);
                theDlg->destroyWindow(hWnd);
            }
            else {
                theDlg = DLGetWindowLongPtr<RollupBase *>(hWnd);
                if (!theDlg) {
                    return FALSE;
                }
            }

            return theDlg->panelProc(hWnd, msg, wParam, lParam);
        }

        int Id;
        HWND mMainWnd;
        HINSTANCE mHInstance;

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
