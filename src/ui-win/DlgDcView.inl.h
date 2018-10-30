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

#include "resource/resource.h"
#include "DlgEditDataref.h"
#include "DlgEditCommand.h"

namespace ui {
namespace win {

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    void DlgDcView<T>::open() {
        mLblCurrPath.setupChild(mDialog, IDC_PATH);
        mLblCurrPathType.setupChild(mDialog, IDC_PATH_TYPE);
        mBtnNewData.setupChild(mDialog, BTN_NEW_DATA);
        mBtnOk.setupChild(mDialog, BTN_OK);
        mBtnCancel.setupChild(mDialog, BTN_CANCEL);
        mBtnClearSearch.setupChild(mDialog, BTN_CLEAR);
        mComboFile.setupChild(mDialog, IDC_COMBO_FILES);
        mListDc.setupChild(mDialog, IDC_LIST);
        mEditKey.setupChild(mDialog, EDIT_KEY);
        mEditSearchKey.setupChild(mDialog, EDIT_SEARCH);
        //-------------------------------------------------
        mDialog.onClose = [&](auto) { mDialog.destroy(0); };
        mDialog.onInit = [&](auto) { initDlg(mDialog); };

        mBtnOk.onClick = [&](auto) { mDialog.destroy(1); };
        mBtnCancel.onClick = [&](auto) { mDialog.destroy(0); };

        mBtnNewData.onClick = [&](auto) { newData(); };
        mBtnClearSearch.onClick = [&](auto) { clearSearchKey(); };

        mComboFile.onSelectionChanged = [&](auto) { fileChanged(); };

        mEditKey.onLostFocus = [&](auto) { keyEntered(); };
        mEditKey.onEnter = mEditKey.onLostFocus;

        mEditSearchKey.onEnter = [&](auto) { searchKey(); };

        mListDc.onSelectionChanged = [&](auto) { keySelected(); };
        //-------------------------------------------------
        const auto res = mDialog.create(GetCOREInterface()->GetMAXHWnd(), DLG_DC);
        if (res == 1) {
            sigKeyChanged(mCurrFile, mCurrKey);
        }
    }

    template<typename T>
    void DlgDcView<T>::setAvailableFiles(const typename T::Files * files) {
        DbgAssert(files);
        mComboFile.clear();
        mBtnNewData.enable(false);
        for (const auto & f : *files) {
            mComboFile.addItem(f->mDisplayName);
        }
    }

    template<typename T>
    void DlgDcView<T>::setCurrKey(const MStr & key) {
        mEditKey.setText(key);
    }

    template<typename T>
    void DlgDcView<T>::setSearchKey(const MStr & key) {
        mEditSearchKey.setText(key);
    }

    template<typename T>
    void DlgDcView<T>::setCurrFile(const typename T::FilePtr & file, const std::optional<std::size_t> selectedItem) {
        mCurrFile = file;

        if (!mCurrFile) {
            mListDc.clear();
            mComboFile.setSelected(-1);
            return;
        }

        mComboFile.setSelected(mCurrFile->mDisplayName);
        mLblCurrPathType.setText(mCurrFile->mIsEditable ? _T("  [ editable ]") : _T("  [ not editable ]"));
        auto currPath = mCurrFile->mFilePath.GetString();
        mLblCurrPath.setText(currPath);

        mListDc.showData(mCurrFile, selectedItem);
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
    void DlgDcView<T>::initDlg(wrapper::ModalDialog & dialog) {
        dialog.centerByParent();
        dialog.setText(tittle());
        mBtnNewData.setToolTip(_T("Create new dataref/command in editable file"));
        mEditSearchKey.setToolTip(_T("Type text for searching and press ENTER"));
        sigReady();
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    void DlgDcView<T>::fileChanged() {
        sigCurrFileChanged(mComboFile.selectedText().value_or(MStr()));
    }

    template<typename T>
    void DlgDcView<T>::keyEntered() {
        mCurrKey = mEditKey.text();
    }

    template<typename T>
    void DlgDcView<T>::searchKey() {
        auto str = mEditSearchKey.text();
        sigSearchKeyChanged(str);
        mListDc.showData(mCurrFile, std::nullopt, xobj::fromMStr(str));
    }

    template<typename T>
    void DlgDcView<T>::clearSearchKey() {
        MStr str;
        mEditSearchKey.setText(str);
        sigSearchKeyChanged(str);
        mListDc.showData(mCurrFile, std::nullopt);
    }

    template<typename T>
    void DlgDcView<T>::keySelected() {
        if (!mCurrFile) {
            return;
        }
        const auto index = mListDc.selectedIndex();
        if (!index || *index >= mCurrFile->mData.size()) {
            mEditKey.setText(_T(""));
            return;
        }
        const auto & data = mCurrFile->mData.at(*index);
        const MStr str = xobj::toMStr(data.mKey);
        mEditKey.setText(str);
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
        if (!mCurrFile) {
            return;
        }
        typename T::data_type currData;
        do {
            const auto res = editData(currData);
            if (res) {

                // This algorithm saves data to the file but the file may have some changes
                // that has been made manually with a text editor or with something else like VCS.
                // So before save the file we try to reload it.
                // TODO it seems this logic should be in the presenter.
                try {
                    mCurrFile->loadData(mCurrFile->mFilePath);
                }
                catch (const std::exception & e) {
                    LError << "Can't load file <" << xobj::fromMStr(mCurrFile->mFilePath.GetString()) << "> reason: " << e.what();
                }

                auto index = mCurrFile->indexOfKey(res->mKey);
                if (index) {
                    MStr message;
                    message.Append(_T("Key [ ")).Append(xobj::toMStr(res->mKey)).Append(_T(" ] already exists"));
                    MessageBox(mDialog.hwnd(), message, _T("Key duplicate"), MB_OK | MB_ICONWARNING);
                    currData = *res;
                    continue;
                }

                mCurrFile->mData.emplace_back(*res);
                mCurrFile->sortData();
                mCurrFile->saveData(mCurrFile->mFilePath);
                index = mCurrFile->indexOfKey(res->mKey);
                index ? mListDc.showData(mCurrFile, index) : mListDc.showData(mCurrFile, std::nullopt);
            }
            break;
        } while (true);
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Functions */////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename T>
    void DlgDcView<T>::setDataEditable(const bool state) {
        mBtnNewData.enable(state);
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}
