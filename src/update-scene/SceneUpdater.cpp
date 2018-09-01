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

#include "SceneUpdater.h"
#include "common/Logger.h"
#include "common/NodeVisitor.h"
#include "models/MdManip.h"
#include "models/MdObjAttr.h"
#include <xpln/obj/ObjMesh.h>
#include "ui-win/Factory.h"

/**************************************************************************************************/
/////////////////////////////////////////* Static area *////////////////////////////////////////////
/**************************************************************************************************/

#define LOG_PREFIX "[SceneUpdater] "

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void SceneUpdater::update(const sts::semver::SemVersion & versionFrom, const sts::semver::SemVersion & versionTo) {
    if (versionFrom < versionTo) {
        LMessage << LOG_PREFIX << "from " << versionFrom.toString(false, false) << " to " << versionTo.toString(false, false);
        panelManipulators(versionFrom);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void SceneUpdater::panelManipulators(const sts::semver::SemVersion & versionFrom) {
    const char * msg = "-> add panel manipulator";
    const TCHAR * msgStartUpdate = _T("The cockpit and manipulator objects need to be updated.\n\r\
Although the scene will be updated automatically you still need to check the result manually!\n\r\
After this update you will see the objects that were affected, so you can save the list and then check the scene using that list.\n\r\
Read the help for more information about this issue.");
    //-------------------------------
    struct ManipGetter : ManipIO::IManipIo {
        void gotAttrManip(const xobj::AttrManipAxisKnob &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipAxisSwitchLeftRight &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipAxisSwitchUpDown &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipCmd &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipCmdAxis &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipCmdKnob &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipCmdSwitchLeftRight &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipCmdSwitchUpDown &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipDelta &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipDragAxis &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipDragAxisPix &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipDragXy &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipNoop &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipPanel &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipPush &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipRadio &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipToggle &) override { mHasManip = true; }
        void gotAttrManip(const xobj::AttrManipWrap &) override { mHasManip = true; }
        void gotNoManip() override { mHasManip = false; }
        bool mHasManip = false;
    };
    //-------------------------------
    if (versionFrom < sts::semver::SemVersion(2, 2, 0)) {
        LMessage << LOG_PREFIX << msg;
        std::vector<INode*> nodes;
        ui::win::Factory::sceneUpdateInfo(msgStartUpdate);
        //-------------------------------
        auto fn = [&nodes](INode * node) ->bool {
            xobj::AttrSet attrSet;
            MdObjAttr mdBaseAttr;
            if (!mdBaseAttr.linkNode(node)) {
                return true;
            }
            mdBaseAttr.loadFromNode(attrSet);
            if (attrSet.cockpit()) {
                MdManip mdManip;
                if (mdManip.linkNode(node)) {
                    ManipGetter mg;
                    mdManip.loadFromNode(&mg);
                    if (mg.mHasManip) {
                        return true;
                    }
                    xobj::AttrManipPanel panelManip;
                    panelManip.setCockpit(attrSet.cockpit());
                    mdManip.saveToNode(panelManip);
                    nodes.emplace_back(node);
                }
            }
            return true;
        };
        //-------------------------------
        NodeVisitor::visitAll(fn);
        LMessage << LOG_PREFIX << msg << " " << nodes.size() << " objects updated";
        ui::win::Factory::showUpdatedObjects(nodes);
        //-------------------------------
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
