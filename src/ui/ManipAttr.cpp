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

#include "ManipAttr.h"
#include "models/MdManip.h"

#include "manip/ManipAttrWrap.h"
#include "manip/ManipAttrToggle.h"
#include "manip/ManipAttrRadio.h"
#include "manip/ManipAttrPush.h"
#include "manip/ManipAttrDragAxis.h"
#include "manip/ManipAttrDragAxisPix.h"
#include "manip/ManipAttrDragXy.h"
#include "manip/ManipAttrDelta.h"
#include "manip/ManipAttrCmd.h"
#include "manip/ManipAttrCmdAxis.h"
#include "manip/ManipAttrAxisKnob.h"
#include "manip/ManipAttrAxisSwitchLr.h"
#include "manip/ManipAttrAxisSwitchUd.h"
#include "manip/ManipAttrCmdKnob.h"
#include "manip/ManipAttrCmdLr.h"
#include "manip/ManipAttrCmdUd.h"
#include "manip/ManipAttrNoop.h"
#include "manip/ManipAttrPanel.h"

#include "resource/ResHelper.h"
#include "resource/resource.h"

namespace ui {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

INT_PTR ManipAttr::panelProc(HWND /*hWnd*/, UINT msg, WPARAM wParam, LPARAM /*lParam*/) {
    switch (msg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case CMB_MANIPTYPE: {
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        destroyAllSubWindows();
                        recalculateSize();
                        createSubWindow();
                    }
                    break;
                }
                default: break;
            }
            break;
        }
        default: break;
    }
    return 0;
}

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

ManipAttr::ManipAttr()
    : RollupBase(ResHelper::hInstance),
      mIRollup(nullptr),
      currSubWin(nullptr),
      mIp(GetCOREInterface()) {

    for (auto m : xobj::EManipulator::list()) {
        mChildren.insert(std::make_pair(sts::toString(m.toUiString()), nullptr));
    }
}

ManipAttr::~ManipAttr() {
    ManipAttr::destroy();
    setCurrManip(nullptr);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::registerCallbacks() {
    RegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
    RegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
    RegisterNotification(slotNodesClone, this, NOTIFY_POST_NODES_CLONED);
}

void ManipAttr::unRegisterCallbacks() {
    UnRegisterNotification(slotNodesClone, this, NOTIFY_POST_NODES_CLONED);
    UnRegisterNotification(slotObjectsDeleted, this, NOTIFY_SEL_NODES_PRE_DELETE);
    UnRegisterNotification(slotSelectionChange, this, NOTIFY_SELECTIONSET_CHANGED);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::slotObjectsDeleted(void * param, NotifyInfo * info) {
    ManipAttr * view = reinterpret_cast<ManipAttr*>(param);
    Tab<INode*> * nodeTab = reinterpret_cast<Tab<INode*>*>(info->callParam);
    if (view->mData.hasLink()) {
        int count = nodeTab->Count();
        for (int i = 0; i < count; ++i) {
            if (view->mData.linkedNode() == *nodeTab->Addr(i)) {
                view->setNoSelected();
                break;
            }
        }
    }
}

void ManipAttr::slotSelectionChange(void * param, NotifyInfo *) {
    ManipAttr * view = reinterpret_cast<ManipAttr*>(param);
    int selectedCount = view->mIp->GetSelNodeCount();
    if (selectedCount == 0) {
        view->setNoSelected();
    }
    else if (selectedCount == 1) {
        view->setNodeSelected(view->mIp->GetSelNode(0));
    }
    else {
        view->setNoSelected();
    }
}

void ManipAttr::slotNodesClone(void * param, NotifyInfo * /*info*/) {
    ManipAttr * view = reinterpret_cast<ManipAttr*>(param);
    view->mData.loadFromNode(view);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::setNodeSelected(INode * node) {
    if (mData.hasLink() && mData.linkedNode() == node) {
        return;
    }
    destroyAllSubWindows();
    recalculateSize();
    if (mData.linkNode(node)) {
        enableControls();
        mData.loadFromNode(this);
    }
    else {
        disableControls();
        cCmbManipType.setCurrSelected(0);
    }
}

void ManipAttr::setNoSelected() {
    destroyAllSubWindows();
    disableControls();
    recalculateSize();
    cCmbManipType.setCurrSelected(0);
    mData.clearLink();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::create(IRollupWindow * rollWin) {
    mIRollup = rollWin;
    create();
}

void ManipAttr::create() {
    createRollup(ROLL_MANIP_MAIN, _T("X Manipulator"), this);
    mOriginRollupSize = win::Base(hwnd()).size();
    disableControls();
    registerCallbacks();
}

void ManipAttr::destroy() {
    unRegisterCallbacks();
    if (hwnd() != nullptr) {
        destroyAllSubWindows();
        deleteRollup();
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::initWindow(HWND hWnd) {
    cCmbManipType.setup(hWnd, CMB_MANIPTYPE);
    for (auto m : xobj::EManipulator::list()) {
        cCmbManipType.addItem(sts::toString(m.toUiString()));
    }
    cCmbManipType.setCurrSelected(0);
}

void ManipAttr::destroyWindow(HWND /*hWnd*/) {
    cCmbManipType.release();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::enableControls() {
    cCmbManipType.enable(true);
}

void ManipAttr::disableControls() {
    cCmbManipType.enable(false);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::recalculateSize() {
    LONG height = mOriginRollupSize.cy;
    if (currSubWin != nullptr) {
        currSubWin->move(POINT{0, height});
        RECT size = currSubWin->rect();
        height += size.bottom - size.top;
    }
    mIRollup->SetPageDlgHeight(mIRollup->GetPanelIndex(hwnd()), height);
    mIRollup->UpdateLayout();
}

void ManipAttr::destroyAllSubWindows() {
    for (auto & curr : mChildren) {
        if (curr.second != nullptr) {
            delete curr.second;
            curr.second = nullptr;
        }
    }
    currSubWin = nullptr;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::createSubWindow() {
    if (!mData.hasLink()) {
        return;
    }
    sts::Str currItem = cCmbManipType.currSelectedText();
    //-------------------------------------------------
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::none).toUiString())) {
        setCurrManip(nullptr);
        mData.saveToNode(xobj::AttrManipNone());
        return;
    }
    //-------------------------------------------------
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::axis_knob).toUiString())) {
        gotAttrManip(xobj::AttrManipAxisKnob());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::axis_switch_lr).toUiString())) {
        gotAttrManip(xobj::AttrManipAxisSwitchLeftRight());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::axis_switch_ud).toUiString())) {
        gotAttrManip(xobj::AttrManipAxisSwitchUpDown());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::command).toUiString())) {
        gotAttrManip(xobj::AttrManipCmd());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::command_axis).toUiString())) {
        gotAttrManip(xobj::AttrManipCmdAxis());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::command_knob).toUiString())) {
        gotAttrManip(xobj::AttrManipCmdKnob());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::command_switch_lr).toUiString())) {
        gotAttrManip(xobj::AttrManipCmdSwitchLeftRight());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::command_switch_ud).toUiString())) {
        gotAttrManip(xobj::AttrManipCmdSwitchUpDown());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::delta).toUiString())) {
        gotAttrManip(xobj::AttrManipDelta());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::drag_axis).toUiString())) {
        gotAttrManip(xobj::AttrManipDragAxis());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::drag_axis_pix).toUiString())) {
        gotAttrManip(xobj::AttrManipDragAxisPix());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::drag_xy).toUiString())) {
        gotAttrManip(xobj::AttrManipDragXy());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::noop).toUiString())) {
        gotAttrManip(xobj::AttrManipNoop());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::panel).toUiString())) {
        gotAttrManip(xobj::AttrManipPanel());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::push).toUiString())) {
        gotAttrManip(xobj::AttrManipPush());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::radio).toUiString())) {
        gotAttrManip(xobj::AttrManipRadio());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::toggle).toUiString())) {
        gotAttrManip(xobj::AttrManipToggle());
        return;
    }
    if (currItem == sts::toString(xobj::EManipulator(xobj::EManipulator::wrap).toUiString())) {
        gotAttrManip(xobj::AttrManipWrap());
        return;
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipAttr::gotAttrManip(const xobj::AttrManipAxisKnob & inManip) {
    createSubWin<xobj::AttrManipAxisKnob, ManipAttrAxisKnob>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipAxisSwitchLeftRight & inManip) {
    createSubWin<xobj::AttrManipAxisSwitchLeftRight, ManipAttrAxisSwitchLr>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipAxisSwitchUpDown & inManip) {
    createSubWin<xobj::AttrManipAxisSwitchUpDown, ManipAttrAxisSwitchUd>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipCmd & inManip) {
    createSubWin<xobj::AttrManipCmd, ManipAttrCmd>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipCmdAxis & inManip) {
    createSubWin<xobj::AttrManipCmdAxis, ManipAttrCmdAxis>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipCmdKnob & inManip) {
    createSubWin<xobj::AttrManipCmdKnob, ManipAttrCmdKnob>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipCmdSwitchLeftRight & inManip) {
    createSubWin<xobj::AttrManipCmdSwitchLeftRight, ManipAttrCmdLr>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipCmdSwitchUpDown & inManip) {
    createSubWin<xobj::AttrManipCmdSwitchUpDown, ManipAttrCmdUd>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipDelta & inManip) {
    createSubWin<xobj::AttrManipDelta, ManipAttrDelta>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipDragAxis & inManip) {
    createSubWin<xobj::AttrManipDragAxis, ManipAttrDragAxis>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipDragAxisPix & inManip) {
    createSubWin<xobj::AttrManipDragAxisPix, ManipAttrDragAxisPix>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipDragXy & inManip) {
    createSubWin<xobj::AttrManipDragXy, ManipAttrDragXy>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipNoop & inManip) {
    createSubWin<xobj::AttrManipNoop, ManipAttrNoop>(inManip);
    mData.saveToNode(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipPanel & inManip) {
    createSubWin<xobj::AttrManipPanel, ManipAttrPanel>(inManip);
    mData.saveToNode(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipPush & inManip) {
    createSubWin<xobj::AttrManipPush, ManipAttrPush>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipRadio & inManip) {
    createSubWin<xobj::AttrManipRadio, ManipAttrRadio>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipToggle & inManip) {
    createSubWin<xobj::AttrManipToggle, ManipAttrToggle>(inManip);
}

void ManipAttr::gotAttrManip(const xobj::AttrManipWrap & inManip) {
    createSubWin<xobj::AttrManipWrap, ManipAttrWrap>(inManip);
}

void ManipAttr::gotNoManip() {
    setCurrManip(nullptr);
    cCmbManipType.setCurrSelected(sts::toString(xobj::EManipulator(xobj::EManipulator::none).toUiString()));
}

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

}
