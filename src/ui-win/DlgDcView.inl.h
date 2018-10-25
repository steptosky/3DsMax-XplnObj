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
#include <3dsmaxport.h>
#pragma warning(pop)

#include <windows.h>
#include <xpln/utils/DatarefsFile.h>
#include "resource/resource.h"
#include "resource/ResHelper.h"
#include "Utils.h"
#include "DlgEditDataref.h"
#include "DlgEditCommand.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    INT_PTR CALLBACK DlgDcView<T>::callBack(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
        DlgDcView<T> * theDlg;
        if (message == WM_INITDIALOG) {
            theDlg = reinterpret_cast<DlgDcView<T>*>(lParam);
            theDlg->mDlgMain.setup(hWnd);
            DLSetWindowLongPtr(hWnd, lParam);
        }
        else {
            if ((theDlg = DLGetWindowLongPtr<DlgDcView<T> *>(hWnd)) == nullptr) {
                return FALSE;
            }
            if (message == WM_DESTROY) {
                theDlg->mDlgMain.release();
            }
        }

        //------------------------------------------------------

        switch (message) {
            case WM_INITDIALOG: {
                theDlg->initDlg(hWnd);
                break;
            }
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case BTN_NEW_DATA: {
                        theDlg->newData();
                        break;
                    }
                    case BTN_OK: {
                        EndDialog(hWnd, 1);
                        break;
                    }
                    case BTN_CANCEL: {
                        EndDialog(hWnd, 0);
                        break;
                    }
                    case BTN_CLEAR: {
                        theDlg->clearSearchKey();
                        break;
                    }
                    case IDC_COMBO_FILES: {
                        if (HIWORD(wParam) == CBN_SELCHANGE) {
                            theDlg->fileChanged();
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case WM_CUSTEDIT_ENTER: {
                switch (LOWORD(wParam)) {
                    case EDIT_KEY: {
                        theDlg->keyEntered();
                        break;
                    }
                    case EDIT_SEARCH: {
                        theDlg->searchKey();
                        break;
                    }
                    default: break;
                }
                break;
            }
            case WM_NOTIFY: {
                switch (LOWORD(wParam)) {
                    case IDC_LIST: {
                        const auto item = reinterpret_cast<LPNMHDR>(lParam);
                        switch (item->code) {
                            case NM_CLICK: {
                                theDlg->keySelected();
                                break;
                            }
                            default: break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case WM_DESTROY: {
                theDlg->destroyDlg(hWnd);
                break;
            }
            case WM_CLOSE: {
                EndDialog(hWnd, 0);
                break;
            }
            default: break;
        }
        return FALSE;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    void DlgDcView<T>::open() {
        const auto res = DialogBoxParam(ResHelper::hInstance, MAKEINTRESOURCE(DLG_DC),
                                        GetCOREInterface()->GetMAXHWnd(),
                                        callBack, reinterpret_cast<LPARAM>(this));
        if (res == 1) {
            sigKeyChanged(mCurrFile, mCurrKey);
        }
    }

    template<typename T>
    void DlgDcView<T>::setAvailableFiles(const typename T::Files * files) {
        DbgAssert(files);
        mCtrlComboFile.clear();
        for (const auto & f : *files) {
            mCtrlComboFile.addItem(f->mDisplayName.data());
        }
    }

    template<typename T>
    void DlgDcView<T>::setCurrKey(const MStr & key) {
        mCtrlKey->SetText(MStr(key));
    }

    template<typename T>
    void DlgDcView<T>::setSearchKey(const MStr & key) {
        mCtrlSearchKey->SetText(MStr(key));
    }

    template<typename T>
    void DlgDcView<T>::setCurrFile(const typename T::FilePtr & file, const std::optional<std::size_t> selectedItem) {
        mCurrFile = file;

        if (!mCurrFile) {
            mCtrlList.clear();
            mCtrlComboFile.setCurrSelected(-1);
            return;
        }

        mCtrlComboFile.setCurrSelected(mCurrFile->mDisplayName.data());
        mCtrlLblCurrPathType.setText(mCurrFile->mIsEditable ? _T("  [ editable ]") : _T("  [ not editable ]"));
        auto currPath = mCurrFile->mFilePath.GetString();
        mCtrlLblCurrPath.setText(currPath.data());

        mCtrlList.showData(mCurrFile, selectedItem);
        setDataEditable(mCurrFile->mIsEditable);

        if (selectedItem) {
            const auto & val = file->mData.at(*selectedItem);
            setCurrKey(xobj::toMStr(val.mKey));
        }
        else {
            setCurrKey(_T(""));
        }
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    const MCHAR * DlgDcView<T>::tittle() {
        if constexpr (std::is_same<xobj::Dataref, typename T::data_type>::value) {
            return _T("Select DatRef");
        }
        else if constexpr (std::is_same<xobj::Command, typename T::data_type>::value) {
            return _T("Select Command");
        }
        else {
            static_assert(false, "unknown type");
            return _T("error");
        }
    }

    template<typename T>
    void DlgDcView<T>::initDlg(const HWND hWnd) {
        CenterWindow(mDlgMain.hwnd(), mDlgMain.parent());
        SetWindowText(hWnd, tittle());
        mCtrlList.setup(GetDlgItem(hWnd, IDC_LIST));
        mCtrlBtnNewData.setup(hWnd, BTN_NEW_DATA);
        mCtrlLblCurrPath.setup(hWnd, IDC_PATH);
        mCtrlLblCurrPathType.setup(hWnd, IDC_PATH_TYPE);
        mCtrlComboFile.setup(hWnd, IDC_COMBO_FILES);
        mCtrlKey = GetICustEdit(GetDlgItem(hWnd, EDIT_KEY));
        mCtrlSearchKey = GetICustEdit(GetDlgItem(hWnd, EDIT_SEARCH));

        mCtrlBtnNewData.setToolTip(_T("Create new dataref/command in editable file"));

#if MAX_VERSION_MAJOR > 10 // 2008
        mCtrlSearchKey->SetTooltip(true, _T("Type text for searching and press ENTER"));
#endif
        sigReady();
    }

    template<typename T>
    void DlgDcView<T>::destroyDlg(const HWND /*hWnd*/) {
        ReleaseICustEdit(mCtrlSearchKey);
        ReleaseICustEdit(mCtrlKey);
        mCtrlBtnNewData.release();
        mCtrlLblCurrPath.release();
        mCtrlLblCurrPathType.release();
        mCtrlComboFile.release();
        mCtrlList.release();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    void DlgDcView<T>::fileChanged() {
        sigCurrFileChanged(MStr(mCtrlComboFile.currSelectedText().data()));
    }

    template<typename T>
    void DlgDcView<T>::keyEntered() {
        Utils::getText(mCtrlKey, mCurrKey);
    }

    template<typename T>
    void DlgDcView<T>::searchKey() {
        if (mCtrlSearchKey->HasFocus()) {
            MStr str;
            Utils::getText(mCtrlSearchKey, str);
            sigSearchKeyChanged(str);
            mCtrlList.showData(mCurrFile, std::nullopt, xobj::fromMStr(str));
        }
    }

    template<typename T>
    void DlgDcView<T>::clearSearchKey() {
        MStr str;
        mCtrlSearchKey->SetText(str);
        sigSearchKeyChanged(str);
        mCtrlList.showData(mCurrFile, std::nullopt);
    }

    template<typename T>
    void DlgDcView<T>::keySelected() {
        const auto index = mCtrlList.selectedIndex();
        if (!index || *index >= mCurrFile->mData.size()) {
            mCtrlKey->SetText(_T(""));
            return;
        }
        const auto & data = mCurrFile->mData.at(*index);
        MStr str = xobj::toMStr(data.mKey);
        mCtrlKey->SetText(str);
        keyEntered();
    }

    inline std::optional<xobj::Dataref> editData(const xobj::Dataref & data) {
        return DlgEditDataref::edit(data);;
    }

    inline std::optional<xobj::Command> editData(const xobj::Command & data) {
        return DlgEditCommand::edit(data);;
    }

    template<typename T>
    void DlgDcView<T>::newData() {
        typename T::data_type currData;
        do {
            const auto res = editData(currData);
            if (res) {
                auto index = mCurrFile->indexOfKey(res->mKey);
                if (index) {
                    MStr message;
                    message.Append(_T("Key [ ")).Append(xobj::toMStr(res->mKey)).Append(_T(" ] already exists"));
                    MessageBox(mDlgMain.hwnd(), message, _T("Key duplicate"), MB_OK | MB_ICONWARNING);
                    currData = *res;
                    continue;
                }
                mCurrFile->mData.emplace_back(*res);
                mCurrFile->sortData();
                mCurrFile->saveData(mCurrFile->mFilePath);
                index = mCurrFile->indexOfKey(res->mKey);
                index ? mCtrlList.showData(mCurrFile, index) : mCtrlList.showData(mCurrFile, std::nullopt);
            }
            break;
        } while (true);
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    void DlgDcView<T>::setDataEditable(const bool state) {
        mCtrlBtnNewData.enable(state);
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
